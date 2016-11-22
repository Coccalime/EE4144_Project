/*
 * Name: EE4144 Term Project 
 * Authors:
 * Description: Using a servo, IR distance sensor and photo diodes are goal is to balance a billiard ball at the center of a metal rod.
 */
  
//Include any needed libraries here.

//Using C, we want to put our pointers that hold the register addresses here, as global variables so we can access them throughout our code.

#define dt .004   //seconds         //250Hz or 4ms refresh rate ensures dt is a contant 4ms 
#define rod_center 25               //25 cm
int pid_setpoint, IRsensor_input;   
double timer;                       //Refresh timer
float pid_p_gain = 0;               //Gain setting for the P-controller 
float pid_i_gain = 0;               //Gain setting for the I-controller
float pid_d_gain = 0;               //Gain setting for the D-controller 
int pid_max = 50;                   //Maximum output of the PID-controller (+/-) 50cm 
float pid_error = 0;                //error from set point (cm)
float pid_output, pid_last_d_error, pid_i, pid_d;


//Function prototypes.
void initialize_interupt_registers();
int getIRsensor_input();
void moveServo(float pid_output);



void setup() {
  //Using C initialize appropriate registers using specific functions. i.e.
  initialize_interupt_registers();
  
}

void loop() {

  //PID Controller.
  //Recieves inputs from IR sensor or photodiode. Outputs a signal to servo for angle adjustment.

  timer = micros();                                                             //Begin refresh timer for a 4ms refresh rate. So our dt is 4ms.

  IRsensor_input = getIRsensor_input();                                         //get input from analog IR sensor.(How far from sensor)
  
  //Proportional 
  pid_error = IRsensor_input - rod_center;                                     //where we are now (sensors) - where we want to be (center of rod) 
  
  //Integral
  pid_i += pid_error*dt;                                                       //adding up all error over small amounts of time 
  if(pid_i > pid_max) pid_i = pid_max;
  else if(pid_i < pid_max * -1) pid_i = pid_max * -1;

  //Derivative 
  pid_d = (pid_error - pid_last_d_error)/dt;                                  //change in error over change in small time. AKA intantaneous slope of error vs time curve

  //Final PID output
  pid_output = pid_p_gain * pid_error + pid_i_gain * pid_i + pid_d_gain * pid_d;     
  if(pid_output > pid_max)pid_output = pid_max;
  else if(pid_output < pid_max * -1)pid_output = pid_max * -1;

  
  pid_last_d_error = pid_error;                                               //Previous error for derivative calculation

  moveServo(pid_output);                                                      //move servo to an angle that will get ball rolling towards center. either +/- angle

  while(micros() - timer < 4000); //wait until 400 microseconds pass (4ms) for a 4ms refresh rate.
   
  
}

void initialize_interupt_registers() {}

void moveServo(float pid_output) {}

int getIRsensor_input() {}


