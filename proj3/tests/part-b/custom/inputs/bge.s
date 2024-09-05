li t0, 7
li t1, 5
bge t0, t1, branch_target
li a0, 1
j end
branch_target:
li a0, 2
end:
