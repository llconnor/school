
START	LDI R2, HEAD
	LDI R3, $0
	PUSH R3
	PUSH R2
	JSR SUM



	.ORG $2000
SUM	LDSF R3, $2
	LDSF R2, $1
	LDR R1, R2
	ADD R3, R1
	INCR R2
	LDR R0, R2
	BRZ FINISH
	PUSH R3
	PUSH R0
	JSR SUM
FINISH	RTN

	.ORG $3000
HEAD	.DW  $1
	.DW FRED

	.ORG $3231
FRED	.DW $27
	.DW SALLY

	.ORG $4242
SALLY	.DW $FE
	.DW SPOT

	.ORG $4747
SPOT	.DW $100
	.DW $0

SUM	.ORG $5000
	.DW $0
