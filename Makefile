all: producer consumer
producer: producer.c
	gcc -Wall -g -o producer producer.c -lpthread

consumer: consumer.c 
	gcc -Wall -g -o consumer consumer.c -lpthread

clean:
	rm -f producer
	rm -f consumer
	rm -f *.o
