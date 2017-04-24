module bit_adder(a, b, Cin, Cout, sum);

input [0:0] a, b, Cin;
output [0:0] Cout, sum;
reg [1:0] temp;

assign Cout = temp[1];
assign sum = temp[0];
 
always @(a or b or Cin)
	temp <= a + b + Cin;
	
endmodule