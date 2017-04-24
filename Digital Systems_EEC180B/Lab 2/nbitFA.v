module nbitFA( b7, a7, b6, a6, b5, a5, b4, a4 ,b3, a3, b2, a2, b1, a1, b0, a0, sum, ovf);
//nsum sum of 4 bit adder
input [0:0] b7, a7, b6, a6, b5, a5, b4, a4 , b3, a3, b2, a2, b1, a1, b0, a0;
output [7:0] sum;
output [0:0] ovf;

parameter zero = 0;
wire [0:0] Cout0, Cout1, Cout2, Cout3, Cout4, Cout5, Cout6, Cout7;
assign ovf = Cout7;


bit_adder x0 (.a(a0), .b(b0), .Cin(zero), .Cout(Cout0), .sum(sum[0]));
bit_adder x1 (.a(a1), .b(b1), .Cin(Cout0), .Cout(Cout1), .sum(sum[1])); 
bit_adder x2 (.a(a2), .b(b2), .Cin(Cout1), .Cout(Cout2), .sum(sum[2]));
bit_adder x3 (.a(a3), .b(b3), .Cin(Cout2), .Cout(Cout3), .sum(sum[3]));
bit_adder x4 (.a(a4), .b(b4), .Cin(Cout3), .Cout(Cout4), .sum(sum[4]));
bit_adder x5 (.a(a5), .b(b5), .Cin(Cout4), .Cout(Cout5), .sum(sum[5])); 
bit_adder x6 (.a(a6), .b(b6), .Cin(Cout5), .Cout(Cout6), .sum(sum[6]));
bit_adder x7 (.a(a7), .b(b7), .Cin(Cout6), .Cout(Cout7), .sum(sum[7]));


endmodule