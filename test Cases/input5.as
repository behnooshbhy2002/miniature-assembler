	lw  2,0,Bad_Offset
	lw	1,2,Fill_2
	lw	4,2,Fill_1
label	add	4,4,1
	slt	5,3,4
	beq	5,0,label
	beq	5,1,END
END	halt
Fill_1	.fill	-5
Fill_2	.fill   END
Bad_Offset	.fill	10