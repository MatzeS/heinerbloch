#pragma once

#include <functional>

namespace {
}

template<class Callable>
class Task {
public:

    Task(Callable callable) : callable{callable} {
        // static assert tno arguments in callable?
        // what exactly happens with the capture, and where is it placed?
        //  could we move it in here?

        // xReturned
        //  BaseType_t xTaskCreate( TaskFunction_t pvTaskCode,
        //                  const char * const pcName,
        //                  const configSTACK_DEPTH_TYPE uxStackDepth,
        //                  void *pvParameters,
        //                  UBaseType_t uxPriority,
        //                  TaskHandle_t *pxCreatedTask
        //                );




    }

    static void run(void* void_this) {
        // TODO check void_this for nullptr
        Task<Callable>& explicit_this = *static_cast<Task<Callable>*>(void_this);
        explicit_this.callable();
        vTaskDelete(NULL); 
    }

    // No move, because the callable must stay in place 
    // because the pointer is moved into the task.
    //
    // If necessary this should be easily adaptable to be movable,
    // but for the moment this implementation is lazy.
    Task(Task &&) = delete;
    Task &operator=(Task &&) = delete;    

    // No copy to avoid a client unintentionally duplicates the task.
    Task(Task const&) = delete;
    Task &operator=(Task const&) = delete;

    ~Task() {
        // TODO check
        // also, only do this if task was creaetd
        // vTaskDelete(task handle); // TODO or nullptr?
    }

private:
    Callable callable;

};
