	addi	1,0,num1
	addi	2,0,num2
	addi	3,0,num3
	slt	4,1,2
	beq	0,4,L1
	lw	5,2,0
L1	lw	5,1,0
	slt	4,5,3
	beq	0,4,L2
	lw	5,3,0
L2	lw	5,5,0
	halt
num1	.fill	5
num2	.fill	-2
num3	.fill	10