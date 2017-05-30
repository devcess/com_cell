
#include "readfunc.h" // how to include other files
#include "serial.h"

bool resetFlag = false;
String DOUBLE_SPACE = "  ";
String SINGLE_SPACE = " ";
String chcal = "";
String info = "";
String point1 = "";
String point2 = "";
String point3 = "";
String point4 = "";
String point5 = "";
String point6 = "";
String point7 = "";
String point8 = "";
String point9 = "";
String point10 = "";
String rawdata;

constexpr int ARRAY_SIZE = 5;
int a = 27;
int b = 13;
int noloop;
int po;
char esc = a;
char car = b;


SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
    Particle.function("cal", calibrate);
    Particle.function("device", device);
    Particle.function("caldata", caldata);
    Particle.function("reboot", reboot);
    Particle.variable("point1", point1);
    Particle.variable("point2", point2);
    Particle.variable("point3", point3);
    Particle.variable("point4", point4);
    Particle.variable("point5", point5);
    Particle.variable("point6", point6);
    Particle.variable("point7", point7);
    Particle.variable("point8", point8);
    Particle.variable("point9", point9);
    Particle.variable("point10", point10);

    Particle.connect();
    Serial1.begin(9600);
    Serial.begin(9600);
    delay(10000);
    Serial1.print(esc); delay(1000);
    Serial1.print("DB \r\n");
    noloop = 0;
    }


void PubSerial(){
  if (Serial1.available())
  {
    rawdata = ReadLine();

    //String rawdata = ReadLine();
    if      (noloop == 1){ ; }
    else if (rawdata == "" or rawdata.charAt(0) != '['){ delay(2000); }
    else{
        Serial.print(rawdata + "Reading Values \r\n");
        int pos = rawdata.indexOf(DOUBLE_SPACE); // finds the first double space
        String water = rawdata.substring(0, pos); water.replace("%", ""); water.remove(0, 4); water.trim();
        rawdata = rawdata.remove(0, pos + DOUBLE_SPACE.length());

        pos = rawdata.indexOf(DOUBLE_SPACE);
        String temp = rawdata.substring(0, pos); temp.replace("C", ""); temp.trim();
        rawdata = rawdata.remove(0, pos + DOUBLE_SPACE.length());

        pos = rawdata.indexOf(DOUBLE_SPACE);
        String rvalue = rawdata.substring(0, pos);

        String data = water + "," + temp + "," + rvalue;
        data = data.replace("[", "");
        data = data.replace("]", "");
        Particle.publish("water", data);
        }
    }
  }

int caldata(String command) {
    // apppend to a string everytime it sends the hook-response
    command = command.replace("\"", "");
    info.concat(command + " ");
}

int reboot(String command){
  resetFlag = true;
  delay(1000);
  return 1;
}

int device(String command) {
    DeviceRead(command);
    Serial1.print(esc); delay(1000);
    Serial1.print("DB \r\n");
    noloop = 0;
    return 1;
    }


int calibrate(String command) {
    command = command.replace("\"", "");
    Serial.print("Command: " + command + " / ");
    Serial.print("INFO: " + info);
    calibratefunction(command);
    delay(2000);
    Serial1.print(esc); delay(1000);
    Serial1.print("DB \r\n");
    return 1;
}

void loop() {
  if (resetFlag){
    System.reset();
    }
  else {
    PubSerial();
    delay(2000);
  }
}
