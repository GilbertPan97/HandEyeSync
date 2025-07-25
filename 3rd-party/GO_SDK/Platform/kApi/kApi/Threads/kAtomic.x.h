/** 
 * @file    kAtomic.x.h
 *
 * @internal
 * Copyright (C) 2012-2024 by LMI Technologies Inc.
 * Licensed under the MIT License.
 * Redistributed files must retain the above copyright notice.
 */
#ifndef K_API_ATOMIC_X_H
#define K_API_ATOMIC_X_H

/**
 * @internal 
 * @class       kAtomic
 * @ingroup     kApi-Threads
 * @brief       Implements shared support functions for atomic variable classes.
 */
typedef struct kAtomicStatic
{
    k32u placeholder;       //unused
} kAtomicStatic; 

kDeclareStaticClassEx(k, kAtomic)

kFx(kStatus) xkAtomic_InitStatic();
kFx(kStatus) xkAtomic_ReleaseStatic();

/*
* Forward Declarations
*/

kInlineFx(void) kAtomic32s_Init(kAtomic32s* atomic, k32s value);
kInlineFx(k32s) kAtomic32s_Increment(kAtomic32s* atomic);
kInlineFx(k32s) kAtomic32s_Decrement(kAtomic32s* atomic);
kInlineFx(k32s) kAtomic32s_Exchange(kAtomic32s* atomic, k32s value);
kInlineFx(kBool) kAtomic32s_CompareExchange(kAtomic32s* atomic, k32s oldValue, k32s value);
kInlineFx(k32s) kAtomic32s_Get(kAtomic32s* atomic);
kInlineFx(void) kAtomicPointer_Init(kAtomicPointer* atomic, kPointer value);
kInlineFx(kPointer) kAtomicPointer_Exchange(kAtomicPointer* atomic, kPointer value);
kInlineFx(kBool) kAtomicPointer_CompareExchange(kAtomicPointer* atomic, kPointer oldValue, kPointer value);
kInlineFx(kPointer) kAtomicPointer_Get(kAtomicPointer* atomic);

#if defined(K_WINDOWS)

#   include <intrin.h>

    kInlineFx(k32s) xkAtomic32s_IncrementImpl(kAtomic32s* atomic)
    {
        return _InterlockedIncrement((volatile long*)atomic); 
    }
    
    kInlineFx(k32s) xkAtomic32s_DecrementImpl(kAtomic32s* atomic)
    {
        return _InterlockedDecrement((volatile long*)atomic);
    }
    
    kInlineFx(k32s) xkAtomic32s_ExchangeImpl(kAtomic32s* atomic, k32s value)
    {
        return _InterlockedExchange((volatile long*)atomic, value);
    }
    
    kInlineFx(kBool) xkAtomic32s_CompareExchangeImpl(kAtomic32s* atomic, k32s oldValue, k32s value)
    {
        return (_InterlockedCompareExchange((volatile long*)atomic, value, oldValue) == oldValue); 
    }
    
    kInlineFx(k32s) xkAtomic32s_GetImpl(kAtomic32s* atomic)
    {
        k32s value; 
    
        do
        {
            value = *atomic; 
        }
        while (!kAtomic32s_CompareExchange(atomic, value, value)); 
    
        return value; 
    }
    
    //c++/cli can't handle interlocked pointer intrinsics
#   if defined(K_CPP_CLI)
#       pragma managed(push, off)
#   endif        

    kInlineFx(kPointer) xkAtomicPointer_ExchangeImpl(kAtomicPointer* atomic, kPointer value)
    {
        return _InterlockedExchangePointer(atomic, value);
    }
    
    kInlineFx(kBool) xkAtomicPointer_CompareExchangeImpl(void* volatile* atomic, kPointer oldValue, kPointer value)
    {
        return (_InterlockedCompareExchangePointer(atomic, value, oldValue) == oldValue); 
    }

#   if defined(K_CPP_CLI)
#       pragma managed(pop)
#   endif        
   
    kInlineFx(kPointer) xkAtomicPointer_GetImpl(kAtomicPointer* atomic)
    {
        kPointer value; 
    
        do
        {
            value = *atomic; 
        }
        while (!kAtomicPointer_CompareExchange(atomic, value, value)); 
    
        return value; 
    }

#elif defined(K_DARWIN)

