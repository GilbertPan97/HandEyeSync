
ifeq ($(OS)$(os), Windows_NT)
	XCOMPILE := 1
	OS_PREFIX := C:
	OS_SUFFIX := .exe
	PYTHON := python
	MKDIR_P := $(PYTHON) ../../../Platform/scripts/Utils/kUtil.py mkdir_p
	RM_F := $(PYTHON) ../../../Platform/scripts/Utils/kUtil.py rm_f
	RM_RF := $(PYTHON) ../../../Platform/scripts/Utils/kUtil.py rm_rf
	CP := $(PYTHON) ../../../Platform/scripts/Utils/kUtil.py cp
else
	BUILD_MACHINE := $(shell uname -m)
	ifneq ($(BUILD_MACHINE), i686)
		XCOMPILE := 1
	else
		XCOMPILE := 0
	endif
	PYTHON := python3
	MKDIR_P := mkdir -p
	RM_F := rm -f
	RM_RF := rm -rf
	CP := cp
endif

TARGET_TRIPLET := x86_64-linux-gnu

ifeq ($(XCOMPILE),1)
	GCC_PATH := $(OS_PREFIX)/tools/GccX64_7.5.0-p3/$(TARGET_TRIPLET)
	GCC_SYSROOT := $(GCC_PATH)/$(TARGET_TRIPLET)/libc
	GCC_PREFIX := $(GCC_PATH)/bin/$(TARGET_TRIPLET)-
endif

GNU_C_COMPILER := $(GCC_PREFIX)gcc$(GCC_SLOT_SUFFIX)$(OS_SUFFIX)
GNU_CXX_COMPILER := $(GCC_PREFIX)g++$(GCC_SLOT_SUFFIX)$(OS_SUFFIX)
GNU_LINKER := $(GCC_PREFIX)g++$(GCC_SLOT_SUFFIX)$(OS_SUFFIX)
GNU_ARCHIVER := $(GCC_PREFIX)ar$(OS_SUFFIX)
GNU_READELF := $(GCC_PREFIX)readelf$(OS_SUFFIX)

KAPPGEN := $(PYTHON) ../../../Platform/scripts/Utils/kAppGen.py

ifndef verbose
	SILENT := @
endif

ifndef config
	config := Debug
endif

# We require GCC to be installed according to specific conventions (see manuals).
# Tool prerequisites may change between major releases; check and report.
ifeq ($(shell $(GNU_C_COMPILER) --version),)
.PHONY: gcc_err
gcc_err:
	$(error Cannot build because of missing prerequisite; please install GCC)
endif

ifeq ($(config),Debug)
	optimize := 0
	strip := 0
	wstack := 0
	TARGET := ../../lib/linux_x86d/libkApi.so
	INTERMEDIATES := 
	OBJ_DIR := ../../build/kApi-gnumk_linux_x86-Debug
	PREBUILD := 
	POSTBUILD := 
	COMPILER_FLAGS := -g -m32 -march=i686 -fpic -fvisibility=hidden
	C_FLAGS := -std=gnu99 -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-unused-value -Wno-missing-braces
	CXX_FLAGS := -std=c++11 -Wall
	INCLUDE_DIRS := -I../kApi
	DEFINES := -DK_DEBUG -DK_EMIT -DK_PLUGIN
	LINKER_FLAGS := -m32 -shared -Wl,-no-undefined -Wl,-rpath,'$$ORIGIN' -Wl,--hash-style=gnu
	LIB_DIRS :=
	LIBS := -Wl,--start-group -lc -lpthread -lrt -lm -ldl -Wl,--end-group
	ifneq ($(optimize),0)
		COMPILER_FLAGS += -O$(optimize)
	endif
	ifeq ($(strip),1)
		LINKER_FLAGS += -Wl,--strip-debug
	endif
	ifeq ($(strip),2)
		LINKER_FLAGS += -Wl,--strip-all
	endif
	ifdef profile
		COMPILER_FLAGS += -pg
		LINKER_FLAGS += -pg
	endif
	ifdef coverage
		COMPILER_FLAGS += --coverage -fprofile-arcs -ftest-coverage
		LINKER_FLAGS += --coverage
		LIBS += -lgcov
	endif
	ifdef sanitize
		COMPILER_FLAGS += -fsanitize=$(sanitize)
		LINKER_FLAGS += -fsanitize=$(sanitize)
	endif
	GNU_COMPILER_FLAGS := $(COMPILER_FLAGS) -fno-gnu-unique
	ifneq ($(wstack),0)
		GNU_COMPILER_FLAGS += -Wstack-usage=$(wstack)
	endif
	OBJECTS := ../../build/kApi-gnumk_linux_x86-Debug/kAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kApiDef.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kApiLib.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kAssembly.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kObject.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kType.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kValue.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kCipher.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kBlowfishCipher.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kCipherStream.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kHash.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kSha1Hash.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kArray1.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kArray2.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kArray3.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kArrayList.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kArrayProvider.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kBitArray.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kBox.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kBytes.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kCollection.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kImage.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kList.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kMath.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kMap.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kString.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kQueue.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kXml.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/lodepng.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/lodepng_ex.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/rbtree.c.o \
	../../build/kApi-gnumk_linux_x86-Debug/kDat5Serializer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kDat6Serializer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kDirectory.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kFile.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kHttpServer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kHttpServerChannel.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kHttpServerRequest.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kHttpServerResponse.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kMemory.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kNetwork.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kPath.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kPipeStream.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kSerializer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kStream.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kSocket.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kTcpClient.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kTcpServer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kUdpClient.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kWebSocket.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kAtomic.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kLock.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kMsgQueue.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kParallel.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kPeriodic.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kThread.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kThreadPool.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kTimer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kSemaphore.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kBackTrace.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kDateTime.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kDebugAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kDynamicLib.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kEvent.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kHeapAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kImageUtils.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kObjectPool.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kPlugin.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kPoolAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kProcess.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kSymbolInfo.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kTimeSpan.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kUserAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Debug/kUtils.cpp.o
	DEP_FILES = ../../build/kApi-gnumk_linux_x86-Debug/kAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kApiDef.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kApiLib.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kAssembly.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kObject.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kType.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kValue.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kCipher.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kBlowfishCipher.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kCipherStream.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kHash.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kSha1Hash.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kArray1.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kArray2.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kArray3.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kArrayList.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kArrayProvider.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kBitArray.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kBox.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kBytes.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kCollection.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kImage.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kList.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kMath.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kMap.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kString.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kQueue.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kXml.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/lodepng.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/lodepng_ex.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/rbtree.c.d \
	../../build/kApi-gnumk_linux_x86-Debug/kDat5Serializer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kDat6Serializer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kDirectory.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kFile.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kHttpServer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kHttpServerChannel.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kHttpServerRequest.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kHttpServerResponse.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kMemory.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kNetwork.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kPath.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kPipeStream.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kSerializer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kStream.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kSocket.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kTcpClient.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kTcpServer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kUdpClient.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kWebSocket.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kAtomic.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kLock.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kMsgQueue.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kParallel.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kPeriodic.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kThread.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kThreadPool.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kTimer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kSemaphore.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kBackTrace.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kDateTime.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kDebugAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kDynamicLib.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kEvent.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kHeapAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kImageUtils.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kObjectPool.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kPlugin.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kPoolAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kProcess.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kSymbolInfo.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kTimeSpan.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kUserAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Debug/kUtils.cpp.d
	TARGET_DEPS = 

