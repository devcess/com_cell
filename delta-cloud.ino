#include "readfunc.h" // how to include other files
#include "serial.h"

String DOUBLE_SPACE = "  ";
String SINGLE_SPACE = " ";
String chcal = "";
String info = "";
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
    Particle.connect();
    Serial1.begin(9600);
    Serial.begin(9600);
    delay(10000);
    Serial1.print(esc); delay(1000);
    Serial1.print("DB \r\n");
    noloop = 0;
    }


void PubSerial(){
    String rawdata = ReadLine();
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
        Particle.publish("water", data);
        }
    }

int caldata(String command) {
    // apppend to a string everytime it sends the hook-response
    command = command.replace("\"", "");
    info.concat(command + " ");
}

int device(String command) {
    DeviceRead(command);
    Serial.print("Sending DB \r\n");
    Serial.print(command);
    Serial1.print("DB \r\n");
    noloop = 0;
    return 1;
    }


int calibrate(String command) {
    command = command.replace("\"", "");
    calibratefunction(command);
    delay(2000);
    //DeviceRead();
    Serial1.print("DB \r\n");
    return 1;
}

void loop() {
    PubSerial();
    delay(2000);
}