#   include <libkern/OSAtomic.h>

    kInlineFx(k32s) xkAtomic32s_IncrementImpl(kAtomic32s* atomic)
    {
        return OSAtomicIncrement32Barrier(atomic);
    }
    
    kInlineFx(k32s) xkAtomic32s_DecrementImpl(kAtomic32s* atomic)
    {
        return OSAtomicDecrement32Barrier(atomic);
    }
    
    kInlineFx(k32s) xkAtomic32s_ExchangeImpl(kAtomic32s* atomic, k32s value)
    {
        k32s oldValue; 
        
        do
        {
            oldValue = *atomic;
        }
        while (!OSAtomicCompareAndSwapIntBarrier(oldValue, value, atomic));
    
        return oldValue; 
    }
    
    kInlineFx(kBool) xkAtomic32s_CompareExchangeImpl(kAtomic32s* atomic, k32s oldValue, k32s value)
    {
        return OSAtomicCompareAndSwapIntBarrier(oldValue, value, atomic); 
    }
    
    kInlineFx(k32s) xkAtomic32s_GetImpl(kAtomic32s* atomic)
    {
        k32s value; 
    
        do
        {
            value = *atomic; 
        }
        while (!kAtomic32s_CompareExchange(atomic, value, value)); 
    
        return value; 
    }
    
    kInlineFx(kPointer) xkAtomicPointer_ExchangeImpl(kAtomicPointer* atomic, kPointer value)
    {
        kPointer oldValue; 
        
        do
        {
            oldValue = *atomic;
        }
        while (!OSAtomicCompareAndSwapPtrBarrier(oldValue, value, atomic));
    
        return oldValue; 
    }
    
    kInlineFx(kBool) xkAtomicPointer_CompareExchangeImpl(kAtomicPointer* atomic, kPointer oldValue, kPointer value)
    {
        return OSAtomicCompareAndSwapPtrBarrier(oldValue, value, atomic); 
    }
    
    kInlineFx(kPointer) xkAtomicPointer_GetImpl(kAtomicPointer* atomic)
    {
        kPointer value; 
    
        do
        {
            value = *atomic; 
        }
        while (!kAtomicPointer_CompareExchange(atomic, value, value)); 
    
        return value; 
    }

#elif defined (K_GCC)

    kInlineFx(k32s) xkAtomic32s_IncrementImpl(kAtomic32s* atomic) 
    {
        return __sync_add_and_fetch(atomic, 1); 
    }
    
    kInlineFx(k32s) xkAtomic32s_DecrementImpl(kAtomic32s* atomic)
    {
        return __sync_sub_and_fetch(atomic, 1); 
    }
    
    kInlineFx(k32s) xkAtomic32s_ExchangeImpl(kAtomic32s* atomic, k32s value) 
    {
        k32s oldValue; 
       
        do 
        {
            oldValue = *atomic;
        }
        while (!__sync_bool_compare_and_swap(atomic, oldValue, value));
    
        return oldValue; 
    }
    
    kInlineFx(kBool) xkAtomic32s_CompareExchangeImpl(kAtomic32s* atomic, k32s oldValue, k32s value)
    {
        return __sync_bool_compare_and_swap(atomic, oldValue, value);
    }
    
    kInlineFx(k32s) xkAtomic32s_GetImpl(kAtomic32s* atomic)
    {
        k32s value; 
    
        do
        {
            value = *atomic; 
        }
        while (!kAtomic32s_CompareExchange(atomic, value, value)); 
    
        return value; 
    }
    
    kInlineFx(kPointer) xkAtomicPointer_ExchangeImpl(kAtomicPointer* atomic, kPointer value)
    {
        kPointer oldValue; 
       
        do 
        {
            oldValue = *atomic;
        }
        while (!__sync_bool_compare_and_swap(atomic, oldValue, value));
    
        return oldValue; 
    }
    
    kInlineFx(kBool) xkAtomicPointer_CompareExchangeImpl(kAtomicPointer* atomic, kPointer oldValue, kPointer value)
    {
        return __sync_bool_compare_and_swap(atomic, oldValue, value);
    }
    
    kInlineFx(kPointer) xkAtomicPointer_GetImpl(kAtomicPointer* atomic)
    {
        kPointer value; 
    
        do
        {
            value = *atomic; 
        }
        while (!kAtomicPointer_CompareExchange(atomic, value, value)); 
    
        return value; 
    }

#else

kFx(k32s) xkAtomic32s_IncrementImpl(kAtomic32s* atomic)
{
    kAssert(kFALSE); 
    return 0; 
}

kFx(k32s) xkAtomic32s_DecrementImpl(kAtomic32s* atomic)
{
    kAssert(kFALSE); 
    return 0; 
}

kFx(k32s) xkAtomic32s_ExchangeImpl(kAtomic32s* atomic, k32s value)
{
    kAssert(kFALSE); 
    return 0; 
}

kFx(kBool) xkAtomic32s_CompareExchangeImpl(kAtomic32s* atomic, k32s oldValue, k32s value)
{
    kAssert(kFALSE); 
    return kFALSE; 
}

kFx(k32s) xkAtomic32s_GetImpl(kAtomic32s* atomic)
{
    kAssert(kFALSE); 
    return 0; 
}

kFx(kPointer) xkAtomicPointer_ExchangeImpl(kAtomicPointer* atomic, kPointer value)
{
    kAssert(kFALSE); 
    return kNULL; 
}

kFx(kBool) xkAtomicPointer_CompareExchangeImpl(kAtomicPointer* atomic, kPointer oldValue, kPointer value)
{
    kAssert(kFALSE); 
    return kFALSE; 
}

kFx(kPointer) xkAtomicPointer_GetImpl(kAtomicPointer* atomic)
{
    kAssert(kFALSE); 
    return kNULL; 
}

#endif

#endif
