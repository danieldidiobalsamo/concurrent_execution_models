# About

Rust / C++ implementation of the three execution models used in concurrent programming:
- producer / consumer
- readers / writers
- dining philosophers

The C++ implementation makes use of [Boost fibers.](https://www.boost.org/doc/libs/1_74_0/libs/fiber/doc/html/index.html), and Rust one uses the standard library only.