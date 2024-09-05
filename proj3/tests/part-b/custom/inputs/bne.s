li t0, 5
li t1, 3
bne t0, t1, branch_target
li a0, 1
j end
branch_target:
li a0, 2
end:
