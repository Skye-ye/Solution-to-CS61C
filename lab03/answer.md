## Exercise 1

1. `.data` directive is used to define the `data` segment of the program, within which, `.word` directive is used to declare and initialize 32-bit data items. `.text` directive marks the beginning of the code segment.

2. 34 This number represents the nineth number in fibonacci sequence.

3. 0x10000010

4. Modify the value in `t3` register to 0xD after this instruction executes:

   ```assembly
   lw t3, 0(t3)
   ```

## Exercise 2

1. `t0`

2. `s0`

3. `s1` and `s2`

4. assembly code:

   ```assembly
   loop:
       slli s3, t0, 2
       add t1, s1, s3
       lw t2, 0(t1)
       beq t2, x0, exit
       add a0, x0, t2
       addi sp, sp, -8
       sw t0, 0(sp)
       sw t2, 4(sp)
       jal fun
       lw t0, 0(sp)
       lw t2, 4(sp)
       addi sp, sp, 8
       add t2, x0, a0
       add t3, s2, s3
       sw t2, 0(t3)
       add s0, s0, t2
       addi t0, t0, 1
       jal x0, loop
   ```

5. Pointers are manipulated primarily through the use of base addresses and offsets.
