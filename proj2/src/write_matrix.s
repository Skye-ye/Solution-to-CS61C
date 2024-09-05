.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 89
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 90
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 92
# ==============================================================================
write_matrix:

    # Prologue
    addi, sp, sp, -20
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw ra, 16(sp)
    # End Prologue
    
    mv s1, a1
    mv s2, a2
    mv s3, a3
    
    mv a1, a0
    li a2, 1
    
    jal fopen
    li t0, -1
    beq a0, t0, fopen_fail
    mv s0, a0
    
    mv a1, s0
    addi, sp, sp, -8
    sw s2, 0(sp)
    sw s3, 4(sp)
    mv a2, sp
    li a3, 2
    li a4, 4
    
    jal fwrite
    li t0, 2
    bne a0, t0, fwrite_fail
    addi sp, sp, 8
    
    mv a1, s0
    mv a2, s1
    mul a3, s2, s3
    mv s2, a3
    li a4, 4
    
    jal fwrite
    bne a0, s2, fwrite_fail
    
    mv a1, s0
    jal fclose
    bne a0, x0, fclose_fail

    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw ra, 16(sp)
    addi sp, sp, 20
    
    ret
    
fopen_fail:
    li a1, 89
    call exit2
    
fclose_fail:
    li a1, 90
    call exit2

fwrite_fail:
    li a1, 92
    call exit2
