#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "segment.h"
#include <Wire.h>
#include <RTClib.h>

#define SERIAL_STOP_BYTE 0x0D       //data line terminator in USART transmission

int anodes[] = {5, 6, 10, 11};      //don't change this order!!!
int timeValues[] = {1, 2, 3, 4};    //@todo: set this to 0 - //display segments
//int anodesIterator = 0;             //iterator of anodes - reflects anode number in use

int data[255];
int byteCounter = 0;

bool getTimeFlag;
bool echoTimeFlag;

Segment *Disp;
RTC_DS1307 rtc;

#define R3_PIN       A0
#define SPEAKER_PIN  11

#define T2_WGM   0b010
#define T2_COMA  0b01
#define T2_CS    0b111


//#define WGM21
//#define WGM20
//#define COM2A1
//#define CS20
//#define OCIE2A

void setup() 
{
  Serial.begin(9600);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  rtc.writeSqwPinMode(SquareWave1HZ);
  
//  EIMSK = 1 << INT0;
//  MCUCR = 1 << ISC01 | 1 << ISC00;

//#define CHANGE 1
//#define FALLING 2
//#define RISING 3

  EICRA = (EICRA & ~((1<<ISC00) | (1<<ISC01))) | (CHANGE << ISC00);
  EIMSK |= (1<<INT0);

  pinMode(2, INPUT);
  pinMode(14, OUTPUT); // светодиод двоеточия

//  attachInterrupt(digitalPinToInterrupt(2), test, CHANGE);

  Disp = new Segment(anodes);
  Disp->setDigits(timeValues);
  
//  TCCR0 = (T2_COMA << 6) | (T2_WGM & 0b011);
//  TCCR0 = ((T2_WGM & 0b100) << 1) | T2_CS;
//  OCR2A = 255;
  
//  TCCR0 &= B 1111 1000;
//  TCCR0 |= B00000001;
//  
//  TCCR2 &= B11111000;1
//  TCCR2 |= B00000001;

  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 0<<CS21 | 0<<CS20;  // предделитель 64 (AS2 = 0 ???)
  TIMSK2 = 1<<TOIE2;                     // разрешение прерываний по 2 таймеру

//setupPWMOutput();

  
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);

  // 0x22 - мерцает
  int bright = 0x23;
  
//  analogWrite(5, bright);
//  analogWrite(6, bright);
//  analogWrite(10, bright);
//  analogWrite(11, bright);
  getTimeFlag = false;
  echoTimeFlag = false;
}

ISR(TIMER2_OVF_vect)
{
  Disp->indicate();
}

ISR(INT0_vect)
{
  Disp->blink();
  getTimeFlag = true;
  echoTimeFlag = true;
}

void test() {
  Disp->blink();
  getTimeFlag = true;
  echoTimeFlag = true;
}


//void setupPWMOutput()                             // Настройка таймера 2 в режим быстрого ШИМ
//{
//  TCCR2A = 0;
//  TCCR2A |= _BV(WGM21) | _BV(WGM20);              // Режим быстрого ШИМ (TOP = 0xFF)
//  TCCR2A |= _BV(COM2A1);                          // Сброс OCR2A при совпадении, 
//                                                  // установка в нижней точке (не-инверсный режим)
//  TCCR2B = 0;
//  TCCR2B |= _BV(CS20);                            // Частота тактового сигнала - предделитель /1
//  TIMSK2 |= _BV(OCIE2A);                          // Разрешить прерывание по совпадению TIMER2_COMPA_vect
//}

void serialEvent() 
{
  while (Serial.available()) 
  {
    // get the new byte:
    int inChar = (int)Serial.read();
    Serial.write(inChar);
    Serial.write(';');
//    display(inChar);
    data[byteCounter] = inChar;
    byteCounter++;
    if (SERIAL_STOP_BYTE == inChar) {
      runCommand();
      byteCounter = 0;
//      data[byteCounter+1] = {0};
    }
  }
}

int runCommand()
{
  Serial.write("run");
  Serial.write(data[0]);
  Disp->display(data[0]);
  delay(1000);
  Serial.write(data[1]);
  Disp->display(data[1]);
  delay(1000);
  switch (data[0])
  {
    case 1: //show number
      Disp->display(data[1]);
      delay(1000);
//      data[] = {};
      break;
    case 2: //brightness
//      showO();
//      delay(1000);
//      analogWrite(data[1], data[2]);
      analogWrite(5, data[2]);
      analogWrite(6, data[2]);
      analogWrite(10, data[2]);
      analogWrite(11, data[2]);
      break;
    default:
      Disp->display(-1);
      delay(1000);
  }
  Serial.write("stop");
  return 0;
}

void loop()
{
  if (getTimeFlag) {
    getTime();
    getTimeFlag = false;
  }
  
  if (echoTimeFlag) {
    echoTime();
    echoTimeFlag = false;
  }
}

inline void getTime() {
  DateTime now = rtc.now();
  int hours = now.hour();
  int minutes = now.minute();
  timeValues[0] = (int) hours / 10;
  timeValues[1] = hours % 10;
  timeValues[2] = (int) minutes / 10;
  timeValues[3] = minutes % 10;
  Disp->setDigits(timeValues);
}

inline void echoTime() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

