	lw 1,0,Five
	lw 2,0,Two
	addi 3,2,-560
	ori	3,3,-6500
	beq 3,0,Zero
	sub	3,3,2
Zero	nand	3,3,1
	halt
Five	.fill	5
Two	.fill	-2