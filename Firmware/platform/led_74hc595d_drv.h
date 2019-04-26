#ifndef _LED_74HC595D_DRV_H_
#define _LED_74HC595D_DRV_H_

#define LED1	0x0001
#define LED2	0x0002
#define	LED3	0x0004
#define LED4	0x0008
#define LED5	0x0010
#define LED6	0x0020
#define LED7	0x0040
#define LED8	0x0080
#define LED9	0x0100
#define LED10	0x0200
#define LED11	0x0400
#define LED12	0x0800
#define LED13	0x1000
#define LED14	0x2000
#define LED15	0x4000
#define LED16 	0x8000

void write_shift_regs(u16 value,u8 index);
void write_shift_init(void);

#endif
