// STD 
#include <iostream>
#include <vector>


// BOOST
#include <boost/thread.hpp>
#include <boost/atomic.hpp>

#define USE_COND_VAR

std::vector<boost::thread*> threads;

std::vector<boost::mutex*> data_ready_mutex;
std::vector<boost::condition_variable*> cond;
std::vector<bool> data_ready;
std::vector<int> num_run;

boost::mutex check_finish_mutex;
std::vector<bool> finished;

boost::atomic<int> data;
boost::atomic<int> next_thread_id;

#ifdef USE_COND_VAR
  boost::mutex finished_task_mutex;
  boost::condition_variable finished_task_cond;
  bool finished_task = false;
#else
  boost::atomic<int> num_threads_done;
  boost::mutex num_threads_done_mutex;
#endif

#ifdef USE_COND_VAR
  void signal_finished(const int& id)
  {
    {
      boost::lock_guard<boost::mutex> lock(finished_task_mutex);
      finished[id] = true;
      finished_task = true;
    }
    finished_task_cond.notify_all();
  }
#endif

void slave_therad()
{
  int id = next_thread_id++;

  std::cout << "( " << id << " ) slave_thread created\n";
  while (true)
  {

    /*if (num_threads_done >= 10)
    {
      bool a = true;
      a = false;
    }*/

    boost::unique_lock<boost::mutex> lock(*data_ready_mutex[id]);
    while (!data_ready[id])
    {
      cond[id]->wait(lock);
    }

#ifdef USE_COND_VAR
    finished[id] = false;
#else
    if (num_threads_done >= 10)
    {
      bool a = true;
      a = false;
    }
#endif

    


    data_ready[id] = false;

    data++;

    num_run[id]++;

#ifdef USE_COND_VAR
    signal_finished(id);
#else
    {
      boost::lock_guard<boost::mutex> lock(num_threads_done_mutex);
      num_threads_done++;
    }

    std::cout << num_threads_done << std::endl;

    if (num_threads_done > threads.size())
    {
      int a = 0;
      a = 10;
    }
#endif
  }
}

void signal_slave(const int& id)
{
  {
    boost::lock_guard<boost::mutex> lock(*data_ready_mutex[id]);

    data_ready[id] = true;
  }
  cond[id]->notify_all();
}

void main_thread()
{
  while (true)
  {
    clock_t start_time = clock();

#ifndef USE_COND_VAR
    {
      boost::lock_guard<boost::mutex> lock(num_threads_done_mutex);
      num_threads_done = 0;
    }

    if (num_threads_done > 0)
    {
      int a = 2;
      a = 3;
    }

#endif

    for (size_t i = 0; i < threads.size(); i++)
      signal_slave(static_cast<int>(i));

    while (true)
    {
#ifdef USE_COND_VAR
      boost::unique_lock<boost::mutex> lock(finished_task_mutex);
      while (!finished_task)
      {
        finished_task_cond.wait(lock);
      }
      finished_task = false;

      size_t i = 0;
      for (; i < finished.size(); i++)
      {
        if (!finished[i]) break;
      }
      if (i == finished.size()) break;
#else
      {
        boost::lock_guard<boost::mutex> lock(num_threads_done_mutex);
        if (num_threads_done == threads.size()) break;
      }
#endif
    }

    clock_t end_time = clock();

    std::cout << "Elapsed Time = " << static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC << std::endl;

    for (size_t i = 0; i < threads.size(); i++)
      finished[i] = false;

  }
}

void main()
{
  size_t nThreads = 10;
  for (size_t i = 0; i < nThreads; i++)
  {
    threads.push_back(new boost::thread(slave_therad));
  }

  data_ready_mutex.resize(nThreads);
  cond.resize(nThreads);
  data_ready.resize(nThreads);
  finished.resize(nThreads);
  num_run.resize(nThreads, 0);
  for (size_t i = 0; i < nThreads; i++)
  {
    data_ready_mutex[i] = new boost::mutex();
    cond[i] = new boost::condition_variable();
    data_ready[i] = false;
    finished[i] = false;
  }  

  boost::thread mThread(main_thread);

  mThread.join();

  for (size_t i = 0; i < nThreads; i++)
  {
    threads[i]->join();
  }

}