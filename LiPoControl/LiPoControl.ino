/*******************************************************************************************
    Battery Controller for LiFePo
    Voltage control & alarming with ADS1115

    Functional overview
    - show the current voltages (min /max cell)
    - alarming on under- and over-voltage
    - Transmit infos to MQTT-Server

 ********************************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1015.h>
#include <U8g2lib.h>


/***********************( Definieren der Objekte und initialisieren )***********************/
/*
  Pinbelegung  Nokia 5110 Display:
  Display:      Nodemcu V3:
  GND           GND
  BL            3.3V (Beleuchtung)
  VCC           3.3V
  Clk           D5 (HSCLK SPI Clock Fix)
  Din           D7 (HMOSI MOSI Fix)
  DC            D3 (GPIO0)
  CE            D2 (GPIO4)
  RST           D1 (GPIO5)

  Pin ADS1115
  SCL           D6 (GPIO12)
  SCA           D4 (GPIO2)
*/

// ======================== GLOBAL ==================

// Nokia 5110 Display
U8G2_PCD8544_84X48_1_4W_HW_SPI u8g2(U8G2_R0, /* CE=*/ 4, /* DC=*/ 0, /* RST=*/ 5);
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

//int16_t adc0, adc1, adc2, adc3; //ADU read values
int16_t adVal[4];//, adc1, adc2, adc3; //ADU read values


/*****************************************( Setup )****************************************/
void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello!");

  u8g2.begin();
  ads.begin(2, 12);
}


/***************************************** AD read ****************************************/

void readAdu (void)
{
  adVal[0] = ads.readADC_SingleEnded(0);
  adVal[1] = ads.readADC_SingleEnded(1);
  adVal[2] = ads.readADC_SingleEnded(2);
  adVal[3] = ads.readADC_SingleEnded(3);

  Serial.print("AIN0: "); Serial.println(adVal[0]);
  Serial.print("AIN1: "); Serial.println(adVal[1]);
  Serial.print("AIN2: "); Serial.println(adVal[2]);
  Serial.print("AIN3: "); Serial.println(adVal[3]);
  Serial.println(" ");
  //delay(1000);
}


void showAdu() {
  static long lastUpdate=0;

  //Update per second
  if ((millis()-lastUpdate) < 1000)
    return;
  lastUpdate = millis();
  
  int dy = 10;
  int dx = 15;
  //int y = 8; //where to start Y

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g_font_profont11);  // select font
    u8g2.drawStr(0, 8, "ADS Werte");  // put string of display at position X, Y

    int y = 8 + dy;
    u8g2.drawStr(0, y, "1:"); u8g2.drawStr(dx, y, String(adVal[0]).c_str());
    y = 8 + (2*dy);
    u8g2.drawStr(0, y, "2:"); u8g2.drawStr(dx, y, String(adVal[1]).c_str());
    y = 8 + (3*dy);
    u8g2.drawStr(0, y, "3:"); u8g2.drawStr(dx, y, String(adVal[2]).c_str());
    y = 8 + (4*dy);
    u8g2.drawStr(0, y, "4:"); u8g2.drawStr(dx, y, String(adVal[3]).c_str());

/*
    for (byte i = 0; i < 4; i++) {
      y += dy;
      String s = String(i+1);
      s += ":";

    Serial.print("Y: "); Serial.println(y);
      Serial.print("Pre: "); Serial.println(s); 
      Serial.print("Val: "); Serial.println(String(adVal[i]));

      u8g2.drawStr(0, y, s.c_str()); u8g2.drawStr(10, y, String(adVal[i]).c_str());
    }*/

  } while ( u8g2.nextPage() );
}


void show1 (void) {

  // u8g2.firstPage() und die while Schlaufe ist nötig da die daten in mehreren schritten
  // an das Display geschickt werden.

  u8g2.clear();                         // Display Löschen
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_helvB08_tf); // Schrift definieren
    u8g2.setCursor(7, 22);              // Cursor auf Pixel X=7 und Y=22 setzen
    u8g2.print("Bastelgarage");         // Text schreiben
    u8g2.setCursor(10, 34);             // Cursor auf Pixel X=10 und Y=34 setzen
    u8g2.print("Onlineshop");           // Text schreiben
    u8g2.drawRFrame(0, 0, 84, 48, 8);   // Rechtek mit Rundung Zeichnen
    u8g2.sendBuffer();                  // Daten an Display schicken
  } while ( u8g2.nextPage() );

  delay(5000);
}

/*************************************(Hauptprogramm)**************************************/
void loop(void) {
  showAdu();
  readAdu();
}
