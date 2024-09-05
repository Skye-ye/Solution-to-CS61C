li t0, 10
li t1, 5
bgeu t0, t1, branch_target
li a0, 1
j end
branch_target:
li a0, 2
end:
