.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 57
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 58
# =======================================================
dot:

    # Prologue
    li t0, 1
    blt a2, t0, return_57
    blt a3, t0, return_58
    blt a4, t0, return_58
    
loop_start:
    li t0, 0 # stores the result
    li t1, 0 # counter

loop_continue:
    lw, t2, 0(a0)
    lw, t3, 0(a1)
    mul t4, t2, t3
    add t0, t0, t4
    
    addi t1, t1, 1
    beq t1, a2, loop_end
    
    slli t2, a3, 2
    add a0, a0, t2
    slli t3, a4, 2
    add a1, a1, t3

    j loop_continue

loop_end:
    add a0, t0 x0

    # Epilogue
    
    ret

return_57:
    li a1, 57
    call exit2

return_58:
    li a1, 58
    call exit2
