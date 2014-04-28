/*
 * rest.cpp
 *
 *  Created on: 18.04.2014
 *      Author: Moritz
 */
//###############################################################################
/* UART Init */
void uart_init(void)
{

//   UBRRH = UBRRH_VALUE;
//   UBRRL = UBRRL_VALUE;
//   /* evtl. verkuerzt falls Register aufeinanderfolgen (vgl. Datenblatt)
//      UBRR = UBRR_VALUE;
//   */
//#if USE_2X
//   /* U2X-Modus erforderlich */
//   UCSRA |= (1 << U2X);
//#else
//   /* U2X-Modus nicht erforderlich */
//   UCSRA &= ~(1 << U2X);
//#endif
//*/

UBRRH = UBRR_VAL >> 8;
UBRRL = UBRR_VAL & 0xFF;
//  UCSRB |= (1<<TXEN);  // UART TX einschalten
//  UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // Asynchron 8N1
   UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // Asynchron 8N1
   UCSRB |= (1<<RXEN);                        // UART RX einschalten
}
//###############################################################################
/* Zeichen empfangen */
uint8_t uart_getc(void)
{
    while (!(UCSRA & (1<<RXC)))   // warten bis Zeichen verfuegbar
        ;
    return UDR;                   // Zeichen aus UDR an Aufrufer zurueckgeben
}
//###############################################################################
/* String empfangen */
void uart_gets( char* Buffer, uint8_t MaxLen, uint8_t Stoppcodon )
{
  uint8_t NextChar;
  uint8_t StringLen = 0;

  NextChar = uart_getc();         // Warte auf und empfange das nächste Zeichen

                                  // Sammle solange Zeichen, bis:
                                  // * entweder das String Ende Zeichen kam
                                  // * oder das aufnehmende Array voll ist
  while( NextChar != Stoppcodon && StringLen < MaxLen - 1 ) {
    *Buffer++ = NextChar;
    StringLen++;
    NextChar = uart_getc();
  }

                                  // Noch ein '\0' anhängen um einen Standard
                                  // C-String daraus zu machen
  *Buffer = '\0';
}


//###############################################################################
//###############################################################################
// G P S
//###############################################################################
//###############################################################################

//###################################################
/* Liest einen Teilstring in line[] bis end, z.b. ',' oder bis '*' dem Beginn der Checksumme*/

unsigned char GetLine(unsigned char end)
{
 unsigned char c,i;

//Achtung: Unten wird die Länge von line[] nicht überprüft !
//Könnte zum Absturz führen. line[] ist aber groß genug für
//die längste NMEA Meldung

 line[0]=0; //Besser is das ! Auf keinen Fall entfernen !
 i=0;
 do
  {
   c=uart_getc();
   if(c=='*') return 0xFF; //Checksumme erreicht
   if(c==',') return i; //Ende eines Wertes erreicht

   if(c!=end) //Buchstaben sammeln
    {
     line[i]=c;
     i++;
     line[i]=0; //End of String updaten
    }

  }while(c!=end);

 return i;
}

void GPRMC(void){
	  uint8_t c;
	  c = uart_getc();
	  if (c=='$'){
		  c = uart_getc();
		  if (c=='G'){
			  c = uart_getc();
			  if (c=='P'){
				  c = uart_getc();
				  if (c=='R'){
					  anzahl = GetLine(',');
					  anzahl = GetLine(',');
					  anzahl = GetLine(',');
					  anzahl = GetLine(',');
					  //Ab hier Breitengrad

					  //lcd_setcursor( 0, 1 );
					  //lcd_string(line);

					  anzahl = GetLine(',');
					  anzahl = GetLine(',');

					  //Ab hier Längengrad

					  //lcd_setcursor( 0, 2);
					  //lcd_string(line);

					  /*char Daten[64];
					  uart_gets( Daten, sizeof( Daten ),'\n' );
					  lcd_setcursor( 0, 1 );
					  lcd_string(Daten);*/
				  }
			  }
		  }
	  }



}





/*while(1){
	  uint8_t c;
	  c = uart_getc();
	  if (c=='$'){
		  c = uart_getc();
		  if (c=='G'){
			  c = uart_getc();
			  if (c=='P'){
				  c = uart_getc();
				  if (c=='R'){
					  anzahl = GetLine(',');
					  anzahl = GetLine(',');
					  anzahl = GetLine(',');
					  anzahl = GetLine(',');
					  //Ab hier Breitengrad

					  lcd_setcursor( 0, 1 );
					  lcd_string(line);

					  anzahl = GetLine(',');
					  anzahl = GetLine(',');

					  //Ab hier Längengrad

					  lcd_setcursor( 0, 2);
					  lcd_string(line);


				  }
			  }
		  }
	  }}*/



/*char Daten[64];
					  uart_gets( Daten, sizeof( Daten ),'\n' );
					  lcd_setcursor( 0, 1 );
					  lcd_string(Daten);*/
	//uint8_t c;
	//c = uart_getc();
  //if (c=='$'){
  //lcd_setcursor( 0, 1 );
  //}
  //lcd_data( c );




/*char Buffer[5];
    itoa( Temp, Buffer, 10 );
   lcd_string( Buffer );
return 0;*/
