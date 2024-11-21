

/* main.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "pthread.h"
// #include <pico/filesystem.h>
#include <pico/stdio.h>

#include <fstream>
#include <iostream>
#include <mutex>
#include <pthread.h>

// extern "C" bool fs_init(void);

extern "C" int pthread_cancel(pthread_t thread){
  printf("\n\nCALLED CANCEL \n\n\n");
  return 0;
}

void test(){}

#include <memory>

std::unique_ptr<std::mutex> lock{nullptr};

void set(uint32_t* bp){
    *bp = 0xAABBCCDD;

}

class Foo {
  public:
  Foo(){
    uint32_t* bp = &buffer;
    set(bp);

    printf("pointer %p content %08lx\n", bp, *bp);
  }

  uint32_t buffer;
};

void main_task(__unused void *params) {

  printf("size of mutex %d\n", sizeof(std::mutex));
  printf("size of pmutex %d\n", sizeof(pthread_mutex_t));
  

  uint32_t a = 0xDEADBEEF;
  uint32_t b = 0xDEADBEEF;
  uint32_t c = 0xDEADBEEF;
  uint32_t d = 0xDEADBEEF;
  uint32_t e = 0xDEADBEEF;
  uint32_t f = 0xDEADBEEF;
  printf("%08lx\n",a);
  printf("%08lx\n",b);
  printf("%08lx\n",c);
  printf("%08lx\n",d);
  printf("pointer: %p \n", &a);
  printf("pointer: %p \n", &b);
  printf("pointer: %p \n", &c);
  printf("pointer: %p \n", &d);
  printf("pointer: %p \n", &e);
  printf("pointer: %p \n", &f);
{

  // handle = xSemaphoreCreateMutexStatic(&xMutexBuffer);
  // printf("semaphore: %d\n", uxSemaphoreGetCount(handle));
  // printf("semaphore take: %d\n", xSemaphoreTake(handle, 0));
  printf("BEOFAFKL:KDSJFL:KJSDL:KFJS:LDKFJ:\n");

  // printf("semaphore take: %d\n", xSemaphoreTake(handle, 0));
  // printf("semaphore take: %d\n", xSemaphoreTake(handle, 0));
  // printf("semaphore: %d\n", uxSemaphoreGetCount(handle));
  // xSemaphoreGive(handle);
  // printf("semaphore: %d\n", uxSemaphoreGetCount(handle));
  // printf("semaphore take: %d\n", xSemaphoreTake(handle, 0));
  // printf("semaphore take: %d\n", xSemaphoreTake(handle, 0));
  // printf("semaphore take: %d\n", xSemaphoreTake(handle, 0));
}

  {
    Foo f{};
    // lock = std::make_unique<std::mutex>();

    std::mutex lock{};
    // std::mutex lock2{};
    printf("locking: %d\n", lock.try_lock());
    printf("locking: %d\n", lock.try_lock());
    lock.unlock();
    printf("locking: %d\n", lock.try_lock());
    printf("locking: %d\n", lock.try_lock());
    printf("locking: %d\n", lock.try_lock());
    lock.unlock();
  }


  printf("%08lx\n",a);
  printf("%08lx\n",b);
  printf("%08lx\n",c);
  printf("%08lx\n",d);
  printf("%08lx\n",e);
  printf("%08lx\n",f);
  printf("pointer: %p \n", &a);
  printf("pointer: %p \n", &d);

  printf("EXITING MAIN\n");
  while(1){
  // printf(".");
  }
}





// Priorities of our threads - higher numbers are higher priority
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )

// Stack sizes of our threads in words (4 bytes)
#define MAIN_TASK_STACK_SIZE (8*1024)

static uint32_t i = 0;
// static std::mutex mutexx;

void other(){
  printf("other function");
}

// void main_task(__unused void *params) {
//   using namespace std::chrono_literals;
//   printf("starting tast\n");
//   // std::this_thread::sleep_for(10ms);
//   for(uint32_t x = 0; x < 1000ULL; x++){
//     // std::lock_guard lock(mutexx);
//     // sample();
//     auto a = i % 2 == 0;
    
//     // std::this_thread::sleep_for(1ms);
//     vTaskDelay(1/portTICK_PERIOD_MS);

//     if(a){
//       i += 3;
//     }else{
//       i += 5;
//     }
//   }

//   printf("value: %d / %d\n", i, 1000ULL);

//   // std::thread x{other};
//   // x.join();

//   // std::this_thread::sleep_for(1000ms);
//   while(1);
// }

void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread1", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
    // TaskHandle_t task2;
    // xTaskCreate(main_task, "MainThread2", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task2);

#if configUSE_CORE_AFFINITY && configNUMBER_OF_CORES > 1
    // we must bind the main task to one core (well at least while the init is called)
    vTaskCoreAffinitySet(task, 1);
//    vTaskCoreAffinitySet(task2, 1);
#endif
  printf("\n\n\nstarting scheduler\n");
  
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main() {
  stdio_init_all();
  vLaunch();
}