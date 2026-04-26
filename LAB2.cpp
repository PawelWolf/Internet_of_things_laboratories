#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27
LiquidCrystal_I2C lcd(I2C_ADDR,16,2);


#define HOTRGB 7
#define NORMALRGB 6
#define COLDRGB 5
#define HOT 4
#define NORMAL 3
#define COLD 2

float hotTemp = 20;
float coldTemp = 15;

void setup() {
  pinMode(HOT, OUTPUT);
  pinMode(NORMAL, OUTPUT);
  pinMode(COLD, OUTPUT);
  pinMode(HOTRGB, OUTPUT);
  pinMode(NORMALRGB, OUTPUT);
  pinMode(COLDRGB, OUTPUT);
  
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

}

void loop() {

// reset RGB
digitalWrite(COLDRGB, HIGH);
digitalWrite(NORMALRGB, HIGH);
digitalWrite(HOTRGB, HIGH);

  int sensor = analogRead(A0);
  float celsius = (sensor * 5.0) / 1023.0;
//  float celsius = (voltage - 0.5) * 100.0;
  celsius = celsius * 100 -7;
  Serial.print("Temperatura: ");
  Serial.println(celsius);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(celsius);
  lcd.print(" C");

  lcd.setCursor(0,1);


if (celsius < coldTemp) {
  lcd.print("Zimno");

  digitalWrite(COLD, HIGH);
  digitalWrite(NORMAL, LOW);
  digitalWrite(HOT, LOW);
//  digitalWrite(NORMALRGB, LOW);
//  digitalWrite(HOTRGB, LOW);
  digitalWrite(COLDRGB, LOW);

} 
else if (celsius <= hotTemp) {
  lcd.print("W sam raz");

  digitalWrite(COLD, LOW);
  digitalWrite(NORMAL, HIGH);
  digitalWrite(HOT, LOW);
//  digitalWrite(COLDRGB, LOW);
//  digitalWrite(HOTRGB, LOW);
  digitalWrite(NORMALRGB, LOW);

} 
else {
  lcd.print("Cieplo");

  digitalWrite(COLD, LOW);
  digitalWrite(NORMAL, LOW);
  digitalWrite(HOT, HIGH);
//  digitalWrite(COLDRGB, LOW);
//  digitalWrite(NORMALRGB, LOW);
  digitalWrite(HOTRGB, LOW);
}

  delay(1000);
}