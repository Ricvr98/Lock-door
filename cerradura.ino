#include <Wire.h>// Incluimos la librería Wire.h
#include <LCD.h>   // Incluimos la librería para el uso del display
#include <LiquidCrystal_I2C.h>  // Librería para la comunicación del display mediante I2C
 
#define I2C_ADDR    0x3F // Dirección del dispositivo I2C
 
LiquidCrystal_I2C             lcd( I2C_ADDR,2, 1, 0, 4, 5, 6, 7);
void setup()
   {
       lcd.begin (16,2);    // Inicializar el display con 16 caracteres 2 lineas
       lcd.setBacklightPin(3,POSITIVE);
       lcd.setBacklight(HIGH); 
   }
void loop()
   {
     lcd.home ();                   // go home
     lcd.print("    THIS  IS ");    // Muestra en pantalla
     lcd.setCursor ( 0, 1 );        // Cursor a la primera posición de la segunda linea
     lcd.print("    CS50X.ni");      // Muestra en pantalla
   }
