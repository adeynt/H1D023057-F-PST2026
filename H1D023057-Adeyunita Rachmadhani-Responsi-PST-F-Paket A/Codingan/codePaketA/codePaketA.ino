const int segmentPins[7] = {13, 12, 11, 10, 9, 8, 7};
const int btnUp = 6;
const int LedRed = 5;
const int LedYellow = 4;
const int LedGreen = 3;

byte digitPattern[10][7] = {

{1,1,1,1,1,1,0,0}, //0
{0,1,1,0,0,0,0,0}, //1
{1,1,0,1,1,0,1,0}, //24
{1,1,1,1,0,0,1,0}, //3
{0,1,1,0,0,1,1,0}, //4
{1,0,1,1,0,1,1,0}, //5
{1,0,1,1,1,1,1,0}, //6
{1,1,1,0,0,0,0,0}, //7
{1,1,1,1,1,1,1,0}, //8
{1,1,1,1,0,1,1,0}, //9
};

bool lastUpState = HIGH;

void displayDigit(int num)
{
    for(int i = 0; i < 7; i++)
    {
        digitalWrite(segmentPins[i], !digitPattern[num][i]);
    }
}


void setup() {
   for(int i = 0; i < 7; i++)
    {
        pinMode(segmentPins[i], OUTPUT);
    }
    pinMode(btnUp, INPUT_PULLUP);
    pinMode(LedRed, LOW);
    pinMode(LedYellow, LOW);
    pinMode(LedGreen, HIGH);
    displayDigit(currentDigit);
}

void loop() {
  bool upState = digitalRead(btnUp);
  if (lastUpState == HIGH && upState == LOW) {
    digitalWrite(LedGreen, LOW);
    delay(300);
    digitalWrite(LedRed, HIGH);
    delay(1000);
    for(int i = 10; i >= 0; i--)   //9 sampai 0
    {
        displayDigit(i);
        delay(1000);
    }
    digitalWrite(LedRed, LOW);
    delay(300);
    digitalWrite(LedYellow, HIGH);
    delay(300);
    digitalWrite(LedYellow, LOW);
    delay(300);
    digitalWrite(LedGreen, HIGH);
    delay(1000);
  }
  else {
    digitalWrite(LedRed, HIGH);
    delay(1000);
    digitalWrite(LedRed, LOW);
    delay(300);
    digitalWrite(LedYellow, HIGH);
    delay(300);
    digitalWrite(LedYellow, LOW);
    delay(300);
    digitalWrite(LedGreen, HIGH);
    delay(1000);
  }
  lastUpState = upState;
}
