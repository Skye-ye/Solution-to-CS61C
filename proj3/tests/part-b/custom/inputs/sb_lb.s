li t0, 0xEF
li t1, 0x404  # Address > 0x3E8
sb t0, 0(t1)
lb t2, 0(t1)
