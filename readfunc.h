#if !defined(__READFUNC_H__)
#define __READFUNC_H__
#include "Particle.h"
#include "serial.h"

extern String DOUBLE_SPACE;
extern String SINGLE_SPACE;
extern char esc;
extern String info;
extern int po;


int DeviceRead(String command) {
    String point;
    String id; String sn; String bsw; int noloop = 1;
    Serial.print("Testing Device \r\n");
    escape();
    Serial1.print("DI");
    for (int i = 0; i < 10; i++){
      String FlashID = Serial1.readStringUntil('\r');
      switch (FlashID.charAt(1)) {
        case 'F' :
          id = FlashID;
          id = id.replace("Flash ID = ", ""); id.trim();
          break;
        case 'S' :
          sn = FlashID;
          sn = sn.replace("S/N:", ""); sn.trim();
          break;
        case 'B' :
          bsw = FlashID;
          break;
        default :
          ;
      }

    }
    delay(5000);
    SendString("DC");
    if (command == "ch"){
      String point = ""
      for (int i = 0; i < po; i++){
        String ratio = ReadLine(); SendString("\r");
        String water = ReadLine(); SendString("\r");
        String temp = ReadLine(); SendString("\r");
        String a = String(i);
        point = "[" + a + "]" + ratio + water + temp;
        Serial.println(point);
        
      }

    }
    //for (int i = 0; i < po; i++) {
      //append 3 times water, temp, ratio
    //}

    else{
      ReadClear(); ReadClear();
      String ratio1 =      ReadLine();SendString("\r");  // Temp
      String water1 =      ReadLine(); SendString("\r"); // R 2 plus slope or product .03
      String temperature = ReadLine(); SendString("\r"); // water from 1 + 10
      String ratio2 =      ReadLine(); SendString("\r"); // Temp
      String water2 =      ReadLine(); SendString("\r");
      ReadLine();
      String tempcomp = ReadLine();

      Serial1.print(esc); delay(1500); ReadLine();

      ratio1 = ratio1.replace("Ratio #1 = ", "");
      water1 = water1.replace("Water Content #1 = ", "");
      temperature = temperature.replace("Temperature", "");
      ratio2 = ratio2.replace("Ratio #2? ", "");
      water2 = water2.replace("Water Content #2 (%)? ", "");
      tempcomp = tempcomp.replace("Temperature C", "");

      String alld = id + "," + sn + "," + bsw + "," + ratio1 + "," + ratio2 + "," + water1 + "," + water2 + "," + temperature + "," + tempcomp;

      Serial.print(alld);
      Particle.publish("dinfo", alld, PRIVATE);
      return 1;
  }
}



