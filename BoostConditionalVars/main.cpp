
// STD
#include <iostream>

// Boost
#include <boost/thread.hpp>

boost::condition_variable thread1_start_condition, thread2_start_condition, thread3_start_condition;
boost::mutex thread1_start_ready_access_mutex, data_access_mutex, thread1_start_mutex, thread2_start_mutex, thread3_start_mutex;
bool thread1_start_ready = false , data_therad1_ready = false;

void thread1(){

  while (true){
    boost::unique_lock<boost::mutex> start_lock(thread1_start_mutex);
    while (!thread1_start_ready){
      thread1_start_condition.wait(start_lock);
    }

    {
      boost::lock_guard<boost::mutex> lock(thread1_start_ready_access_mutex);
      thread1_start_ready = false;
    }
    

    std::cout << "Thread 1 done.\n";

    // Because the lock_guard will lock in the object creation and will unlock when the object's destructor is called.
    {
      boost::lock_guard<boost::mutex> lock(data_access_mutex);
      data_therad1_ready = true;
    }
    thread2_start_condition.notify_one();
  }
}

void thread2(){

  while (true){
    boost::unique_lock<boost::mutex> lock(thread2_start_mutex);
    while (!data_therad1_ready){
      thread2_start_condition.wait(lock);
    }

    {
      boost::lock_guard<boost::mutex> lock(data_access_mutex);
      data_therad1_ready = false;
    }

    std::cout << "Thread 2 done.\n";

    {
      boost::lock_guard<boost::mutex> lock(thread1_start_ready_access_mutex);
      thread1_start_ready = true;
    }

    thread1_start_condition.notify_one();
   // boost::lock_guard<boost::mutex> thread1_start_lock(thread1_start_mutex);
   // data_therad1_ready = true;
  }
}

void main(){

  boost::thread t1(thread1), t2(thread2);

  {
    boost::lock_guard<boost::mutex> lock(thread1_start_ready_access_mutex);
    thread1_start_ready = true;
  }
  thread1_start_condition.notify_one();

  t1.join();
  t2.join();
}