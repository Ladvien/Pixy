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
int TiltLowLimit = 75; 
int TiltHighLimit = 95;


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


RightMargin = 320-(OOIX1 + pixy.blocks[pMaxIXY].width);  //This assumes the Pixy has a field of view 320 pixels wide.
LeftMargin = OOIX1; //Object of interest's X is the same as the left margin marker.
BottomMargin = 240-(OOIY1 + pixy.blocks[pMaxIXY].height); //This assumes the Pixy has a field of view 240 pixels high.
TopMargin = OOIY1; //The object of interest's Y is the same as the top margin marker.


//First, let's check to see if the OOI is centered enough.

if (OOIX1 > 100 && OOIX1 < 220){
    Serial.println("Centered.");
    delay(100);
}

else { //It's not centered, let's move servos and center it.
      //First, see which is greater left or right margin.
      //Serial.println(servoPosX);
      if(RightMargin >= LeftMargin){
        if(servoPosX > PanLowLimit){ 
          int moveX;
          moveX = LeftMargin/2;
          moveX = map(moveX, LeftMargin, 320, 90, 180);
          servoPosX = moveX;
          pixyServoX.write(servoPosX);
          //Serial.println(servoPosX);
          Serial.println(RightMargin);
          //Serial.println(moveX);
          Serial.println("Right.");
          delay(130);
          }
      }
    else{  
    //if (LeftMargin > RightMargin){
     //Left Margin was greater, so lets move stage-right. 
     if(servoPosX < PanHighLimit){ 
          //Right Margin was greater, so lets move stage-left.
          int moveX;
          moveX = RightMargin/2;
          moveX = map(moveX, 0, RightMargin, 0, 90);
          servoPosX = moveX;
          pixyServoX.write(servoPosX);
          //Serial.println(servoPosX);
          Serial.println(LeftMargin);
          //Serial.println(moveX);
          Serial.println("Left.");
          delay(130);
        }
    }
}



  /*
//X
  if(OOIX1 > 280){
    if(servoPosX>7){
      servoPosX=servoPosX-7;
      pixyServoX.write(servoPosX);    
      delay(10);
    }
  }
  if(OOIX1 < 40){
    if(servoPosX<173)
    {
      servoPosX=servoPosX+7;
      pixyServoX.write(servoPosX);
      delay(10);    
    }
  }
  
  if(OOIX1 > 220){
    if(servoPosX>4){
      servoPosX=servoPosX-3;
      pixyServoX.write(servoPosX);    
      delay(1);
    }
  }
  if(OOIX1 < 80){
    if(servoPosX<177)
    {
      servoPosX=servoPosX+3;
      pixyServoX.write(servoPosX);
      delay(1);    
    }
  }
  
//Y
  if(OOIY1 < 60){
    if(servoPosY>0)
    {
      servoPosY=servoPosY-2;
      pixyServoY.write(servoPosY);
      delay(5);    
    }
  }
  if(OOIY1 > 170){
    if(servoPosY<91)
    {
      servoPosY=servoPosY+2;
      pixyServoY.write(servoPosY);
      delay(5);    
    }
  }  
  */
}
