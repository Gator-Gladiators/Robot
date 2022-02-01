// Cup Competition 11/12/21

const int leftForwardIRPin = 12; // pin number which left-forward-facing IR is connected to (target aquisition)
const int leftDownwardIRPin = 3; // left-downward-facing IR sensor (floor detection)
const int rightForwardIRPin = 11;
const int rightDownwardIRPin = 4;
const int centerForwardIRPin = 13;

const int motor1pin1 = 6; 
const int motor1pin2 = 7;
const int motor2pin1 = 8;
const int motor2pin2 = 2;
const int motor1ENA = 5;
const int motor2ENA = 9;

int LFSensorValue;
int LDSensorValue;
int RFSensorValue;
int RDSensorValue;
int CFSensorValue;

int nextShouldMoveTime = 0;
bool shouldMove = false;

int nextShouldSpinTime = 0;
bool shouldSpinWhenIdle = true;

unsigned long lStart;

void setup() 
{
  Serial.begin(9600);
  pinMode(leftForwardIRPin, INPUT); // make each pin an INPUT pin
  pinMode(leftDownwardIRPin, INPUT);
  pinMode(rightForwardIRPin, INPUT);
  pinMode(rightDownwardIRPin, INPUT);
  pinMode(rightDownwardIRPin, INPUT);
  pinMode(centerForwardIRPin, INPUT);

  pinMode(motor1pin1, OUTPUT); // make each pin an OUTPUT pin
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  shouldMove = false;
  shouldSpinWhenIdle = true;
}

void loop() 
{
    GetSensorData();
    SetShouldMove();
    if (shouldMove)
    {
      MakeMove();
    }
    else
    {
      StopMoving();
    }
}

void MakeMove()
{

  if (nextShouldSpinTime < millis())
  {
    shouldSpinWhenIdle = !shouldSpinWhenIdle;
    nextShouldSpinTime = millis() + 10000;
  }
  
  if (!LD() && !RD())
  {
    Serial.println("MOVE BACK");
    MoveBackward(255, 2000);
    Serial.println("TURN");
    TurnLeft(255, 1000);
  }
  else if (LF() || RF() || CF())
  {
    MoveForward(170);
  }
  else if (LF())
  {
    TurnLeft(255, 50);
  }
  else if (RF())
  {
    TurnRight(255, 50);
  }
  else
  {
    if (shouldSpinWhenIdle)
    {
      TurnLeft(255);
    }
    else
    {
      MoveForward(170);
    }
  }
}

void SetShouldMove()
{
  if (analogRead(A0) < 150 && nextShouldMoveTime < millis())
  {
    shouldMove = !shouldMove;
    nextShouldMoveTime = millis() + 2000;
  }
}

void MoveForward (unsigned int givenSpeed, int timeInMs)
{
  MoveForward(givenSpeed);
  delay(timeInMs);
  StopMoving;
}
void MoveBackward (unsigned int givenSpeed, int timeInMs)
{
  MoveBackward(givenSpeed);
  delay(timeInMs);
  StopMoving;
}
void TurnLeft (unsigned int givenSpeed, int timeInMs)
{
  TurnLeft(givenSpeed);
  delay(timeInMs);
  StopMoving;
}
void TurnRight (unsigned int givenSpeed, int timeInMs)
{
  TurnRight(givenSpeed);
  delay(timeInMs);
  StopMoving;
}

void StopMoving ()
{
  MoveForward(0);
}

float wheelSpeedRatio = 160.0f / 255.0f;

void MoveForward (unsigned int givenSpeed)
{
  Move(givenSpeed, true, givenSpeed * wheelSpeedRatio, true);
}
void MoveBackward (unsigned int givenSpeed)
{
  Move(givenSpeed, false, givenSpeed * wheelSpeedRatio, false);
}
void TurnLeft (unsigned int givenSpeed)
{
  Move(givenSpeed, true, givenSpeed * wheelSpeedRatio, false);
}
void TurnRight (unsigned int givenSpeed)
{
  Move(givenSpeed, false, givenSpeed * wheelSpeedRatio, true);
}

void Move(unsigned int speed1, bool isGoingForward1, unsigned int speed2, bool isGoingForward2)
{
  
  analogWrite(motor1ENA, speed1);
  analogWrite(motor2ENA,speed2);
  
  //sets direction of spin
  if (isGoingForward1)
  {
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
  }
  else
  {
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
  }

  if (isGoingForward2)
  {
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
  }
  else
  {
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
  }
  
}

bool LF()
{
  return LFSensorValue == 1;
}

bool LD()
{
  return LDSensorValue == 1;
}

bool RF()
{
  return RFSensorValue == 1;
}

bool RD()
{
  return RDSensorValue == 1;
}

bool CF()
{
  return CFSensorValue == 1;
}

void GetSensorData()
{
  //read data from each pin as 0 (no detection) or 1 (detection)
  LFSensorValue = digitalRead(leftForwardIRPin) == 0 ? 1 : 0; // this "?" flips incoming data to fit above scheme
  LDSensorValue = digitalRead(leftDownwardIRPin) == 0 ? 1 : 0;
  RFSensorValue = digitalRead(rightForwardIRPin) == 0 ? 1 : 0;
  RDSensorValue = digitalRead(rightDownwardIRPin) == 0 ? 1 : 0;
  CFSensorValue = digitalRead(centerForwardIRPin); // center pin does not need to be flipped since it already fits above scheme

  //Serial.println("Sensor Data:");
    /*Serial.print("  LF: ");
    Serial.println(LFSensorValue);
    
    Serial.print("  LD: ");
    Serial.println(LDSensorValue);
    
    Serial.print("  RF: ");
    Serial.println(RFSensorValue);
    
    Serial.print("  RD: ");
    Serial.println(RDSensorValue);

    /*
    Serial.print("  CF: ");
    Serial.println(CFSensorValue);
    */
}
