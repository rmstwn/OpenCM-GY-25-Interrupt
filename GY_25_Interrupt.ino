/* Timer_Interrupt_Serial
 
                  Compatibility
 CM900                  O
 OpenCM9.04             O
 
 created 16 Nov 2012
 by ROBOTIS CO,.LTD.
 */
 
// Connect RX of GY 25 to TX3 OpenCM9.04(D24) and TX of GY 25 to RX3 OpenCM9.04(D25) 
#include <string.h>

HardwareTimer Timer(1);

//Variable for GY-25
int Roll,Pitch,Yaw;
unsigned char Re_buf[8],counter=0;

void setup()
{ 
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(BOARD_LED_PIN, OUTPUT);
  
  Serial3.begin(115200); // Baud rate for GY 25 
  SerialUSB.begin(115200); // Baud rate for SerialUSB Monitor
  
  //GY-25 calibration  
  Serial3.write(0XA5); 
  Serial3.write(0X54);
  
  delay(100);      
  
  Serial3.write(0XA5); 
  Serial3.write(0X55);
  
  delay(100);
  
  Serial3.write(0XA5); 
  Serial3.write(0X52);
  
  //Timer Initialization
  Timer.pause();
  Timer.setPeriod(10000);
  Timer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  Timer.setCompare(TIMER_CH1, 1); 
  Timer.attachInterrupt(TIMER_CH1, handler_gy);
  Timer.refresh();
  Timer.resume();
  
}

void loop() {
  
} 

void handler_gy(){ 
  int buttonState = digitalRead(BOARD_BUTTON_PIN);
  if(buttonState==HIGH){  
    
  //GY-25 calibration  
  Serial3.write(0XA5); 
  Serial3.write(0X54);
  
  delay(100);      
  
  Serial3.write(0XA5); 
  Serial3.write(0X55);
  
  delay(100);
  
  Serial3.write(0XA5); 
  Serial3.write(0X52); 
  
  digitalWrite(BOARD_LED_PIN, LOW);
  }
  
  //Parsing data from GY-25
  while (Serial3.available()) {   
      Re_buf[counter]=(unsigned char)Serial3.read();
      if(counter==0&&Re_buf[0]!=0xAA) return;        
      counter++;       
      if(counter==8)                
      {
          counter=0;                  
        if(Re_buf[0]==0xAA && Re_buf[7]==0x55)  // data package is correct      
          {          
           Yaw=(float)(Re_buf[1]<<8|Re_buf[2])/100;
           if(Yaw > 327) { Yaw -= 655; }
           else  Yaw==Yaw; 
           Pitch=(float)(Re_buf[3]<<8|Re_buf[4])/100;
           if(Pitch > 327) { Pitch -= 655; }
           else  Pitch==Pitch; 
           Roll=(float)(Re_buf[5]<<8|Re_buf[6])/100;
           if(Roll > 327) { Roll -= 655; }
           else  Roll==Roll; 
          }      
      }  
    }
    
    digitalWrite(BOARD_LED_PIN, HIGH);
    
    //show result to SerialUSB Monitor
    SerialUSB.print(Yaw);
    SerialUSB.print(",");
    SerialUSB.print(Pitch);
    SerialUSB.print(",");
    SerialUSB.print(Roll);
    SerialUSB.print(".");
    SerialUSB.print("\n");
    delay(10);
}

