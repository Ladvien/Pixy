//Pan/Tilt code for Pixy CMUcam5
//C. Thomas Brittain
//Ladvien, http://letsmakerobots.com/node/40753


//Pixy Stuff (includes)
#include <Servo.h>
#include <SPI.h>
#include <Pixy.h>

//Lets setup the Pixy.
Pixy pixy;

//Variables for indexing the largest mass and printing the info.
int pIndex;
int printPixy = 0;


//Variables for tracking the largest mass.
int pHeightPlusWidth = 0;
int pHeightPlusWidthMax = 0;


//Holds the index number of the largest mass.
int pMaxIXY = 0;

Servo pixyServoX; //Attach the pan servo.
Servo pixyServoY; //Attach the tilt servo.

//Set servos to initial position.
int posX = 90; //Servo position.
int posY = 90; //Servo position.


//Servo position.
int servoPosX = 90;
int servoPosY = 90;

//These are the limit settings for the Servo's movement range.
//Given, 90 is middle.
int PanLowLimit = 1; 
int PanHighLimit = 180; 
int TiltLowLimit = 50; 
int TiltHighLimit = 95;

//Adjust to control how many servo positions are changed at once.
int servoMaxJumpX = 7;
int servoMaxJumpY = 3;

//Centered margin limits.
int xLowCenterLimit = 80;
int xHighCenterLimit = 240;
int yLowCenterLimit = 40;
int yHighCenterLimit = 160;

int servoJumpDelay = 20;

//X and Y of the largest mass.
int OOIX1;  //Object of Interest X1
int OOIY1;  //Object of Interest Y1
int OOIX2;  //Object of Interest X1
int OOIY2;  //Object of Interest Y1


//For calculating servo positions.
int RightMargin; int LeftMargin; int BottomMargin; int TopMargin;

void setup() {

  //Begin Serial.
  Serial.begin(9600);

  //Attach servos
  pixyServoX.attach(6); //Tilt (Y)
  pixyServoY.attach(5); //Pan (X)
  pixyServoX.write(servoPosX);
  pixyServoY.write(servoPosY);
  
  //Delay to prepare the Pixy.
  delay(2000);

}

void loop() {

  //Get the first set of data from the Pixy.  
  pixy.getBlocks();  
  //Serial.println("Got blocks!");
  
  //Run through the first 40 indexes
  for(int i = 0; i < 40; i++){    
   //Or until the widith of objects is unreasonable (garabage data).
    if(pixy.blocks[i].width < 999){
      //Rough estimate of the OOI's mass.
      printPixy = pixy.blocks[i].height + pixy.blocks[i].width;
      //Compare the mass to the last largest mass found.
      if(pHeightPlusWidth > pHeightPlusWidthMax){
        //If this is the new largest mass, load it into the Max variable.
        pHeightPlusWidthMax = pixy.blocks[i].height + pixy.blocks[i].width;
        //Also, load the largest mass' index number.
        pMaxIXY = i;        
      }
   }
}
/*
  //Print the OOI info.
  Serial.print(pixy.blocks[pMaxIXY].x);
  Serial.print(" X, ");
  Serial.print(pixy.blocks[pMaxIXY].y);
  Serial.println(" Y ");
*/
  //Load the largest mass X and Y into the object of interest variables.  
  OOIX1 = pixy.blocks[pMaxIXY].x;
  OOIY1 = pixy.blocks[pMaxIXY].y;
  
  //Open the servo write function.
  ServoWrite();
  
  //Set the largest mass index back to 0.
  pMaxIXY=0;  
}


void ServoWrite(){
  
  RightMargin = 320 - (OOIX1 + (pixy.blocks[pMaxIXY].width/2));  //This assumes the Pixy has a field of view 320 pixels wide.
  LeftMargin = OOIX1; //Object of interest's X is the same as the left margin marker.
  BottomMargin = 240 - (OOIY1 + (pixy.blocks[pMaxIXY].height/2)); //This assumes the Pixy has a field of view 240 pixels high.
  TopMargin = OOIY1; //The object of interest's Y is the same as the top margin marker.
  
  
  //First, let's check to see if the OOI is centered enough.
  if (OOIX1 > xLowCenterLimit && OOIX1 < xHighCenterLimit){
      delay(servoJumpDelay);
  }
  else { //It's not centered, let's move servos and center it.
        //First, see which is greater left or right margin.
        if(RightMargin >= LeftMargin){
          if(servoPosX < PanHighLimit){ 
            int moveX;
            moveX = RightMargin;
            moveX = map(moveX, 0, RightMargin, 0, servoMaxJumpX);
            servoPosX = servoPosX + moveX;
            pixyServoX.write(servoPosX);
            delay(servoJumpDelay);
            }
        }
      else{  
       //Left Margin was greater, so lets move stage-right. 
       if(servoPosX > PanLowLimit){ 
            //Right Margin was greater, so lets move stage-left.
            int moveX;
            moveX = LeftMargin;
            moveX = map(moveX, 0, LeftMargin, 0, servoMaxJumpX);
            servoPosX = servoPosX - moveX;
            pixyServoX.write(servoPosX);
            delay(servoJumpDelay);
          }
      }
  }


  //First, let's check to see if the OOI is centered enough.
  if (OOIY1 > yLowCenterLimit && OOIY1 < yHighCenterLimit){
      delay(servoJumpDelay);
  }
  else { //It's not centered, let's move servos and center it.
        //First, see which is greater bottom or top margin.
        if(BottomMargin >= TopMargin){
          if(servoPosY > TiltLowLimit){ 
            int moveY;
            moveY = BottomMargin;
            moveY = map(moveY, 0, BottomMargin, 0, servoMaxJumpY);
            servoPosY = servoPosY - moveY;
            pixyServoY.write(servoPosY);
            delay(servoJumpDelay);
            }
        }
      else{  
       //Top Margin was greater, so lets move stage-up. 
       if(servoPosY < TiltHighLimit){ 
            //Topt Margin was greater, so lets move stage-up.
            int moveY;
            moveY = TopMargin;
            moveY = map(moveY, 0, TopMargin, 0, servoMaxJumpY);
            servoPosY = servoPosY + moveY;
            pixyServoY.write(servoPosY);
            delay(servoJumpDelay);
          }
      }
  }

} //End ServoWrite()
