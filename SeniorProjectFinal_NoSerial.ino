// This code serves as the basis for operation of the device created by F25 AquaCool
// The code controls the basic operation of the device to allow for monitoring of the water temperature and level
// Through the use a of 3 buttons, the desired behavior/temperature of the system can be adjusted

// Pins for the peripherals that are connected to the device

//Importing libraries
//#include "HD44780_LCD_PCF8574.h" //This library has routines to operate the PCF8574 I2C port expander connected to a HD44780 LCD 
#include <OneWire.h> //This library is in charge or simplying one wire communication 
#include <DallasTemperature.h> //This library uses the onewire libraryr to convert the ccommunication from the temperature probes to an utput

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2 
#define TEMPERATURE_PRECISION 9 //This is the precision of the data reading 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int TempChange=0;
char TempPrint = "323.32";
int Interval_Count=0;// This variable counts the amount of intervals that the system has gone 
//through before outputting a reading

int ThermoCheck=1; //This value indicates how many thermometers to check
const int RelayPin=12; //The pin that controls the relay is the digital pin 12

// Variables that are constnatly used throughout the code

  //Defining the pins for the optical water level sensors
  #define Wat1 A0 //For water level sensor 1
  #define Wat2 A2 //For water level sensor 2
  #define Wat3 A7 //For water level sensor 3
  //Defining Variables that correpond to water sensors
  int     FullSequence=0; //This variable is in charge of determining the order of events
  int HelloThere=0;
  int     WaterLevel=0 ;
  //int     WaterSeq1=0;
  //int     WaterSeq2=0;
  //int     WaterSeq3=0;
  int     WaterLevel1=0; //Integer for water level 1 reading
  int     WaterLevel2=0; //Integer for water level 2 reading
  int     WaterLevel3=0; //Integer for water level 3 reading
  bool    Water1=false; //For water level sensor 1
  bool    Water2=false; //For water level sensor 2
  bool    Water3=false; //For water level sensor 3
  int     WaterRead1=0; //Reading the water temperature sensor 20 times to ignore noise
  int     WaterRead2=0; //Reading the water temperature sensor 20 times to ignore noise
  int     WaterRead3=0; //Reading the water temperature sensor 20 times to ignore noise
  #define WatThresh 200
  //Setting the defination for button digital input pins
int IncreaseTemp=0;
int DecreaseTemp=0;


  //Defining variables for the temperature sensors
  float   Temperature ; //This variable stores the current temperature of the sytem


int    Target_Temp=59 ; //This variable stores the target temperature for the system to reach before shutting off
int   EnteredTemp;  //This variable stores the tempetaure that is currently being changed before being moved to target temp
  int     Entered_Value=0; // This variable allows for cross communication of button pressing instead of having returns

int    SystemUpdate=0; // This variables signifiies whether the system should be turned off or on in regards to temperature


#define button1 4 //Button one is connected to this digital input
#define button2 6 //Button two is connected to this digital input
#define button3 9 //Button three is connected to this digital input
const int Switch1 = 4; //These repeat the above
const int Switch2 = 6;
const int Switch3=9;
int ChangeToggle=0;
char PrintTemp[10];
char PrintTarget[10];
char PrintNew[10];
int  OUTPUT2_ON=0;
int  OUTPUT2_OFF=0;
int  OUTPUT1_ON=0;


int       Return_Value= 0; //This is the return value for the button pressing to be used

int       ButtonReset = 0; //This value determines whether there is a new entry to be entered

float Temp1;
float Temp2;
float Temp3;
// Including the library for the PCF to LCD 

#include "HD44780_LCD_PCF8574.h"

// Section: Defines
#define DISPLAY_DELAY_INIT 50 // mS
// **** NOTE ::  Comment in If using STM32 bluepill ****
//#define STM32_BLUE_PILL_SETUP


// Section: Globals
#ifdef STM32_BLUE_PILL_SETUP // *** STM32 Blue Pill *** 
// Set-up Choice I2C interface 1 or 2 :: pick one and one only 
TwoWire Wire2(1,I2C_FAST_MODE); // Use STM32 I2C1 
//TwoWire Wire2(2,I2C_FAST_MODE); // Use STM32 I2C2 
HD44780LCD myLCD(4, 20, 0x27, &Wire2); // LCD object.rows ,cols ,PCF8574 I2C addr, Interface)
#else
// myLCD(rows , cols , PCF8574 I2C address)
HD44780LCD myLCD( 4, 20, 0x27); // instantiate an object
#endif


DeviceAddress Thermo1, Thermo2, Thermo3;



void setup() {
  Serial.begin(115200);
  delay(DISPLAY_DELAY_INIT);
  myLCD.PCF8574_LCDInit(LCDCursorTypeOff);
  myLCD.PCF8574_LCDClearScreen();
  myLCD.PCF8574_LCDBackLightSet(true);

  myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
  myLCD.PCF8574_LCDSendString("   Portable Water   ");
  myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
  myLCD.PCF8574_LCDSendString("       Cooler       ");
  myLCD.PCF8574_LCDGOTO(LCDLineNumberThree, 0);
  myLCD.PCF8574_LCDSendString("   By F25AquaCool   ");
  myLCD.PCF8574_LCDGOTO(LCDLineNumberFour, 0);
  myLCD.PCF8574_LCDSendString("                    ");
  delay(3000);
  myLCD.PCF8574_LCDClearScreen();
// This is the part of the code that sets up the thermometers usings the
 sensors.begin();
 sensors.getDeviceCount();
  if (!sensors.getAddress(Thermo1, 0)) Serial.println("Unable to find address for Device 0");
  if (!sensors.getAddress(Thermo2, 1)) Serial.println("Unable to find address for Device 1");
  if (!sensors.getAddress(Thermo3, 2)) Serial.println("Unable to find address for Device 2");

sensors.setResolution(Thermo1, TEMPERATURE_PRECISION);
sensors.setResolution(Thermo2, TEMPERATURE_PRECISION);
sensors.setResolution(Thermo3, TEMPERATURE_PRECISION);

pinMode(button1, INPUT_PULLUP);
pinMode(button2, INPUT_PULLUP);
pinMode(button3, INPUT_PULLUP);
pinMode(RelayPin, OUTPUT);
}

// Section: Main Loop

void loop() {

 //delay(100);

ForeverLoop();
//Serial.println();
//Serial.print("Current Status");
//Serial.println(FullSequence);
//delay(1000);
}

void ForeverLoop(){
// This state contains the forever loops that is checking and monitoring the system
// It is undecided whether the button should be interrupt or a constant polling nature
  State1(); //State 1 is the button polling of the code
  ButtonMap();
  //Serial.println();
  //Serial.print("Current Status After State 1: ");
  //Serial.println(FullSequence);
//delay(1000);
  State2(); ///state 2 is checking the water level to display an estimated value
  //Serial.println();
  //Serial.print("Current Status After State 2: ");
  //Serial.println(FullSequence);



  State3(); //State 3 checks the temperature of the system
  //Serial.println();
  //Serial.print("Current Status After State 3: ");
  //Serial.println(FullSequence);

  State5(); //State 5 allows for the system to turn off an on depending on the temperature
  
  State4(); //State 4 updates the LCD
  //Serial.println();
  //Serial.print("Current Status After State 4: ");
  //Serial.println(FullSequence);


  //delay(1000);



  if (FullSequence==1){
    State6(); //State 6 is the temperature change screen that statys within that location
    //State7(); // State 7 changes the target temperature to be lower
    //State8(); //State 8 changes the target temperature to be higher
    //State4(); // This state updates the LCD

  }
  else if (FullSequence==2){
  State9();
  }
  
  if(ChangeToggle==1){ChangeToggle=0; State10(); }

}


