li t0, 4
li t1, 4
beq t0, t1, branch_target
li a0, 1
j end
branch_target:
li a0, 2
end:
