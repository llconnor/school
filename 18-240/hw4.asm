	.ORG $3000
X	.DW $A234
	.DW $CDEF
	.DW $7324
	.DW $1526
	.DW $0123
	.DW $0237
	.DW $78A0
	.DW $6543
	.DW $2A00
	.DW $2323

	.ORG $4000
Y	.DW $1000
	.DW $4000
	.DW $0234
	.DW $1998
	.DW $1303
	.DW $ABCD
	.DW $DEF0
	.DW $1290
	.DW $2111
	.DW $A213

	LDI R1, $0009
	LDI R2, $3000
	LDI R3, $4000
	LDI R6, $5000
START 	LDA R4, R2
	LDA R5, R3
	ADD R7, R4
	ADD R7, R3
	STR R6, R7
	DECR R1
	BRN EXIT
	INCR R2
	INCR R3
	INCR R6
	BRA START
EXIT	STOP
	
