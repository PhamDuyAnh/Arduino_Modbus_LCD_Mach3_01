/*
  Modbus-Arduino Example - Switch (Modbus Serial)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino

  Modbus functions supported:
    0x01 - Read Coils
    0x02 - Read Input Status (Read Discrete Inputs)
    0x03 - Read Holding Registers
    0x04 - Read Input Registers
    0x05 - Write Single Coil
    0x06 - Write Single Register
    0x0F - Write Multiple Coils
    0x10 - Write Multiple Registers

  Modbus Jargon
    In this library was decided to use the terms used in Modbus to the methods names, then is important clarify the names of register types:
   ___________________________________________________________________________
  | Register type     | Use as          | Acces         | Library methods     |
  |-Coil                Digital Output    Read/Write      addCoil(), Coil()   |
  |-Holding Register    Analog Output     Read/Write      addHreg(), Hreg()   |
  |-Input Status        Digital Input     Read Only       addIsts(), Ists()   |
  |-Input Register      Analog Input      Read Only       addIreg(), Ireg()   |

  Notes:
  *  Input Status is sometimes called Discrete Input.
  *  Holding Register or just Register is also used to store values in the slave.
  *  Examples of use: A Coil can be used to drive a lamp or LED. A Holding Register to store a counter or drive a Servo Motor.
      A Input Status can be used with a reed switch in a door sensor and a Input Register with a temperature sensor.

  Adds registers and configures initial value if specified.
  * void addCoil (offset word, bool value)
  * void addHreg (offset word, word value)
  * void addIsts (offset word, bool value)
  * void addIreg (offset word, word value)
  
  Sets a value to the register.
  * bool Coil (offset word, bool value)
  * bool Hreg (offset word, word value)
  * bool Ists (offset word, bool value)
  * bool Ireg (offset word, word value)
  
  Returns the value of a register.
  * bool Coil (offset word)
  * word Hreg (word offset)
  * bool Ists (offset word)
  * word Ireg (word offset)


 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you  can usually tell them by the 16-pin interface.
 The circuit:
 * LCD RS pin to digital pin 8
 * LCD En pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
//                RS  EN  D4  D5 D6 D7
LiquidCrystal lcd( 8,  9,  4,  5, 6, 7);
 
#include <Modbus.h>
#include <ModbusSerial.h>
//ModbusSerial object
ModbusSerial mb;

#define baurate 115200  //
#define slaveid 1       // Set the Slave ID (1-247)

word hreg, ireg;
String Xstr, Ystr;
void setup()
{
  // Config Modbus Serial (port, speed, byte format) 
  mb.config(&Serial, baurate, SERIAL_8N1);
  // Set the Slave ID (1-247)
  mb.setSlaveId(slaveid);
  //Input Register
  //mb.addIreg(0);
  //mb.Ireg(0,256);
  //Input Status or Discrete Input
  //mb.addIsts(0);
  //mb.Ists(0, true);
  //Holding Register
  mb.addHreg(0);
  mb.addHreg(1);
  mb.addHreg(2);
  mb.addHreg(3);
  mb.addHreg(4);
  mb.addHreg(5);
  mb.addHreg(6);
  //Coil
  //mb.addCoil(0);
  //mb.Coil(0, true);
  Setup_LCD();
}

void loop()
{
  //Call once inside loop() - all magic here
  mb.Hreg(6, get_key());
  mb.task();
  Xstr = "X:";
  if (mb.Hreg(0) > 0 )
  {
    Xstr = Xstr + "+";
  }
  else
  {
    Xstr = Xstr + "-";
  }
  Xstr = Xstr + String(mb.Hreg(1) + mb.Hreg(2)/100.00) + "         ";
  lcd.setCursor(0,0);
  lcd.print(Xstr);

  Ystr = "Y:";
  if (mb.Hreg(3) > 0 )
  {
    Ystr = Ystr + "+";
  }
  else
  {
    Ystr = Ystr + "-";
  }
  Ystr = Ystr + String(mb.Hreg(4) + mb.Hreg(5)/100.00) + "         ";
  lcd.setCursor(0,1);
  lcd.print(Ystr);
  //delay(10);
  /*
  lcd.setCursor(0,1);
  lcd.print("0:  1:  2:  3:  ");
  lcd.setCursor(2,1);
  lcd.print(mb.Coil(0));
  lcd.setCursor(6,1);
  lcd.print(mb.Coil(1));
  lcd.setCursor(10,1);
  lcd.print(mb.Coil(2));
  lcd.setCursor(14,1);
  lcd.print(mb.Coil(3));
  */
}

void Setup_LCD()
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);  
  // startup LOGO  
  lcd.setCursor(4,0);
  lcd.print("CNCProVN");
  lcd.setCursor(2,1);
  lcd.print("Pham Duy Anh");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("www.cncprovn.com");
  lcd.setCursor(0,1);
  lcd.print("LCD & RS232 test");
  delay(1000);
  lcd.clear(); 
}

int get_key()
{ 
  int adc_key_val[5] ={50, 150, 300, 500, 1000 };
  int input = analogRead(0);   // read the value from the sensor
  int k;
  for (k = 0; k < 5; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }
  if (k >= 5) k = -1;  // No valid key pressed
  return k;
}
