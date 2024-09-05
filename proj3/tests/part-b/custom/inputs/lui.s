lui t0, 0x12345  # This will load 0x12345000 into t0
li t1, 0x12345000  # Expected value
beq t0, t1, lui_ok
j lui_fail
lui_ok:
    li a0, 1  # Test passed
    j lui_end
lui_fail:
    li a0, 0  # Test failed
lui_end:
