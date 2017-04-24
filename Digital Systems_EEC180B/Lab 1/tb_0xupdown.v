`timescale 1ns / 1ps

module tb_HEXupdown;

wire [0:0] SW;		// Updown switch 1=up, 0=down
wire [1:0] KEY;		// KEY[1] = Clock, KEY[0] = Reset_n
wire [31:0] LEDR;	// Display binary count (active high) on Red LEDs
wire [1:0] LEDG;	// Display Clock on LEDG[1], Reset_n on LEDG[0]
wire [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0;
reg Clock, Reset_n, Updown;

assign KEY[1:0] = {Clock, Reset_n};
assign SW[0] = Updown;


HEXupdown UUT (
	      .SW(SW),
	      .KEY(KEY),
	      .LEDR(LEDR),
	      .LEDG(LEDG),
	      .HEX7(HEX7),
	      .HEX6(HEX6),
	      .HEX5(HEX5),
	      .HEX4(HEX4),
	      .HEX3(HEX3),
	      .HEX2(HEX2),
	      .HEX1(HEX1),
	      .HEX0(HEX0)
	      );

initial			// Clock generator
 begin
   Clock = 0;
   forever #20 Clock = ~Clock;
 end

initial			// Test stimulus
 begin
   Reset_n = 0;		// reset counter
   #30 Reset_n = 1;
 end

endmodule
