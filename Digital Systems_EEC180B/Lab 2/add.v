module add	( SW ,HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0);
input [15:0] SW;
output [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0;

wire [0:6] w7, w6, w5, w4, w3, w2, w1, w0;

parameter off = 7'b111_1111;

assign HEX7 = off;
assign HEX6 = off;
assign HEX5 = off;
assign HEX4 = off;
assign HEX3 = w3;
assign HEX2 = w2;
assign HEX1 = w1;
assign HEX0 = w0;




hex_7seg h3 (.hex(SW[15:12]), .seg(w3));
hex_7seg h2 (.hex(SW[11:8]), .seg(w2));
hex_7seg h1 (.hex(SW[7:4]), .seg(w1));
hex_7seg h0 (.hex(SW[3:0]), .seg(w0));

endmodule