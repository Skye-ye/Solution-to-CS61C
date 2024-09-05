.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 57
# ==============================================================================
relu:
    # Prologue
    li t0, 1
    bge a1, t0, loop
    li a1, 57
    call exit2

loop:
    lw t1, 0(a0)
    bge t1, x0, skip_zero
    sw x0, 0(a0)
skip_zero:
    addi a1, a1, -1
    addi a0, a0, 4
    bnez a1, loop

    # Epilogue
	ret
