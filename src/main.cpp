/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
#include <CST816S.h>

// #define ESP321 1
#define Pico 1

#if defined(ESP321)
#define TFT_CS 5
// #define TFT_CS -1 // for display without CS pin
// #define TFT_DC 16
#define TFT_DC 27
// #define TFT_DC -1 // for display without DC pin (9-bit SPI)
// #define TFT_RST 17
#define TFT_RST 33
#define TFT_BL 22
CST816S touch(4, 16, 0, 17);	// sda, scl, rst, irq

#elif defined(Pico)
#define TFT_CS 19
// #define TFT_CS -1 // for display without CS pin
// #define TFT_DC 16
#define TFT_DC 21
// #define TFT_DC -1 // for display without DC pin (9-bit SPI)
// #define TFT_RST 17
#define TFT_RST 22
// #define TFT_BL 25/////
CST816S touch(33, 32, 0, 25);	// sda, scl, rst, irq
#endif


/*
 * Step 1: Initize one databus for your display
*/

// ESP32 hardware SPI, more customizable parameters
#if defined(ESP321)
 Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);

#elif defined(Pico)
 Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, 18 /* SCK */, 26 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);
#endif

 Arduino_GC9A01 *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
void page0();
void page1();
void page2();

void setup(void)
{
  Serial.begin(115200);
  touch.begin();

  gfx->begin();   //初始化LCD
  gfx->fillScreen(BLACK); //用黑色清除螢幕
}

static bool swipe = true;

void loop()
{
  static signed char state = 1;
  static String gesture = "NONE";
  char points = 1;
  int x=0,y=0,oldx=0,oldy=0;
  gfx->setCursor(0, 100);
  gfx->setTextSize(2);
  gfx->print("<<                >>");
  if (touch.available()) {
    if(touch.gesture()!= "NONE"){
      x = touch.data.x;
      y = touch.data.y;
      Serial.print(touch.gesture());
      Serial.print("\t");
      Serial.print(touch.data.points);
      Serial.print("\t");
      Serial.print(touch.data.event);
      Serial.print("\t");
      Serial.printf("x:%d\ty:%d\n",x,y);
      gesture = touch.gesture();
      points = touch.data.points;
      gfx->fillCircle(x,y,5,GREEN);
      oldx = x, oldy = y;
      Serial.printf("oldx:%d,oldy:%d",oldx,oldy);
    }
}
    gfx->fillCircle(oldx,oldy,5,BLUE);

  if ((x < 20 && (110 < y && y < 123)) || (gesture == "SWIPE LEFT" && points == 0)){
    gfx->fillScreen(BLACK);
    state -= 1;
    gesture = "NONE";
  }else if (x > 215 && (110 < y && y < 130) || (gesture == "SWIPE RIGHT" && points == 0)){
    gfx->fillScreen(BLACK);
    state += 1;
    gesture = "NONE";
  }

  if (state < 0) state = 0;
  else if (state>2) state = 2;
  // Serial.printf("\tstate:%d\n",state);

  switch (state){
  case 0:
    page0();
    break;
  case 1:
    page1();
    break;
  case 2:
    page2();
    break;
  default:
    break;
  }
}

void page0(){
  gfx->setTextColor(MAGENTA); //設定文字顏色
  gfx->setCursor(95, 60); //設定座標(x,y)
  gfx->setTextSize(2);
  gfx->print("Page0");   //顯示文字
  // delay(400);
  swipe=true;
}

void page1(){
  gfx->setTextColor(MAGENTA); //設定文字顏色
  gfx->setCursor(95, 60); //設定座標(x,y)
  gfx->setTextSize(2);
  gfx->print("M5Stack");   //顯示文字
  gfx->setTextSize(2);
  gfx->setCursor(50, 100);
  gfx->setTextColor(WHITE);
  gfx->print("Hello World !");
  gfx->setCursor(30, 140);
  gfx->setTextColor(YELLOW);
  gfx->print("Hello, M5Stack!");
  gfx->setCursor(30, 160);
  gfx->setTextColor(BLUE);
  gfx->print("www.M5Stack.com");
  swipe=true;
}

void page2(){
  gfx->setTextColor(MAGENTA); //設定文字顏色
  gfx->setCursor(95, 60); //設定座標(x,y)
  gfx->setTextSize(2);
  gfx->print("Page2");   //顯示文字
  swipe=true;
}

