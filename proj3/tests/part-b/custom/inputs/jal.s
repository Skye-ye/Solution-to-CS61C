jal ra, jal_target  # Jump to jal_target and store return address in ra
li a0, 0  # This should be skipped
j jal_end
jal_target:
    li t0, 4  # Expected return address offset (assuming 4-byte instructions)
    sub t1, ra, sp  # Calculate actual offset (assuming sp hasn't changed)
    beq t0, t1, jal_ok
    j jal_fail
jal_ok:
    li a0, 1  # Test passed
    ret
jal_fail:
    li a0, 0  # Test failed
    ret
jal_end: