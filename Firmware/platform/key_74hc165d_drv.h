#ifndef _KEY_74HC165D_DRV_H_
#define _KEY_74HC165D_DRV_H_


#define KEY1				0X0001
#define KEY2				0X0002
#define KEY3				0X0004
#define KEY4				0X0008
#define KEY5				0X0010
#define KEY6				0X0020
#define KEY7				0X0040
#define KEY8				0X0080
#define KEY9				0X0100
#define KEY10				0X0200
#define KEY11				0X0400
#define KEY12				0X0800
#define KEY13				0X1000
#define KEY14				0X2000
#define KEY15				0X4000
#define KEY16				0X8000

typedef struct{
	u8 chip_index;	//0 -- 2 chip index
	u8 key_index;	//0 -- 15 key index 	
	u8 key_status;	//0:released 1:hold 2:pressed
}key_state_t;


void read_shift_init(void);

#if 0
u8 read_shift_regs(u8 *index);
#else
u8 read_shift_regs(u16 *value1,u16 *value2,u16 *value3);
#endif

#endif
