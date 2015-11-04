#include <Arduino.h>

#define ANODES_TOTAL 4              //total anodes in schema

class Segment {
  public:
    Segment(int[4]); 
    void display(int);
    void setDigits(int[4]);
    void indicate();
    void blink();
    
  private:
    int anodeIterator;
    int* anodes = new int[4];
    int* digits = new int[4];
    
    void off();
    void on();
    
    void show0();
    void show1();
    void show2();
    void show3();
    void show4();
    void show5();
    void show6();
    void show7();
    void show8();
    void show9();
    void showE();
    void offAll();
};
