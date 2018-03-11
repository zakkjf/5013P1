#ifndef _ADPS9301_H_
#define _ADPS9301_H_

#include <stdint.h>

#define BIT(n) (0x1U << (n))

/* Command register bits */
#define APDS9300_CMD	BIT(7) /* Select command register. Must write as 1 */
#define APDS9300_WORD	BIT(5) /* I2C write/read: if 1 word, if 0 byte */
#define APDS9300_CLEAR	BIT(6) /* Interrupt clear. Clears pending interrupt */

/* Register set */
#define APDS9300_CONTROL	0x00 /* Control of basic functions */
#define APDS9300_THRESHLOWLOW	0x02 /* Low byte of low interrupt threshold */
#define APDS9300_THRESHHIGHLOW	0x04 /* Low byte of high interrupt threshold */
#define APDS9300_INTERRUPT	0x06 /* Interrupt control */
#define APDS9300_DATA0LOW	0x0C /* Low byte of ADC channel 0 */
#define APDS9300_DATA0HIGH	0x0D /* High byte of ADC channel 0 */
#define APDS9300_DATA1LOW	0x0E /* Low byte of ADC channel 1 */
#define APDS9300_DATA1HIGH	0x0F /* High byte of ADC channel 1 */


/* Power on/off value for APDS9300_CONTROL register */
#define APDS9300_POWER_ON	0x03
#define APDS9300_POWER_OFF	0x00

/* Interrupts */
#define APDS9300_INTR_ENABLE	0x10
/* Interrupt Persist Function: Any value outside of threshold range */
#define APDS9300_THRESH_INTR	0x01

#define APDS9300_THRESH_MAX	0xffff /* Max threshold value */

int ADPS9301_get_light(int addr, char* interface, uint16_t* value);

//device must be powered on before reading
int ADPS9301_power_on(int addr, char* interface);

#endif

