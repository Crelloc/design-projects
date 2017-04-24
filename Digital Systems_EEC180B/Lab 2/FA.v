module FA(a, b, ovf, sum);//part 2 of lab2


input  [7:0] a, b;
output [0:0]ovf;
output [7:0]sum;
parameter zero = 0;
wire [0:0] Cout0, Cout1, Cout2, Cout3, Cout4, Cout5, Cout6, Cout7;
assign ovf = Cout7;

bit_adder a1(.a(a[0]), .b(b[0]), .Cin(zero), .Cout(Cout0), .sum(sum[0]));
bit_adder a2(.a(a[1]), .b(b[1]), .Cin(Cout0), .Cout(Cout1), .sum(sum[1])); 
bit_adder a3(.a(a[2]), .b(b[2]), .Cin(Cout1), .Cout(Cout2), .sum(sum[2]));
bit_adder a4(.a(a[3]), .b(b[3]), .Cin(Cout2), .Cout(Cout3), .sum(sum[3]));
bit_adder a5(.a(a[4]), .b(b[4]), .Cin(Cout3), .Cout(Cout4), .sum(sum[4]));
bit_adder a6(.a(a[5]), .b(b[5]), .Cin(Cout4), .Cout(Cout5), .sum(sum[5]));
bit_adder a7(.a(a[6]), .b(b[6]), .Cin(Cout5), .Cout(Cout6), .sum(sum[6]));
bit_adder a8(.a(a[7]), .b(b[7]), .Cin(Cout6), .Cout(Cout7), .sum(sum[7]));

 

endmodule