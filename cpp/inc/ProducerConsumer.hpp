#pragma once

#include <boost/fiber/all.hpp>

typedef boost::fibers::buffered_channel<int> channel_t;

namespace ProducerConsumer {
void run();
void produce(channel_t &chan);
void consume(channel_t &chan);
}; // namespace ProducerConsumer