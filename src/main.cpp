#include <SPI.h>
// EPD
#include <Display_EPD_W21_spi.h>
#include <Display_EPD_W21.h>
#include <Ap_29demo.h>

// NTP
#include <WiFi.h>
#include <time.h>

// 文字描画
#include <FrameBuffer.h>
#include <EPDConsole.h>

// 時計ウィジェット
#include <ClockWidget.h>

// WiFi設定情報をここに記載
#include <secret.h>
/* secret.h (shuld be placed in include/secret.h)
static const char* ssid = "your_ssid";
static const char* password = "your_password";
*/

FrameBuffer fb;
EPDConsole console(fb);
ClockWidget cw(200, 150, 100);

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
    while (!Serial) {
        delay(10);
    }

    console.clear();
    console.println("E-Paper Clock");

    EPD_Init();
    EPD_Display(fb.buf);
    EPD_DeepSleep();

    // WiFi接続
    auto connect_wifi = [&]() {
        WiFi.begin(ssid, password);
        console.print("Connecting to WiFi... ");
        EPD_Init_Fast();
        EPD_Display(fb.buf);
        EPD_DeepSleep();

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        }

        console.println("Done.");
        console.print("IP address: ");
        console.println(WiFi.localIP().toString().c_str());
        EPD_Init_Fast();
        EPD_Display(fb.buf);
        EPD_DeepSleep();
    };
    connect_wifi();

    // NTP設定
    const char *ntpServer = "ntp.nict.jp"; // 日本標準時
    const long gmtOffset_sec = 9 * 3600;   // GMT+9 (日本時間)
    const int daylightOffset_sec = 0;      // サマータイムなし

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    console.print("NTP server: ");
    console.println(ntpServer);

    console.println("Synchronizing time...");
    EPD_Init_Fast();
    EPD_Display(fb.buf);
    EPD_DeepSleep();

    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        EPD_Init_Fast();
        console.println("Time not synchronized yet. Retrying...");
        EPD_Display(fb.buf);
        EPD_DeepSleep();
        delay(1000);
    }

    char timebuf[64], datebuf[64];
    strftime(timebuf, sizeof(timebuf), "%H:%M:%S", &timeinfo);
    strftime(datebuf, sizeof(datebuf), "%Y-%m-%d", &timeinfo);

    console.println("Time synchronized successfully.");
    console.print("Current time: ");
    console.println(timebuf);
    console.print("Current date: ");
    console.println(datebuf);
    EPD_Init();
    EPD_Display(fb.buf);
    EPD_DeepSleep();
    delay(2000);

    fb.blitImage(0 , 32, 32, 32, gImage_num1, 2);
    fb.blitImage(64, 32, 32, 32, gImage_num2, 2);
    fb.blitImage(128, 32, 32, 32, gImage_maohao, 2);
    fb.blitImage(192, 32, 32, 32, gImage_num3, 2);
    fb.blitImage(256, 32, 32, 32, gImage_num4, 2);
    EPD_Init();
    EPD_Display(fb.buf);
    EPD_DeepSleep();
    EPD_Init();
    EPD_SetRAMValue_BaseMap(fb.buf); // 背景色の設定（部分更新の安定化のために必要）

    int bytes_per_row = 400 / 8; // 1行あたりのバイト数
    int line_y = 0;
    int line_h = 8;
    char int_buf[4];

    console.setCursor(0, 0);
    console.println("Clock Widget Demo");
    for (int i = 0; i < 5; i++) {
        snprintf(int_buf, sizeof(int_buf), "%d", i+1);
        console.print(int_buf);
        console.println(" Updating time every second...");
    };

    EPD_Init_Part(); // 部分更新の初期化
    for (int i = 0; i < 6; i++) {
        EPD_Dis_Part(0, i * line_h, fb.buf + i * line_h * bytes_per_row, line_h, 400, 0);
        delay(1000);
    }

    console.setCursor(0, 0);
    console.println("Overwite test");
    for (int i = 0; i < 5; i++) {
        snprintf(int_buf, sizeof(int_buf), "%d", i+1);
        console.print(int_buf);
        console.println(" Overwritting time every second...");
    };
    for (int i = 0; i < 6; i++) {
        EPD_Dis_Part(0, i * line_h, fb.buf + i * line_h * bytes_per_row, line_h, 400, 1);
        delay(1000);
    }

    EPD_DeepSleep();
    EPD_Init_Fast();
    EPD_Display(fb.buf);
    EPD_DeepSleep();

    // EPD_Init();
    // EPD_Display(fb.buf); // 初期表示
    // EPD_DeepSleep();

    // EPD_Init();
    // EPD_WhiteScreen_White();
    // EPD_DeepSleep();
    // delay(2000);
}

void loop() {
#if 0 // Full screen update, fast update, and partial update demostration.
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
#if 0 // Demo of using partial update to update the full screen, to enable this feature, please change 0 to 1.
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