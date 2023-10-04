#pragma once

namespace DiningPhilosophers {
enum class State {
  THINKING, // hold no forks
  HUNGRY,   // try to get 2 forks
  EATING,   // hold 2 forks
};

void run();
void think(int philosopher);
void philosopher(int philosopher);
void takeForks(int philosopher);
void putForks(int philosopher);
void eat(int philosopher);
int left(int i);
int right(int i);
} // namespace DiningPhilosophers
