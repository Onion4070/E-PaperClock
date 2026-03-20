#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"

unsigned char oldData[15000];
void delay_xms(unsigned int xms) {
    delay(xms);
}

// UC8276
void EPD_Init(void) {
    EPD_W21_RST_0; // Module reset
    delay_xms(10); // At least 10ms delay
    EPD_W21_RST_1;
    delay_xms(10); // At least 10ms delay

    lcd_chkstatus();

    EPD_W21_WriteCMD(0x04);
    lcd_chkstatus(); // waiting for the electronic paper IC to release the idle signal

    EPD_W21_WriteCMD(0x00);  // panel setting
    EPD_W21_WriteDATA(0x1f); // LUT from OTP£¬KW-BF   KWR-AF	BWROTP 0f	BWOTP 1f

    EPD_W21_WriteCMD(0x61); // resolution setting
    EPD_W21_WriteDATA(EPD_WIDTH / 256);
    EPD_W21_WriteDATA(EPD_WIDTH % 256);
    EPD_W21_WriteDATA(EPD_HEIGHT / 256);
    EPD_W21_WriteDATA(EPD_HEIGHT % 256);

    EPD_W21_WriteCMD(0X50);  // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0x97); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

// UC8276
void EPD_Init_Fast(void) {
    EPD_W21_RST_0; // Module reset
    delay_xms(10); // At least 10ms delay
    EPD_W21_RST_1;
    delay_xms(10); // At least 10ms delay
    lcd_chkstatus();

    EPD_W21_WriteCMD(0x04);
    lcd_chkstatus();

    EPD_W21_WriteCMD(0x00);  // panel setting
    EPD_W21_WriteDATA(0x1f); // KW-bf   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_W21_WriteCMD(0xE0);
    EPD_W21_WriteDATA(0x02);

    EPD_W21_WriteCMD(0xE5);
    EPD_W21_WriteDATA(0x64);

    EPD_W21_WriteCMD(0X50);  // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0x97); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

// UC8276
void EPD_Init_Part(void) {
    EPD_W21_RST_0; // Module reset
    delay_xms(10); // At least 10ms delay
    EPD_W21_RST_1;
    delay_xms(10); // At least 10ms delay
    lcd_chkstatus();

    EPD_W21_WriteCMD(0x04);
    lcd_chkstatus();

    EPD_W21_WriteCMD(0x00);  // panel setting
    EPD_W21_WriteDATA(0x1f); // KW-bf   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_W21_WriteCMD(0xE0);
    EPD_W21_WriteDATA(0x02);

    EPD_W21_WriteCMD(0xE5);
    EPD_W21_WriteDATA(0xF1);

    EPD_W21_WriteCMD(0X50);  // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0x97); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

void EPD_standby(void) {
    EPD_W21_WriteCMD(0X50);  // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0xf7); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

    EPD_W21_WriteCMD(0X02); // power off
    lcd_chkstatus();        // waiting for the electronic paper IC to release the idle signal
    delay_xms(100);         //!!!The delay here is necessary,100mS at least!!!         //waiting for the electronic paper IC to release the idle signal
}

void EPD_DeepSleep(void) {
    EPD_W21_WriteCMD(0X02); // power off
    lcd_chkstatus();        // waiting for the electronic paper IC to release the idle signal
    delay_xms(100);         //!!!The delay here is necessary,100mS at least!!!
    EPD_W21_WriteCMD(0X07); // deep sleep
    EPD_W21_WriteDATA(0xA5);
}

// Full screen update function
void EPD_Update(void) {
    // update
    EPD_W21_WriteCMD(0x12); // DISPLAY update
    delay_xms(1);           //!!!The delay here is necessary, 200uS at least!!!
    lcd_chkstatus();        // waiting for the electronic paper IC to release the idle signal
}

void EPD_WhiteScreen_ALL(const unsigned char *datas) {
    unsigned int i;
    // Write Data
    EPD_W21_WriteCMD(0x10); // Transfer old data
    for (i = 0; i < EPD_ARRAY; i++) {
        EPD_W21_WriteDATA(0xFF); // Transfer the actual displayed data
    }
    EPD_W21_WriteCMD(0x13); // Transfer new data
    for (i = 0; i < EPD_ARRAY; i++) {
        EPD_W21_WriteDATA(datas[i]); // Transfer the actual displayed data
    }
    EPD_Update();
}

void EPD_WhiteScreen_White(void) {
    unsigned int i;
    // Write Data
    EPD_W21_WriteCMD(0x10); // Transfer old data
    for (i = 0; i < 15000; i++) {
        EPD_W21_WriteDATA(0xFF);
    }
    EPD_W21_WriteCMD(0x13); // Transfer new data
    for (i = 0; i < 15000; i++) {
        EPD_W21_WriteDATA(0xFF); // Transfer the actual displayed data
    }
    // update
    EPD_Update();
}

void lcd_chkstatus(void) {
    while (isEPD_W21_BUSY == 0);
}

// Partial update of background display, this function is necessary, please do not delete it!!!
void EPD_SetRAMValue_BaseMap(const unsigned char *datas) {
    unsigned int i;
    EPD_W21_WriteCMD(0x10); // write old data
    for (i = 0; i < EPD_ARRAY; i++) {
        EPD_W21_WriteDATA(0xFF); // is  different
    }
    EPD_W21_WriteCMD(0x13); // write new data
    for (i = 0; i < EPD_ARRAY; i++) {
        EPD_W21_WriteDATA(datas[i]);
    }
    EPD_Update();
} 
                                                                                                                                            // mode 0: first  1: other...
void EPD_Dis_Part(unsigned int x_start, unsigned int y_start, const unsigned char *new_data, unsigned int PART_COLUMN, unsigned int PART_LINE, unsigned char mode) {
    unsigned int datas, i;
    unsigned int x_end, y_end;

    x_end = x_start + PART_LINE - 1;
    x_end = 400 - x_end;
    x_start = 400 - x_start;

    y_end = y_start + PART_COLUMN - 1;
    datas = PART_LINE * PART_COLUMN / 8;

    EPD_W21_WriteCMD(0x91); // This command makes the display enter partial mode
    EPD_W21_WriteCMD(0x90); // resolution setting
    EPD_W21_WriteDATA(x_end / 256);
    EPD_W21_WriteDATA(x_end % 256); // x-start

    EPD_W21_WriteDATA(x_start / 256);
    EPD_W21_WriteDATA(x_start % 256 - 1); // x-end

    EPD_W21_WriteDATA(y_start / 256);
    EPD_W21_WriteDATA(y_start % 256); // y-start

    EPD_W21_WriteDATA(y_end / 256);
    EPD_W21_WriteDATA(y_end % 256 - 1); // y-end
    EPD_W21_WriteDATA(0x28);

    EPD_W21_WriteCMD(0x10); // writes Old data to SRAM for programming
    for (i = 0; i < datas; i++) {
        if (mode == 0)
            EPD_W21_WriteDATA(0xff);
        else
            EPD_W21_WriteDATA(oldData[i]);
    }

    EPD_W21_WriteCMD(0x13); // writes New data to SRAM.
    for (i = 0; i < datas; i++) {
        EPD_W21_WriteDATA(new_data[i]);
        oldData[i] = new_data[i];
    }
    // update
    EPD_Update();
    EPD_W21_WriteCMD(0X50);  // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0xd7); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

// Full screen partial update display
void EPD_Dis_PartAll(const unsigned char *datas) {
    unsigned int i;
    EPD_Init_Part(); // Partial update initialization.
    // Write Data
    EPD_W21_WriteCMD(0x10); // Transfer old data
    for (i = 0; i < EPD_ARRAY; i++) {
        EPD_W21_WriteDATA(oldData[i]); // Transfer the actual displayed data
    }
    EPD_W21_WriteCMD(0x13); // Transfer new data
    for (i = 0; i < EPD_ARRAY; i++) {
        EPD_W21_WriteDATA(datas[i]); // Transfer the actual displayed data
        oldData[i] = datas[i];
    }

    EPD_Update();
    EPD_W21_WriteCMD(0X50);  // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0xd7); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

////////////////////////////////Other newly added functions////////////////////////////////////////////
// Display rotation 180 degrees initialization
void EPD_Init_180(void) {
    EPD_W21_RST_0; // Module reset
    delay_xms(10); // At least 10ms delay
    EPD_W21_RST_1;
    delay_xms(10); // At least 10ms delay

    lcd_chkstatus();

    EPD_W21_WriteCMD(0x04);
    lcd_chkstatus(); // waiting for the electronic paper IC to release the idle signal

    EPD_W21_WriteCMD(0x00);  // panel setting
    EPD_W21_WriteDATA(0x13); // LUT from OTP£¬KW-BF   KWR-AF	BWROTP 0f	BWOTP 1f

    EPD_W21_WriteCMD(0x61); // resolution setting
    EPD_W21_WriteDATA(EPD_WIDTH / 256);
    EPD_W21_WriteDATA(EPD_WIDTH % 256);
    EPD_W21_WriteDATA(EPD_HEIGHT / 256);
    EPD_W21_WriteDATA(EPD_HEIGHT % 256);

    EPD_W21_WriteCMD(0X50);  // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0x97); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

void EPD_Display(unsigned char *Image) {
    unsigned int Width, Height, i, j;
    Width = (EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_W21_WriteCMD(0x10);
    for (j = 0; j < Height; j++) {
        for (i = 0; i < Width; i++) {
            EPD_W21_WriteDATA(0x00);
        }
    }

    EPD_W21_WriteCMD(0x13);
    for (j = 0; j < Height; j++) {
        for (i = 0; i < Width; i++) {
            EPD_W21_WriteDATA(Image[i + j * Width]);
        }
    }
    EPD_Update();
}

/***********************************************************
                        end file
***********************************************************/
