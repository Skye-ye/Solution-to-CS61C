li t0, -1  # This is the largest unsigned 32-bit value
li t1, 5
bltu t1, t0, branch_target
li a0, 1
j end
branch_target:
li a0, 2
end:
