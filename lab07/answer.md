## Exercise 1

### Scenario 1

1. The hit rate is zero. The block size of cache is exactly equal to the stepsize times 4, so for each step, the pointer steps exactlya block size in the memory, causing conflict misses at the first row in the cache.
2. Always zero
3. change step size to 10 will significantly increase the hit rate. if the rep count is big enough, the hit rate will be very close to 1.

### Scenario 2

1. two
2. mhhh
3. 1
4. **HINT**: You **do not** want to iterate through the entire array at once because it's much bigger than your cache. Doing so would reduce the amount of temporal locality your program exhibits, which makes cache hit rate suffer. We want to exhibit more locality so that our caches can take advantage of our predictable behavior. **SO**, instead, we should try to access `one element` of the array at a time and apply all of the `function` to that `element` so we can be completely done with it before moving on, thereby keeping that `block` hot in the cache and not having to circle back to it later on! (The 1st, 3rd, and 4th blanks should be the same. It's not some vocabulary term you should use to fill them in. It's more of an idea that you should have.)

### Scenario 3

1. L1 cache: 0.5, L2 cache: 0, overall: 0.5
2. There are 32 accesses and 16 of them are misses.
3. 16 accesses
4. rep count
5. When we increase the number of blocks in L1, the L1 and L2 hit rate stay the same; When we increase the L1 block size, the L1 hit rate increases and L2 hit rate stays the same.

## Exercise 2

1. jki, kji
2. ijk, jik

## Exercise 3

### Part 1

```bash
❯ ./transpose 100 20
Testing naive transpose: 0.007 milliseconds
Testing transpose with blocking: 0.01 milliseconds
❯ ./transpose 1000 20
Testing naive transpose: 0.773 milliseconds
Testing transpose with blocking: 0.71 milliseconds
❯ ./transpose 2000 20
Testing naive transpose: 2.878 milliseconds
Testing transpose with blocking: 2.416 milliseconds
❯ ./transpose 5000 20
Testing naive transpose: 63.4 milliseconds
Testing transpose with blocking: 18.321 milliseconds
❯ ./transpose 10000 20
Testing naive transpose: 213.405 milliseconds
Testing transpose with blocking: 75.825 milliseconds
```

#### Question 1

When `n` is equal to 1000, the cache blocked verison becomes slightly faster than non-cached version. However, there's little difference between these two version until `n` is equal to 5000.

#### Question 2

**Reason 1:** The cache blocked verison have one more loop (although does not increase time complexity) and few more comparation, which might overweight the benefits of improved cache usage for small matrices.

**Reason 2:** When the matrices size is relatively small, it's close to the block size, which means that the naive version also gets lots of cache hit.

### Part 2

```bash 
❯ ./transpose 10000 20
Testing naive transpose: 211.534 milliseconds
Testing transpose with blocking: 75.357 milliseconds
❯ ./transpose 10000 50
Testing naive transpose: 212.98 milliseconds
Testing transpose with blocking: 71.55 milliseconds
❯ ./transpose 10000 100
Testing naive transpose: 210.294 milliseconds
Testing transpose with blocking: 74.746 milliseconds
❯ ./transpose 10000 500
Testing naive transpose: 210.207 milliseconds
Testing transpose with blocking: 80.961 milliseconds
❯ ./transpose 10000 1000
Testing naive transpose: 210.204 milliseconds
Testing transpose with blocking: 76.005 milliseconds
❯ ./transpose 10000 5000
Testing naive transpose: 211.728 milliseconds
Testing transpose with blocking: 219.731 milliseconds
```

#### Question 3

**Performance Trend:** Performance improves as blocksize goes from 20 to 50, then slightly worsens but remains relatively stable from 100 to 1000. There's a significant performance drop at block size 5000, where it becomes slower than the naive approach.

**Reason:** When the blocksize is relatively small (like 20), it takes cache blocked version much time to jump between loops when compared to blocksize of 50 since the loop count is larger. When the block size is relatively larger (like 5000, which is half of the matrix's width), it may takes longer time to read data from memory and store it in the cache. It may also takes longer time to find the data in cache.