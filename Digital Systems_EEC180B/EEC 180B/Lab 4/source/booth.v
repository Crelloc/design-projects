//Thomas Turner 
//Lab3 Booths Multiplier

module booth(SW, KEY, HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0, LEDR, LEDG, oProduct);

input [17:0] SW; //switches
input [1:0] KEY; //KEY[0]= reset, KEY[1] = clock;
output [0:6] HEX7, HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0; //outputs for 7-seg LED
output [15:0] LEDR;	// Display binary count (active high) on Red LEDs
output [1:0] LEDG;
output [15:0] oProduct;
parameter n = 8;

parameter STATE1 = 2'b01, STATE2 = 2'b10, STATE3 = 2'b11;

reg [0:0] Done;
reg [1:0] state;
reg [3:0] count;	//count up to number 8 then set Done flag
//reg [n:0]  A, B, C; 
reg [17:0] product; 
wire [0:6] h6, h5, h4, h3, h2, h1, h0, h7;
wire Clock, Reset_n, start;
wire [7:0] B, C;
reg [8:0] A;  //b - multiplier, c- multiplicand

assign LEDR[15]= Done;
assign LEDG = state;
assign LEDR[3:0] = count;
assign B = SW[15:8];
assign C = SW[7:0];
assign Clock = KEY[1];
assign Reset_n = KEY[0];
assign start = SW[17];
assign oProduct = product[16:1];
 
assign HEX7 = h7;
assign HEX6 = h6;
assign HEX5 = h5;
assign HEX4 = h4;
assign HEX3 = h3;
assign HEX2 = h2;
assign HEX1 = h1;
assign HEX0 = h0;


reg [8:0] Mplier, Mpcand, twosMpcand;

always @(C)begin
	Mpcand = {C[7], C}; //sign extend to 9 bits
	twosMpcand = ~Mpcand + 1;
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
		  product[17:9] <= 9'b0000_00000;
		  product[8:1] <= B; //note that product is 18 bits long 
		  product[0] <= 1'b0;
		  //next state
		  count <= 4'b0000;
		  Done <= 1'b0;
		  state <= #1 STATE2;
		end
	
		else begin //Done flag is set
		  Done <= 1'b0;
		  state <= #1 STATE1;
		end
      end
      
      STATE2: begin
      
		
		
		if(product[1:0]==2'b00 || product[1:0]==2'b11)begin
		   product <= {product[17], product[17:1]};
		   if(count >= 4'b0111)begin
			Done <= 1'b1; //set done flag
			state <= #1 STATE1; //next state is state 1
		   end
		   
		   else begin //otherwise count++ and next state is state2
			count <= count + 1;  
			state <= #1 STATE2;
		   end
		end
		
		else if(product[1:0] == 2'b01)begin
			
		  //add C to A
			
			product[17:9] <= product[17:9] + Mpcand;
			
			state <= #1 STATE3; //next state is state3
		end
		
		else if(product[1:0] == 2'b10)begin
		   
		   //add 2's compl. of Mpcand(C) to A
		   
		   product[17:9] <= product[17:9]+ twosMpcand;
		   
		   state <= #1 STATE3; //next state is state3
		end
	
      end
      
      STATE3: begin
		product <= {product[17], product[17:1]};
	   if(count >= 4'b0111)begin
		Done <= 1'b1; //set done flag
		state <= #1 STATE1; //next state is state 1
	   end
	   
	   else begin //otherwise count++ and next state is state2
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