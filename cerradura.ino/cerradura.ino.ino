#include <MFRC522.h>//Descargar e incluir la librería
#include <Wire.h>// Incluimos la librería Wire.h
#include <LCD.h>   // Incluimos la librería para el uso del display
#include <LiquidCrystal_I2C.h>  // Librería para la comunicación del display mediante I2C
#include <SPI.h>
#include <Servo.h>
/*
Pins  SPI    UNO  Mega2560  Leonardo
1 (NSS) SAD (SS)   10     53        10
2       SCK        13     52        SCK1
3       MOSI       11     51        MOSI1
4       MISO       12     50        MISO1
5       IRQ        *      *         *
6       GND       GND     GND       GND
7       RST        5      ?         Reset
8      +3.3V (VCC) 3V3     3V3       3.3V
* No necesario
*/

#define SAD 10
#define RST 5
MFRC522 nfc(SAD, RST);
Servo miServo;

#define ledPinAbierto    2
#define ledPinCerrado  3
#define I2C_ADDR    0x3F // Dirección del dispositivo I2C
LiquidCrystal_I2C             lcd( I2C_ADDR,2, 1, 0, 4, 5, 6, 7); 

void setup() 
{
  pinMode(ledPinAbierto  , OUTPUT);   
  pinMode(ledPinCerrado, OUTPUT);   
  SPI.begin();
  Serial.begin(115200);
  Serial.println("Buscando RC522");
  nfc.begin();
  byte version = nfc.getFirmwareVersion();
  lcd.begin (16,2);    // Inicializar el display con 16 caracteres 2 lineas
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  if (! version) 
  {//Entra si no encuentra el módulo.
    Serial.print("No ha encontrado RC522");
    lcd.home ();                   // go home
    lcd.print("RC522 no detectada");    // Muestra en pantalla
    while(1); //detener
  }
  Serial.print("Ha encontrado RC522");
  Serial.print("Firmware version 0x");
  Serial.print(version, HEX);
  Serial.println(".");
  lcd.home ();
  lcd.print("RC522 encontrada");
  miServo.attach(6);
}

#define AUTHORIZED_COUNT 1 //Para autoriazar más tarjetas ponemos el número aqui y la añadimos abajo
byte Authorized[AUTHORIZED_COUNT][6] = {{0x33, 0xA3, 0x62, 0x8B, 0x79}//,{0x10, 0x14, 0x39, 0x2E, 0xFF, 0xFF, } ejemplo de como autorizar más tarjetas  0x83,.....
                            
                          };
                          
void printSerial(byte *serial);
boolean isSame(byte *key, byte *serial);
boolean isAuthorized(byte *serial);



void loop() 
{
  byte status;
  byte data[MAX_LEN];
  byte serial[5];
  boolean Abierto = false;
  digitalWrite(ledPinAbierto, Abierto);
  digitalWrite(ledPinCerrado, !Abierto);
  status = nfc.requestTag(MF1_REQIDL, data);

  if (status == MI_OK) {
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);
    
    if(isAuthorized(serial))
    { 
      Serial.println("Autorizado");
      Abierto = true;
      lcd.home ();
      lcd.print("Acceso concedido");
      miServo.write(180);
      delay(3000);
      miServo.write(0);
    }
    else
    { 
      printSerial(serial);
      Serial.println("NO autorizado");
      Abierto = false;
      lcd.home ();
      lcd.print("Acceso denegado");
    }
    
    nfc.haltTag();
    digitalWrite(ledPinAbierto, Abierto);
    digitalWrite(ledPinCerrado, !Abierto);
    delay(2000);
  }//if (status == MI_OK)

  delay(500);

}//void loop()

boolean isSame(byte *key, byte *serial)
{
    for (int i = 0; i < 4; i++) {
      if (key[i] != serial[i])
      { 
        return false; 
      }
    }
    
    return true;

}

boolean isAuthorized(byte *serial)
{
    for(int i = 0; i<AUTHORIZED_COUNT; i++)
    {
      if(isSame(serial, Authorized[i]))
        return true;
    }
   return false;
}

void printSerial(byte *serial)
{
        Serial.print("Serial:");
    for (int i = 0; i < 5; i++) {// aumentar a 5 para leer el número de la tarjeta completo
      Serial.print(serial[i], HEX);
      Serial.print(" ");
    }
}

