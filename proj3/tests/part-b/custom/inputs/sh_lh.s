li t0, -1
li t1, 0x402  # Address > 0x3E8
sh t0, 0(t1)
lh t2, 0(t1)
