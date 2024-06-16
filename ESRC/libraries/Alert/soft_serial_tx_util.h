//
// Low qulaity: Rudimentary Software Serial Transmit
//

#ifndef SOFT_SERIAL_TX_UTIL_H
        #define SOFT_SERIAL_TX_UTIL_H

          ///////////#define SOFT_TX_BAUD 115200
        #define SOFT_TX_BAUD       9600
        
        // Fixed to ONE STOP BIT
        //#define SOFT_TX_STOP_BITS  1

        //  ES32-CAM-CH340 Board Specific pin number
        #define SOFT_TX_PIN_NO     1

        void software_serial_configure();
        void ss_putc(char c);
        void ss_puts(char *s);
        void ss_puts_p(const char *progmem_s );
        void ss_put_int(int  message_int);
        void ss_put_uint(unsigned int  message_uint);
        void ss_put_long(long message_long);
        void ss_put_float(float  message_float);

        


#endif // SOFT_SERIAL_TX_UTIL_H 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


