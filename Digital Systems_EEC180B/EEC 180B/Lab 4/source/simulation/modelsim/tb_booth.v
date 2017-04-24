`timescale 1ns / 1ps

module tb_booth;

wire [17:0]SW;
wire [1:0] KEY;		// KEY[1] = Clock, KEY[0] = Reset_n
wire [15:0] LEDR;	// Display binary count (active high) on Red LEDs
wire [1:0] LEDG;	// Display Clock on LEDG[1], Reset_n on LEDG[0]
parameter n=8; // nbit Booth multiplier
parameter num_vectors=8;
reg Clock, Resetn, Start;
wire Done;
reg [n-1:0] Mplier, Mcand;
wire [n+n-1:0] Product;
reg [n+n-1:0] vectors [0:num_vectors-1];
integer i;
wire [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0;
assign KEY[1:0] = {Clock, Resetn};
assign SW[17] = Start;
assign SW[15:8] = Mplier;
assign SW[7:0] = Mcand;
assign Done = LEDR[15];
booth UUT (.SW(SW),.KEY(KEY), .HEX7(HEX7), .HEX6(HEX6), .HEX5(HEX5), .HEX4(HEX4), .HEX3(HEX3), .HEX2(HEX2), .HEX1(HEX1), .HEX0(HEX0), .LEDR(LEDR), .LEDG(LEDG), .oProduct(Product));

initial // Clock generator
  begin
    Clock = 1'b0;
    forever #20 Clock = ~Clock;    // Clock period = 40 ns
  end
initial  // Test stimulus
  begin
    Resetn = 1'b0;	//synchronous reset of state machine
    Start = 1'b0;// set Start to ‘false’
    #80 Resetn = 1'b1; //reset low for 2 Clock periods
      
    $readmemb ("testvecs", vectors);// read testvecs file
    for (i=0; i<num_vectors; i=i+1) begin
       {Mplier, Mcand} = vectors[i];// load Mplier, Mcand
       #20 Start = 1'b1; //Start =‘true’
       #80 Start = 1'b0;// After 2 clock cycles, reset Start
       wait (Done==1);
       wait (Done==0);
       $display("Mplier=%h, Mcand=%h, Product=%h",Mplier,Mcand,Product);
    end
  end
endmodule
