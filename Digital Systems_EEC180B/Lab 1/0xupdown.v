// EEC180B Tutorial
//Thomas Turner

module HEXupdown (SW, KEY, LEDR, LEDG, HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0);
input [0:0] SW;		// Updown switch 1=up, 0=down
input [1:0] KEY;	// KEY[1] = Clock, KEY[0] = Reset_n
output [31:0] LEDR;	// Display binary count (active high) on Red LEDs
output [1:0] LEDG;	// Display Clock on LEDG[1], Reset_n on LEDG[0]
output [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0; //added to Display chars on 7sLED

wire Clock, Reset_n, Updown;
wire [0:6] w7, w6, w5, w4, w3, w2, w1, w0;
reg [31:0] Count; //32-bit reg


assign Clock = KEY[1];
assign Reset_n = KEY[0];
assign Updown = SW[0];
assign LEDR = Count;

assign LEDG[1:0] = {Clock, Reset_n};

assign HEX7 = w7;
assign HEX6 = w6;
assign HEX5 = w5;
assign HEX4 = w4;
assign HEX3 = w3;
assign HEX2 = w2;
assign HEX1 = w1;
assign HEX0 = w0;

always @(negedge Reset_n, posedge Clock)
  
	  if (Reset_n == 0)		// active-low asynchronous reset
		  Count <= 0;
	  else if (Updown)		// if Reset_n != 0, Clock rising edge
		  Count <= Count + 1;	// count up if Updown=1
	  else
		  Count <= Count - 1;	// count down in Updown=0
	  
 

	hex_7seg h7 (.hex(Count[31:28]), .seg(w7));
	hex_7seg h6 (.hex(Count[27:24]), .seg(w6));
	hex_7seg h5 (.hex(Count[23:20]), .seg(w5));
	hex_7seg h4 (.hex(Count[19:16]), .seg(w4));
	hex_7seg h3 (.hex(Count[15:12]), .seg(w3));
	hex_7seg h2 (.hex(Count[11:8]), .seg(w2));
	hex_7seg h1 (.hex(Count[7:4]), .seg(w1));
	hex_7seg h0 (.hex(Count[3:0]), .seg(w0));
		
endmodule
		
