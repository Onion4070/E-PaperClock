#include <SPI.h>
// EPD
#include <Display_EPD_W21_spi.h>
#include <Display_EPD_W21.h>
#include <Ap_29demo.h>

// NTP
#include <WiFi.h>
#include <time.h>

void setup() {
    pinMode(A14, INPUT);  // BUSY
    pinMode(A15, OUTPUT); // RES
    pinMode(A16, OUTPUT); // DC
    pinMode(A17, OUTPUT); // CS
    // SPI
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    SPI.begin();

    // Serial
    Serial.begin(115200);
    Serial.println("E-Paper Clock");

    // WiFi設定
    const char* ssid = "your_ssid";
    const char* password = "your_password";

    // WiFi接続
    auto connect_wifi = [&]() {
        WiFi.begin(ssid, password);
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("Connected to WiFi");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    };
    connect_wifi();

    // NTP設定
    const char *ntpServer = "ntp.nict.jp"; // 日本標準時
    const long gmtOffset_sec = 9 * 3600;   // GMT+9 (日本時間)
    const int daylightOffset_sec = 0;      // サマータイムなし

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    Serial.println("Waiting for time synchronization...");
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        Serial.println("Time not synchronized yet. Retrying...");
        delay(1000);
    }

    Serial.println("Time synchronized successfully.");
}

void loop() {
#if 1 // Full screen update, fast update, and partial update demostration.
    /************Full display(3s)*******************/
    EPD_Init();                    // Full screen update initialization.
    EPD_WhiteScreen_ALL(gImage_1); // To Display one image using full screen update.
    EPD_DeepSleep();               // Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000);                   // Delay for 2s.

    /************Full display(2s)*******************/
    EPD_Init_Fast();               // Full screen update initialization.
    EPD_WhiteScreen_ALL(gImage_2); // To Display one image using full screen update.
    EPD_DeepSleep();               // Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000);                   // Delay for 2s.

#endif
#if 0 // Demo of using partial update to update the full screen, to enable this feature, please change 0 to 1.
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
#if 1 // Demo of using partial update to update the full screen, to enable this feature, please change 0 to 1.
    // After 5 partial updates, implement a full screen update to clear the ghosting caused by partial updatees.
    //////////////////////Partial update time demo/////////////////////////////////////
    EPD_Init();                     // Full screen update initialization.
    EPD_WhiteScreen_ALL(gImage_p1); // To Display one image using full screen update.
    EPD_Dis_PartAll(gImage_p1);
    EPD_Dis_PartAll(gImage_p2);
    EPD_Dis_PartAll(gImage_p3);
    EPD_Dis_PartAll(gImage_p4);
    EPD_Dis_PartAll(gImage_p5);
    EPD_Dis_PartAll(gImage_p6);
    EPD_DeepSleep(); // Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000);     // Delay for 2s.

    EPD_Init();              // Full screen update initialization.
    EPD_WhiteScreen_White(); // Clear screen function.
    EPD_DeepSleep();         // Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
    delay(2000);             // Delay for 2s.
#endif

    while (1)
        ; // The program stops here
}