void State1(){
// This state constantly polls the button for inputs rather than use interrupts
//This state constnatly polls for the buttons to see if there has been any changes
//Button Combination Returns
// None : 0
// 1 : 1
// 2 : 2
// 3: 3
// 1 & 2: 12
// 1 & 3: 13
// 2 & 3: 23
// 1, 2, & 3

if(ButtonReset==0){
  if (digitalRead(Switch1)!=HIGH){
  delay(100);
  Return_Value=1;
    if(digitalRead(Switch2)!=HIGH){
      Return_Value=12;
      if(digitalRead(Switch3)!= HIGH);
        Return_Value= 123;
    }
    else if(digitalRead(Switch3)!= HIGH){
      Return_Value=13;
      if(digitalRead(Switch2)!= HIGH){
        Return_Value=123;
      }
    }
  ButtonReset=1;

  }


  else if (digitalRead(Switch2)!=HIGH){
  delay(100);
  Return_Value=2;
    if(digitalRead(Switch3)!=HIGH){
      Return_Value=23;
      if(digitalRead(Switch1)!= HIGH);
      Serial.println("122i");
        Return_Value= 123;
    }
    else if(digitalRead(Switch1)!= HIGH){
      Return_Value=12;
      if(digitalRead(Switch3)!= HIGH){
        Serial.println("12");
        Return_Value=123;
      }
    }
  ButtonReset=1;

  }



  else if (digitalRead(Switch3)!=HIGH){
  delay(100);
  Return_Value=3;
    if(digitalRead(Switch2)!=HIGH){
      Return_Value=23;
      if(digitalRead(Switch1)!= HIGH);
      Serial.println("132");
        Return_Value= 123;
    }
    else if(digitalRead(Switch1)!= HIGH){
      Return_Value=13;
      if(digitalRead(Switch2)!= HIGH){
          Serial.println("122");
        Return_Value=123;
      }
    }
  ButtonReset=1;

  }

 //Serial.println(String(Return_Value)) ;
 //delay(3000);
}


if(ButtonReset==1){
  //Return_Value=0; //Put this back after the testing is fnished for the return value
  //Serial.println(String(Return_Value));


  if(digitalRead(Switch1)==HIGH && digitalRead(Switch2)==HIGH && digitalRead(Switch3)==HIGH){
    ButtonReset=0;
    Entered_Value=Return_Value;
    Return_Value =0;
    //Serial.println("Off");
    //Serial.print("Enter Value");
    //Serial.println(Entered_Value);
    //delay(5000);

}
}
}
//--------------------------------------------



void State2(){
// Checking the water level of the system

WaterLevel1= analogRead(Wat1); //Getting the value analog output from the first water level sensor
WaterLevel2= analogRead(Wat2); //Getting the value analog output from the second water level sensor
WaterLevel3= analogRead(Wat3); //Getting the value analog output from the third water level sensor 

if (Interval_Count<20){
  //Serial.println(Interval_Count);
  //Serial.println(WaterLevel1);
    //Serial.println(WaterLevel2);
      //Serial.println(WaterLevel3);
if (WaterLevel1<WatThresh){
   WaterRead1+=1; //Increasing the reading in response toa value above the given threshold

}
else if (WaterLevel1>=WatThresh) {
  WaterRead1-=1;
}
if (WaterLevel2<WatThresh){
   WaterRead2+=1; //Increasing the reading in response toa value above the given threshold

}
else if (WaterLevel2>=WatThresh) {
  WaterRead2-=1;
}
if (WaterLevel3<WatThresh){
   WaterRead3+=1; //Increasing the reading in response toa value above the given threshold

}
else if (WaterLevel3>=WatThresh) {
  WaterRead3-=1;
}
Interval_Count+=1;
}

else if( Interval_Count >= 20){
  //Serial.println(WaterRead1);
  //Serial.println(WaterRead2);
  //Serial.println(WaterRead3);
  //delay(3000);
Interval_Count=0;
if(WaterRead1>= Interval_Count/2){
  WaterLevel=1;
  ThermoCheck=1;
  if(WaterRead2>= Interval_Count/2){
    WaterLevel=2;
    ThermoCheck=2;
    if(WaterRead3>= Interval_Count/2){
      WaterLevel=3;
      ThermoCheck=3;
      
    }

  }


}

else if (WaterRead1<Interval_Count/2){
  WaterLevel=0;
  ThermoCheck=1;
}
//Serial.print("WaterLevel: ");
//Serial.print(WaterLevel);
//delay(999);
WaterRead1=0;
WaterRead2=0;
WaterRead3=0;




}




WaterRead1+=1; //Increasing the reading in response toa value above the given threshold
WaterRead2+=1; //Increasing the reading in response toa value above the given threshold
WaterRead3+=1; //Increasing the reading in response toa value above the given threshold

WaterRead1-=1; //Decreasing the reading in response toa value above the given threshold
WaterRead2-=1; //Decreasing the reading in response toa value above the given threshold
WaterRead3-=1; //Decreasing the reading in response toa value above the given threshold
}
void State3(){
// Checking the temperature of the system 

sensors.requestTemperatures();

Temp1= sensors.getTempF(Thermo1); // This is the temperature for the first probe
Temp2= sensors.getTempF(Thermo2); //This is the temperature for the second probe
Temp3= sensors.getTempF(Thermo3); //this is the temeprarture for teh third probe

if (ThermoCheck==1){
 Temperature= Temp1;
 //Serial.print("Sensor 1: ");
//Serial.println(Temp1);

//Serial.print("Sensor 2: ");
//Serial.println(Temp2);

//Serial.print("Sensor 3: ");
//Serial.println(Temp3);
  //myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
  //myLCD.PCF8574_LCDSendString("Temperature:  ");
  dtostrf(Temperature, 5, 2, PrintTemp);
   //myLCD.PCF8574_LCDSendString(PrintTemp); 
}

else if(ThermoCheck==2){
  Temperature= (Temp1+Temp2)/2;
  //Serial.print("Sensor 1: ");
//Serial.println(Temp1);

//Serial.print("Sensor 2: ");
//Serial.println(Temp2);

//Serial.print("Sensor 3: ");
//Serial.println(Temp3);
  //myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
  //myLCD.PCF8574_LCDSendString("Temperature:  ");
  dtostrf(Temperature, 4, 2, PrintTemp);
   //myLCD.PCF8574_LCDSendString(PrintTemp); 
}


else if(ThermoCheck==3){
  Temperature=(Temp1+Temp2+Temp3)/3;
  //Serial.print("Sensor 1: ");
//Serial.println(Temp1);

//Serial.print("Sensor 2: ");
//Serial.println(Temp2);

//Serial.print("Sensor 3: ");
//Serial.println(Temp3);
  //myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
  //myLCD.PCF8574_LCDSendString("Temperature:  ");
  dtostrf(Temperature, 4, 2, PrintTemp);
   //myLCD.PCF8574_LCDSendString(PrintTemp); 
}




}



