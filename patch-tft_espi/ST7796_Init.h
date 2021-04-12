
// This is the command sequence that initialises the ST7796 driver
//
// This setup information uses simple 8 bit SPI writecommand() and writedata() functions
//
// See ST7735_Setup.h file for an alternative format

#define TFT_INIT_DELAY 0
{
	delay(120);

	writecommand(0x01); //Software reset
	delay(120);

	writecommand(0x11); //Sleep exit                                            
	delay(120);

	// same
	writecommand(0xF0); //Command Set control                                 
	writedata(0xC3);    //Enable extension command 2 partI
	
	// same
	writecommand(0xF0); //Command Set control                                 
	writedata(0x96);    //Enable extension command 2 partII
	
	// same
	writecommand(0x36); //Memory Data Access Control MX, MY, RGB mode                                    
	writedata(0x48);    //X-Mirror, Top-Left to right-Buttom, RGB  
	
	// different
	writecommand(0x3A); //Interface Pixel Format                                    
	// OLD
	writedata(0x55);    //Control interface color format set to 16
	// NEW
	//writedata(0x50);    //Control interface color format set to 16
	
	// same
	writecommand(0xB4); //Column inversion 
	writedata(0x01);    //1-dot inversion

	// NEW !!!!!
/*
	writecommand(0xB1);//FRMCTR1     
	writedata(0x80);   //FRS[D7-D4], DIVA[D1-D0] 81 for 15Hz
	writedata(0x10);   //RTNA[4:0]
*/

	// different
	writecommand(0xB6); //Display Function Control
	// OLD
	writedata(0x80);    //Bypass
	// NEW
	//writedata(0x20); //set rgb interface and DE mode -----------> NOT WORKING
	writedata(0x02);    //Source Output Scan from S1 to S960, Gate Output scan from G1 to G480, scan cycle=2
	writedata(0x3B);    //LCD Drive Line=8*(59+1)

	// NEW
	writecommand(0XB0);  //Interface Mode Control  
	writedata(0x00); 

	// NEW
	writecommand(0xC0);//Power Control 1      
	writedata(0x80);  //AVDD ,AVCL  									   `
	writedata(0x64); //VGH=15V VGL=-10V  


	// NEW !!!!!
	writecommand(0xE8); //Display Output Ctrl Adjust
	writedata(0x40);
	writedata(0x8A);	
	writedata(0x00);
	writedata(0x00);
	writedata(0x29);    //Source eqaulizing period time= 22.5 us
	writedata(0x19);    //Timing for "Gate start"=25 (Tclk)
	writedata(0xA5);    //Timing for "Gate End"=37 (Tclk), Gate driver EQ function ON
	writedata(0x33);
	
	// different
	writecommand(0xC1); //Power control2                          
	// OLD
	//writedata(0x06);    //VAP(GVDD)=3.85+( vcom+vcom offset), VAN(GVCL)=-3.85+( vcom+vcom offset)
	// NEW   
	writedata(0x13);  //VOP=4.5V
	 
	// same
	writecommand(0xC2); //Power control 3                                      
	writedata(0xA7);    //Source driving current level=low, Gamma driving current level=High
	
	// different
	writecommand(0xC5); //VCOM Control
	// OLD
	//writedata(0x18);    //VCOM=0.9
	// NEW  
	writedata(0x10);   


	delay(120);
	
	/* NEW */
	writecommand(0xE8); //Display Output Ctrl Adjust     
	writedata(0x40);   
	writedata(0x8a);   
	writedata(0x00);   
	writedata(0x00);   
	writedata(0x29);   
	writedata(0x19);   
	writedata(0xA5);   
	writedata(0x33);   


	/*
	ST7796_DSI(st7796,0xe0,0xf0,0x0b,0x11,0x0b,0x0a,0x27,0x3c,0x55,
	0x51,0x37,0x15,0x17,0x31,0x35);

	ST7796_DSI(st7796,0xe1,0x4e,0x15,0x19,0x0b,0x09,0x27,0x34,0x32,
	0x46,0x38,0x14,0x16,0x26,0x2a);
	*/

	//ST7796 Gamma Sequence
	writecommand(0xE0); //Gamma"+"                                             
/*
	writedata(0xF0);
	writedata(0x09); 
	writedata(0x0b);
	writedata(0x06); 
	writedata(0x04);
	writedata(0x15); 
	writedata(0x2F);
	writedata(0x54); 
	writedata(0x42);
	writedata(0x3C); 
	writedata(0x17);
	writedata(0x14);
	writedata(0x18); 
	writedata(0x1B); 
*/
	writedata(0xF0);
	writedata(0x06);
	writedata(0x0B);
	writedata(0x07);
	writedata(0x06);
	writedata(0x05);
	writedata(0x2E);
	writedata(0x33);
	writedata(0x47);
	writedata(0x3A);
	writedata(0x17);
	writedata(0x16);
	writedata(0x2E);
	writedata(0x31);

	writecommand(0xE1); //Gamma"-"                                             
	/*
	writedata(0xE0);
	writedata(0x09); 
	writedata(0x0B);
	writedata(0x06); 
	writedata(0x04);
	writedata(0x03); 
	writedata(0x2B);
	writedata(0x43); 
	writedata(0x42);
	writedata(0x3B); 
	writedata(0x16);
	writedata(0x14);
	writedata(0x17); 
	writedata(0x1B);
*/
	writedata(0xF0);
	writedata(0x09);
	writedata(0x0D);
	writedata(0x09);
	writedata(0x08);
	writedata(0x23);
	writedata(0x2E);
	writedata(0x33);
	writedata(0x46);
	writedata(0x38);
	writedata(0x13);
	writedata(0x13);
	writedata(0x2C);
	writedata(0x32);

  	delay(120);
	
	// same
	writecommand(0xF0); //Command Set control                                 
	writedata(0x3C);    //Disable extension command 2 partI

	// same
	writecommand(0xF0); //Command Set control                                 
	writedata(0x69);    //Disable extension command 2 partII

	end_tft_write();
	delay(120);
	begin_tft_write();

	writecommand(0x29); //Display on                                          	
}