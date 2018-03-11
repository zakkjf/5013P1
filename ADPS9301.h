#ifndef _ADPS9301_H_
#define _ADPS9301_H_

#include <stdint.h>

#define BIT(n) (0x1U << (n))

/* Command register bits */
#define APDS9301_CMD			BIT(7) /* Select command register. Must write as 1 */
#define APDS9301_WORD			BIT(5) /* I2C write/read: if 1 word, if 0 byte */
#define APDS9301_CLEAR			BIT(6) /* Interrupt clear. Clears pending interrupt */

/* Register set */
#define APDS9301_CONTROL		0x00 /* Control of basic functions */
#define APDS9301_THRESHLOWLOW	0x02 /* Low byte of low interrupt threshold */
#define APDS9301_THRESHHIGHLOW	0x04 /* Low byte of high interrupt threshold */
#define APDS9301_INTERRUPT		0x06 /* Interrupt control */


#define APDS9301_CH0LOW			0x0C /* Low byte of ADC channel 0 */
#define APDS9301_CH0HIGH		0x0D /* High byte of ADC channel 0 */
#define APDS9301_CH1LOW			0x0E /* Low byte of ADC channel 1 */
#define APDS9301_CH1HIGH		0x0F /* High byte of ADC channel 1 */

/* Power on/off value for APDS9301_CONTROL register */
#define APDS9301_POWER_ON		0x03
#define APDS9301_POWER_OFF		0x00

/* Interrupts */
#define APDS9301_INTR_ENABLE	0x10
/* Interrupt Persist Function: Any value outside of threshold range */
#define APDS9301_THRESH_INTR	0x01

#define APDS9301_THRESH_MAX		0xFFFF /* Max threshold value */

int APDS9301_get_CH0(int addr, char* interface, uint16_t* value);

int APDS9301_get_CH1(int addr, char* interface, uint16_t* value);

int APDS9301_get_lux(int addr, char* interface, double* value);

//device must be powered on before reading
int APDS9301_power_on(int addr, char* interface);

#endif