void State4(){
// Updating the LCD Display to display the latest information about the water
  //String TemperatureString = String(Temperature); //This converts the temperature value into a displayable string
  //String WaterString = String(WaterLevel); //This converts the temperature 
//This section is in charge of printing the temeprature of the system

if(FullSequence==0){
  myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
  myLCD.PCF8574_LCDSendString("Water Temp:         ");
  //myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 12);
  myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 13);
  myLCD.PCF8574_LCDSendString(PrintTemp);
    myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 18);
  myLCD.PCF8574_LCDSendString("F");

//This part is in charge of printing the water level of the system 
  myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
  myLCD.PCF8574_LCDSendString("Water Level: ");
  //myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 11);
  //myLCD.PCF8574_LCDSendString(WaterString);
  if(WaterLevel==0){myLCD.PCF8574_LCDSendString("<25%");}
  else if(WaterLevel==1){myLCD.PCF8574_LCDSendString(">25%");}
  else if(WaterLevel==2){myLCD.PCF8574_LCDSendString(">50%");}
  else if(WaterLevel==3){myLCD.PCF8574_LCDSendString(">75%");}


  myLCD.PCF8574_LCDGOTO(LCDLineNumberThree, 0);
  myLCD.PCF8574_LCDSendString("Auto Update:        ");
  myLCD.PCF8574_LCDGOTO(LCDLineNumberThree, 13);
  if (SystemUpdate==1){myLCD.PCF8574_LCDSendString("ON");}
  if (SystemUpdate==0){myLCD.PCF8574_LCDSendString("OFF");}



  myLCD.PCF8574_LCDGOTO(LCDLineNumberFour, 0);
  myLCD.PCF8574_LCDSendString("Change Temp: Button3");
}

