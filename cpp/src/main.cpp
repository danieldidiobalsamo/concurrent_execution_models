#include "ProducerConsumer.hpp"
#include "ReadersWriters.hpp"

int main() {
  // ProducerConsumer::run();
  ReadersWriters::run(3, 1);
  return 0;
}