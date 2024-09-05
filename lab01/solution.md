# Solution to Lab01

## Exercise 1

```c
#define V0 3
#define V1 3
#define V2 3
#define V3 3
```

## Exercise 2

```bash
1. set args [arg1] [arg2] [arg3]...
2. break (or simply b) [function_name] (break main)/ [filename:linenumber] (break main.c:50)/ [*address] (break *0x401920)
3. nexti
4. stepi
5. continue
6. print [variable_name] (print 1 + 2 is also valid)
7. display [variable_name]
8. info locals
9. q
```

## Exercise 3

1. put your input in a file (like `in.txt`)

2. run `cgdb` and run the command inside `cgdb`: `set args < in.txt` where "<" is used to redirect stdin

## Exercise 4

1. Valgrind is vital for C and C++ developers, helping detect memory leaks, threading issues, and access errors. It provides detailed diagnostics to improve software reliability and prevent crashes, making it essential for developing robust applications.

2. ```bash
   valgrind [options] ./your_program [program_options]
   ```

3. my error message (part of):

   ```
   ==6187== Invalid write of size 4
   ==6187==    at 0x10914F: main (segfault_ex.c:4)
   ==6187==  Address 0x1fff001000 is not stack'd, malloc'd or (recently) free'd
   ==6187==
   ==6187==
   ==6187== Process terminating with default action of signal 11 (SIGSEGV)
   ==6187==  Access not within mapped region at address 0x1FFF001000
   ==6187==    at 0x10914F: main (segfault_ex.c:4)
   ==6187==  If you believe this happened as a result of a stack
   ==6187==  overflow in your program's main thread (unlikely but
   ==6187==  possible), you can try to increase the size of the
   ==6187==  main thread stack using the --main-stacksize= flag.
   ==6187==  The main thread stack size used in this run was 8388608.
   ```

   it tells me there's a access to a not mapped region and the error code lies in segfault_ex.c:4, which is :

   ```c
   a[i] = i;
   ```

4. Uninitialized variables in languages like C and C++ are not automatically set to zero. Instead, they hold indeterminate values, which depend on whatever data previously resided at the memory location assigned to them. This can vary each time the program runs, depending on system state, memory usage, and other factors.

5. Although the program steps outside the array `a` far away(loop 20 times instead of 5 times), it does not produce a segmentation fault. The reason may be that the memory accesses do not step outside the bounds allocated to the process by the operating system, so there's no protection behavoir (segmentation fault).

6. because every time the program runs, the operating system maps different part of physical memory to the process, there's no way to know what garbage values lie in these areas;

7. because `sizeof` returns the number of bytes the parameter takes, not the number of elements. The correct representation is:

   ```c
   for (int j = 0; j < sizeof(a) / sizeof(a[0]); j++)
   ```

## Exercise 5

my version of `ll_has_cycle()`:

```c
int ll_has_cycle(node *head) {
  node *tortoise = head;
  node *hare = head;
  while (hare != NULL && hare->next != NULL) {
    tortoise = tortoise->next;
    hare = hare->next->next;
    if (tortoise == hare) {
      return 1;
    }
  }
  return 0;
}
```



