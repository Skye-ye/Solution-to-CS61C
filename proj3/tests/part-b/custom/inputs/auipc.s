auipc t0, 0x12345  # This will load PC + 0x12345000 into t0
li t1, 0x12345000  # Expected upper bits
srli t2, t0, 12    # Shift right to isolate upper bits
beq t1, t2, auipc_ok
j auipc_fail
auipc_ok:
    li a0, 1  # Test passed
    j auipc_end
auipc_fail:
    li a0, 0  # Test failed
auipc_end:
