module RAMstate(SW, KEY, HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0);

input [17:0] SW; //switches
input [1:0] KEY; //KEY[1]= reset, KEY[0] = clock;
output [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0; //outputs for 7-seg LED

parameter off = 7'b111_1111;
parameter STATE0 = 1'b0, STATE1 = 1'b1;


wire [0:6] h6, h5, h4, h3, h2, h1, h0;
wire we, reset; //if set write to address, else dont.
reg we0, we1, state;		
wire clk;	//clock
wire sel; //sel ram to write to.
wire [3:0]address;	//address to store data
wire [7:0]data_in;	//data to store in Ram
wire [7:0]data_out1; //data in Ram1
wire [7:0]data_out0; //data in Ram0

assign reset = KEY[1];
assign HEX7 = off;
assign HEX6 = h6;
assign HEX5 = h5;
assign HEX4 = h4;
assign HEX3 = h3;
assign HEX2 = h2;
assign HEX1 = h1;
assign HEX0 = h0;
assign clk	= KEY[0];
assign we = SW[17];
assign sel = SW[16];
assign address = SW[13:10];
assign data_in = SW[7:0];


always @(negedge reset, posedge clk) begin: FSM

	if(reset == 1'b0)begin
		state <= #1 STATE0;
		
	end
	
	case(state)
		STATE0:
			if(sel)begin
				we1 <= we;
				we0 <= 1'b0;
				state <= #1 STATE1; 
			end	else begin
				we1 <= 1'b0;
				we0 <= we;
				state <= #1 STATE0;
			end
		STATE1:
			if(sel)begin
				we1 <= we;
				we0 <= 1'b0;
				state <= #1 STATE1; 
			end	else begin
				we1 <= 1'b0;
				we0 <= we;
				state <= #1 STATE0;
			end
		default: state <= #1 STATE0;
	endcase	
	
	

end

ram_single r0 (.q(data_out0), .a(address), .d(data_in), .we(we0), .clk(clk));
ram_single r1 (.q(data_out1), .a(address), .d(data_in), .we(we1), .clk(clk));

	
hex_7seg x6 (.hex(address), .seg(h6));
hex_7seg x5 (.hex(data_in[7:4]), .seg(h5));
hex_7seg x4 (.hex(data_in[3:0]), .seg(h4));
hex_7seg x3 (.hex(data_out1[7:4]), .seg(h3));
hex_7seg x2 (.hex(data_out1[3:0]), .seg(h2));
hex_7seg x1 (.hex(data_out0[7:4]), .seg(h1));
hex_7seg x0 (.hex(data_out0[3:0]), .seg(h0));


endmodule

