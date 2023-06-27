// Revisi 26/06/23
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


byte customChar[] = { B11000, B00100, B11000, B00100, B11000, B00000, B00000, B00000 };


const int ledH = 8;
const int ledK = 9;
const int ledM = 10;
const int buzzer = 3;
int trig = 4;
int echo = 5;
int duration;
int jarak;
int jarak_normal = 30;
int waspada;
int awas;
volatile int pulsa_sensor;
unsigned int literPerjam;
unsigned char pinFlowsensor = 2;
unsigned long waktuAktual;
unsigned long waktuLoop;
double liter;

void cacahPulsa()
{
  pulsa_sensor++;
}

void setup() {
  Serial.begin(9600);

pinMode(ledH, OUTPUT);
pinMode(ledK, OUTPUT);
pinMode(ledM, OUTPUT);
pinMode(trig, OUTPUT);
pinMode(echo, INPUT);
pinMode(buzzer, OUTPUT);

  pinMode(pinFlowsensor, INPUT);
  digitalWrite(pinFlowsensor, HIGH);
  attachInterrupt(0, cacahPulsa, RISING);
  sei();
  waktuAktual = millis();
  waktuLoop = waktuAktual;
lcd.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("ALAT  PERINGATAN");
lcd.setCursor(2,1);
lcd.print("DINI  BANJIR");
delay(2000);
lcd.clear();
lcd.createChar(0, customChar); 
}
void loop() {
  waspada = (jarak_normal / 3)* 2.5;
  awas = (jarak_normal / 3) * 1.5;
  digitalWrite(trig, LOW);delayMicroseconds(2);
  digitalWrite(trig, HIGH);delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  jarak = duration/58;
  
    
    Serial.print(literPerjam, DEC);
    Serial.println(" L/jam");
    Serial.print("Jarak :");
    Serial.print(jarak);
    Serial.println(" Cm");
  
  waktuAktual = millis();
  if(waktuAktual >= (waktuLoop + 1000))
  {
    waktuLoop = waktuAktual;
    literPerjam = (pulsa_sensor*60/7.5);
    pulsa_sensor = 0;
  }
  else{}

    if(jarak >= jarak_normal - 1)
    { 
    digitalWrite(ledH, HIGH);
    digitalWrite(ledK, LOW);
    digitalWrite(ledM, LOW);
    digitalWrite(buzzer, LOW);
    
    lcd.clear();   
    lcd.setCursor(0,0);
    lcd.print("Q:");
    lcd.setCursor(2,0);
    lcd.print(literPerjam, DEC);
    lcd.print("m");
    lcd.write(0);
    lcd.print("/j");
    lcd.setCursor(9,0);
    lcd.print("J:");
    lcd.setCursor(11,0);
    lcd.print(jarak);
    lcd.print("Cm");
    lcd.setCursor(5,1);
    lcd.print("-AMAN-");
    delay(2000);
    }
    
 else if(jarak <= waspada && jarak >= awas)
  {
    lcd.setCursor(0,0);
    lcd.print("Q:");
    lcd.setCursor(2,0);
    lcd.print(literPerjam, DEC);
    lcd.print("m");
    lcd.write(0);
    lcd.print("/j");
    lcd.setCursor(9,0);
    lcd.print("J:");
    lcd.setCursor(11,0);
    lcd.print(jarak);
    lcd.print("Cm");
    lcd.setCursor(0,1);
    lcd.print("-WASPADA BANJIR-");

    digitalWrite(ledK, HIGH);
    digitalWrite(ledM, LOW);
    digitalWrite(ledH, LOW);
    
    digitalWrite(buzzer, 80);
    delay(500);
    digitalWrite(buzzer, 0);
    delay(4500);
  }
  else if(jarak < awas)
  {    
    lcd.setCursor(0,0);
    lcd.print("Q:");
    lcd.setCursor(2,0);
    lcd.print(literPerjam, DEC);
    lcd.print("m");
    lcd.write(0);
    lcd.print("/j");
    lcd.setCursor(9,0);
    lcd.print("J:");
    lcd.setCursor(11,0);
    lcd.print(jarak);
    lcd.print("Cm");
    lcd.setCursor(1,1);
    lcd.print("-AWAS BANJIR!-");

    digitalWrite(ledM, HIGH);
    digitalWrite(ledK, LOW);
    digitalWrite(ledH, LOW);
    
    digitalWrite(buzzer, 80);
    delay(500);
    digitalWrite(buzzer, 0);
    delay(500);
    
  }
  else{
    digitalWrite(ledH, LOW);
    digitalWrite(ledK, LOW);
    digitalWrite(ledM, LOW);
    digitalWrite(buzzer, LOW);
    }
}
