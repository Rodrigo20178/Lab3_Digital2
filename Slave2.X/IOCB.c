#include "IOCB.h"
#include <xc.h>


void ioc_int(char pin) {
    switch(pin)
    {
        case 0:
            IOCBbits.IOCB0 = 1;
            WPUBbits.WPUB0 = 1;
            break;
        case 1:
            IOCBbits.IOCB1 = 1;
            WPUBbits.WPUB1 = 1;
            break;
        case 2:
            IOCBbits.IOCB2 = 1;
            WPUBbits.WPUB2 = 1;
            break;
        case 3:
            IOCBbits.IOCB3 = 1;
            WPUBbits.WPUB3 = 1;
            break;
        case 4:
            IOCBbits.IOCB4 = 1;
            WPUBbits.WPUB4 = 1;
            break;
        case 5:
            IOCBbits.IOCB5 = 1;
            WPUBbits.WPUB5 = 1;
            break;
        case 6:
            IOCBbits.IOCB6 = 1;
            WPUBbits.WPUB6 = 1;
            break;
        case 7:
            IOCBbits.IOCB7 = 1;
            WPUBbits.WPUB7 = 1;
            break;
        default:
            IOCBbits.IOCB = 0;
            WPUBbits.WPUB = 0;
    }
}
