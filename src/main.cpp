#include <SPI.h>
// EPD
#include <Display_EPD_W21_spi.h>
#include <Display_EPD_W21.h>

// NTP
#include <WiFi.h>
#include <time.h>

// 文字描画
#include <FrameBuffer.h>
#include <EPDConsole.h>
#include <ClockWidget.h>

// WiFi設定情報をここに記載
#include <secret.h>
/* secret.h (shuld be placed in include/secret.h)
static const char* ssid = "your_ssid";
static const char* password = "your_password";
*/

FrameBuffer fb;
EPDConsole console(fb);
ClockWidget cw(fb);

struct tm timeinfo;

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

    // EPD Console
    console.clear();
    console.println("E-Paper Clock");

    // 初回表示
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
    EPD_Init_Fast();
    EPD_Display(fb.buf);
    EPD_DeepSleep();

    // 時計表示に向け準備
    console.clear();
    EPD_Init();
    EPD_SetRAMValue_BaseMap(fb.buf);
}


void loop() {
    if (getLocalTime(&timeinfo)) {
        static int last_min = -1;
        if (timeinfo.tm_min != last_min) {
            last_min = timeinfo.tm_min;
            cw.update(&timeinfo);
        }
    }
    delay(10);
}