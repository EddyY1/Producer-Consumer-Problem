# Producer-Consumer-Problem
School assignment solving the producer-consumer problem using mutexes and condition variables
HOW TO RUN THE PROGRAM:

1. Make 
2. ./producer <file name> then ./consumer <output name>
Or 
3. ./consumer <output name> then ./producer <file name>

:)


This program provides a solution to the producer-consumer problem using mutexes and condition variables to allow efficient data processing using a circular buffer that is created through shared memory. 

This program actually wasn't as bad as I thought it would be, but it was difficult to test output at times and successful memory creation. It also didn't help that Russel's algorithms for updating the circular buffer were incorrect, so once again I blame him for me not finishing in an efficient manner. The main troubles I had were processing the input files and making sure the buffer count was correct. Because Russel's algorithm for updating the front of the buffer was incorrect I was stuck for about 2 days trying to figure out why the latter invoked program did not produce the correct output. After I figured that out I just needed to swap the bytes, which was not bad at all. 

Both programs begin by either creating or finding a shared memory segment, then proceeding into their respective loops to begin either reading the file or extracting data. I followed Russel's algorithm for these two so I don't have to really explain it besides the fact that I use EOF flags to indicate when the programs should end. To swap the bytes I basically read in two bytes at a time and reversed the order from which they are put into an output file, while also accounting for odd number of bytes from which it justs add that last byte normally at the end. 

