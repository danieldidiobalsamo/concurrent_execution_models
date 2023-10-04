#include "DiningPhilosophers.hpp"

#include <boost/fiber/all.hpp>
#include <mutex>
#include <random>
#include <time.h>

namespace DiningPhilosophers {

constexpr const int nbPhilosophers = 5;
State states[nbPhilosophers];
boost::fibers::mutex criticalRegion{}; // when taking or putting a fork
boost::fibers::mutex outputMutex{};
std::binary_semaphore bothForksAvailable[nbPhilosophers]{
    std::binary_semaphore{0}, std::binary_semaphore{0},
    std::binary_semaphore{0}, std::binary_semaphore{0},
    std::binary_semaphore{0}};

std::default_random_engine
    randomIntGenerator(static_cast<unsigned int>(time(0)));

int random(int min, int max) {
  std::uniform_int_distribution<int> distrib(min, max);
  return distrib(randomIntGenerator);
}

void run() {
  std::vector<boost::fibers::fiber> philosophers;
  philosophers.reserve(nbPhilosophers);

  try {
    for (int i = 0; i < nbPhilosophers; ++i) {
      boost::fibers::fiber f(std::bind(philosopher, i));
      philosophers.push_back(std::move(f));
    }

    for (auto &p : philosophers) {
      p.join();
    }

  } catch (std::exception const &e) {
    std::cerr << "exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "unhandled exception" << std::endl;
  }
}

void philosopher(int philosopher) {
  while (true) {
    think(philosopher);
    takeForks(philosopher);
    eat(philosopher);
    putForks(philosopher);
  }
}

void think(int philosopher) {
  int duration = random(400, 800);
  std::unique_lock<boost::fibers::mutex> lk(outputMutex);
  std::cout << philosopher << " is thinking " << duration << "ms" << std::endl;
  lk.unlock();
  boost::this_fiber::sleep_for(std::chrono::milliseconds(duration));
}

void tryGetTwoForks(int philosopher) {
  if (states[philosopher] == State::HUNGRY &&
      states[left(philosopher)] != State::EATING &&
      states[right(philosopher)] != State::EATING) {
    states[philosopher] = State::EATING;
    bothForksAvailable[philosopher].release();
  }
}

void eat(int philosopher) {
  int duration = random(400, 800);
  std::unique_lock<boost::fibers::mutex> lk(outputMutex);
  std::cout << "\t\t\t\t" << philosopher << " is eating " << duration << "ms\n";
  lk.unlock();
  boost::this_fiber::sleep_for(std::chrono::milliseconds(duration));
}

void takeForks(int philosopher) {
  std::unique_lock<boost::fibers::mutex> criticalLk(criticalRegion);
  states[philosopher] = State::HUNGRY;

  std::unique_lock<boost::fibers::mutex> outputLk(outputMutex);
  std::cout << "\t\t" << philosopher << " is hungry" << std::endl;
  outputLk.unlock();

  tryGetTwoForks(philosopher);
  criticalLk.unlock();

  while (!bothForksAvailable[philosopher].try_acquire()) {
    // can't use acquire, otherwise the thread would be blocked, not the fiber
    boost::this_fiber::yield();
  }

  outputLk.lock();
  std::cout << "\t\t\t\t" << philosopher << " takes forks " << std::endl;
  outputLk.unlock();
}

void putForks(int philosopher) {
  std::unique_lock<boost::fibers::mutex> criticalLk(criticalRegion);

  states[philosopher] = State::THINKING;
  tryGetTwoForks(left(philosopher));  // see if left neighbor can now eat
  tryGetTwoForks(right(philosopher)); // see if right neighbor can now eat

  criticalLk.unlock();

  std::unique_lock<boost::fibers::mutex> lk(outputMutex);
  std::cout << "\t\t\t\t" << philosopher << " puts forks " << std::endl;
  lk.unlock();
}

int left(int i) { return (i - 1 + nbPhilosophers) % nbPhilosophers; }
int right(int i) { return (i + 1) % nbPhilosophers; }

}; // namespace DiningPhilosophers