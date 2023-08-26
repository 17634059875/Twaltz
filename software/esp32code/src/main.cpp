#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Chrono.h>

#define oled_scl 18
#define oled_sda 17
#define oled_rst 21
#define DATA_INPUT_PIN 7
#define LED 35
#define EN 6
#define MODE 0
#define Vbias 5
#define backscatter 2

int ledState = LOW;
float p1 = 13.45;
float p2 = -127.6;
float p3 = 498.3;
float p4 = -1031;
float p5 = 1213;
float p6 = -803.9;
float p7 = 301.3;
float p8 = -100.3;

int v;
float volt;
float fitP;
float bit;
float plot;
float Pvef;
int KeyNum=1;
int k;
int T;
int T_1;
Chrono myChrono1;
Chrono myChrono2;
Chrono myChrono3;

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/oled_scl, /* data=*/oled_sda, /* reset=*/oled_rst);

void PinCount()
{
  v = analogRead(DATA_INPUT_PIN);
  volt = v * (3.3 / 4096.0);
  fitP = p1 * pow(volt, 7) + p2 * pow(volt, 6) + p3 * pow(volt, 5) + p4 * pow(volt, 4) + p5 * pow(volt, 3) + p6 * pow(volt, 2) + p7 * volt + p8;
}

void timetest()
{
  Serial.print(micros());
  Serial.print(",");
}

float average(float value)
{
  int len = 10;
  float sum = 0;
  float average = 0;
  for (int i = 0; i < len; i++)
  {
    sum = sum + value;
  }
  average = sum / len;
  return average;
}

void serialprint()
{
  Serial.print(v);
  Serial.print(",");
  Serial.print(",");
  Serial.print(volt);
  Serial.print(",");
  Serial.println(fitP);
}

void oledprint()
{
  u8g2.firstPage();
  for (; u8g2.nextPage();)
  {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.setCursor(0, 20);
    u8g2.print(String(v));
    u8g2.setCursor(0, 40);
    u8g2.print(volt);
    u8g2.print("V ");
    u8g2.setCursor(0, 60);
    u8g2.print(fitP);
    u8g2.println("dBm");
  }
}

void inter()
{
  if (fitP > -50 && ledState == LOW)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }
}

void comp(float Pvef)
{
  if (fitP > Pvef)
  {
    bit = 1;
  }
  else
    bit = 0;
}

void seriallin()
{
  Serial.print("/*");
  Serial.print(fitP);
  Serial.print(",");
  Serial.println(bit);
  Serial.print(",");
  Serial.println(volt);
  Serial.print("*/");
}

void VDC()
{
    if (digitalRead(MODE) == 0)
  {
    delay(500);
    KeyNum++;
    delay(500); 
    if (KeyNum >= 7)
    {
      KeyNum = 1;
    }
  }
  switch (KeyNum)
  {
  case 1:
    k = 10;
    break;
  case 2:
    k = 11;
    break;
  case 3:
    k = 12;
    break;
  case 4:
    k = 13;
    break;
  case 5:
    k = 14;
  case 6:
    k = 15;
    break;
  default:
    Serial.println("程序异常");
    break;
  }
}

void key()
{
  if (digitalRead(MODE) == 0)
  {
    delay(500);
    KeyNum++;
    delay(500); 
    if (KeyNum >= 7)
    {
      KeyNum = 1;
    }
  }
  switch (KeyNum)
  {
  case 1:
    Pvef = -50;
    break;
  case 2:
    Pvef = -40;
    break;
  case 3:
    Pvef = -30;
    break;
  case 4:
    Pvef = -20;
    break;
  case 5:
    Pvef = -10;
  case 6:
    Pvef = 0;
    break;
  default:
    Serial.println("程序异常");
    break;
  }
}

void Backscatter(int T)
{
  digitalWrite(backscatter, LOW);
  delayMicroseconds(2*T);
  digitalWrite(backscatter, HIGH);
  delayMicroseconds(1*T);
  digitalWrite(backscatter, LOW);
  delayMicroseconds(1*T);
  digitalWrite(backscatter, HIGH);
  delayMicroseconds(1*T);
  digitalWrite(backscatter, LOW);
  delayMicroseconds(3*T);
  digitalWrite(backscatter, HIGH);
  delayMicroseconds(2*T);
  digitalWrite(backscatter, LOW);
  delayMicroseconds(4*T);
  digitalWrite(backscatter, HIGH);
  delayMicroseconds(2*T);
}

void Backscatter1()
{
  for (int i = 0; i < 82*2; i++)
  {
    digitalWrite(backscatter, HIGH);
    delayMicroseconds(50);
    digitalWrite(backscatter, LOW);
    delayMicroseconds(50);
  }
  delayMicroseconds(8200);
  for (int i = 0; i < 82; i++)
  {
    digitalWrite(backscatter, HIGH);
    delayMicroseconds(50);
    digitalWrite(backscatter, LOW);
    delayMicroseconds(50);
  }
  delayMicroseconds(8200);
  for (int i = 0; i < 82*3; i++)
  {
    digitalWrite(backscatter, HIGH);
    delayMicroseconds(50);
    digitalWrite(backscatter, LOW);
    delayMicroseconds(50);
  }
  delayMicroseconds(8200*2);
  for (int i = 0; i < 82*4; i++)
  {
    digitalWrite(backscatter, HIGH);
    delayMicroseconds(50);
    digitalWrite(backscatter, LOW);
    delayMicroseconds(50);
  }
  delayMicroseconds(8200*18);
}

void Tunnel(int T_1)
{
  analogWrite(Vbias, 0);
  delayMicroseconds(2 * T_1);
  analogWrite(Vbias, k);
  delayMicroseconds(1 * T_1);
  analogWrite(Vbias, 0);
  delayMicroseconds(1 * T_1);
  analogWrite(Vbias, k);
  delayMicroseconds(1 * T_1);
  analogWrite(Vbias, 0);
  delayMicroseconds(3 * T_1);
  analogWrite(Vbias, k);
  delayMicroseconds(2 * T_1);
  analogWrite(Vbias,0); 
  delayMicroseconds(4*T_1);
  analogWrite(Vbias,k);  
  delayMicroseconds(2*T_1);
}

void chron()
{
  if (myChrono1.hasPassed(1000))
  {
    myChrono1.restart();
    PinCount();
    timetest();
    serialprint();
  }

  if (myChrono2.hasPassed(1000))
  {
    myChrono2.restart();
    oledprint();
  }

  if (myChrono3.hasPassed(1000))
  {
    myChrono3.restart();
    inter();
  }
}

void setup()
{
  pinMode(DATA_INPUT_PIN, INPUT);
  pinMode(EN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(Vbias, OUTPUT);
  pinMode(backscatter, OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(EN, HIGH);
  Serial.begin(115200);
  u8g2.begin();
}

void loop()
{
  // key();
  PinCount();
  // comp(Pvef);
  seriallin();

  // oledprint();

  // VDC();
  // Tunnel(10);

  // Backscatter(5);

  // Backscatter1();
}


