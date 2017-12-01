# LRU

Least recently used cache implementation using Data Structures in c.
- Uses Hash table for cache.
- Uses queues to track least recently used page number.
- Uses an array for RAM.

## lru.c 
Gives a broad understanding of the program.

![text](https://github.com/Dweepa/LRU/blob/master/screenshots/lru%20screenshot.png)

#### TO EXECUTE:
$gcc lru.c
$./a.out

#### WHAT THE PROGRAM DOES:
- The program mimics a cache using hash tables and a queue. The hash tables provide for easy access of the element and the queue keeps track of the most recently and least recently used elements.

- This program times the references from caches with 3 different capacities and direct linear searching through ram to see which is the most efficient.

    1. Cache capacity:3 - slowest among all 3 caches.
    2. Cache capacity:5 - faster than the previous one but slower than the next one.
    3. Cache capacity:10 - fastest among all 3 caches.
    4. RAM linear search - slower than all the cache

## lru_copy.c 
Gives the detailed approach of the program.

#### TO EXECUTE lru_copy.c
$gcc lru_copy.c

$./a.out

When prompted to enter the cache capacity please enter the capacities 3, 5 and 10 to see each of their working.

Enter 0 after entering all the three cache capacities to see the integrated result.

![text](https://github.com/Dweepa/LRU/blob/master/screenshots/cache_3.png)
![text](https://github.com/Dweepa/LRU/blob/master/screenshots/cache_5.png)
![text](https://github.com/Dweepa/LRU/blob/master/screenshots/cache_10.png)
![text](https://github.com/Dweepa/LRU/blob/master/screenshots/cache_everything.png)


