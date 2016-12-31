#if !defined(__SERIAL_H__)
#define __SERIAL_H__
#include "Particle.h"
extern char esc;

 int escape(){
   Serial1.print(esc); delay(1000);
 }

int ReadClear(){
  Serial1.print("\r"); delay(1500);
  Serial1.readStringUntil('\r');
}

int SendString(String send){
  Serial1.print(send);
  delay(1500);
}

String ReadLine(){
  String line = Serial1.readStringUntil('\r');
  return line;
}
#endif
