la t0, jalr_target  # Load address of jalr_target into t0
jalr ra, t0, 0  # Jump to address in t0 and store return address in ra
li a0, 0  # This should be skipped
j jalr_end
jalr_target:
    li t1, 4  # Expected return address offset (assuming 4-byte instructions)
    sub t2, ra, sp  # Calculate actual offset (assuming sp hasn't changed)
    beq t1, t2, jalr_ok
    j jalr_fail
jalr_ok:
    li a0, 1  # Test passed
    ret
jalr_fail:
    li a0, 0  # Test failed
    ret
jalr_end: