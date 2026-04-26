#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

double cm;
double cale;

// RGB
const int redPin = 4;
const int greenPin = 5;
const int bluePin = 6;

// buzzer
const int buzzerPin = 8;

// bluetooth
char bt;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

void setColor(bool r, bool g, bool b)
{
  digitalWrite(redPin, r);
  digitalWrite(greenPin, g);
  digitalWrite(bluePin, b);
}

void setup()
{
  lcd.init();
  lcd.backlight();

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600); // HC-05

  lcd.setCursor(0,0);
  lcd.print("Start...");
  delay(1500);
  lcd.clear();
}

void loop()
{
  // ===== BLUETOOTH =====
  if (Serial.available())
  {
    bt = Serial.read();

    if (bt == 'R') setColor(HIGH, LOW, LOW);
    if (bt == 'G') setColor(LOW, HIGH, LOW);
    if (bt == 'Y') setColor(HIGH, HIGH, LOW);

    if (bt == '1')
    {
    digitalWrite(buzzerPin, HIGH);
    delay(5000);
    digitalWrite(buzzerPin, LOW);
    }
    if (bt == '0') digitalWrite(buzzerPin, LOW);
  }

  // ===== ULTRADZWIEK =====
  cm = 0.0344/2 * readUltrasonicDistance(3, 2);
  cale = cm / 2.54;

  String status = "";

  if (cm > 40)
  {
    setColor(HIGH, LOW, HIGH);
    status = "GRN";
    digitalWrite(buzzerPin, LOW);
  }
  else if (cm >= 20)
  {
    setColor(HIGH, LOW, LOW);
    status = "YEL";

    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(300);
  }
  else
  {
    setColor(HIGH, HIGH, LOW);
    status = "RED";

    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }

  // ===== LCD =====
  lcd.setCursor(0,0);
  lcd.print("In:");
  lcd.print(cale,1);
  lcd.print(" Cm:");
  lcd.print(cm,1);
  lcd.print("   ");

  lcd.setCursor(0,1);
  lcd.print("Status:");
  lcd.setCursor(8,1);
  lcd.print(status);
  lcd.print("   ");
}