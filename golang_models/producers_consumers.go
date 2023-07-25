package main

import (
	"fmt"
	"sync"
)

const nbProducers int = 5
const nbConsumers int = 3

func produce(datas chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()

	for i := 0; i < 3; i++ {
		datas <- i
	}
}

func consume(datas <-chan int, wg *sync.WaitGroup) {
	defer wg.Done()

	for d := range datas {
		fmt.Println(d)
	}
}

func ProducersConsumers() {
	datas := make(chan int)
	waitProducers := &sync.WaitGroup{}
	waitConsumers := &sync.WaitGroup{}

	waitProducers.Add(nbProducers)
	waitConsumers.Add(nbConsumers)

	for i := 0; i < nbProducers; i++ {
		go produce(datas, waitProducers)
	}

	for i := 0; i < nbConsumers; i++ {
		go consume(datas, waitConsumers)
	}

	waitProducers.Wait()
	close(datas)
	waitConsumers.Wait()
}
