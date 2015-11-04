/**
 * Класс, отвечающий за отображение данных на блоке семисегментных индикаторов
 *
 */
#include "segment.h"

//         cathodes
//            a 7

//     f 3          b 8

//            g 4

//     e 9          c 13

//             d12

//      13, 14, 13', 14' - anodes
//      6   5   11   10

/**
 * В конструкторе назначим, какие ноги за что отвечают
 *
 */
Segment::Segment(int _anodes[])
{
  this->anodeIterator = 0;
  
//  this->anodes[0]   = 5;
//  this->anodes[1]   = 6;
//  this->anodes[2]   = 10;
//  this->anodes[3]   = 11;

  for (int i = 0; i < ANODES_TOTAL; i++) {
    this->anodes[i] = _anodes[i];    
}
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void Segment::setDigits(int digits[]) {
  for (int i = 0; i < ANODES_TOTAL; i++) {
    this->digits[i] = digits[i];
  }
}

void Segment::indicate() {
  this->off();
  this->display(this->digits[this->anodeIterator]);
  this->on();
}

void Segment::off() {
  digitalWrite(this->anodes[this->anodeIterator], LOW); 
  this->anodeIterator += 1;
  if (this->anodeIterator >= ANODES_TOTAL) {
    this->anodeIterator = 0;
  }
}

void Segment::on() {
  digitalWrite(this->anodes[this->anodeIterator], HIGH);
}

void Segment::blink() {
  PORTC ^= 0x01;
}

void Segment::display(int number)
{
  this->offAll();
  switch(number)
  {
    case 0: 
      show0();
//      delay(500);
      break;
    case 1: 
      show1();
//      delay(500);
      break;
    case 2: 
      show2();
//      delay(500);
      break;
    case 3: 
      show3();
//      delay(500);
      break;
    case 4: 
      show4();
//      delay(500);
      break;
    case 5:
      show5();
//      delay(500);
      break;
    case 6:
      show6();
//      delay(500);
      break;
    case 7:
      show7();
//      delay(500);
      break;
    case 8:
      show8();
//      delay(500);
      break;
    case 9:
      show9();
//      delay(500);
      break;
    default:
      showE();
//      delay(500);
      break;
  }
}

void Segment::show0()
{
//  PORTD &= B11101110;
//  PORTB &= B00110011;

  PORTD &= B01110111;
  PORTB &= B11001100;
  
//  digitalWrite(7, LOW);
//  digitalWrite(8, LOW);
//  digitalWrite(13, LOW);
//  digitalWrite(12, LOW);
//  digitalWrite(9, LOW);
//  digitalWrite(3, LOW);
}
void Segment::show1()
{
  PORTB &= B11011110;
  digitalWrite(13, LOW);
  digitalWrite(8, LOW);
}
void Segment::show2()
{
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(4, LOW);
  digitalWrite(9, LOW);
  digitalWrite(12, LOW);
}
void Segment::show3()
{
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(4, LOW); 
}
void Segment::show4()
{
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW);
}
void Segment::show5()
{
  digitalWrite(7, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW); 
}
void Segment::show6()
{
  digitalWrite(7, LOW);
  digitalWrite(3, LOW);
  digitalWrite(9, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(4, LOW);
}
void Segment::show7()
{
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW);
}
void Segment::show8()
{
  digitalWrite(7, LOW);
  digitalWrite(3, LOW);
  digitalWrite(8, LOW);
  digitalWrite(4, LOW);
  digitalWrite(9, LOW);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW); 
}
void Segment::show9()
{
  digitalWrite(7, LOW);
  digitalWrite(3, LOW);
  digitalWrite(8, LOW);
  digitalWrite(4, LOW);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}
void Segment::showE()
{
  digitalWrite(7, LOW);
  digitalWrite(4, LOW);
  digitalWrite(12, LOW);
  digitalWrite(3, LOW);
  digitalWrite(9, LOW); 
}
void Segment::offAll()
{
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
}
