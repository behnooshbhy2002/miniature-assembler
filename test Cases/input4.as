	lw	1,0,Ten1
start	addi	2,0,Ten2
	or	3,1,2
	beq	3,0,start
	addi	4,1,100
	beq 3,0,Done
	sub	3,3,2
Done	halt
Ten1	.fill	10
Ten2	.fill	100