.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 57
# =================================================================
argmax:

    # Prologue
    li t0, 1
    bge a1, t0, loop_start
    li a1, 57
    call exit2

loop_start:
    li t0, 0 # t0 stores the index of largest element
    lw t1, 0(a0) # t1 store the largest element
    li t2, 0 # current index

loop_continue:
    lw t3, 0(a0) # t2 stores the current element
    blt t3, t1, skip
    add t0, t2, x0
    add t1, t3, x0
skip:
    addi t2, t2, 1
    addi a0, a0, 4
    blt t2, a1, loop_continue

loop_end:
    add a0, t0, x0

    # Epilogue


    ret
