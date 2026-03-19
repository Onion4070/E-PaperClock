#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_

#define EPD_WIDTH   400
#define EPD_HEIGHT  300
#define EPD_ARRAY  EPD_WIDTH*EPD_HEIGHT/8  

//EPD
void EPD_Init(void);
void EPD_WhiteScreen_ALL(const unsigned char *datas);
void EPD_DeepSleep(void);
void EPD_Update(void);
void lcd_chkstatus(void);
void EPD_WhiteScreen_White(void);
//Display canvas function
void EPD_Display(unsigned char *Image); 
//LUT
void EPD_Init_Part(void);
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * new_data,unsigned int PART_COLUMN,unsigned int PART_LINE,unsigned char mode); //mode 0: first  1: other...
void EPD_SetRAMValue_BaseMap( const unsigned char * datas);
void EPD_standby(void);
void EPD_Init_180(void);
void EPD_Dis_PartAll(const unsigned char * datas);

void EPD_Init_Fast(void);


#endif
/***********************************************************
						end file
***********************************************************/


