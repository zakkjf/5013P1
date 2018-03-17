#ifndef _ADPS9301_H_
#define _ADPS9301_H_

#include <stdint.h>

#define BIT(n) (0x1U << (n))

/* Command register bits */
#define ADPS9301_CMD			BIT(7) /* Select command register. Must write as 1 */
#define ADPS9301_WORD			BIT(5) /* I2C write/read: if 1 word, if 0 byte */
#define ADPS9301_CLEAR			BIT(6) /* Interrupt clear. Clears pending interrupt */

/* Register set */
#define ADPS9301_CONTROL		0x00 /* Control of basic functions */
#define ADPS9301_TIMING			0x01 /* Timing and gain register */
#define ADPS9301_THRESHLOWLOW	0x02 /* Low byte of low interrupt threshold */
#define ADPS9301_THRESHLOWHIGH	0x03 /* Low byte of low interrupt threshold */
#define ADPS9301_THRESHHIGHLOW	0x04 /* Low byte of high interrupt threshold */
#define ADPS9301_THRESHHIGHHIGH	0x05 /* Low byte of high interrupt threshold */
#define ADPS9301_INTERRUPT		0x06 /* Interrupt control */
#define ADPS9301_ID				0x0A /* Product ID (Test Reg)*/

#define ADPS9301_CH0LOW			0x0C /* Low byte of ADC channel 0 */
#define ADPS9301_CH0HIGH		0x0D /* High byte of ADC channel 0 */
#define ADPS9301_CH1LOW			0x0E /* Low byte of ADC channel 1 */
#define ADPS9301_CH1HIGH		0x0F /* High byte of ADC channel 1 */

/* Power on/off value for ADPS9301_CONTROL register */
#define ADPS9301_POWER_ON		0x03
#define ADPS9301_POWER_OFF		0x00

#define ADPS9301_GAIN_MASK		0x04
#define ADPS9301_INTR_MASK   	0x04
#define ADPS9301_TIMING_MASK	0x03

int ADPS9301_get_CH0(int addr, char* interface, int16_t* value);

int ADPS9301_get_CH1(int addr, char* interface, int16_t* value);

int ADPS9301_get_lux(int addr, char* interface, double* value);
//device must be powered on before reading
int ADPS9301_power_on(int addr, char* interface);

int ADPS9301_run_everything(int addr, char* interface);
//device must be powered on before reading
int ADPS9301_set_timing(int addr, char* interface, uint8_t msg);

int ADPS9301_get_timing(int addr, char* interface, uint8_t* val);

int ADPS9301_set_high_gain(int addr, char* interface);

int ADPS9301_set_low_gain(int addr, char* interface);

int ADPS9301_set_integ_time(int addr, char* interface, uint8_t msg);

int ADPS9301_enable_int(int addr, char* interface);

int ADPS9301_disable_int(int addr, char* interface);

int ADPS9301_get_id(int addr, char* interface, uint8_t* value);

//device must be powered on before reading
int ADPS9301_set_thresholds(int addr, char* interface, uint16_t low, uint16_t high);

int ADPS9301_get_thresholds(int addr, char* interface, uint16_t* low, uint16_t* high);

int ADPS9301_write_single(int addr, char* interface, char reg, char msg);

int ADPS9301_read_single(int addr, char* interface, char reg, uint8_t* value);

#endif

