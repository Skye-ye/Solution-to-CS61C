.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero,
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 72
    # - If malloc fails, this function terminates the program with exit code 88
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
    
     
    li t0, 5
    bne a0, t0, arg_number_fail
    
    # Prologue
    addi sp, sp, -52
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw s8, 32(sp)
    sw s9, 36(sp)
    sw s10, 40(sp)
    sw s11, 44(sp)
    sw ra, 48(sp)
    # End Prologue
    
    mv s1, a1
    mv s2, a2

	# =====================================
    # LOAD MATRICES
    # =====================================
    
    addi sp, sp, -8
    
    # Load pretrained m0
    lw a0, 4(s1)
    addi, a1, sp, 0
    addi, a2, sp, 4
    jal read_matrix
    mv s3, a0
    lw s4, 0(sp) # number of rows in m0
    lw s5, 4(sp) # number of columns in m0
    
    # Load pretrained m1
    lw a0, 8(s1)
    addi, a1, sp, 0
    addi, a2, sp, 4
    jal read_matrix
    mv s6, a0
    lw s7, 0(sp) # number of rows in m1
    lw s8, 4(sp) # number of columns in m1
    
    # Load input matrix
    lw a0, 12(s1)
    addi, a1, sp, 0
    addi, a2, sp, 4
    jal read_matrix
    mv s9, a0
    lw s10, 0(sp) # number of rows in input
    lw s11, 4(sp) # number of columns in input
    
    addi sp, sp, 8

    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)
    
    # malloc space for hidden_layer matrix
    mul a0, s4, s11
    slli a0, a0, 2
    jal malloc
    beqz a0, malloc_fail
    mv s0, a0
    
    # call matmul
    mv a0, s3
    mv a1, s4
    mv a2, s5
    mv a3, s9
    mv a4, s10
    mv a5, s11
    mv a6, s0
    jal matmul
    
    # call relu
    mv a0, s0
    mul a1, s4, s11
    jal relu
    
    # malloc space for scores matrix
    mul a0, s7, s11
    slli a0, a0, 2
    jal malloc
    beqz a0, malloc_fail
    mv s5, a0
    
    # call matmul
    mv a0, s6
    mv a1, s7
    mv a2, s8
    mv a3, s0
    mv a4, s4
    mv a5, s11
    mv a6, s5
    jal matmul

    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix
    
    lw a0, 16(s1)
    mv a1, s5
    mv a2, s7
    mv a3, s11
    jal write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax

    mv a0, s5
    mul a1, s7, s11
    jal argmax
    mv s4, a0

    # Print classification
    bnez s2, done
    mv a1, s4
    jal print_int

    # Print newline afterwards for clarity
    li a1, '\n'
    jal print_char

done:
    # free space
    mv a0, s0
    jal free
    mv a0, s3
    jal free
    mv a0, s5
    jal free
    mv a0, s6
    jal free
    mv a0, s9
    jal free
    
    # store return value
    mv a0, s4
    
    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw s8, 32(sp)
    lw s9, 36(sp)
    lw s10, 40(sp)
    lw s11, 44(sp)
    lw ra, 48(sp)
    addi sp, sp, 52
    # End Epilogue

    ret
    
arg_number_fail:
    li a1, 72
    jal exit2
    
malloc_fail:
    li a1, 88
    jal exit2