endif

ifeq ($(config),Release)
	optimize := 2
	strip := 1
	wstack := 0
	TARGET := ../../lib/linux_x86/libkApi.so
	INTERMEDIATES := 
	OBJ_DIR := ../../build/kApi-gnumk_linux_x86-Release
	PREBUILD := 
	POSTBUILD := 
	COMPILER_FLAGS := -m32 -march=i686 -fpic -fvisibility=hidden
	C_FLAGS := -std=gnu99 -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-unused-value -Wno-missing-braces
	CXX_FLAGS := -std=c++11 -Wall
	INCLUDE_DIRS := -I../kApi
	DEFINES := -DK_EMIT -DK_PLUGIN
	LINKER_FLAGS := -m32 -shared -Wl,-no-undefined -Wl,-rpath,'$$ORIGIN' -Wl,-O1 -Wl,--hash-style=gnu
	LIB_DIRS :=
	LIBS := -Wl,--start-group -lc -lpthread -lrt -lm -ldl -Wl,--end-group
	ifneq ($(optimize),0)
		COMPILER_FLAGS += -O$(optimize)
	endif
	ifeq ($(strip),1)
		LINKER_FLAGS += -Wl,--strip-debug
	endif
	ifeq ($(strip),2)
		LINKER_FLAGS += -Wl,--strip-all
	endif
	ifdef profile
		COMPILER_FLAGS += -pg
		LINKER_FLAGS += -pg
	endif
	ifdef coverage
		COMPILER_FLAGS += --coverage -fprofile-arcs -ftest-coverage
		LINKER_FLAGS += --coverage
		LIBS += -lgcov
	endif
	ifdef sanitize
		COMPILER_FLAGS += -fsanitize=$(sanitize)
		LINKER_FLAGS += -fsanitize=$(sanitize)
	endif
	GNU_COMPILER_FLAGS := $(COMPILER_FLAGS) -fno-gnu-unique
	ifneq ($(wstack),0)
		GNU_COMPILER_FLAGS += -Wstack-usage=$(wstack)
	endif
	OBJECTS := ../../build/kApi-gnumk_linux_x86-Release/kAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kApiDef.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kApiLib.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kAssembly.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kObject.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kType.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kValue.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kCipher.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kBlowfishCipher.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kCipherStream.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kHash.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kSha1Hash.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kArray1.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kArray2.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kArray3.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kArrayList.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kArrayProvider.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kBitArray.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kBox.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kBytes.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kCollection.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kImage.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kList.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kMath.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kMap.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kString.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kQueue.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kXml.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/lodepng.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/lodepng_ex.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/rbtree.c.o \
	../../build/kApi-gnumk_linux_x86-Release/kDat5Serializer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kDat6Serializer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kDirectory.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kFile.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kHttpServer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kHttpServerChannel.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kHttpServerRequest.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kHttpServerResponse.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kMemory.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kNetwork.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kPath.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kPipeStream.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kSerializer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kStream.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kSocket.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kTcpClient.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kTcpServer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kUdpClient.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kWebSocket.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kAtomic.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kLock.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kMsgQueue.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kParallel.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kPeriodic.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kThread.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kThreadPool.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kTimer.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kSemaphore.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kBackTrace.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kDateTime.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kDebugAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kDynamicLib.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kEvent.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kHeapAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kImageUtils.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kObjectPool.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kPlugin.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kPoolAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kProcess.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kSymbolInfo.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kTimeSpan.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kUserAlloc.cpp.o \
	../../build/kApi-gnumk_linux_x86-Release/kUtils.cpp.o
	DEP_FILES = ../../build/kApi-gnumk_linux_x86-Release/kAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kApiDef.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kApiLib.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kAssembly.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kObject.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kType.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kValue.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kCipher.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kBlowfishCipher.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kCipherStream.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kHash.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kSha1Hash.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kArray1.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kArray2.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kArray3.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kArrayList.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kArrayProvider.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kBitArray.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kBox.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kBytes.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kCollection.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kImage.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kList.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kMath.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kMap.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kString.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kQueue.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kXml.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/lodepng.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/lodepng_ex.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/rbtree.c.d \
	../../build/kApi-gnumk_linux_x86-Release/kDat5Serializer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kDat6Serializer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kDirectory.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kFile.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kHttpServer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kHttpServerChannel.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kHttpServerRequest.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kHttpServerResponse.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kMemory.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kNetwork.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kPath.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kPipeStream.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kSerializer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kStream.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kSocket.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kTcpClient.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kTcpServer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kUdpClient.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kWebSocket.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kAtomic.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kLock.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kMsgQueue.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kParallel.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kPeriodic.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kThread.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kThreadPool.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kTimer.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kSemaphore.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kBackTrace.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kDateTime.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kDebugAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kDynamicLib.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kEvent.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kHeapAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kImageUtils.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kObjectPool.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kPlugin.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kPoolAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kProcess.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kSymbolInfo.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kTimeSpan.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kUserAlloc.cpp.d \
	../../build/kApi-gnumk_linux_x86-Release/kUtils.cpp.d
	TARGET_DEPS = 

