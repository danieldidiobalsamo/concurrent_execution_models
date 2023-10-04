#include "ProducerConsumer.hpp"
#include <iostream>

namespace ProducerConsumer {
void produce(channel_t &chan) {
  for (int i = 0; i < 10; ++i) {
    chan.push(i);
  }

  chan.close();
}
void consume(channel_t &chan) {
  for (int val : chan) {
    std::cout << "consumed " << val << std::endl;
  }
}

void run() {
  channel_t chan{2};

  boost::fibers::fiber producer(std::bind(produce, std::ref(chan)));
  boost::fibers::fiber consumer(std::bind(consume, std::ref(chan)));

  producer.join();
  consumer.join();
}
} // namespace ProducerConsumer
