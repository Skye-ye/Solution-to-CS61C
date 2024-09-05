li t0, 3
li t1, 5
blt t0, t1, branch_target
li a0, 1
j end
branch_target:
li a0, 2
end:
