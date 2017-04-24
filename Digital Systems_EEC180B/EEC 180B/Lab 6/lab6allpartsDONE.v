
module hex_7seg (
input [3:0] hex,
output reg [0:6] seg
);

//012_3456 (segments are active-low)
parameter ZERO = 7'b000_0001;
parameter ONE = 7'b100_1111;
parameter TWO = 7'b001_0010;
parameter THREE = 7'b000_0110;
parameter FOUR = 7'b100_1100;
parameter FIVE = 7'b010_0100;
parameter SIX = 7'b010_0000;
parameter SEVEN = 7'b000_1111;
parameter EIGHT = 7'b000_0000;
parameter NINE = 7'b000_0100;
parameter A = 7'b000_1000;
parameter B = 7'b110_0000;
parameter C = 7'b011_0001;
parameter D = 7'b100_0010;
parameter E = 7'b011_0000;
parameter F = 7'b011_1000;

always @(hex)

case (hex)
0: seg = ZERO;
1: seg = ONE;
2: seg = TWO;
3: seg = THREE;
4: seg = FOUR;
5: seg = FIVE;
6: seg = SIX;
7: seg = SEVEN;
8: seg = EIGHT;
9: seg = NINE;
10: seg = A;
11: seg = B;
12: seg = C;
13: seg = D;
14: seg = E;
15: seg = F;
endcase

endmodule

module lab6(SW, KEY, LEDG, LEDR, HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0);

input [17:0] SW; //used to select number from RAM, 1-10
input [3:0] KEY; //used for Clock, Resetn, and to select 1/sqrt (10), reciprocal (01), or sqrt (11) --> for now only reciprocal will be implemented

output [8:0] LEDG; //used to display iteration count 
output [17:0] LEDR; //used to display input count
output [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0; //used to output our answer


reg [31:0] X [31:0]; //memory to store IEEE 754 format numbers (2^5 test cases*, each 32 bits), will use 1-10 for sure (lab specifications)
reg [31:0] D; //register that holds the X we want to work with
reg [24:0] Dz; //register will hold D fixed point D value
reg [31:0] F; //this will hold our answer, will be iterated upon and become more accurate
reg [22:0] xi; //23 bit mantissa
reg [49:0] term1; //xi*D, 0.1Md * 1.Mx0, 25 bits * 25 bits = 50 bits
reg [25:0] ans1; //00.top 24 bits of term1 --> 26 bits
reg [25:0] term2; //2-xi*D
reg [51:0] term3; //xi(26-bit)*term2(26-bit)
reg [22:0] Si;
reg [31:0] S;

reg [3:0] incount; //input count, which input are we calculating the recip. for?
reg [3:0] itcount; //iteration count, what iteration are we on?
reg [1:0] ex; //extra bits
reg [1:0] mode;
//localparam a = 32'b0_100_0000_0010_0101_0111_1110_1011_1011; //(2.58586), floating point format
//localparam b = 32'b1_100_0000_1011_1010_0010_1110_1000_1000; //(-5.81818)
//localparam c = 32'b0_100_0000_1000_0111_1100_0001_1110_1000; //(4.24242)
localparam magical_number = 32'h5f3759df;

localparam delimeter = 7;

localparam a = 48'h2_95FAEBC408E; //(2.58586), fixed point format
localparam b = 48'h5_D1743E963DC; //(5.81818), fixed point format
localparam c = 48'h4_3E0F3CB3E57; //(4.24242), fixed point format

localparam er = 8'b0111_1111; //exponent for reciprocal and inverse square root , always 127 when input x is 0.5 < x < 1
localparam es = 8'b0111_1110; //exponent for square root, always 126 when input x is 0.5 < x < 1
localparam two = 26'h2000000;

wire Clock, Resetn;
reg done;
assign {Clock, Resetn} = KEY[1:0]; //using KEY for clock unless told otherwise!
assign LEDG[7:0] = itcount; //let the user know what iteration we are doing, iteration = LED value - 1
assign LEDR[4:0] = incount; //let the user know which input is being manipulated, X = 1 --> 10
assign LEDG[8] = done; //display done signal on LED

//reg [2:0] state; //we will use this after we get reciprocal and square root goin'
//localparam state3 = 3'b100;
//localparam state2 = 3'b010;
//localparam state1 = 3'b001;

initial begin //let's try an initial block, other group used this
	//SET X test cases here
//	X[1] = 32'b0011_1111_0100_0000_0000_0000_0000_0000;  //0.75, test case #1
//	X[2] = 32'b00111111001100001010001111010111; //0.69
//	X[3] = 32'b00111111010011110101110000101001; //0.81
//	X[4] = 32'b00111111001000010100011110101110; //0.63
//	X[5] = 32'b00111111010111101011100001010010; //0.87
//	X[6] = 32'b00111111000100011110101110000101; //0.57
//	X[7] = 32'b00111111011011100001010001111011; //0.93
//	X[8] = 32'b00111111000000101000111101011100; //0.51
//	X[9] = 32'b00111111011111010111000010100100; //0.99
//	X[10] = 32'b00111111000000000000000000000000; //0.5
	
//	
//	X[1] = 32'h3F40_0000; //0.75 --> 1.33333333
//	//0x3faa_aaab
//	X[2] = 32'h3F00_0000; //0.5 --> 1
//	//0x3f80_0000
//	X[3] = 32'h3F5F_DF3B; //0.8745 --> 1.1435105774728416238
//	//0x3f92_5e8e
//	X[4] = 32'h3F26_6666; //0.65 --> 1.5384615384615385
//	//0x3fc4_ec4f
//	X[5] = 32'h3F73_3333; //0.95 --> 1.0526315789473684
//	//0x3f86_bca2
//	X[6] = 32'h3F32_DE01; //0.6987 --> 1.4312294260770001
//	//0x3fb7_3287
//	X[7] = 32'h3F06_21FB; //0.523956 --> 1.9085572070937254
//	//0x3ff4_4b9a
//	X[8] = 32'h3F1E_B852; //
//	
//	X[9] = 32'h3F0B_49E0; //
//	
//	X[10] = 32'h3F76_6EB9; //
//	

	
	X[1] = 32'h3f40_0000; //0.75 --> 1.33333333
	//0x3faa_aaab
	X[2] = 32'h3F00_0000; //0.5 --> 1
	//0x3f80_0000
	X[3] = 32'h3F5F_DF3B; //0.8745 --> 1.1435105774728416238
	//0x3f92_5e8e
	X[4] = 32'h3F26_6666; //0.65 --> 1.5384615384615385
	//0x3fc4_ec4f
	X[5] = 32'h3F73_3333; //0.95 --> 1.0526315789473684
	//0x3f86_bca2
	X[6] = 32'h3F32_DE01; //0.6987 --> 1.4312294260770001
	//0x3fb7_3287
	X[7] = 32'h3F06_21FB; //0.523956 --> 1.9085572070937254
	//0x3ff4_4b9a
	X[8] = 32'h3F1E_B852; //
	
	X[9] = 32'h3F0B_49E0; //
	
	X[10] = 32'h3F76_6EB9; //
	
	D = X[1]; //load first X value into D
	incount = 1; //set input count to first value
end

reg [51:0] temp1;
reg [51:0] temp2;
reg [25:0] temp3;
reg [51:0] temp4;
reg [51:0] temp5;
reg [51:0] sqrRoot;
localparam three = 26'h3000000;
localparam half = 26'b00100000000000000000000000;
always @(itcount, done) begin
	if (itcount >= 1 && !done) begin
		//xi = xi*(2 - xi*D);
		case(mode)
			2'h1: begin
				Dz[24:0] = {2'b01,D[22:0]};
				term1[49:0] = {1'b1,xi,1'b0}*Dz[24:0]; //xi*D, 0.1Md * 1.Mx0, 25 bits * 25 bits = 50 bits
				ans1[25:0] = term1[49:24]; //top 26 bits of term1
				term2[25:0] = two - term1[49:24]; //2-xi*D, 26-bits
				term3[51:0] = {1'b1,xi,2'b00}*term2; //(1+23+2) bits * 26 bits = 52 bits
			end
			2'h2: begin
				//Si = 0.5 *  Si * (3 - D*Si*Si);
				
				temp5 = {2'b01, Si, 1'b0} * {2'b01, Si, 1'b0};  // decimal place of temp5 is 24+24 bits from the right
				temp4 = temp5[49:24] * {3'b001, D[22:0] }; //
				  
				temp3 = three - temp4[49:24] ;
				
				temp2 = half * temp3; //decimal is two positions from left ---> XX.xxxxx
				temp1 = {2'b01, Si, 1'b0} * temp2[49:24];
			end
			
			2'h3: begin
		      //Si = 0.5 *  Si * (3 - D*Si*Si);
            
           
			temp5 = {2'b01, Si, 1'b0} * {2'b01, Si, 1'b0};  // decimal place of temp5 is 24+24 bits from the right
			temp4 = temp5[49:24] * {3'b001, D[22:0] }; //
			  
			temp3 = three - temp4[49:24] ;
			
			temp2 = half * temp3; //decimal is two positions from left ---> XX.xxxxx
			temp1 = {2'b01, Si, 1'b0} * temp2[49:24];
			
            sqrRoot = {3'b001, D[22:0]} * temp1[49:24]; //sqrRoot
			end
			
			default: ;
		endcase
	end

//	if (itcount == 3) begin //perform second iteration
//		term1[49:0] = {1'b1,xi,1'b0}*{2'b01,D[22:0]}; //xi*D, 0.1Md * 1.Mx0, 25 bits * 25 bits = 50 bits
//		ans1[25:0] = {2'b00, term1[49:26]}; //00.top 24 bits of term1 --> 26 bits
//		term2[25:0] = 26'h2000000 - ans1; //2-xi*D, 26-bits
//		term3[51:0] = {xi,1'b0}*term2; //(25+1) bits * 26 bits = 52 bits
//		F[22:0] = term3[49:27];
//	end	
	
//	
//	if (itcount == 4) begin //
//		D = X[incount + 1]; //load next input
//		F = X[incount + 1]; //display next input
//	end
end

localparam sp1 = 32'h3fa30f28; //1.2739
localparam sp2 = 32'h3e958106; //.292
reg [51:0]temp6;
reg [25:0]temp7;

//always @(D)begin

	//S = D;
	//S = S >> 1;
	//S = magical_number - S ;
	
	//1.2739-0.292x ----->polynomial for inv of the sqr root
	//temp4 = {2'b01 , sp2[22:0]} * {2'b01, D[22:0]}; //0.292*x
	//temp5 =  {1'b1,sp1[22:0], 2'b00} - temp4[49:24]; // 
	
//end

always @(posedge Clock, negedge Resetn) begin
	if (!Resetn) begin
		case(SW[1:0])
			2'b01: mode <= 2'h1; //reciprocal
			2'b10: mode <= 2'h2; //inverse of square root
			2'b11: mode <= 2'h3; //square root
			default: begin 
				mode <= 2'h0;
				itcount <= delimeter;
			end
		endcase
		D <= X[1]; //load first X value into D
		F <= X[1]; //set answer to input to show user what we are analyzing
		incount <= 1; //set input count to first value
		itcount <= 0; //reset iteration counter
		done <= 0; //clear done signal
	end
	else begin
		if (incount <= 10) begin
			if (itcount == 0) begin //load
				case(mode)
					2'b01: begin
						F[30:23] <= er; //calculate exponent for F (will always be 127 when input D is: 0.5 < D < 1
						xi <= 23'b0; //Mantissa of xi, 23 zeros, xi = 1 to start (x0)
						F[31] <= D[31]; //calculate sign bit for F (pass through)
						itcount <= itcount + 1; //increment iteration counter
						done <= 0; //make sure done signal is low
					end
					2'b10: begin
						F[30:23] <= er;
						F[31] <= D[31];
						//Si <=  S[22:0]; //using magical number
						Si <= 23'b0; //Xo=1
						//Si <= temp5[24:3]; //using polynomial
						//F[22:0] = temp5[24:3];
						itcount <= itcount + 1; //increment iteration counter
						done <= 0; //make sure done signal is low
				
				
					end
					2'b11: begin
					   F[30:23] <= es;
				      F[31] <= D[31];
                  //Si <=  S[22:0]; //using magical number
                  Si <= 23'b0; //Xo=1
                  //Si <= temp5[24:3]; //using polynomial
                  //F[22:0] = temp5[24:3];
                  itcount <= itcount + 1; //increment iteration counter
                  done <= 0; //make sure done signal is low
					end
					
					default: ;
				endcase
			end
			
			if (itcount >= 1 && itcount <=(delimeter-2)) begin //perform iterations
				case(mode)
					2'b01: begin
						F[22:0] <= term3[48:26];
						xi <= term3[48:26];
					end
					2'b10: begin
						F[22:0] <= temp1[47:25];
						Si <= temp1[47:25];
					end
					2'b11: begin
				      F[22:0] <= sqrRoot[46:24];
					  Si <= temp1[47:25];
					end
					
					default: ;
					
				endcase
				itcount <= itcount + 1; //increment iteration counter
				done <= 0; //make sure done signal is low
			end
			
			if (itcount == (delimeter-1)) begin //perform last iteration
				case(mode)
					2'b01: begin
						F[22:0] <= term3[48:26];
						xi <= term3[48:26];
					end
					2'b10: begin
						F[22:0] <= temp1[47:25];
						Si <= temp1[47:25];
					end
					2'b11: begin
				       F[22:0] <= sqrRoot[46:24];
						Si <= temp1[47:25];
					end
					
					default: ;
					
				endcase
				
				itcount <= itcount + 1; //increment iteration counter
				done <= 1; //set done signal
				
			end
			
			
			if (itcount == delimeter) begin //load and display next input
				D <= X[incount + 1]; //load next input
				F <= X[incount + 1]; //display next input
				incount <= incount + 1; //update input counter
				itcount <= 0; //reset iteration counter
				done <= 0; //reset done signal
			end
		end //end if incount
	end //end else
end //end always



hex_7seg h7(F[31:28],HEX7);
hex_7seg h6(F[27:24],HEX6);
hex_7seg h5(F[23:20],HEX5);
hex_7seg h4(F[19:16],HEX4);
hex_7seg h3(F[15:12],HEX3);
hex_7seg h2(F[11:8],HEX2);
hex_7seg h1(F[7:4],HEX1);
hex_7seg h0(F[3:0],HEX0);

endmodule


//module RSU(key, X); //this module is currently unused
//
//input [1:0] key; //won't actually be used until after we finish the reciprocal part
//input [31:0] X; //from memory for initial value at least
//
//
//endmodule
