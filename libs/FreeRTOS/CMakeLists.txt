add_library(FreeRTOS-Config INTERFACE)
target_include_directories(FreeRTOS-Config INTERFACE ${CMAKE_CURRENT_LIST_DIR}/config)

add_library(FreeRTOS-POSIX INTERFACE)
set(FREE_RTOS_POSIX_ROOT_DIRECTORY /opt/FreeRTOS-POSIX/)
target_sources(FreeRTOS-POSIX INTERFACE
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_semaphore.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_sched.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_mqueue.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_utils.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_barrier.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_cond.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_mutex.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_unistd.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_clock.c
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_timer.c
)
target_include_directories(FreeRTOS-POSIX
    INTERFACE
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/include
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/FreeRTOS-Plus-POSIX/include/portable
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/include
    ${FREE_RTOS_POSIX_ROOT_DIRECTORY}/include/private
)
target_link_libraries(FreeRTOS-POSIX
    INTERFACE
    FreeRTOS-POSIX-Port
)

add_library(FreeRTOS-POSIX-Port INTERFACE)
target_include_directories(FreeRTOS-POSIX-Port INTERFACE ${CMAKE_CURRENT_LIST_DIR}/port)