else if(FullSequence==1){

  myLCD.PCF8574_LCDGOTO(LCDLineNumberOne, 0);
  myLCD.PCF8574_LCDSendString("Changing Temperature");
  myLCD.PCF8574_LCDGOTO(LCDLineNumberTwo, 0);
  myLCD.PCF8574_LCDSendString("  Up: B1  Down: B2  ");
  myLCD.PCF8574_LCDGOTO(LCDLineNumberThree, 0);
  myLCD.PCF8574_LCDSendString("New Temp:           ");
  myLCD.PCF8574_LCDGOTO(LCDLineNumberThree, 14); 
  dtostrf(EnteredTemp, 4, 2, PrintNew); 
  myLCD.PCF8574_LCDSendString(PrintNew);
  myLCD.PCF8574_LCDGOTO(LCDLineNumberFour, 0);
  myLCD.PCF8574_LCDSendString("Current Set:        ");
  myLCD.PCF8574_LCDGOTO(LCDLineNumberFour, 14); 
dtostrf(Target_Temp, 4, 2, PrintTarget);


  myLCD.PCF8574_LCDSendString(PrintTarget);
}
}

void State5(){
// Turning the system on and off dependent on the temperature
//If system update is enbaled, the systme will allow for cycling between on and off state
if (SystemUpdate==0){
  OUTPUT2_ON=0;
  OUTPUT2_OFF=0;

  if(OUTPUT1_ON==0){digitalWrite(RelayPin, HIGH); OUTPUT1_ON=1;}
  else if(OUTPUT1_ON==1){}
}

else if(SystemUpdate==1){
  OUTPUT1_ON=0; //This resets the cycle for the previous to allow for it to flow through
//If the temperture of the system is below the target tempeture and some, the system will turn off


  if(Temperature <= Target_Temp){
    if(OUTPUT2_OFF==0){
    digitalWrite(RelayPin, LOW);
    OUTPUT2_OFF=1;
    OUTPUT2_ON=0;
    }
    else if(OUTPUT2_OFF==1){}

  }
//If the temperture of the system is above the target tempeture and some, the system will turn on
  else if(Temperature >= Target_Temp+8){
    if(OUTPUT2_ON==0){
    digitalWrite(RelayPin, HIGH);
    OUTPUT2_ON=1;
    OUTPUT2_OFF=0;
    }
    else if(OUTPUT2_ON==1){}

  }
}

}





void State6(){
// Changing the target temperature of the system
if(TempChange==0){EnteredTemp=Target_Temp; TempChange=1;}

else if (TempChange==1){
  if(IncreaseTemp==1){EnteredTemp+=1;IncreaseTemp=0;}
  else if(DecreaseTemp==1){EnteredTemp-=1;DecreaseTemp=0;}
  }

}



  




void State7(){
// Decreasing the temperature when the corresponding button is pressed
   
}
void State8(){
// Increasing the temperatur when the corresponding button is pressed
 
}



void State9(){
// Setting the target water temperature
  Target_Temp= EnteredTemp;
  TempChange=0;
  FullSequence=0; ///The system will return to operating at the normal function
}


void State10(){
// Dictates whether the system should auto shut off or not
  //SystemUpdate != SystemUpdate; //Changes the state pf the variable
  if(SystemUpdate==1){SystemUpdate=0;}
  else if(SystemUpdate==0){SystemUpdate=1;}
}

void ButtonMap(){
if(FullSequence==0){
  if(Entered_Value==1){Entered_Value=0; ChangeToggle=1;}//Make this so that this changes toggle
  else if(Entered_Value==2){Entered_Value=0;}
  else if(Entered_Value==3){FullSequence=1;Entered_Value=0; }//Serial.print("Changed Sequence");
  else if(Entered_Value==12){Entered_Value=0;}
  else if(Entered_Value==13){Entered_Value=0;}
  else if(Entered_Value==123){Entered_Value=0;}
  else if(Entered_Value==23){Entered_Value=0;}
}

else if(FullSequence==1){
  if(Entered_Value==1){IncreaseTemp=1;Entered_Value=0;}
  else if(Entered_Value==2){DecreaseTemp=1;Entered_Value=0;}
  else if(Entered_Value==3){FullSequence=2;Entered_Value=0;} //Serial.print("Changed Sequence");}
  else if(Entered_Value==12){Entered_Value=0;}
  else if(Entered_Value==13){Entered_Value=0;}
  else if(Entered_Value==123){Entered_Value=0;}
  else if(Entered_Value==23){Entered_Value=0;}
}


}