/*
 * main.cpp
 *
 *  Created on: 17.04.2014
 *      Author: Moritz
 */

/*
 *
 *  Created on: 15.04.2014
 *      Author: Moritz
 */

//
// Anpassungen im makefile:
//    ATMega8 => MCU=atmega8 im makefile einstellen
//    lcd-routines.c in SRC = ... Zeile anhängen
//
#include <avr/io.h>
#include "lcd-routines.h"
#include <avr/interrupt.h>
#include "uart.h"
#include "nmea.h"
#include <math.h>
#include <util/delay.h>
#include <stdlib.h>

#ifndef F_CPU
#define F_CPU 8000000
#endif

#define BAUD 9600UL      // Baudrate

// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!
#endif
//#include <util/setbaud.h>

//char line[80];
//uint8_t anzahl;

//###############################################################################
//###############################################################################
// K L A S S E N
//###############################################################################
//###############################################################################
//###############################################################################

UART	uart0; // gps module
NMEA nmea;

//###############################################################################
//###############################################################################
// U A R T
//###############################################################################
//###############################################################################
//###############################################################################

//USART0 RX interrupt this code is executed when we recieve a character
ISR(USART_RXC_vect){
	volatile char c = UDR; //Read the value out of the UART buffer
	nmea.fusedata(c);
}

//###############################################################################
//###############################################################################
// M A I N
//###############################################################################
//###############################################################################

int main(void)
{
  // Initialisierung des LCD
  // Nach der Initialisierung müssen auf dem LCD vorhandene schwarze Balken
  // verschwunden sein
  lcd_init();
  lcd_setcursor( 0, 1 );
  lcd_string("GPS-Test!");

  //uart_init();

  // Text in einzelnen Zeichen ausgeben
  lcd_data( 'T' );
  lcd_data( 'e' );
  lcd_data( 's' );
  lcd_data( 't' );

  uart0.Init(0,9600, true); //uart0: GPS
  //lcd_string("DONE!");
  //lcd_string_format("Hallo","Check");

  while(1){
      lcd_setcursor( 0, 2 );
      // char Buffer[5];
	  //itoa( nmea.getAltitude(), Buffer, 10 );
	  //lcd_string( Buffer );
      //lcd_setcursor( 5, 1 );
      char Buffer2[5];
	  itoa( nmea.getMinute(), Buffer2, 10 );
	  lcd_string( Buffer2 );


	  //char Breitengrad[5];


	  // !!!!!!! Hier weiter! 	Mit fettem 644er AVR, wegen fließkommaarithmetik







      //dtostrf(nmea.getMinute(),5,2,Breitengrad);
      //lcd_string(Breitengrad );
	 // _delay_ms( 500 );
	  //lcd_string_format("%f",nmea.getLatitude());



  }

}













