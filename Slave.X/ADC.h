/* 
 * File:   ADC.h
 * Author: rodri
 *
 * Created on 26 de julio de 2023, 11:11 PM
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void adc_init(char Select);

void adc_change_channel(uint8_t canal);

int adc_get_channel(void);

int adc_read(void);

#endif	/* ADC_H */