endif

.PHONY: all all-obj all-dep clean

all: $(OBJ_DIR)
	$(PREBUILD)
	$(SILENT) $(MAKE) -f kApi-Linux_X86.mk all-dep
	$(SILENT) $(MAKE) -f kApi-Linux_X86.mk all-obj

clean:
	$(SILENT) $(info Cleaning $(OBJ_DIR))
	$(SILENT) $(RM_RF) $(OBJ_DIR)
	$(SILENT) $(info Cleaning $(TARGET) $(INTERMEDIATES))
	$(SILENT) $(RM_F) $(TARGET) $(INTERMEDIATES)

all-obj: $(OBJ_DIR) $(TARGET)
all-dep: $(OBJ_DIR) $(DEP_FILES)

$(OBJ_DIR):
	$(SILENT) $(MKDIR_P) $@

ifeq ($(config),Debug)

$(TARGET): $(OBJECTS) $(TARGET_DEPS)
	$(SILENT) $(info LdX86 $(TARGET))
	$(SILENT) $(GNU_LINKER) $(OBJECTS) $(LINKER_FLAGS) $(LIBS) $(LIB_DIRS) -o$(TARGET)

endif

ifeq ($(config),Release)

$(TARGET): $(OBJECTS) $(TARGET_DEPS)
	$(SILENT) $(info LdX86 $(TARGET))
	$(SILENT) $(GNU_LINKER) $(OBJECTS) $(LINKER_FLAGS) $(LIBS) $(LIB_DIRS) -o$(TARGET)

endif

ifeq ($(config),Debug)

