// Pins
int CLK = 26; 
int DT = 27;  
int INT1 = 23 ;
int INT2 = 22 ;

// Rotations
int encoderPosCount = 0;
int CLK_last;
int CLK_val;
bool bCW;

// Speed Calculations
long newposition;
long oldposition = 0;
long newtime;
long oldtime = 0;
long vel;
double pwmFactor = 0 ;

// Delay
unsigned long update_ticker;
unsigned long update_interval = 600L;

// Serial Interupt
String inputString = "";
bool stringComplete = false;
 
void setup() { 
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(INT1, OUTPUT) ;
  pinMode(INT2, OUTPUT) ; 

  inputString.reserve(200);
  Serial.begin(115200);
  Serial.println("START");

  analogWrite(INT1, 255*pwmFactor) ;  
  analogWrite(INT2, 0) ;  

  CLK_last = digitalRead(CLK) ;
} 
 
void loop()
{
  if (stringComplete) {
    pwmFactor = inputString.toDouble() ;
    
    inputString = "";
    stringComplete = false;
    
    analogWrite(INT1, 255*pwmFactor) ;  
    analogWrite(INT2, 0) ; 
  }

  CLK_val = digitalRead(CLK);
  if (CLK_val != CLK_last){ // Rotating
    if (digitalRead(DT) != CLK_val) { // Means CLK Changed first - Clockwise
      encoderPosCount++;
      bCW = true;
    } else {// Otherwise DT changed first - Counter Clockwise
      bCW = false;
      encoderPosCount--;
    }
  }
  CLK_last = CLK_val;

  if (millis() - update_ticker > update_interval){ // Instead of delay
    // Serial.println(millis()) ;  
    newposition = encoderPosCount ;    
    newtime = millis();
    vel = (newposition-oldposition) * 1000 /(newtime-oldtime);
    // Serial.print("vel=");
    Serial.println(vel);
    
    oldposition = newposition;
    oldtime = newtime;
		update_ticker = millis();
    // Serial.println(millis()) ;  
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
