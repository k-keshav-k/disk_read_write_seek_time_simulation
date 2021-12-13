README

1.  What does this program do 
This program simulates read, write and seek operations of a disk.
It takes in parameters - rotational speed of disk, r revolutions per minute, average seek time and sector size.
It then makes random requests in the form of platter, cylinder, sector and number of sectors requested.
Then a suitable disk scheduler algorithm is used to order the requests and then these requests are sent to the disk.
Random, FIFO, SSTF, SCAN, C-SCAN are implemented disk scheduler algorithms.

2.  A description of how this program works
This program works by taking rotations per minute, seek time, sector size, number of requests and which of the disk scheduler algorithm to use as inputs. 
Then the program generates random requests containing platter number, cylinder number, sector number and number of sectors to be accessed.
Then a function is called which according to user input, runs a suitable function which orders the requests corresponding to the selected disk scheduler algorithm.
Then it calculates response time for the ordered requests as 
time = time for previous + difference in track number * time to cross 1 track + difference in sector number * time to cross 1 sector + (number of sectors requested - 1) * time to cross 1 sector.
Average, minimum, maximum, standard deviation and throughput of the response time are calculated and comparred for different disk scheduler algorithms.

3.  How to compile and run this program
compilation: 	gcc disk_scheduling.c -o a -lm
execution:	./a

4.  Provide a snapshot of a sample run

Rotational speed(in revolutions/min):  7500
Average seek time(in ms):  4
Sector size:(in bytes):  512
Number of requests:  10
Algorithm number:  1
(4 12 17 16)    (2 11 6 13)     (2 22 2 8)      (3 10 3 7)      (1 2 12 17)     (4 19 7 10)     (3 6 2 4)        (4 11 9 3)      (3 9 9 8)       (2 7 11 3)
(2 11 6 13)     (4 11 9 3)      (1 2 12 17)     (4 12 17 16)    (3 10 3 7)      (4 19 7 10)     (2 7 11 3)       (3 6 2 4)       (2 22 2 8)      (3 9 9 8)
RANDOM is used
7.680000 12.080001 23.200001 37.599998 44.559998 53.279995 61.839993 67.119987 78.799988 87.839989 
Throughput:     113.843369 responses/sec
Avg response:   47.399998 ms
min response:   7.680000 ms
max response:   87.839989 ms
StdDev time:    26.015850 ms

Rotational speed(in revolutions/min):  7500
Average seek time(in ms):  4
Sector size:(in bytes):  512
Number of requests:  10
Algorithm number:  3
(4 12 17 16)    (2 11 6 13)     (2 22 2 8)      (3 10 3 7)      (1 2 12 17)     (4 19 7 10)     (3 6 2 4)        (4 11 9 3)      (3 9 9 8)       (2 7 11 3)
(4 12 17 16)    (2 11 6 13)     (4 11 9 3)      (3 10 3 7)      (3 9 9 8)       (2 7 11 3)      (3 6 2 4)        (1 2 12 17)     (4 19 7 10)     (2 22 2 8)
SSTF is used
7.200000 14.880000 19.279999 25.359999 28.639997 32.399998 37.679996 48.799995 60.959995 67.599998 
Throughput:     147.929001 responses/sec
Avg response:   34.279999 ms
min response:   7.200000 ms
max response:   67.599998 ms
StdDev time:    18.692327 ms
