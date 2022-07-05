	lui	1,15
	lui	2,20
	lw	12,0,label	#comment
	nand	3,2,3	
	ori	4,0,123
	or	4,1,4
	beq	1,2,-3
	sub	1,3,4	
Exit	halt
label	.fill	80