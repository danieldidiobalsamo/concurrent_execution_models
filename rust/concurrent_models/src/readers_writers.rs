use std::sync::{Arc, RwLock};
use std::thread;

pub fn run(nb_readers: usize, nb_writers: usize, data: usize) {
    let mut scenario = ReadersWriters::new(nb_readers, nb_writers, data);
    scenario.run();
}

fn write(counter: &Arc<RwLock<usize>>, id: usize) {
    let mut w = counter.write().unwrap();
    *w += 1;
    println!("Writer {id} updates data, now {}", *w);
}
fn read(counter: &Arc<RwLock<usize>>, id: usize) {
    println!("reader {id} : {}", counter.read().unwrap());
}

struct ReadersWriters {
    nb_readers: usize,
    nb_writers: usize,
    counter: Arc<RwLock<usize>>,
}

impl ReadersWriters {
    fn new(nb_readers: usize, nb_writers: usize, data: usize) -> Self {
        ReadersWriters {
            nb_readers,
            nb_writers,
            counter: Arc::new(RwLock::new(data)),
        }
    }

    fn run(&mut self) {
        let mut handles = Vec::new();

        for i in 0..self.nb_readers {
            let counter = Arc::clone(&self.counter);
            let handle = thread::spawn(move || loop {
                read(&counter, i)
            });
            handles.push(handle);
        }

        for i in 0..self.nb_writers {
            let counter = Arc::clone(&self.counter);
            let handle = thread::spawn(move || loop {
                write(&counter, i);
            });
            handles.push(handle);
        }

        for handle in handles {
            handle.join().unwrap();
        }
    }
}
