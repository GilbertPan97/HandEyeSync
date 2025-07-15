/*                                                                  
 * POSIX Real Time Example
 * using a single pthread as RT thread
 */
 
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "stream_motion.hpp"

#define ROBOT_IP_ADDRESS "169.254.249.94"       /* Specify the IP address of the actual robot. */
#define ROBOT_PORT 60015

void print_current_time() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;

#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::cout << std::put_time(&tm, "%Y_%m-%d %H:%M:%S")
              << "." << std::setw(3) << std::setfill('0') << (value.count() % 1000) << std::endl;
}

void *thread_func(void *data)
{
    printf("Real-time thread is running\n");
    StreamMotion *motion = static_cast<StreamMotion *>(data);

    if (!motion->init(ROBOT_IP_ADDRESS, ROBOT_PORT)) {
        fprintf(stderr, "Initialization failed for StreamMotion\n");
        return NULL;   
    }

    if (!motion->run()) {
        fprintf(stderr, "Error running StreamMotion\n");
        return NULL;
    }
    // for (size_t i = 0; i < 100; i++)
    // {
    //     print_current_time();
    // }

    return NULL;
}
 
int main(int argc, char* argv[])
{
    struct sched_param param;
    pthread_attr_t attr;
    pthread_t thread;
    int ret;
    StreamMotion motion; // Create an instance of StreamMotion
    
    /* Lock memory */
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
        printf("mlockall failed: %m\n");
        exit(-2);
    }

    /* Initialize pthread attributes (default values) */
    ret = pthread_attr_init(&attr);
    if (ret) {
        printf("init pthread attributes failed\n");
        goto out;
    }

    /* Set a specific stack size  */
    ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
    if (ret) {
        printf("pthread setstacksize failed\n");
        goto out;
    }

    /* Set scheduler policy and priority of pthread */
    ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if (ret) {
        printf("pthread setschedpolicy failed\n");
        goto out;
    }
    param.sched_priority = 80;
    ret = pthread_attr_setschedparam(&attr, &param);
    if (ret) {
        printf("pthread setschedparam failed\n");
        goto out;
    }
    /* Use scheduling parameters of attr */
    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (ret) {
        printf("pthread setinheritsched failed\n");
        goto out;
    }

    /* Create a pthread with specified attributes */
    ret = pthread_create(&thread, &attr, thread_func, &motion);
    if (ret) {
        printf("create pthread failed\n");
        goto out;
    }

    /* Join the thread and wait until it is done */
    ret = pthread_join(thread, NULL);
    if (ret)
        printf("join pthread failed: %m\n");

out:
    return ret;
}



