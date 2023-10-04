#pragma once

#include <boost/fiber/all.hpp>

namespace ReadersWriters {

void read();
void write();
void run(int nbReaders, int nbWriters);
} // namespace ReadersWriters