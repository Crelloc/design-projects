//Thomas Turner
//Jesse  Anzelc
//Lab5 Radix-4 Booths Multiplier

module booth2(SW, Mplier1, Mpcand1, KEY, HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0, LEDR, LEDG, oProduct);

parameter n = 16;
input [17:0] SW; //switches
input [n-1:0] Mplier1, Mpcand1;
input [1:0] KEY; //KEY[0]= reset, KEY[1] = clock;
output [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0; //outputs for 7-seg LED
output [15:0] LEDR;	// Display binary count (active high) on Red LEDs
output [1:0] LEDG;
output [n+n-1:0] oProduct;

parameter LIMIT = (n/2)-1;
parameter STATE1 = 2'b01, STATE2 = 2'b10;

reg [0:0] Done;
reg [1:0] state;
reg [n+n-1:0] count;	
//reg [n:0]  A, B, C; 
reg [n+2+n+1-1:0] product; 
wire [0:6] h6, h5, h4, h3, h2, h1, h0, h7;
wire Clock, Reset_n, start;
wire [n-1:0] B, C;
reg [n+1:0] A;  //b - multiplier, c- multiplicand

assign LEDR[15]= Done;
assign LEDG = state;
assign LEDR[14:0] = count;
assign B = Mplier1;
assign C = Mpcand1;
assign Clock = KEY[1];
assign Reset_n = KEY[0];
assign start = SW[17];
assign oProduct = product[n+n:1];
 
assign HEX7 = h7;
assign HEX6 = h6;
assign HEX5 = h5;
assign HEX4 = h4;
assign HEX3 = h3;
assign HEX2 = h2;
assign HEX1 = h1;
assign HEX0 = h0;


reg [n+1:0] Mplier, Mpcand, doubleMpcand;
reg [n+1:0] sum;

always @(product,C)begin
	Mpcand = {C[n-1],C[n-1], C}; //
	doubleMpcand = Mpcand << 1;
	
	case(product[2:0])
		3'b000, 3'b111:
			sum = product[n+2+n+1-1:n+1];
		3'b001, 3'b010:
			sum = product[n+2+n+1-1:n+1] + Mpcand;
		3'b011:
			sum = product[n+2+n+1-1:n+1] + doubleMpcand;
		3'b100:
			sum = product[n+2+n+1-1:n+1] - doubleMpcand;
		3'b101, 3'b110:
			sum = product[n+2+n+1-1:n+1] - Mpcand;
		default:
			sum = product[n+2+n+1-1:n+1];
	
	endcase
end 

always @(negedge Reset_n, posedge Clock) begin: FSM

  if(!Reset_n) begin
	product <= 0;
    //count <= 4'b0000; //initialize count=0;
    //Done <= 1'b0;	//initialize Done=0;
    state <= #1 STATE1; //initialize next state to be STATE1
    
  end
  
  else begin
    case(state)
    
      STATE1: begin
    	if (start) begin
		  //Load 
		  product[n+2+n+1-1:n+1] <= {(n+2){1'b0}};
		  product[n:1] <= B; //note that product is 18 bits long 
		  product[0] <= 1'b0;
		  //next state
		  count <= 0;
		  Done <= 1'b0;
		  state <= #1 STATE2;
		end
	
		else begin //Done flag is set
		  Done <= 1'b0;
		  state <= #1 STATE1;
		end
      end
      
      STATE2: begin
      
		product <= {sum[n+1], sum[n+1], sum, product[n:2]};
		if(count >= LIMIT)begin
			state <= #1 STATE1;
			Done <= 1;
		end
		else begin
		
			count <= count + 1;
			state <= #1 STATE2;
			
		end
	
      end
      
     
	endcase
  end
end



hex_7seg x7 (.hex(SW[15:12]), .flag(Done), .print(1),.seg(h7) );
hex_7seg x6 (.hex(SW[11:8]), .flag(Done), .print(1), .seg(h6));
hex_7seg x5 (.hex(SW[7:4]), .flag(Done), .print(1), .seg(h5));
hex_7seg x4 (.hex(SW[3:0]), .flag(Done), .print(1), .seg(h4));
hex_7seg x3 (.hex(product[16:13]), .flag(Done), .print(1), .seg(h3));
hex_7seg x2 (.hex(product[12:9]), .flag(Done), .print(1), .seg(h2));
hex_7seg x1 (.hex(product[8:5]), .flag(Done), .print(1), .seg(h1));
hex_7seg x0 (.hex(product[4:1]), .flag(Done), .print(1), .seg(h0));


endmodule