../../build/kApi-gnumk_linux_x86-Debug/kAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kAlloc.cpp.d: kApi/kAlloc.cpp
	$(SILENT) $(info GccX86 kApi/kAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kAlloc.cpp.o -c kApi/kAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kApiDef.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kApiDef.cpp.d: kApi/kApiDef.cpp
	$(SILENT) $(info GccX86 kApi/kApiDef.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kApiDef.cpp.o -c kApi/kApiDef.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kApiLib.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kApiLib.cpp.d: kApi/kApiLib.cpp
	$(SILENT) $(info GccX86 kApi/kApiLib.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kApiLib.cpp.o -c kApi/kApiLib.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kAssembly.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kAssembly.cpp.d: kApi/kAssembly.cpp
	$(SILENT) $(info GccX86 kApi/kAssembly.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kAssembly.cpp.o -c kApi/kAssembly.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kObject.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kObject.cpp.d: kApi/kObject.cpp
	$(SILENT) $(info GccX86 kApi/kObject.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kObject.cpp.o -c kApi/kObject.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kType.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kType.cpp.d: kApi/kType.cpp
	$(SILENT) $(info GccX86 kApi/kType.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kType.cpp.o -c kApi/kType.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kValue.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kValue.cpp.d: kApi/kValue.cpp
	$(SILENT) $(info GccX86 kApi/kValue.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kValue.cpp.o -c kApi/kValue.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kCipher.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kCipher.cpp.d: kApi/Crypto/kCipher.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kCipher.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kCipher.cpp.o -c kApi/Crypto/kCipher.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kBlowfishCipher.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kBlowfishCipher.cpp.d: kApi/Crypto/kBlowfishCipher.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kBlowfishCipher.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kBlowfishCipher.cpp.o -c kApi/Crypto/kBlowfishCipher.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kCipherStream.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kCipherStream.cpp.d: kApi/Crypto/kCipherStream.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kCipherStream.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kCipherStream.cpp.o -c kApi/Crypto/kCipherStream.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kHash.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kHash.cpp.d: kApi/Crypto/kHash.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kHash.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kHash.cpp.o -c kApi/Crypto/kHash.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kSha1Hash.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kSha1Hash.cpp.d: kApi/Crypto/kSha1Hash.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kSha1Hash.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kSha1Hash.cpp.o -c kApi/Crypto/kSha1Hash.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kArray1.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kArray1.cpp.d: kApi/Data/kArray1.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArray1.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kArray1.cpp.o -c kApi/Data/kArray1.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kArray2.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kArray2.cpp.d: kApi/Data/kArray2.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArray2.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kArray2.cpp.o -c kApi/Data/kArray2.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kArray3.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kArray3.cpp.d: kApi/Data/kArray3.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArray3.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kArray3.cpp.o -c kApi/Data/kArray3.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kArrayList.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kArrayList.cpp.d: kApi/Data/kArrayList.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArrayList.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kArrayList.cpp.o -c kApi/Data/kArrayList.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kArrayProvider.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kArrayProvider.cpp.d: kApi/Data/kArrayProvider.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArrayProvider.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kArrayProvider.cpp.o -c kApi/Data/kArrayProvider.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kBitArray.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kBitArray.cpp.d: kApi/Data/kBitArray.cpp
	$(SILENT) $(info GccX86 kApi/Data/kBitArray.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kBitArray.cpp.o -c kApi/Data/kBitArray.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kBox.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kBox.cpp.d: kApi/Data/kBox.cpp
	$(SILENT) $(info GccX86 kApi/Data/kBox.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kBox.cpp.o -c kApi/Data/kBox.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kBytes.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kBytes.cpp.d: kApi/Data/kBytes.cpp
	$(SILENT) $(info GccX86 kApi/Data/kBytes.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kBytes.cpp.o -c kApi/Data/kBytes.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kCollection.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kCollection.cpp.d: kApi/Data/kCollection.cpp
	$(SILENT) $(info GccX86 kApi/Data/kCollection.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kCollection.cpp.o -c kApi/Data/kCollection.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kImage.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kImage.cpp.d: kApi/Data/kImage.cpp
	$(SILENT) $(info GccX86 kApi/Data/kImage.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kImage.cpp.o -c kApi/Data/kImage.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kList.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kList.cpp.d: kApi/Data/kList.cpp
	$(SILENT) $(info GccX86 kApi/Data/kList.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kList.cpp.o -c kApi/Data/kList.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kMath.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kMath.cpp.d: kApi/Data/kMath.cpp
	$(SILENT) $(info GccX86 kApi/Data/kMath.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kMath.cpp.o -c kApi/Data/kMath.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kMap.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kMap.cpp.d: kApi/Data/kMap.cpp
	$(SILENT) $(info GccX86 kApi/Data/kMap.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kMap.cpp.o -c kApi/Data/kMap.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kString.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kString.cpp.d: kApi/Data/kString.cpp
	$(SILENT) $(info GccX86 kApi/Data/kString.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kString.cpp.o -c kApi/Data/kString.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kQueue.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kQueue.cpp.d: kApi/Data/kQueue.cpp
	$(SILENT) $(info GccX86 kApi/Data/kQueue.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kQueue.cpp.o -c kApi/Data/kQueue.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kXml.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kXml.cpp.d: kApi/Data/kXml.cpp
	$(SILENT) $(info GccX86 kApi/Data/kXml.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kXml.cpp.o -c kApi/Data/kXml.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/lodepng.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/lodepng.cpp.d: kApi.extern/lodepng/lodepng.cpp
	$(SILENT) $(info GccX86 kApi.extern/lodepng/lodepng.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/lodepng.cpp.o -c kApi.extern/lodepng/lodepng.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/lodepng_ex.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/lodepng_ex.cpp.d: kApi.extern/lodepng/lodepng_ex.cpp
	$(SILENT) $(info GccX86 kApi.extern/lodepng/lodepng_ex.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/lodepng_ex.cpp.o -c kApi.extern/lodepng/lodepng_ex.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/rbtree.c.o ../../build/kApi-gnumk_linux_x86-Debug/rbtree.c.d: kApi.extern/rbtree/rbtree.c
	$(SILENT) $(info GccX86 kApi.extern/rbtree/rbtree.c)
	$(SILENT) $(GNU_C_COMPILER) $(GNU_COMPILER_FLAGS) $(C_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/rbtree.c.o -c kApi.extern/rbtree/rbtree.c -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kDat5Serializer.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kDat5Serializer.cpp.d: kApi/Io/kDat5Serializer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kDat5Serializer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kDat5Serializer.cpp.o -c kApi/Io/kDat5Serializer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kDat6Serializer.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kDat6Serializer.cpp.d: kApi/Io/kDat6Serializer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kDat6Serializer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kDat6Serializer.cpp.o -c kApi/Io/kDat6Serializer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kDirectory.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kDirectory.cpp.d: kApi/Io/kDirectory.cpp
	$(SILENT) $(info GccX86 kApi/Io/kDirectory.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kDirectory.cpp.o -c kApi/Io/kDirectory.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kFile.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kFile.cpp.d: kApi/Io/kFile.cpp
	$(SILENT) $(info GccX86 kApi/Io/kFile.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kFile.cpp.o -c kApi/Io/kFile.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kHttpServer.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kHttpServer.cpp.d: kApi/Io/kHttpServer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kHttpServer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kHttpServer.cpp.o -c kApi/Io/kHttpServer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kHttpServerChannel.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerChannel.cpp.d: kApi/Io/kHttpServerChannel.cpp
	$(SILENT) $(info GccX86 kApi/Io/kHttpServerChannel.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerChannel.cpp.o -c kApi/Io/kHttpServerChannel.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kHttpServerRequest.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerRequest.cpp.d: kApi/Io/kHttpServerRequest.cpp
	$(SILENT) $(info GccX86 kApi/Io/kHttpServerRequest.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerRequest.cpp.o -c kApi/Io/kHttpServerRequest.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kHttpServerResponse.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerResponse.cpp.d: kApi/Io/kHttpServerResponse.cpp
	$(SILENT) $(info GccX86 kApi/Io/kHttpServerResponse.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerResponse.cpp.o -c kApi/Io/kHttpServerResponse.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kMemory.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kMemory.cpp.d: kApi/Io/kMemory.cpp
	$(SILENT) $(info GccX86 kApi/Io/kMemory.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kMemory.cpp.o -c kApi/Io/kMemory.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kNetwork.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kNetwork.cpp.d: kApi/Io/kNetwork.cpp
	$(SILENT) $(info GccX86 kApi/Io/kNetwork.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kNetwork.cpp.o -c kApi/Io/kNetwork.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kPath.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kPath.cpp.d: kApi/Io/kPath.cpp
	$(SILENT) $(info GccX86 kApi/Io/kPath.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kPath.cpp.o -c kApi/Io/kPath.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kPipeStream.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kPipeStream.cpp.d: kApi/Io/kPipeStream.cpp
	$(SILENT) $(info GccX86 kApi/Io/kPipeStream.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kPipeStream.cpp.o -c kApi/Io/kPipeStream.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kSerializer.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kSerializer.cpp.d: kApi/Io/kSerializer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kSerializer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kSerializer.cpp.o -c kApi/Io/kSerializer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kStream.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kStream.cpp.d: kApi/Io/kStream.cpp
	$(SILENT) $(info GccX86 kApi/Io/kStream.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kStream.cpp.o -c kApi/Io/kStream.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kSocket.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kSocket.cpp.d: kApi/Io/kSocket.cpp
	$(SILENT) $(info GccX86 kApi/Io/kSocket.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kSocket.cpp.o -c kApi/Io/kSocket.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kTcpClient.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kTcpClient.cpp.d: kApi/Io/kTcpClient.cpp
	$(SILENT) $(info GccX86 kApi/Io/kTcpClient.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kTcpClient.cpp.o -c kApi/Io/kTcpClient.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kTcpServer.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kTcpServer.cpp.d: kApi/Io/kTcpServer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kTcpServer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kTcpServer.cpp.o -c kApi/Io/kTcpServer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kUdpClient.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kUdpClient.cpp.d: kApi/Io/kUdpClient.cpp
	$(SILENT) $(info GccX86 kApi/Io/kUdpClient.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kUdpClient.cpp.o -c kApi/Io/kUdpClient.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kWebSocket.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kWebSocket.cpp.d: kApi/Io/kWebSocket.cpp
	$(SILENT) $(info GccX86 kApi/Io/kWebSocket.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kWebSocket.cpp.o -c kApi/Io/kWebSocket.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kAtomic.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kAtomic.cpp.d: kApi/Threads/kAtomic.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kAtomic.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kAtomic.cpp.o -c kApi/Threads/kAtomic.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kLock.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kLock.cpp.d: kApi/Threads/kLock.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kLock.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kLock.cpp.o -c kApi/Threads/kLock.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kMsgQueue.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kMsgQueue.cpp.d: kApi/Threads/kMsgQueue.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kMsgQueue.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kMsgQueue.cpp.o -c kApi/Threads/kMsgQueue.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kParallel.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kParallel.cpp.d: kApi/Threads/kParallel.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kParallel.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kParallel.cpp.o -c kApi/Threads/kParallel.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kPeriodic.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kPeriodic.cpp.d: kApi/Threads/kPeriodic.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kPeriodic.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kPeriodic.cpp.o -c kApi/Threads/kPeriodic.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kThread.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kThread.cpp.d: kApi/Threads/kThread.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kThread.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kThread.cpp.o -c kApi/Threads/kThread.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kThreadPool.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kThreadPool.cpp.d: kApi/Threads/kThreadPool.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kThreadPool.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kThreadPool.cpp.o -c kApi/Threads/kThreadPool.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kTimer.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kTimer.cpp.d: kApi/Threads/kTimer.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kTimer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kTimer.cpp.o -c kApi/Threads/kTimer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kSemaphore.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kSemaphore.cpp.d: kApi/Threads/kSemaphore.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kSemaphore.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kSemaphore.cpp.o -c kApi/Threads/kSemaphore.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kBackTrace.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kBackTrace.cpp.d: kApi/Utils/kBackTrace.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kBackTrace.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kBackTrace.cpp.o -c kApi/Utils/kBackTrace.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kDateTime.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kDateTime.cpp.d: kApi/Utils/kDateTime.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kDateTime.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kDateTime.cpp.o -c kApi/Utils/kDateTime.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kDebugAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kDebugAlloc.cpp.d: kApi/Utils/kDebugAlloc.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kDebugAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kDebugAlloc.cpp.o -c kApi/Utils/kDebugAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kDynamicLib.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kDynamicLib.cpp.d: kApi/Utils/kDynamicLib.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kDynamicLib.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kDynamicLib.cpp.o -c kApi/Utils/kDynamicLib.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kEvent.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kEvent.cpp.d: kApi/Utils/kEvent.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kEvent.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kEvent.cpp.o -c kApi/Utils/kEvent.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kHeapAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kHeapAlloc.cpp.d: kApi/Utils/kHeapAlloc.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kHeapAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kHeapAlloc.cpp.o -c kApi/Utils/kHeapAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kImageUtils.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kImageUtils.cpp.d: kApi/Utils/kImageUtils.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kImageUtils.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kImageUtils.cpp.o -c kApi/Utils/kImageUtils.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kObjectPool.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kObjectPool.cpp.d: kApi/Utils/kObjectPool.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kObjectPool.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kObjectPool.cpp.o -c kApi/Utils/kObjectPool.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kPlugin.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kPlugin.cpp.d: kApi/Utils/kPlugin.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kPlugin.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kPlugin.cpp.o -c kApi/Utils/kPlugin.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kPoolAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kPoolAlloc.cpp.d: kApi/Utils/kPoolAlloc.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kPoolAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kPoolAlloc.cpp.o -c kApi/Utils/kPoolAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kProcess.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kProcess.cpp.d: kApi/Utils/kProcess.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kProcess.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kProcess.cpp.o -c kApi/Utils/kProcess.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kSymbolInfo.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kSymbolInfo.cpp.d: kApi/Utils/kSymbolInfo.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kSymbolInfo.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kSymbolInfo.cpp.o -c kApi/Utils/kSymbolInfo.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kTimeSpan.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kTimeSpan.cpp.d: kApi/Utils/kTimeSpan.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kTimeSpan.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kTimeSpan.cpp.o -c kApi/Utils/kTimeSpan.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kUserAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kUserAlloc.cpp.d: kApi/Utils/kUserAlloc.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kUserAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kUserAlloc.cpp.o -c kApi/Utils/kUserAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Debug/kUtils.cpp.o ../../build/kApi-gnumk_linux_x86-Debug/kUtils.cpp.d: kApi/Utils/kUtils.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kUtils.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Debug/kUtils.cpp.o -c kApi/Utils/kUtils.cpp -MMD -MP

endif

ifeq ($(config),Release)

../../build/kApi-gnumk_linux_x86-Release/kAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kAlloc.cpp.d: kApi/kAlloc.cpp
	$(SILENT) $(info GccX86 kApi/kAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kAlloc.cpp.o -c kApi/kAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kApiDef.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kApiDef.cpp.d: kApi/kApiDef.cpp
	$(SILENT) $(info GccX86 kApi/kApiDef.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kApiDef.cpp.o -c kApi/kApiDef.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kApiLib.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kApiLib.cpp.d: kApi/kApiLib.cpp
	$(SILENT) $(info GccX86 kApi/kApiLib.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kApiLib.cpp.o -c kApi/kApiLib.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kAssembly.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kAssembly.cpp.d: kApi/kAssembly.cpp
	$(SILENT) $(info GccX86 kApi/kAssembly.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kAssembly.cpp.o -c kApi/kAssembly.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kObject.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kObject.cpp.d: kApi/kObject.cpp
	$(SILENT) $(info GccX86 kApi/kObject.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kObject.cpp.o -c kApi/kObject.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kType.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kType.cpp.d: kApi/kType.cpp
	$(SILENT) $(info GccX86 kApi/kType.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kType.cpp.o -c kApi/kType.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kValue.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kValue.cpp.d: kApi/kValue.cpp
	$(SILENT) $(info GccX86 kApi/kValue.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kValue.cpp.o -c kApi/kValue.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kCipher.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kCipher.cpp.d: kApi/Crypto/kCipher.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kCipher.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kCipher.cpp.o -c kApi/Crypto/kCipher.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kBlowfishCipher.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kBlowfishCipher.cpp.d: kApi/Crypto/kBlowfishCipher.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kBlowfishCipher.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kBlowfishCipher.cpp.o -c kApi/Crypto/kBlowfishCipher.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kCipherStream.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kCipherStream.cpp.d: kApi/Crypto/kCipherStream.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kCipherStream.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kCipherStream.cpp.o -c kApi/Crypto/kCipherStream.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kHash.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kHash.cpp.d: kApi/Crypto/kHash.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kHash.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kHash.cpp.o -c kApi/Crypto/kHash.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kSha1Hash.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kSha1Hash.cpp.d: kApi/Crypto/kSha1Hash.cpp
	$(SILENT) $(info GccX86 kApi/Crypto/kSha1Hash.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kSha1Hash.cpp.o -c kApi/Crypto/kSha1Hash.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kArray1.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kArray1.cpp.d: kApi/Data/kArray1.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArray1.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kArray1.cpp.o -c kApi/Data/kArray1.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kArray2.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kArray2.cpp.d: kApi/Data/kArray2.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArray2.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kArray2.cpp.o -c kApi/Data/kArray2.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kArray3.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kArray3.cpp.d: kApi/Data/kArray3.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArray3.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kArray3.cpp.o -c kApi/Data/kArray3.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kArrayList.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kArrayList.cpp.d: kApi/Data/kArrayList.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArrayList.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kArrayList.cpp.o -c kApi/Data/kArrayList.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kArrayProvider.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kArrayProvider.cpp.d: kApi/Data/kArrayProvider.cpp
	$(SILENT) $(info GccX86 kApi/Data/kArrayProvider.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kArrayProvider.cpp.o -c kApi/Data/kArrayProvider.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kBitArray.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kBitArray.cpp.d: kApi/Data/kBitArray.cpp
	$(SILENT) $(info GccX86 kApi/Data/kBitArray.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kBitArray.cpp.o -c kApi/Data/kBitArray.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kBox.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kBox.cpp.d: kApi/Data/kBox.cpp
	$(SILENT) $(info GccX86 kApi/Data/kBox.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kBox.cpp.o -c kApi/Data/kBox.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kBytes.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kBytes.cpp.d: kApi/Data/kBytes.cpp
	$(SILENT) $(info GccX86 kApi/Data/kBytes.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kBytes.cpp.o -c kApi/Data/kBytes.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kCollection.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kCollection.cpp.d: kApi/Data/kCollection.cpp
	$(SILENT) $(info GccX86 kApi/Data/kCollection.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kCollection.cpp.o -c kApi/Data/kCollection.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kImage.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kImage.cpp.d: kApi/Data/kImage.cpp
	$(SILENT) $(info GccX86 kApi/Data/kImage.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kImage.cpp.o -c kApi/Data/kImage.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kList.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kList.cpp.d: kApi/Data/kList.cpp
	$(SILENT) $(info GccX86 kApi/Data/kList.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kList.cpp.o -c kApi/Data/kList.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kMath.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kMath.cpp.d: kApi/Data/kMath.cpp
	$(SILENT) $(info GccX86 kApi/Data/kMath.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kMath.cpp.o -c kApi/Data/kMath.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kMap.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kMap.cpp.d: kApi/Data/kMap.cpp
	$(SILENT) $(info GccX86 kApi/Data/kMap.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kMap.cpp.o -c kApi/Data/kMap.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kString.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kString.cpp.d: kApi/Data/kString.cpp
	$(SILENT) $(info GccX86 kApi/Data/kString.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kString.cpp.o -c kApi/Data/kString.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kQueue.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kQueue.cpp.d: kApi/Data/kQueue.cpp
	$(SILENT) $(info GccX86 kApi/Data/kQueue.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kQueue.cpp.o -c kApi/Data/kQueue.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kXml.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kXml.cpp.d: kApi/Data/kXml.cpp
	$(SILENT) $(info GccX86 kApi/Data/kXml.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kXml.cpp.o -c kApi/Data/kXml.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/lodepng.cpp.o ../../build/kApi-gnumk_linux_x86-Release/lodepng.cpp.d: kApi.extern/lodepng/lodepng.cpp
	$(SILENT) $(info GccX86 kApi.extern/lodepng/lodepng.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/lodepng.cpp.o -c kApi.extern/lodepng/lodepng.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/lodepng_ex.cpp.o ../../build/kApi-gnumk_linux_x86-Release/lodepng_ex.cpp.d: kApi.extern/lodepng/lodepng_ex.cpp
	$(SILENT) $(info GccX86 kApi.extern/lodepng/lodepng_ex.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/lodepng_ex.cpp.o -c kApi.extern/lodepng/lodepng_ex.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/rbtree.c.o ../../build/kApi-gnumk_linux_x86-Release/rbtree.c.d: kApi.extern/rbtree/rbtree.c
	$(SILENT) $(info GccX86 kApi.extern/rbtree/rbtree.c)
	$(SILENT) $(GNU_C_COMPILER) $(GNU_COMPILER_FLAGS) $(C_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/rbtree.c.o -c kApi.extern/rbtree/rbtree.c -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kDat5Serializer.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kDat5Serializer.cpp.d: kApi/Io/kDat5Serializer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kDat5Serializer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kDat5Serializer.cpp.o -c kApi/Io/kDat5Serializer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kDat6Serializer.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kDat6Serializer.cpp.d: kApi/Io/kDat6Serializer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kDat6Serializer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kDat6Serializer.cpp.o -c kApi/Io/kDat6Serializer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kDirectory.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kDirectory.cpp.d: kApi/Io/kDirectory.cpp
	$(SILENT) $(info GccX86 kApi/Io/kDirectory.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kDirectory.cpp.o -c kApi/Io/kDirectory.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kFile.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kFile.cpp.d: kApi/Io/kFile.cpp
	$(SILENT) $(info GccX86 kApi/Io/kFile.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kFile.cpp.o -c kApi/Io/kFile.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kHttpServer.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kHttpServer.cpp.d: kApi/Io/kHttpServer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kHttpServer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kHttpServer.cpp.o -c kApi/Io/kHttpServer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kHttpServerChannel.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kHttpServerChannel.cpp.d: kApi/Io/kHttpServerChannel.cpp
	$(SILENT) $(info GccX86 kApi/Io/kHttpServerChannel.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kHttpServerChannel.cpp.o -c kApi/Io/kHttpServerChannel.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kHttpServerRequest.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kHttpServerRequest.cpp.d: kApi/Io/kHttpServerRequest.cpp
	$(SILENT) $(info GccX86 kApi/Io/kHttpServerRequest.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kHttpServerRequest.cpp.o -c kApi/Io/kHttpServerRequest.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kHttpServerResponse.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kHttpServerResponse.cpp.d: kApi/Io/kHttpServerResponse.cpp
	$(SILENT) $(info GccX86 kApi/Io/kHttpServerResponse.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kHttpServerResponse.cpp.o -c kApi/Io/kHttpServerResponse.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kMemory.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kMemory.cpp.d: kApi/Io/kMemory.cpp
	$(SILENT) $(info GccX86 kApi/Io/kMemory.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kMemory.cpp.o -c kApi/Io/kMemory.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kNetwork.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kNetwork.cpp.d: kApi/Io/kNetwork.cpp
	$(SILENT) $(info GccX86 kApi/Io/kNetwork.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kNetwork.cpp.o -c kApi/Io/kNetwork.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kPath.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kPath.cpp.d: kApi/Io/kPath.cpp
	$(SILENT) $(info GccX86 kApi/Io/kPath.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kPath.cpp.o -c kApi/Io/kPath.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kPipeStream.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kPipeStream.cpp.d: kApi/Io/kPipeStream.cpp
	$(SILENT) $(info GccX86 kApi/Io/kPipeStream.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kPipeStream.cpp.o -c kApi/Io/kPipeStream.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kSerializer.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kSerializer.cpp.d: kApi/Io/kSerializer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kSerializer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kSerializer.cpp.o -c kApi/Io/kSerializer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kStream.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kStream.cpp.d: kApi/Io/kStream.cpp
	$(SILENT) $(info GccX86 kApi/Io/kStream.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kStream.cpp.o -c kApi/Io/kStream.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kSocket.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kSocket.cpp.d: kApi/Io/kSocket.cpp
	$(SILENT) $(info GccX86 kApi/Io/kSocket.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kSocket.cpp.o -c kApi/Io/kSocket.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kTcpClient.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kTcpClient.cpp.d: kApi/Io/kTcpClient.cpp
	$(SILENT) $(info GccX86 kApi/Io/kTcpClient.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kTcpClient.cpp.o -c kApi/Io/kTcpClient.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kTcpServer.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kTcpServer.cpp.d: kApi/Io/kTcpServer.cpp
	$(SILENT) $(info GccX86 kApi/Io/kTcpServer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kTcpServer.cpp.o -c kApi/Io/kTcpServer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kUdpClient.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kUdpClient.cpp.d: kApi/Io/kUdpClient.cpp
	$(SILENT) $(info GccX86 kApi/Io/kUdpClient.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kUdpClient.cpp.o -c kApi/Io/kUdpClient.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kWebSocket.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kWebSocket.cpp.d: kApi/Io/kWebSocket.cpp
	$(SILENT) $(info GccX86 kApi/Io/kWebSocket.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kWebSocket.cpp.o -c kApi/Io/kWebSocket.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kAtomic.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kAtomic.cpp.d: kApi/Threads/kAtomic.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kAtomic.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kAtomic.cpp.o -c kApi/Threads/kAtomic.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kLock.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kLock.cpp.d: kApi/Threads/kLock.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kLock.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kLock.cpp.o -c kApi/Threads/kLock.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kMsgQueue.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kMsgQueue.cpp.d: kApi/Threads/kMsgQueue.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kMsgQueue.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kMsgQueue.cpp.o -c kApi/Threads/kMsgQueue.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kParallel.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kParallel.cpp.d: kApi/Threads/kParallel.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kParallel.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kParallel.cpp.o -c kApi/Threads/kParallel.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kPeriodic.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kPeriodic.cpp.d: kApi/Threads/kPeriodic.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kPeriodic.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kPeriodic.cpp.o -c kApi/Threads/kPeriodic.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kThread.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kThread.cpp.d: kApi/Threads/kThread.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kThread.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kThread.cpp.o -c kApi/Threads/kThread.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kThreadPool.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kThreadPool.cpp.d: kApi/Threads/kThreadPool.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kThreadPool.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kThreadPool.cpp.o -c kApi/Threads/kThreadPool.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kTimer.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kTimer.cpp.d: kApi/Threads/kTimer.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kTimer.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kTimer.cpp.o -c kApi/Threads/kTimer.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kSemaphore.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kSemaphore.cpp.d: kApi/Threads/kSemaphore.cpp
	$(SILENT) $(info GccX86 kApi/Threads/kSemaphore.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kSemaphore.cpp.o -c kApi/Threads/kSemaphore.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kBackTrace.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kBackTrace.cpp.d: kApi/Utils/kBackTrace.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kBackTrace.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kBackTrace.cpp.o -c kApi/Utils/kBackTrace.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kDateTime.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kDateTime.cpp.d: kApi/Utils/kDateTime.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kDateTime.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kDateTime.cpp.o -c kApi/Utils/kDateTime.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kDebugAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kDebugAlloc.cpp.d: kApi/Utils/kDebugAlloc.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kDebugAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kDebugAlloc.cpp.o -c kApi/Utils/kDebugAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kDynamicLib.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kDynamicLib.cpp.d: kApi/Utils/kDynamicLib.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kDynamicLib.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kDynamicLib.cpp.o -c kApi/Utils/kDynamicLib.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kEvent.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kEvent.cpp.d: kApi/Utils/kEvent.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kEvent.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kEvent.cpp.o -c kApi/Utils/kEvent.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kHeapAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kHeapAlloc.cpp.d: kApi/Utils/kHeapAlloc.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kHeapAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kHeapAlloc.cpp.o -c kApi/Utils/kHeapAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kImageUtils.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kImageUtils.cpp.d: kApi/Utils/kImageUtils.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kImageUtils.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kImageUtils.cpp.o -c kApi/Utils/kImageUtils.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kObjectPool.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kObjectPool.cpp.d: kApi/Utils/kObjectPool.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kObjectPool.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kObjectPool.cpp.o -c kApi/Utils/kObjectPool.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kPlugin.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kPlugin.cpp.d: kApi/Utils/kPlugin.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kPlugin.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kPlugin.cpp.o -c kApi/Utils/kPlugin.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kPoolAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kPoolAlloc.cpp.d: kApi/Utils/kPoolAlloc.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kPoolAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kPoolAlloc.cpp.o -c kApi/Utils/kPoolAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kProcess.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kProcess.cpp.d: kApi/Utils/kProcess.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kProcess.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kProcess.cpp.o -c kApi/Utils/kProcess.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kSymbolInfo.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kSymbolInfo.cpp.d: kApi/Utils/kSymbolInfo.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kSymbolInfo.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kSymbolInfo.cpp.o -c kApi/Utils/kSymbolInfo.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kTimeSpan.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kTimeSpan.cpp.d: kApi/Utils/kTimeSpan.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kTimeSpan.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kTimeSpan.cpp.o -c kApi/Utils/kTimeSpan.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kUserAlloc.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kUserAlloc.cpp.d: kApi/Utils/kUserAlloc.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kUserAlloc.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kUserAlloc.cpp.o -c kApi/Utils/kUserAlloc.cpp -MMD -MP

../../build/kApi-gnumk_linux_x86-Release/kUtils.cpp.o ../../build/kApi-gnumk_linux_x86-Release/kUtils.cpp.d: kApi/Utils/kUtils.cpp
	$(SILENT) $(info GccX86 kApi/Utils/kUtils.cpp)
	$(SILENT) $(GNU_CXX_COMPILER) $(GNU_COMPILER_FLAGS) $(CXX_FLAGS) $(DEFINES) $(INCLUDE_DIRS) -o ../../build/kApi-gnumk_linux_x86-Release/kUtils.cpp.o -c kApi/Utils/kUtils.cpp -MMD -MP

endif

ifeq ($(MAKECMDGOALS),all-obj)

ifeq ($(config),Debug)

include ../../build/kApi-gnumk_linux_x86-Debug/kAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kApiDef.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kApiLib.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kAssembly.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kObject.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kType.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kValue.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kCipher.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kBlowfishCipher.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kCipherStream.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kHash.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kSha1Hash.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kArray1.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kArray2.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kArray3.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kArrayList.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kArrayProvider.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kBitArray.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kBox.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kBytes.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kCollection.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kImage.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kList.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kMath.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kMap.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kString.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kQueue.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kXml.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/lodepng.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/lodepng_ex.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/rbtree.c.d
include ../../build/kApi-gnumk_linux_x86-Debug/kDat5Serializer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kDat6Serializer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kDirectory.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kFile.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kHttpServer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerChannel.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerRequest.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kHttpServerResponse.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kMemory.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kNetwork.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kPath.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kPipeStream.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kSerializer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kStream.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kSocket.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kTcpClient.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kTcpServer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kUdpClient.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kWebSocket.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kAtomic.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kLock.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kMsgQueue.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kParallel.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kPeriodic.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kThread.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kThreadPool.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kTimer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kSemaphore.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kBackTrace.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kDateTime.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kDebugAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kDynamicLib.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kEvent.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kHeapAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kImageUtils.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kObjectPool.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kPlugin.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kPoolAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kProcess.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kSymbolInfo.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kTimeSpan.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kUserAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Debug/kUtils.cpp.d

endif

ifeq ($(config),Release)

include ../../build/kApi-gnumk_linux_x86-Release/kAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kApiDef.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kApiLib.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kAssembly.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kObject.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kType.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kValue.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kCipher.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kBlowfishCipher.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kCipherStream.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kHash.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kSha1Hash.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kArray1.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kArray2.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kArray3.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kArrayList.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kArrayProvider.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kBitArray.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kBox.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kBytes.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kCollection.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kImage.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kList.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kMath.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kMap.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kString.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kQueue.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kXml.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/lodepng.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/lodepng_ex.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/rbtree.c.d
include ../../build/kApi-gnumk_linux_x86-Release/kDat5Serializer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kDat6Serializer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kDirectory.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kFile.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kHttpServer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kHttpServerChannel.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kHttpServerRequest.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kHttpServerResponse.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kMemory.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kNetwork.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kPath.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kPipeStream.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kSerializer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kStream.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kSocket.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kTcpClient.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kTcpServer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kUdpClient.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kWebSocket.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kAtomic.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kLock.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kMsgQueue.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kParallel.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kPeriodic.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kThread.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kThreadPool.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kTimer.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kSemaphore.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kBackTrace.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kDateTime.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kDebugAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kDynamicLib.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kEvent.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kHeapAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kImageUtils.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kObjectPool.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kPlugin.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kPoolAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kProcess.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kSymbolInfo.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kTimeSpan.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kUserAlloc.cpp.d
include ../../build/kApi-gnumk_linux_x86-Release/kUtils.cpp.d

endif

endif

