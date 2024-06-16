#include "soft_serial_tx_util.h"
#include <Arduino.h>
#include <stdarg.h>
#include <stdint.h>
//#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////
        ///// NOTE: 1000000UL is NOT the F_CPU 
        /////       1000000UL is 1000000UL micro-seconds. There are 1000000UL uS in ONE second

const unsigned long MICROSECOND_PER_BIT = (1000000UL / SOFT_TX_BAUD);
/////////////////////////////////////////////////////
// void software_serial_configure() {

//        pinMode(SOFT_TX_PIN_NO, OUTPUT);    

// }
/////////////////////////////////////////////////////////////
 
void ss_printf(const char* format, ...)
{
  #define MAX_PRINTF_STRING_LENGTH 80
  char buffer_temp[MAX_PRINTF_STRING_LENGTH]; 
  va_list va;
  va_start(va, format);
  vsnprintf(buffer_temp, MAX_PRINTF_STRING_LENGTH, format, va);
  va_end(va);
  ss_puts(buffer_temp);                //and transmit string to software serial
}
/////////////////////////////////////////////////////////////
void ss_putc(char c) {

    uint8_t  bit_mask;

    digitalWrite(12, HIGH); // Disable TFT TFT_CS
    pinMode(SOFT_TX_PIN_NO, OUTPUT);

    // Transmit START bit
    digitalWrite(SOFT_TX_PIN_NO, LOW); 
    delayMicroseconds(MICROSECOND_PER_BIT);

    // Transmit 8-data bits
    for (bit_mask=0x01; bit_mask; bit_mask<<=1) {
        if (c & bit_mask) {
            digitalWrite(SOFT_TX_PIN_NO, HIGH);
        }
        else {
            digitalWrite(SOFT_TX_PIN_NO, LOW);
        }
        delayMicroseconds(MICROSECOND_PER_BIT);
    }

    // Transmit STOP bit(s)
    digitalWrite(SOFT_TX_PIN_NO, HIGH);
    delayMicroseconds(MICROSECOND_PER_BIT); // Fixed to ONE STOP Bit
    
}
///////////////////////////////////////////////////////////
void ss_puts(char *s)
{
    while(*s) 
        ss_putc(*s++);
}
////////////////////////////////////////////
void ss_puts_p(const char *progmem_s )
{
   register uint8_t c;
   
   while ( (c = pgm_read_byte(progmem_s++)) )
   ss_putc(c);

}
///////////////////////////////////////////////////////////////////
void ss_put_int(int  message_int){
   char buffer_temp[10];
   itoa( message_int, buffer_temp, 10);   //convert int to string (decimal format)
   ss_puts(buffer_temp);                //and transmit string to software serial
}
///////////////////////////////////////////////////////////////////
void ss_put_uint(unsigned int  message_uint){
   char buffer_temp[10];
   utoa( message_uint, buffer_temp, 10);   //convert unsigned int to string (decimal format)
   ss_puts(buffer_temp);                //and transmit string to software serial
}
///////////////////////////////////////////////////////////////////
void ss_put_long(long message_long){
   char buffer_temp[15];
   ltoa(message_long, buffer_temp, 10);   //convert long to string
   //ss_puts(" HELLO ");
   ss_puts(buffer_temp);                //and transmit string to software serial
}
////////////////////////////////////////////////////////////////////
void ss_put_float(float  message_float){
   char buffer_temp[15];
   dtostrf( message_float, 1, 2, buffer_temp); //  convert float to str
   ss_puts(buffer_temp);                //and transmit string to software serial
}
///////////////////////////////////////////////////////////////////
