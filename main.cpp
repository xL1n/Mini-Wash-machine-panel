

#include "mbed.h"
#include <cstdio>

void led_ini();
void button_ini();
void slowspin();
void regularmode();
int weightcheck();
//void cycle(int times);
BufferedSerial pc(USBTX, USBRX, 115200);        //establish serial communications between PC and NUCLEO
#define WAIT_TIME_MS 500 
AnalogIn TEM(PA_1); //temperatuer simulator sensor pin
double tem_value = 0; //representation of simulating "water temperature"

AnalogIn FRC(PC_3); //light sensor which simulating door situation(if too bright - door is not shut, no washing)
double force = 0;

DigitalIn powerswitch(PC_10);   //on/off control
DigitalOut led_offpower(PC_0);    //it indicates off stastus
DigitalOut led_onpower(PC_1); // on state
DigitalIn fastmode(PC_11);  //pressed, machine will be regular-fast spin mode
DigitalIn delicatemode(PD_2);  //pressed, machine will be delicate-slow spin mode
    
BusOut SegDis(PA_11, PA_12, PB_1, PB_15, PB_14, PB_12, PB_11); //for sigment dispay which indicate the spin of machinie
//one cycle for sigments is from F-A-B-C-D-E
int cyclearray[] = {0x20, 0x21, 0x23, 0x27, 0x2F, 0x3F};
int finishsign[] = {0x71, 0x06, 0x37 , 0x06, 0x6D, 0x76}; // display "finish" on 7-segment

int main()
{
    printf("Welcome to use Washing Machine ^_^ \n");
    led_ini();
    button_ini();

while (true){
        if(powerswitch.read() == 1){   // power is on - able to implement the following controls
        led_offpower.write(0);
        led_onpower.write(1); // green led on which indicates machine is on

        //first: select 2 mode (1:regular-fast spin; 2:Delicate - slow spin)
        if (fastmode.read() == 1) { //selected regular mode
        printf("You have chosen REGULAR MODE - washing for 1 minute \n");
           if(weightcheck() == 1){ //check the weight of cloths put in

            //it will spin faster in REGULAR
            regularmode(); //implementing "washing"
            SegDis.write(0x00); //turn off the dispay
         }
         else{printf("Waring! You have put TOO HEAVEY Laundry \n");
              thread_sleep_for(5000);} //machine will not run if too many colths put in
        }

        else if (delicatemode.read() == 1) { //for delicate wash
        printf("You have chosen DELICATE MODE - washing for 1 minute\n");
           if(weightcheck() == 1){
        //it will spin relative slow
            slowspin(); //washing
            SegDis.write(0x00); //turn off the dispay
           }
           else{printf("Waring! You have put TOO MANY colths \n");
                thread_sleep_for(5000);}
        }
    }

        else {led_offpower.write(1);led_onpower.write(0);} // if not turned on, red led is on
    }


    
}

void led_ini(){
 led_offpower.write(0);
 led_onpower.write(0);
}

void button_ini(){
    fastmode.mode(PullNone);
    delicatemode.mode(PullNone);
}

int weightcheck(){ //check the weight of cloth putin to the machine
    force = FRC.read()*100;
    if(force >= 85){
        return 0; //means too much force on machine - too many cloth, which machine will not run
    }
    else{return 1;}
}



void regularmode(){ //regular mode washing implementation
 //vary numbers of spin represent different speeds of machine
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature before start is : %f", tem_value);
        printf(" dergee\n");
        thread_sleep_for(20);
    for(int spin = 1; spin <= 8; spin++){//first stage - start - spin quite low (8 spins)
          for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(250ms);
         }
        }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f", tem_value);
        printf(" dergee\n");
       for(int spin = 1; spin <= 16; spin++){//second stage - mid - spin medium (16 spins)
          for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(125ms);
         }
        }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f", tem_value);
        printf(" dergee\n");
       for(int spin = 1; spin <= 32; spin++){//third stage - fast - spin fast (32 spins)
          for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(62ms);
         }
        }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f", tem_value);
        printf(" dergee\n");
       for(int spin = 1; spin <= 16; spin++){//forth stage - mid - spin medium (16 spins)
          for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(125ms);
          }
        }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f", tem_value);
        printf(" dergee\n");
      for(int spin = 1; spin <= 8; spin++){//final stage - end - spin quite low (8 spins)
         for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(250ms);
        }
       }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f", tem_value);
        printf(" dergee\n");
      for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(finishsign[i]);
            ThisThread::sleep_for(500ms);
        }
}

void slowspin(){ //delicate mode washing implementation
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature before start is : %f", tem_value);
        printf(" dergee\n");
        thread_sleep_for(20);
 //vary numbers of spin represent different speeds of machine
    for(int spin = 1; spin <= 8; spin++){//first stage - start - spin low (4 spins)
          for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(500ms);
         }
        }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f", tem_value);
        printf(" dergee\n");
       for(int spin = 1; spin <= 16; spin++){//second stage - mid - spin medium (8 spins)
          for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(250ms);
         }
        }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f", tem_value);
        printf(" dergee\n");
       for(int spin = 1; spin <= 32; spin++){//third stage - fast - spin fast (16 spins)
          for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(125ms);
         }
        }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f", tem_value);
        printf(" dergee\n");
       for(int spin = 1; spin <= 16; spin++){//forth stage - mid - spin medium (8 spins)
          for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(250ms);
          }
        }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : %f\n", tem_value);
        printf(" dergee\n");
      for(int spin = 1; spin <= 8; spin++){//final stage - end - spin quite low (4 spins)
         for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(cyclearray[i]);
            ThisThread::sleep_for(500ms);
        }
       }
        tem_value = (TEM.read()*60)*(TEM.read()*60);
        printf("water temperature is now : .2%f\n", tem_value);
        printf(" dergee\n");
      for(int i = 0; i <= 5 ; i++){ 
            SegDis.write(finishsign[i]);
            ThisThread::sleep_for(500ms);
        }
}