int calibratefunction(String command) {
    Serial.print("Calibrating \r\n" + command + "\r\n");

    int pos = command.indexOf(SINGLE_SPACE); // Takes string from calibrate and assigns it to pos
    String command1 = command.substring(0, pos); // takes the command out
    command = command.remove(0, pos + SINGLE_SPACE.length()); // Removes command
    Serial.print(command1 + "\r\n");

    if(command1 == "cal"){
        // calibrate command format
        // cal,water,rvalue,tempin,slope,tempcomp
        // cal 10.00 0.01458 25 0.03
        pos = command.indexOf(SINGLE_SPACE); // next index
        String watercut = command.substring(0, pos); // removes watercut
        command = command.remove(0, pos + SINGLE_SPACE.length()); // deletes
        Serial.print(watercut + "\r\n");

        pos = command.indexOf(SINGLE_SPACE); // next index
        String rvaluein = command.substring(0, pos); // removes r value
        command = command.remove(0, pos + SINGLE_SPACE.length()); // deletes
        Serial.print(rvaluein + "\r\n");

        pos = command.indexOf(SINGLE_SPACE);
        String tempin = command.substring(0, pos);
        command = command.remove(0, pos + SINGLE_SPACE.length());
        Serial.print(tempin + "\r\n");

        pos = command.indexOf(SINGLE_SPACE);
        String slope = command.substring(0, pos);
        Serial.print(slope + "\r\n");

        float offsetf = slope.toFloat() + rvaluein.toFloat();
        String offset = String(offsetf);

        float watercutf = watercut.toFloat() + 10.00;
        String watercutoffset = String(watercutf);
        watercutoffset = watercutoffset.remove(4);
        escape();
        Serial1.print("DC"); delay(1000);
        Serial1.print(rvaluein + "\r");
        Serial1.print(watercut + "\r"); delay(1000); // Water
        Serial1.print(tempin   + "\r"); delay(1000); // Temp
        Serial1.print(offset   + "\r"); delay(1000); // R 2 plus slope or product .03
        Serial1.print(watercutoffset + "\r"); delay(1000); // water from 1 + 10
        Serial1.print(tempin + "\r"); delay(1000); // Temp
        Serial1.print("-0.03 %/C \r"); delay(1000);
        Serial1.print("Y \r"); delay(1000);
      }
    else if(command1 == "calch"){
        //calch points temp1 ratio1 temp2 ratio2
        //calch 1 0.01456 0.01566 25 25
        pos = command.indexOf(SINGLE_SPACE);
        String sampleqty = command.substring(0, pos);
        command = command.remove(0, pos + SINGLE_SPACE.length());
        Serial.print(sampleqty + "\r\n");

        pos = command.indexOf(SINGLE_SPACE);
        String ratio1 = command.substring(0, pos);
        command = command.remove(0, pos + SINGLE_SPACE.length());
        Serial.print(ratio1 + "\r\n");

        pos = command.indexOf(SINGLE_SPACE);
        String temp1 = command.substring(0, pos);
        command = command.remove(0, pos + SINGLE_SPACE.length());
        Serial.print(temp1 + "\r\n");

        pos = command.indexOf(SINGLE_SPACE);
        String ratio2 = command.substring(0, pos);
        command = command.remove(0, pos + SINGLE_SPACE.length());
        Serial.print(ratio2 + "\r\n");

        pos = command.indexOf(SINGLE_SPACE);
        String temp2 = command.substring(0, pos);
        command = command.remove(0, pos + SINGLE_SPACE.length());
        Serial.print(temp2 + "\r\n");

        // THIS IS FOR CC DO THIS FIRST //
        // send CC
        //SendString("CC");
        // send D
        //SendString("D");
        // ratio 1
        //SendString(ratio1 + "\r");
        // temperature 1
        //SendString(temp1 + "\r");
        // send D
        //SendString("D");
        // ratio 2
        //SendString(ratio2 + "\r");
        // temperature 2
        //SendString(temp2 + "\r");
        // Send Y
        //SendString("Y \r");
        // CC DONE //

        //THIS IS FOR THE CH SECTION //
        // send CH
        escape();
        Serial1.print("CH"); delay(1000);
        // number of points
        po = sampleqty.toInt(); po = po + 1;
        String points = String(po);
        // count = count + 1;
        Serial1.print(points + "\r"); delay(1000);
        for (int i = 1; i < po; i++) {

          pos = info.indexOf(SINGLE_SPACE);
          String water = info.substring(0, pos);
          info = info.remove(0, pos + SINGLE_SPACE.length());

          pos = info.indexOf(SINGLE_SPACE);
          String ratio = info.substring(0, pos);
          info = info.remove(0, pos + SINGLE_SPACE.length());

          pos = info.indexOf(SINGLE_SPACE);
          String temp = info.substring(0, pos);
          info = info.remove(0, pos + SINGLE_SPACE.length());

          // send D
          Serial1.print("D");  delay(1000);
          // ratio
          Serial1.print(ratio + "\r"); delay(1000);
          // temperature
          Serial1.print(temp + "\r"); delay(1000);
          // water content
          Serial1.print(water + "\r"); delay(1000);
          // REPEATE FOR ALL POINTS
        }
        // Send done I think Y
        Serial1.print("Y \r");  delay(1000);

        info = ""; // clear info
      }
    else return -1;
  }

#endif
