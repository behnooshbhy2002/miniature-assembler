	lw	1,0,Ten
	lw	2,0,One
	lw	3,0,Zero
Foo	add	3,3,2
	addi	2,2,One
	slt 4,2,1
	beq	4,0,End
End	halt
Ten	.fill 10
One	.fill 1
Zero	.fill 0