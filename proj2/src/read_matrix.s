.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 89
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 90
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp -24
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw ra, 20(sp)
    # End Prologue
    
    mv s0, a0
    mv s1, a1
    mv s2, a2
    
    mv a1, s0
    li a2, 0
    
    jal fopen
    li t0, -1
    beq a0, t0, fopen_fail
    mv s0, a0
    
    # Read number of rows and columns
    addi sp, sp, -8 # Save the number of rows and columns on the stack temporarily
    
    mv a1, s0
    mv a2, sp
    li a3, 8
    
    jal fread
    li t0, 8
    bne a0, t0, fread_fail
    
    lw t0, 0(sp) # store the number of rows
    sw t0, 0(s1)
    lw t1, 4(sp) # store the number of columns
    sw t1, 0(s2)
    
    addi sp, sp, 8
    
    # Calculate total size and allocate memory
    mul a0, t0, t1
    slli a0, a0, 2
    mv s3, a0
    
    jal malloc
    beqz a0, malloc_fail
    mv s4, a0
    
    # Read matrix data
    mv a1, s0
    mv a2, s4
    mv a3, s3
    
    jal fread
    bne a0, s3, fread_fail
    
    # Close the file
    mv a1, s0
    
    jal fclose
    bne a0, x0, fclose_fail
    
    # Return matrix pointer
    mv a0, s4

    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw ra, 20(sp)
    addi sp, sp, 24
    # End Epilogue

    ret

malloc_fail:
    li a1, 88
    call exit2
    
fopen_fail:
    li a1, 89
    call exit2
    
fclose_fail:
    li a1, 90
    call exit2
    
fread_fail:
    li a1, 91
    call exit2
