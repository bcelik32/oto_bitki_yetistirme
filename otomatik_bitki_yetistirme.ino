#define sensorPin A0
#include "DHT.h"
#define DHTPIN A1   // veri okuma pini
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
int su_motor=10; // Role modulunun sinyal pininin bulundugu yeri kart degişkenine tanımladık.  
int fan=11; // Role modulunun sinyal pininin bulundugu yeri kart degişkenine tanımladık.  
int lcdpower=7;
int buton=5; //Butonun bağlı olduğu pin numarasını buton adlı değişkene aktardık.
#include <AvrI2c_Greiman.h>
#include <LiquidCrystal_I2C_AvrI2C.h>
LiquidCrystal_I2C_AvrI2C lcd(0x3f,16,2);
unsigned long oncekiMillis = 0;
const unsigned long interval = 1000; // 1 saniye
int sayac = 0;
int lcdsayac=0;
int havalandirmasayac=0;
unsigned long baslangicZamani; 
int dhtpin=8;
int lcdslider=0;
int tnpin=6;
byte celsiusSymbol[8] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
void setup() {
    baslangicZamani = millis();  // Başlangıç zamanını kaydetme

dht.begin();
pinMode(su_motor, OUTPUT); // Role kartının bulundugu pini açılmasını sağladık. 
pinMode(fan, OUTPUT); // Role kartının bulundugu pini açılmasını sağladık. 
Serial.begin(9600);
digitalWrite(fan, HIGH);
digitalWrite(su_motor, HIGH);
pinMode(lcdpower, OUTPUT);
digitalWrite(lcdpower, LOW);
pinMode(buton, INPUT);
pinMode(dhtpin, OUTPUT);
pinMode(tnpin, OUTPUT);
digitalWrite(tnpin, HIGH);
digitalWrite(dhtpin, HIGH);//Butonun bağlı olduğu (3 numaları) pini giriş pini olarak belirttik.   
lcd.begin();
  lcd.backlight();
    lcd.createChar(0, celsiusSymbol);

    lcd.clear();
}
void arttirSayac() {
  unsigned long suankiMillis = millis();

  if (suankiMillis - oncekiMillis >= interval) {
    sayac++;
    lcdsayac++;
    oncekiMillis = suankiMillis;
  }
    if (sayac==5400){
      digitalWrite(dhtpin, LOW);
      digitalWrite(tnpin, LOW);
      delay(600000);
      digitalWrite(dhtpin, HIGH);
      digitalWrite(tnpin, HIGH);
      }
}
void loop() {
   int tnpdurum = digitalRead(tnpin);

unsigned long gecenSure = millis() - baslangicZamani; // Geçen süreyi hesapla

  unsigned long gecenGun = gecenSure / (1000UL * 60UL * 60UL * 24UL); // Geçen günü hesapla
  unsigned long gecenSaat = (gecenSure / (1000UL * 60UL * 60UL)) % 24; // Geçen saati hesapla
  unsigned long gecenDakika = (gecenSure / (1000UL * 60UL)) % 60; // Geçen dakikayı hesapla
  unsigned long gecenSaniye = (gecenSure / 1000UL) % 60; // Geçen saniyeyi hesapla

int toprak_nem = analogRead(sensorPin);
int tnem2 = map(toprak_nem,0,1023,0,100);
int yuzde_toprak_nem =(100-tnem2);
float h = dht.readHumidity();
float t = dht.readTemperature();
float f = dht.readTemperature(true);
int nemc = map(h,0,1023,0,100);
int yuzde_nem =(100-nemc);


if (toprak_nem>600){
  if (tnpdurum == HIGH || tnpdurum==1){
digitalWrite(su_motor, LOW);
digitalWrite(fan, HIGH);
delay(1000);
digitalWrite(su_motor, HIGH);
delay(20000);
}

  }
if (toprak_nem<601){

digitalWrite(su_motor, HIGH);
  }
//if (toprak_nem>549&&toprak_nem<601){
//  digitalWrite(su_motor, HIGH);
 // }
if (t>30){
if (toprak_nem<601){
digitalWrite(fan, LOW);
  }
  }
if (t<31){
digitalWrite(fan, HIGH);
  }
//if (t>24&&t<28){
  //digitalWrite(fan, HIGH);
  //}
    //if(digitalRead(buton)==1){  //Buton değeri 1' eşitse....
//lcdsayac=lcdsayac+1;  }

if (lcdsayac<6){
lcd.setCursor(0,0);
lcd.print("Sicaklik: "); //İlk satıra yazalım
lcd.println(t);
lcd.setCursor(12,0);
lcd.print("C"); //İlk satıra yazalım
  lcd.write((byte)0);

lcd.println("     "); //İ//Led'in bağlı olduğu pine güç verdik
lcd.setCursor(0,1);
lcd.print("Toprak Nemi:%"); //İlk satıra yazalım
lcd.println(String(yuzde_toprak_nem)+" ");
}

if (lcdsayac>5 &&  lcdsayac<11){
  lcd.setCursor(0,0);
lcd.print("Sera Nemi:%"); //İlk satıra yazalım
lcd.println(String(h));
lcd.setCursor(13,0);
lcd.println("   ");
lcd.setCursor(0,1);
lcd.println("Aktif:");
  lcd.setCursor(6,1);

    lcd.print(gecenGun);
lcd.print(":");
lcd.print(gecenSaat);
  lcd.print(":");
  if (gecenDakika < 10) {
    lcd.print("0");
  }
  lcd.print(gecenDakika);
  lcd.print(":");
  if (gecenSaniye < 10) {
    lcd.print("0");
  }
  lcd.print(gecenSaniye);

  
}

if (lcdsayac>10 && lcdsayac<16){
  lcd.setCursor(0,0);
lcd.print(" Havalandirmaya"); //İlk satıra yazalım
lcd.setCursor(0,1);
lcd.print("   "+String(5400-sayac));
lcd.print(" Kaldi    ");
  }
  if (lcdsayac>15){
    
    lcdsayac=0;}

Serial.println(tnpdurum);
Serial.println(t);
Serial.println(h);
Serial.println(toprak_nem);
Serial.println(digitalRead(buton));
  arttirSayac();
    Serial.print("Havalandırmaya Kalan Süre: ");


  Serial.println(5400-sayac);
delay(1000);

    }
