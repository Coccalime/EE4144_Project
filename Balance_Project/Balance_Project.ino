//LEFT MOST PHOTODIODE IS THE ONE WITH THE ELECTRIC TAPE STICKING OUT , WHITE WITH THE ELECTRIC TAPE STICKING OUT IS THE GROUND FOR RIGHT MOST
#include <Servo.h>

//#define IRsensor
#ifdef IRsensor
#define sensorIR 5  
float getIRsensor_input();
float sensorValue, IRsensor_output, inches, cm; 
#endif

#define right_most_diode 4
#define center_diode 6
#define center_left_diode 7
#define center_right_diode 5

//THIS IS A BOOLEAN THAT HANDS CONTROL OF THE SERVO TO THE PHOTODIODES. INITIALLY THE IRsensor HAS THE CONTROL.ALSO USE BALL_DIRECTION TO ENSURE THE BALL IS MOVING IN RIGHT DIRECTION.
int transfer_control = 0 , ball_direction_right_diode = 0, ball_direction_left_diode = 0; 
int middle = 30, variance = 5;
Servo myservo; 


void setup() {
  pinMode(right_most_diode, INPUT_PULLUP);
    pinMode(center_diode, INPUT_PULLUP);
    pinMode(center_left_diode, INPUT_PULLUP);
    pinMode(center_right_diode, INPUT_PULLUP);
    myservo.attach(9);
    Serial.begin(9600);
    //INITIAL CONDITION FOR SERVO. WE WANT THIS TO BE A SLIGHT TILT AWAY FROM THE IRsensor SO THAT THE SENSOR CAN ACTUALLY SEE THE BALL.
    myservo.write(30);   //117 is center. we want to tilt a bit.
}

void loop() {
  #ifdef IRsensor
  if(transfer_control == 0) {                    //IF THE IRsensor IS IN CONTROL. (WE ARE IN INITIALIZATION STAGE)
    IRsensor_output = getIRsensor_input();    
    if(IRsensor_output >= 40){
      myservo.write(30);           //INITIATE KNOWN STARTING CONDITION (PULL SERVO ARM DOWN). WE KNOW THE BALL IS ROLLING TO DIODES FROM THE LEFT.
      transfer_control = 1;
    }
  }
  #endif

  if(transfer_control == 0) {
    myservo.write(30);
    if(digitalRead(right_most_diode)){
      myservo.write(middle - variance);
      transfer_control = 1;
    }
  }  
  else {                                        //IF INITIAL CONDITION IS COMPLETE WE CAN HAND CONTROL TO THE PHOTODIODES. FOR THE REST OF THE TIME WE WILL ONLY BE IN THIS ELSE STATEMENT.
                                                //POLL THE PINS FOR A HIT (PULLS LOW WHEN HIT). NO NEED FOR INTERUPTS SINCE THIS CODE IS FAST ENOUGH.
     
    if(digitalRead(center_right_diode)) {  //CHECK IF PIN IS LOW. (HIT) ------------ RIGHT DIODE
      if(ball_direction_right_diode == 0){                                  
        myservo.write(middle + variance);
        ball_direction_right_diode = !(ball_direction_right_diode);}
      else if(ball_direction_right_diode == 1) {
        myservo.write(middle - variance);
        ball_direction_right_diode = !(ball_direction_right_diode);}
    }
    else if(digitalRead(center_diode)) {  //CHECK IF PIN IS LOW. (HIT) ---------------- CENTER DIODE                                  
      myservo.write(middle);
      //ball_direction = !(ball_direction);
    }
    else if(digitalRead(center_left_diode)) {  //CHECK IF PIN IS LOW. (HIT) --------------- LEFT DIODE                                   
      if(ball_direction_left_diode == 0){ 
        myservo.write(middle + variance);
        ball_direction_left_diode= !(ball_direction_left_diode);}
      else if(ball_direction_left_diode == 1){
        myservo.write(middle- variance);
        ball_direction_left_diode = !(ball_direction_left_diode);}
    }
    
  }
}



#ifdef IRsensor
float getIRsensor_input() {

  sensorValue = analogRead(sensorIR);
  inches = 4192.936 * pow(sensorValue,-0.935) - 3.937;
  //cm = 10650.08 * pow(sensorValue,-0.935) - 10;
  delay(100);
  Serial.print("Inches: ");
  Serial.println(inches);
  return inches;
}
#endif
