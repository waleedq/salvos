

//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source (9v battery + term)
//connect GRD to power source (9v battery - term)


int stp = 12;  //connect pin 13 to step
int dir = 13;  // connect pin 12 to dir
int en = 3;
int stp2 = 10;  //connect pin 13 to step
int dir2 = 11;  // connect pin 12 to dir
int en2 = 2;
int a = 0;     //  gen counter

int screen_height = 120;
int screen_width = 120;
int zoom = 1;

int xPos = 0;
int yPos = 0;
int yTest = 0;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean drawBox = false;
boolean drawLineH = false;
boolean drawLineV = false;
boolean drawCicrle = false;
boolean drawRand = false;

double const pi = 3.1415926535;

void setup() 
{            
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);  
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);   
  pinMode(en, OUTPUT);
  pinMode(stp2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(en2, OUTPUT);

  digitalWrite(en, LOW);  
  digitalWrite(en2, LOW);  
  digitalWrite(4, LOW); 
}


void loop() {
  digitalWrite(4, HIGH);
  if (stringComplete) { 
    if(inputString.indexOf("xl") > -1){
      digitalWrite(dir, HIGH);
    }
    if(inputString.indexOf("xr") > -1){
      digitalWrite(dir, LOW);
    }
    if(inputString.indexOf("yr") > -1){
      digitalWrite(dir2, HIGH);
    }
    if(inputString.indexOf("yl") > -1){
      digitalWrite(dir2, LOW);
    }
    if(inputString.indexOf("x") > -1){
      for (int x = 0; x < 2; x++) {
        digitalWrite(stp, HIGH);   
        delay(10);               
        digitalWrite(stp, LOW);  
        delay(10);
      }
    }
    if(inputString.indexOf("y") > -1){
      for (int y = 0; y < 2; y++) {
        digitalWrite(stp2, HIGH);   
        delay(10);               
        digitalWrite(stp2, LOW);  
        delay(10);
      } 
    }
    
    if(inputString.indexOf("homex") > -1){
      moveTo(0,yPos, 300);
    }
   
    if(inputString.indexOf("homey") > -1){
      moveTo(xPos,0, 300);
    }
   
    if(inputString.indexOf("homeall") > -1){
      moveTo(0,0, 300);
    }
   
    if(inputString.indexOf("khara") > -1){
      //moveTo(yTest,yTest++, 300);
      cicrle();
    }
   
    if(inputString.indexOf("stop") > -1){
     drawBox = false;
     drawLineH = false;
     drawLineV = false;
     drawCicrle = false;
     drawRand = false;
    } 
    
    if(inputString.indexOf("box") > -1){
     drawBox = true;
     drawLineH = false;
     drawLineV = false;
     drawCicrle = false;
     drawRand = false;
    }
    if(inputString.indexOf("lineh") > -1){
     drawBox = false;
     drawLineH = true;
     drawLineV = false;
     drawCicrle = false;
     drawRand = false;
    }
    if(inputString.indexOf("linev") > -1){
     drawBox = false;
     drawLineH = false;
     drawLineV = true;
     drawCicrle = false;
     drawRand = false;
    }
    if(inputString.indexOf("circle") > -1){
     drawBox = false;
     drawLineH = false;
     drawLineV = false;
     drawCicrle = true;
     drawRand = false;
    }
    if(inputString.indexOf("rand") > -1){
     drawBox = false;
     drawLineH = false;
     drawLineV = false;
     drawCicrle = false;
     drawRand = true;
    }    
    inputString = "";
    stringComplete = false;
  }
  
  if(drawRand){
   Serial.println("rand");
   moveTo(0, 0, 300);
   randomShapes();
  }
  if(drawBox){
    Serial.println("box");
    moveTo(0, 0, 300);
    box();
  }
  
  if(drawLineH){
    Serial.println("lineh");
    moveTo(0, 0, 300);
    lineh();
  }
  
  if(drawLineV){
    Serial.println("linev");
    moveTo(0, 0, 300);
    linev();
  }
  
  if(drawCicrle){
    Serial.println("circle");
    moveTo(0, 0, 300);
    cicrle();
  }
}

void moveTo(int x, int y, int sp){
 int xInc = 1;
 int yInc = 1;
 int screenX = zoom * x + screen_width / 2;
 int screenY = screen_height / 2 - zoom * y;
 
 int screenY_abs = screenY - yPos;
 int screenX_abs  = screenX - xPos;
 
 if(screenX < 0) screenX = 0;
 if(screenX > screen_width) screenX = screen_width;
 if(screenY < 0) screenY = 0;
 if(screenY > screen_height) screenY = screen_height;
 
 int stepsX = screenX_abs;
 if(stepsX > 0){
  digitalWrite(dir, LOW);
 }else{
  digitalWrite(dir, HIGH); 
  stepsX = stepsX * -1;
 }
 
 int stepsY = screenY_abs;
 if(stepsY > 0){
  digitalWrite(dir2, HIGH);
 }else{
  digitalWrite(dir2, LOW); 
  stepsY = stepsY * -1;
 }
      
 for (int x = 0; x < stepsX; x++) {
  digitalWrite(stp, HIGH);             
  digitalWrite(stp, LOW);  
  delayMicroseconds(sp); 
 }
 
 for (int y = 0; y < stepsY; y++) {
  digitalWrite(stp2, HIGH);             
  digitalWrite(stp2, LOW);  
  delayMicroseconds(sp); 
 }
 
 xPos = screenX;
 yPos = screenY;
 /*
 Serial.print("x: "); 
 Serial.println(x);
 Serial.print("y: "); 
 Serial.println(y); 
 Serial.print("screenX: "); 
 Serial.println(screenX);
 Serial.print("screenY: "); 
 Serial.println(screenY); 
 Serial.print("screenX_abs: "); 
 Serial.println(screenX_abs);
 Serial.print("screenY_abs: "); 
 Serial.println(screenY_abs); 
 Serial.print("stepsX: "); 
 Serial.println(stepsX);
 Serial.print("stepsY: "); 
 Serial.println(stepsY); 
 Serial.print("xPos: "); 
 Serial.println(xPos);
 Serial.print("yPos: "); 
 Serial.println(yPos); 
 Serial.println("------");
 delayMicroseconds(sp); 
 */
}
void lineh(){
   //while(drawLineH){
      for (int x = 0; x < 60; x++) {
       moveTo(x,0, 300);
      }
   //} 
}

void linev(){
   //while(drawLineV){
      for (int y = 0; y < 60; y++) {
        moveTo(0,y, 300);
      }
   //} 
}

void cicrle(){
      double i, angle, x1, y1;
      int r = 20;
      for(i = 0; i < 360; i++)
      {
            angle = i;
            x1 = r * cos(angle * PI / 180);
            y1 = r * sin(angle * PI / 180);
            moveTo(x1, y1, 300);
      }  
}


void randomShapes(){
    //linev();
    //lineh();
    //moveTo(0, 0, 300);
    cicrle();
    //cicrle();
    //moveTo(0, 0, 300);
    //linev();
    //lineh();
}

void box(){
   //while(drawBox){
    moveTo(0, 0, 300);
    for (int x = 0; x < 60; x++) {
      for (int y = 0; y < 60; y++) {
        moveTo(y,x , 1000);
      }
    }
   //} 
}
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

