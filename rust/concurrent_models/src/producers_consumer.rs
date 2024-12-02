use std::sync::mpsc;
use std::thread;
use std::time::Duration;

pub fn run() {
    let (tx1, rx) = mpsc::channel();

    let tx2 = tx1.clone();

    thread::spawn(move || {
        let vals = vec![1, 2, 3, 4, 5];
        for v in vals {
            tx1.send(v).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });

    thread::spawn(move || {
        let vals = vec![6, 7, 8, 9, 10];
        for v in vals {
            tx2.send(v).unwrap();
            thread::sleep(Duration::from_secs(1));
        }
    });

    for received in rx {
        println!("{received}");
    }
}
