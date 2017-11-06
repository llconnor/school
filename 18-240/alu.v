/*	File: 		ALU.v
 *	Modified:	1998-11-20
 *	Project:	WileE240 Synthesizable Verilog Description
 *	Functionality:	Implements the WileE240 Arithmetic Logic Unit	
 * 
 *	Notes:
 *		The condition codes are ordered as ZCNV
 *
 *	Description:
 *		The ALU has a variety of functions, determined by
 *		the ALUop control signal.  See the comments below
 *		for a complete description of the ALU functionality.
 */

module ALU (out, CCout, inA, inB, ALUop);
  output [15:0] out;
  output [3:0]  CCout;
  input  [15:0] inA, inB;
  input  [3:0]  ALUop;
  reg    [15:0] out;
  reg           Z, C, N, V;
  reg    [3:0]  CCout;
  reg    [16:0] temp;

  always @(inA or inB or ALUop)
   begin
    case (ALUop)
      	4'b0000 : temp = inA;           /* Pass A */
      	4'b0001 : temp = inA + 1;       /* A+1 */
      	4'b0010 : temp = inA + inB;     /* A+B */
      	4'b0011 : temp = inA + inB + 1; /* A+B+1 */
      	4'b0100 : temp = inA - inB - 1; /* A-B-1 */
    	4'b0101 :                       /* A-B and set Carry */
		begin
			temp = inA - inB;	
			if (inA > inB) C = 0;
			else C = 1;
		end
      	4'b0110 : temp = inA - 1;       /* A-1 */
      	4'b0111 : temp = inB;           /* Pass B */
		default: temp = inA;      	
	endcase

      /* Now we set the carry bit for the
       * operations that didn't set it above
       */
      if( ALUop != 4'b0101 )
		begin
			C = temp[16];
		end

      out = temp[15:0];

      /* Now, set the rest of the condition codes */
      N = out[15];
      Z = (out == 16'h0000) ? 1 : 0;
      V = ( inA[15] & inB[15] & ~out[15] ) | ( ~inA[15] & ~inB[15] & out[15] ); 

      CCout = {Z, C, N, V};
   end
endmodule



