#include "ReadersWriters.hpp"
#include <vector>

namespace ReadersWriters {

int readCount = 0;
boost::fibers::mutex readMutex{};
boost::fibers::mutex writeMutex{};
boost::fibers::mutex
    serviceQueue{}; // this mutex avoids writers or readers starvation

int sharedInt = 0;

void write() {

  while (true) {
    std::unique_lock serviceLk(serviceQueue);
    std::unique_lock writeLk(writeMutex);
    serviceLk.unlock();
    ++sharedInt;
    writeLk.unlock();

    boost::this_fiber::yield();
  }
}

void read() {
  while (true) {
    std::unique_lock serviceLk(serviceQueue);
    std::unique_lock readLk(readMutex); // check if first reader to come
    std::unique_lock writeLk(writeMutex, std::defer_lock);
    ++readCount;

    if (readCount == 1) { // first reader
      writeLk.lock();     // prevents writer to write while readers read
    }

    serviceLk.unlock();
    readLk.unlock(); // allow other readers to come

    std::cout << "fiber ID: " << boost::this_fiber::get_id()
              << " sharedInt = " << sharedInt << std::endl;

    readLk.lock();
    --readCount;

    if (readCount == 0) {
      writeLk.unlock();
    }
    readLk.unlock();

    boost::this_fiber::yield();
  }
}

void run(int nbReaders, int nbWriters) {
  std::vector<boost::fibers::fiber> writers;
  writers.reserve(nbWriters);
  std::vector<boost::fibers::fiber> readers;
  readers.reserve(nbReaders);

  try {
    for (int i = 0; i < nbWriters; ++i) {
      boost::fibers::fiber f(std::bind(write));
      writers.push_back(std::move(f));
    }

    for (int i = 0; i < nbReaders; ++i) {
      boost::fibers::fiber f(std::bind(read));
      readers.push_back(std::move(f));
    }

    for (auto &w : writers) {
      w.join();
    }

    for (auto &r : readers) {
      r.join();
    }

  } catch (std::exception const &e) {
    std::cerr << "exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "unhandled exception" << std::endl;
  }
}
} // namespace ReadersWriters