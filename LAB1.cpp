#define BUTTON1 13
#define BUTTON2 3

void setup(){

pinMode(12, OUTPUT);
pinMode(11, OUTPUT);
pinMode(10, OUTPUT);

pinMode(2, OUTPUT);
pinMode(1, OUTPUT);
pinMode(0, OUTPUT);

pinMode(BUTTON1, INPUT);
pinMode(BUTTON2, INPUT);

// stan początkowy
digitalWrite(10, HIGH); // zielone strona 1
digitalWrite(2, HIGH);  // czerwone strona 2

}

void loop(){

// samochód od strony 2
if(digitalRead(BUTTON2) == HIGH){

digitalWrite(10, LOW);
digitalWrite(11, HIGH);
delay(2000);

digitalWrite(11, LOW);
digitalWrite(12, HIGH);

delay(3000);

digitalWrite(1, HIGH);
delay(2000);

digitalWrite(1, LOW);
digitalWrite(2, LOW);
digitalWrite(0, HIGH);

}

// samochód od strony 1
if(digitalRead(BUTTON1) == HIGH){

digitalWrite(0, LOW);
digitalWrite(1, HIGH);
delay(2000);

digitalWrite(1, LOW);
digitalWrite(2, HIGH);

delay(3000);

digitalWrite(11, HIGH);
delay(2000);

digitalWrite(11, LOW);
digitalWrite(12, LOW);
digitalWrite(10, HIGH);

}

}