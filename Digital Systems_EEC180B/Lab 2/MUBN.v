module MUBN(SW, result, HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0 );
input [15:0] SW;

output [15:0] result;
output [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0;

parameter zero = 0;
wire [0:0]c0, c1, c2, c3, c4, c5, c6;
wire [7:0]s0, s1, s2, s3, s4, s5, s6;

wire [7:0] a, b;
wire [0:6] w7, w6, w5, w4, w3, w2, w1, w0;

//parameter off = 7'b111_1111;
assign a = SW[15:8];
assign b = SW[7:0];
assign HEX7 = w3;
assign HEX6 = w2;
assign HEX5 = w1;
assign HEX4 = w0;

assign HEX3 = w7;
assign HEX2 = w6;
assign HEX1 = w5;
assign HEX0 = w4;

//display A and B on 7seg
hex_7seg h7 (.hex(a[7:4]), .seg(w3));
hex_7seg h6 (.hex(a[3:0]), .seg(w2));
hex_7seg h5 (.hex(b[7:4]), .seg(w1));
hex_7seg h4 (.hex(b[3:0]), .seg(w0));

wire a0b0;


assign result[0] = a0b0;
assign result[1] = s0[0];
assign result[2] = s1[0];
assign result[3] = s2[0];
assign result[4] = s3[0];
assign result[5] = s4[0];
assign result[6] = s5[0];
assign result[7] = s6[0];
assign result[8] = s6[1];
assign result[9] = s6[2];
assign result[10] = s6[3];
assign result[11] = s6[4];
assign result[12] = s6[5];
assign result[13] = s6[6];
assign result[14] = s6[7];
assign result[15] = c6;

and(a0b0, a[0], b[0]);
wire a1b0;
and(a1b0, a[1], b[0]);
wire a2b0;
and(a2b0, a[2], b[0]);
wire a3b0;
and(a3b0, a[3], b[0]);
wire a4b0;
and(a4b0, a[4], b[0]);
wire a5b0;
and(a5b0, a[5], b[0]);
wire a6b0;
and(a6b0, a[6], b[0]);
wire a7b0;
and(a7b0, a[7], b[0]);

wire a0b1;
and(a0b1, a[0], b[1]);
wire a1b1;
and(a1b1, a[1], b[1]);
wire a2b1;
and(a2b1, a[2], b[1]);
wire a3b1;
and(a3b1, a[3], b[1]);
wire a4b1;
and(a4b1, a[4], b[1]);
wire a5b1;
and(a5b1, a[5], b[1]);
wire a6b1;
and(a6b1, a[6], b[1]);
wire a7b1;
and(a7b1, a[7], b[1]);


wire a0b2;
and(a0b2, a[0], b[2]);
wire a1b2;
and(a1b2, a[1], b[2]);
wire a2b2;
and(a2b2, a[2], b[2]);
wire a3b2;
and(a3b2, a[3], b[2]);
wire a4b2;
and(a4b2, a[4], b[2]);
wire a5b2;
and(a5b2, a[5], b[2]);
wire a6b2;
and(a6b2, a[6], b[2]);
wire a7b2;
and(a7b2, a[7], b[2]);

wire a0b3;
and(a0b3, a[0], b[3]);
wire a1b3;
and(a1b3, a[1], b[3]);
wire a2b3;
and(a2b3, a[2], b[3]);
wire a3b3;
and(a3b3, a[3], b[3]);
wire a4b3;
and(a4b3, a[4], b[3]);
wire a5b3;
and(a5b3, a[5], b[3]);
wire a6b3;
and(a6b3, a[6], b[3]);
wire a7b3;
and(a7b3, a[7], b[3]);

wire a0b4;
and(a0b4, a[0], b[4]);
wire a1b4;
and(a1b4, a[1], b[4]);
wire a2b4;
and(a2b4, a[2], b[4]);
wire a3b4;
and(a3b4, a[3], b[4]);
wire a4b4;
and(a4b4, a[4], b[4]);
wire a5b4;
and(a5b4, a[5], b[4]);
wire a6b4;
and(a6b4, a[6], b[4]);
wire a7b4;
and(a7b4, a[7], b[4]);

wire a0b5;
and(a0b5, a[0], b[5]);
wire a1b5;
and(a1b5, a[1], b[5]);
wire a2b5;
and(a2b5, a[2], b[5]);
wire a3b5;
and(a3b5, a[3], b[5]);
wire a4b5;
and(a4b5, a[4], b[5]);
wire a5b5;
and(a5b5, a[5], b[5]);
wire a6b5;
and(a6b5, a[6], b[5]);
wire a7b5;
and(a7b5, a[7], b[5]);

wire a0b6;
and(a0b6, a[0], b[6]);
wire a1b6;
and(a1b6, a[1], b[6]);
wire a2b6;
and(a2b6, a[2], b[6]);
wire a3b6;
and(a3b6, a[3], b[6]);
wire a4b6;
and(a4b6, a[4], b[6]);
wire a5b6;
and(a5b6, a[5], b[6]);
wire a6b6;
and(a6b6, a[6], b[6]);
wire a7b6;
and(a7b6, a[7], b[6]);

wire a0b7;
and(a0b7, a[0], b[7]);
wire a1b7;
and(a1b7, a[1], b[7]);
wire a2b7;
and(a2b7, a[2], b[7]);
wire a3b7;
and(a3b7, a[3], b[7]);
wire a4b7;
and(a4b7, a[4], b[7]);
wire a5b7;
and(a5b7, a[5], b[7]);
wire a6b7;
and(a6b7, a[6], b[7]);
wire a7b7;
and(a7b7, a[7], b[7]);


nbitFA m1 ( .b7(a7b1), .a7(zero), .b6(a6b1), .a6(a7b0), .b5(a5b1), .a5(a6b0), .b4(a4b1), .a4(a5b0) ,.b3(a3b1), .a3(a4b0), .b2(a2b1), .a2(a3b0), .b1(a1b1), .a1(a2b0), .b0(a1b0), .a0(a0b1), .sum(s0), .ovf(c0));
nbitFA m2 ( .b7(a7b2), .a7(c0), .b6(s0[7]), .a6(a6b2), .b5(s0[6]), .a5(a5b2), .b4(s0[5]), .a4(a4b2) ,.b3(s0[4]), .a3(a3b2), .b2(s0[3]), .a2(a2b2), .b1(s0[2]), .a1(a1b2), .b0(s0[1]), .a0(a0b2), .sum(s1), .ovf(c1));
nbitFA m3 ( .b7(a7b3), .a7(c1), .b6(s1[7]), .a6(a6b3), .b5(s1[6]), .a5(a5b3), .b4(s1[5]), .a4(a4b3) ,.b3(s1[4]), .a3(a3b3), .b2(s1[3]), .a2(a2b3), .b1(s1[2]), .a1(a1b3), .b0(s1[1]), .a0(a0b3), .sum(s2), .ovf(c2));
nbitFA m4 ( .b7(a7b4), .a7(c2), .b6(s2[7]), .a6(a6b4), .b5(s2[6]), .a5(a5b4), .b4(s2[5]), .a4(a4b4) ,.b3(s2[4]), .a3(a3b4), .b2(s2[3]), .a2(a2b4), .b1(s2[2]), .a1(a1b4), .b0(s2[1]), .a0(a0b4), .sum(s3), .ovf(c3));
nbitFA m5 ( .b7(a7b5), .a7(c3), .b6(s3[7]), .a6(a6b5), .b5(s3[6]), .a5(a5b5), .b4(s3[5]), .a4(a4b5) ,.b3(s3[4]), .a3(a3b5), .b2(s3[3]), .a2(a2b5), .b1(s3[2]), .a1(a1b5), .b0(s3[1]), .a0(a0b5), .sum(s4), .ovf(c4));
nbitFA m6 ( .b7(a7b6), .a7(c4), .b6(s4[7]), .a6(a6b6), .b5(s4[6]), .a5(a5b6), .b4(s4[5]), .a4(a4b6) ,.b3(s4[4]), .a3(a3b6), .b2(s4[3]), .a2(a2b6), .b1(s4[2]), .a1(a1b6), .b0(s4[1]), .a0(a0b6), .sum(s5), .ovf(c5));
nbitFA m7 ( .b7(a7b7), .a7(c5), .b6(s5[7]), .a6(a6b7), .b5(s5[6]), .a5(a5b7), .b4(s5[5]), .a4(a4b7) ,.b3(s5[4]), .a3(a3b7), .b2(s5[3]), .a2(a2b7), .b1(s5[2]), .a1(a1b7), .b0(s5[1]), .a0(a0b7), .sum(s6), .ovf(c6));

//display P = A X B on 7 seg
hex_7seg h3 (.hex(result[15:12]), .seg(w7));
hex_7seg h2 (.hex(result[11:8]), .seg(w6));
hex_7seg h1 (.hex(result[7:4]), .seg(w5));
hex_7seg h0 (.hex(result[3:0]), .seg(w4));

endmodule