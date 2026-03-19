#include <SPI.h>
//EPD
// #include "Display_EPD_W21_spi.h"
// #include "Display_EPD_W21.h"
// #include "Ap_29demo.h"  
#include <Display_EPD_W21_spi.h>
#include <Display_EPD_W21.h>
#include <Ap_29demo.h>

void setup() {
   pinMode(A14, INPUT);  //BUSY
   pinMode(A15, OUTPUT); //RES 
   pinMode(A16, OUTPUT); //DC   
   pinMode(A17, OUTPUT); //CS   
   //SPI
   SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); 
   SPI.begin ();  
}

//Tips//
/*
1.Flickering is normal when EPD is performing a full screen update to clear ghosting from the previous image so to ensure better clarity and legibility for the new image.
2.There will be no flicker when EPD performs a partial refresh.
3.Please make sue that EPD enters sleep mode when refresh is completed and always leave the sleep mode command. Otherwise, this may result in a reduced lifespan of EPD.
4.Please refrain from inserting EPD to the FPC socket or unplugging it when the MCU is being powered to prevent potential damage.)
5.Re-initialization is required for every full screen update.
6.When porting the program, set the BUSY pin to input mode and other pins to output mode.
*/
void loop() {
   unsigned char i;
#if 1 //Full screen update, fast update, and partial update demostration.
		 /************Full display(3s)*******************/
		  EPD_Init(); //Full screen update initialization.
			EPD_WhiteScreen_ALL(gImage_1); //To Display one image using full screen update.
			EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
			delay(2000); //Delay for 2s.

		 /************Full display(2s)*******************/
		  EPD_Init_Fast(); //Full screen update initialization.
			EPD_WhiteScreen_ALL(gImage_2); //To Display one image using full screen update.
			EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
			delay(2000); //Delay for 2s.

	#endif				
  #if 0    //Demo of using partial update to update the full screen, to enable this feature, please change 0 to 1.
	//After 5 partial updatees, implement a full screen update to clear the ghosting caused by partial updatees.
	//////////////////////Partial update  demo///////////////////////////////////// 
		EPD_Init(); //Full screen update initialization.			
		EPD_SetRAMValue_BaseMap(gImage_basemap); //Please do not delete the background color function, otherwise it will cause unstable display during partial update.		  	
		EPD_Init_Part(); //Partial update initialization.
		EPD_Dis_Part(0,0,gImage_num1,32,32,0); //partial display 1    //x_start,y_start,picData,column,line,mode
		EPD_Dis_Part(0,0,gImage_num2,32,32,1); //partial display 2
		EPD_Dis_Part(0,0,gImage_num3,32,32,1); //partial display 3
		EPD_Dis_Part(0,0,gImage_num4,32,32,1); //partial display 3
		EPD_Dis_Part(0,0,gImage_num5,32,32,1); //partial display 3
		EPD_DeepSleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000); //Delay for 2s.

		EPD_Init(); //Full screen update initialization.
		EPD_WhiteScreen_White(); //Clear screen function.
		EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
		delay(2000); //Delay for 2s.		
	
		#endif
  #if 1   //Demo of using partial update to update the full screen, to enable this feature, please change 0 to 1.
	//After 5 partial updates, implement a full screen update to clear the ghosting caused by partial updatees.
	//////////////////////Partial update time demo/////////////////////////////////////
			EPD_Init(); //Full screen update initialization.
		  EPD_WhiteScreen_ALL(gImage_p1); //To Display one image using full screen update.
			EPD_Dis_PartAll(gImage_p1); 
			EPD_Dis_PartAll(gImage_p2);
			EPD_Dis_PartAll(gImage_p3);
			EPD_Dis_PartAll(gImage_p4);  
			EPD_Dis_PartAll(gImage_p5); 
			EPD_Dis_PartAll(gImage_p6); 
			EPD_DeepSleep();//Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
			delay(2000); //Delay for 2s.

			EPD_Init(); //Full screen update initialization.
			EPD_WhiteScreen_White(); //Clear screen function.
			EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
			delay(2000); //Delay for 2s.
	#endif	
	
	#if 0 //Demonstration of full screen update with 180-degree rotation, to enable this feature, please change 0 to 1.
			/************Full display(2s)*******************/
			EPD_Init_180(); //Full screen update initialization.
			EPD_WhiteScreen_ALL(gImage_1); //To Display one image using full screen update.
			EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
			delay(2000); //Delay for 2s.
	#endif		
    while(1); // The program stops here   
}



//////////////////////////////////END//////////////////////////////////////////////////
