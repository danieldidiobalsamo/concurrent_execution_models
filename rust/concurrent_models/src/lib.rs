mod dining_philosophers;
mod producers_consumer;
mod readers_writers;

pub fn producers_consumer() {
    producers_consumer::run();
}

pub fn readers_writers() {
    readers_writers::run(3, 2, 0);
}

pub fn dining_philosophers() {
    dining_philosophers::run(5);
}
