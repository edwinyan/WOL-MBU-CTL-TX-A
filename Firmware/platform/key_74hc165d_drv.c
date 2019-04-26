#include "common.h"
#include "stm32f4xx_gpio.h"
#include "key_74hc165d_drv.h"
#include "gpio_drv.h"
#include "bsp.h"
#include "bsp_os.h"

#define NUMBER_OF_SHIFT_CHIPS	2
#define DATA_WIDTH_165			NUMBER_OF_SHIFT_CHIPS*8
#define NUMBER_OF_KEY			48

key_state_t key_state[NUMBER_OF_KEY]={{0,0,0},{0,1,0},{0,2,0},{0,3,0},{0,4,0},{0,5,0},{0,6,0},{0,7,0},
									  {0,8,0},{0,9,0},{0,10,0},{0,11,0},{0,12,0},{0,13,0},{0,14,0},{0,15,0},
									  {1,0,0},{1,1,0},{1,2,0},{1,3,0},{1,4,0},{1,5,0},{1,6,0},{1,7,0},
									  {1,8,0},{1,9,0},{1,10,0},{1,11,0},{1,12,0},{1,13,0},{1,14,0},{1,15,0},
									  {2,0,0},{2,1,0},{2,2,0},{2,3,0},{2,4,0},{2,5,0},{2,6,0},{2,7,0},
									  {2,8,0},{2,9,0},{2,10,0},{2,11,0},{2,12,0},{2,13,0},{2,14,0},{2,15,0}};

STATIC  void delay(u16 count)
{
	while(count--)
		;
}

STATIC void delay_us(u16 us)
{
	u16 count;
	
	for(count=0;count<us;count++)
		delay(9);
}
	
/*----------------------------------------------------------------------------*/
#if 0
u8 read_shift_regs(u8 *index)
{
	u8 num;
	u16 value[3]={0};
	static u16 oldvalue[3];
	u8 ret=0;
	u8 bit[3];
	u16 changed=0;
	u8 chip_index=0;
	u8 isPressed = 0;

	GPIO_SetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	delay_us(5);
	GPIO_ResetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	delay_us(5);

	for(num=0;num<DATA_WIDTH_165;num++)
	{
		bit[0] = GPIO_ReadInputDataBit(OUT_165_1_GPIO,OUT_165_1);
		bit[1] = GPIO_ReadInputDataBit(OUT_165_2_GPIO,OUT_165_2);
		bit[2] = GPIO_ReadInputDataBit(OUT_165_3_GPIO,OUT_165_3);

		value[0] |= (bit[0] << ((DATA_WIDTH_165-1) - num));
		value[1] |= (bit[1] << ((DATA_WIDTH_165-1) - num));
		value[2] |= (bit[2] << ((DATA_WIDTH_165-1) - num));

		GPIO_SetBits(SPI_CLK_1_GPIO,SPI_CLK_1);
		GPIO_SetBits(SPI_CLK_2_GPIO,SPI_CLK_2);
		GPIO_SetBits(SPI_CLK_3_GPIO,SPI_CLK_3);
		delay_us(5);
		GPIO_ResetBits(SPI_CLK_1_GPIO,SPI_CLK_1);
		GPIO_ResetBits(SPI_CLK_2_GPIO,SPI_CLK_2);
		GPIO_ResetBits(SPI_CLK_3_GPIO,SPI_CLK_3);
	}
	//current value didnt equal prev value ,so some key has been pressed or released
	if(oldvalue[0] != value[0] || oldvalue[1] != value[1] || oldvalue[2] != value[2])
	{
		if(oldvalue[0] != value[0])//key1 -- key16 has been pressed or released
		{
			changed = oldvalue[0] ^ value[0];
			isPressed = changed & value[0];
			chip_index = 0;
			//ret = 1;
		}else if(oldvalue[1] != value[1]){ //key17 -- key32 has been pressed or released
			changed = oldvalue[1] ^ value[1];
			isPressed = changed & value[1];
			chip_index = 1;
			//ret = 1;
		}else{	//key33 -- key48 has been pressed or released
			changed = oldvalue[2] ^ value[2];
			isPressed = changed & value[2];
			chip_index = 2;
			//ret = 1;
		}
		switch(changed)
		{
			case KEY1:
				if(isPressed)
				{
					*index = 1+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY2:
				if(isPressed)
				{
					*index = 2+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY3:
				if(isPressed)
				{
					*index = 3+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY4:
				if(isPressed)
				{
					*index = 4+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY5:
				if(isPressed)
				{
					*index = 5+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY6:
				if(isPressed)
				{
					*index = 6+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY7:
				if(isPressed)
				{
					*index = 7+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY8:
				if(isPressed)
				{
					*index = 8+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY9:
				if(isPressed)
				{
					*index = 9+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY10:
				if(isPressed)
				{
					*index = 10+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY11:
				if(isPressed)
				{
					*index = 11+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY12:
				if(isPressed)
				{
					*index = 12+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY13:
				if(isPressed)
				{
					*index = 13+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY14:
				if(isPressed)
				{
					*index = 14+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY15:
				if(isPressed)
				{
					*index = 15+DATA_WIDTH_165*chip_index;
				}
				break;
			case KEY16:
				if(isPressed)
				{
					*index = 16+DATA_WIDTH_165*chip_index;
				}
				break;
			default:
				*index = 0;
				break;
		}
		ret = 1;
	}
	
	oldvalue[0] = value[0];
	oldvalue[1] = value[1];
	oldvalue[2] = value[2];

	return ret;
}
#else
u8 read_shift_regs(u16 *value1,u16 *value2,u16 *value3)
{
	u8 num;
	//u16 value[3]={0};
	static u16 oldvalue[3];
	u8 ret=0;
	u8 bit[3];
	u16 changed=0;
	u8 chip_index=0;
	u8 isPressed = 0;

	*value1=0;
	*value2=0;
	*value3=0;
	
	GPIO_ResetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);

	//GPIO_SetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	delay_us(5);
	//BSP_OS_TimeDly(1);
	//GPIO_ResetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	GPIO_SetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	delay_us(5);
	//BSP_OS_TimeDly(1);
#if 1
	for(num=0;num<DATA_WIDTH_165;num++)
	{
		bit[0] = GPIO_ReadInputDataBit(OUT_165_1_GPIO,OUT_165_1);
		bit[1] = GPIO_ReadInputDataBit(OUT_165_2_GPIO,OUT_165_2);
		bit[2] = GPIO_ReadInputDataBit(OUT_165_3_GPIO,OUT_165_3);

		*value1 |= (bit[0] << ((DATA_WIDTH_165-1) - num));
		*value2 |= (bit[1] << ((DATA_WIDTH_165-1) - num));
		*value3 |= (bit[2] << ((DATA_WIDTH_165-1) - num));

		GPIO_SetBits(SPI_CLK_1_GPIO,SPI_CLK_1);
		GPIO_SetBits(SPI_CLK_2_GPIO,SPI_CLK_2);
		GPIO_SetBits(SPI_CLK_3_GPIO,SPI_CLK_3);
		delay_us(5);
		GPIO_ResetBits(SPI_CLK_1_GPIO,SPI_CLK_1);
		GPIO_ResetBits(SPI_CLK_2_GPIO,SPI_CLK_2);
		GPIO_ResetBits(SPI_CLK_3_GPIO,SPI_CLK_3);
		delay_us(4);
	}
	//current value didnt equal prev value ,so some key has been pressed or released
	if(oldvalue[0] != *value1 || oldvalue[1] != *value2 || oldvalue[2] != *value3)
	{
		if(oldvalue[0] != *value1)//key1 -- key16 has been pressed or released
		{
			changed = oldvalue[0] ^ *value1;
			isPressed = changed & *value1;
			chip_index = 1;
			ret = 1;
		}else if(oldvalue[1] != *value2){ //key17 -- key32 has been pressed or released
			changed = oldvalue[1] ^ *value2;
			isPressed = changed & *value2;
			chip_index = 2;
			ret = 1;
		}else{	//key33 -- key48 has been pressed or released
			changed = oldvalue[2] ^ *value3;
			isPressed = changed & *value3;
			chip_index = 3;
			ret = 1;
		}
	}
	oldvalue[0] = *value1;
	oldvalue[1] = *value2;
	oldvalue[2] = *value3;

#endif
	return ret;
}
#endif


void read_shift_init(void)
{
	GPIO_SetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	GPIO_ResetBits(SPI_CLK_1_GPIO,SPI_CLK_1);
	GPIO_ResetBits(SPI_CLK_2_GPIO,SPI_CLK_2);
	GPIO_ResetBits(SPI_CLK_3_GPIO,SPI_CLK_3);

//	memset(key_state,0,sizeof(key_state)*NUMBER_OF_SHIFT_CHIPS);
}































