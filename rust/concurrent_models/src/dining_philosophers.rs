use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

pub fn run(n: usize) {
    let mut philosophers = Vec::with_capacity(n);
    let mut forks = Vec::with_capacity(n);

    for i in 0..n {
        philosophers.push(i);
        forks.push(Arc::new(Mutex::new(Fork)));
    }

    let mut handles = Vec::new();

    for i in 0..n {
        let mut right = Arc::clone(&forks[i]);
        let mut left = Arc::clone(&forks[(i + 1) % forks.len()]);

        // break symmetry to avoid deadlock: if all philosophers are hungry in the same time
        // then they all tries to lock their left fork and everyone is locked
        if i == forks.len() - 1 {
            std::mem::swap(&mut left, &mut right);
        }

        let philosopher = Philosopher { id: i, left, right };

        let handle = thread::spawn(move || loop {
            philosopher.eat();
            philosopher.think();
        });

        handles.push(handle);
    }

    for h in handles {
        h.join().unwrap();
    }
}

struct Fork;
struct Philosopher {
    id: usize,
    left: Arc<Mutex<Fork>>,
    right: Arc<Mutex<Fork>>,
}

impl Philosopher {
    fn think(&self) {
        println!("{} is thinking", self.id);
        thread::sleep(Duration::from_millis(1000));
    }

    fn eat(&self) {
        println!("{} is trying to eat", &self.id);

        let _l = self.left.lock().unwrap();
        let _r = self.right.lock().unwrap();

        println!("{} is eating", &self.id);
        thread::sleep(Duration::from_millis(500));
        println!("{} finished eating", &self.id);
    }
}
