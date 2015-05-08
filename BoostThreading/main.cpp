
// STD
#include <iostream>

// Boost
#include <boost/thread.hpp>

int global_val = 0;
int before_breakpoint = 0, after_breakpoint = 0;

void mythread(){

  for (size_t i = 0; i < 500000; i++);

  if (global_val == 0)
    before_breakpoint++;
  else
    after_breakpoint++;

}

void main() {

  size_t num_threads = 160 ;
  std::vector<boost::thread*> threads(num_threads);

  for (size_t i = 0; i < num_threads; i++){
    threads[i] = new boost::thread(mythread);
  }

  //for (size_t i = 0; i < 100000000000; i++);

  global_val = 1;
  std::cout << "Before Breakpoint threads = " << before_breakpoint << std::endl;

  for (size_t i = 0; i < num_threads; i++){
    threads[i]->join();
   
    //delete threads[i];
  }

  std::cout << " After Breakpoint threads = " << after_breakpoint << std::endl;
  
}