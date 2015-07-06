#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <iostream>

namespace /*static*/ {
    boost::atomic<int> data;

    struct Work {
        Work(boost::barrier& barrier) : _barrier(&barrier) { }

        void signal_slave()
        {
            boost::lock_guard<boost::mutex> lock(data_ready_mutex);
            data_ready = true;
            cond.notify_all();
        }

        void slave_thread()
        {
            static boost::atomic_int _id_gen(0);
            id = _id_gen++;

            std::cout << "(" << id << ") slave_thread created\n";
            while (true) {

                boost::unique_lock<boost::mutex> lock(data_ready_mutex);
                cond.wait(lock, [&] { return data_ready; });

                data_ready = false;

                data++;

                num_run++;

                _barrier->count_down_and_wait();
            }
        }

    private:
        int id = 0;
        bool data_ready = false;
        int num_run = 0;

        boost::barrier* _barrier;
        boost::mutex data_ready_mutex;
        boost::condition_variable cond;
    };
}

#include <boost/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>

using hrc = boost::chrono::high_resolution_clock;

int main()
{
    boost::thread_group tg;

    size_t nThreads = 10;

    boost::barrier finish(nThreads + 1); // one for the main thread

    boost::ptr_vector<Work> works;
    works.reserve(nThreads); // avoid reallocations

    for (size_t i = 0; i < nThreads; i++) {
        works.push_back(new Work(finish));
        tg.create_thread(boost::bind(&Work::slave_thread, boost::ref(works.back())));
    }

    while (true) {
        auto start_time = hrc::now();

        for (auto& w : works)
            w.signal_slave();

        // Wait for slave threads to finish.
        finish.count_down_and_wait();

        std::cout << "Elapsed Time =" << std::setw(10) << std::right << (hrc::now() - start_time) << std::endl;
    }

    tg.join_all();
}
