#include "common.h"
#include "stm32f4xx_gpio.h"
#include "key_74hc165d_drv.h"
#include "gpio_drv.h"
#include "bsp.h"
#include "bsp_os.h"


key_state_t key_state[NUMBER_OF_KEY]={{0,0,0},{0,1,0},{0,2,0},{0,3,0},{0,4,0},{0,5,0},{0,6,0},{0,7,0},
									  {0,8,0},{0,9,0},{0,10,0},{0,11,0},{0,12,0},{0,13,0},{0,14,0},{0,15,0},
									  {1,0,0},{1,1,0},{1,2,0},{1,3,0},{1,4,0},{1,5,0},{1,6,0},{1,7,0},
									  {1,8,0},{1,9,0},{1,10,0},{1,11,0},{1,12,0},{1,13,0},{1,14,0},{1,15,0},
									  {2,0,0},{2,1,0},{2,2,0},{2,3,0},{2,4,0},{2,5,0},{2,6,0},{2,7,0},
									  {2,8,0},{2,9,0},{2,10,0},{2,11,0},{2,12,0},{2,13,0},{2,14,0},{2,15,0}};

const u16 key_index[16] = {KEY1,KEY2,KEY3,KEY4,KEY5,KEY6,KEY7,KEY8,KEY9,KEY10,KEY11,KEY12,KEY13,KEY14,KEY15,KEY16};

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

u8 read_shift_regs(u8 *index,u8 *state)
{
	u8 num;
	u16 value[3]={0};
	static u16 oldvalue[3];
	u8 ret=0;
	u8 bit[3];
	u16 changed=0;
	u8 chip_index=0;
	u16 isPressed = 0;
	u8 chip_count,key_count;

	//*value1=0;
	//*value2=0;
	//*value3=0;
	
	GPIO_ResetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	delay_us(5);
	GPIO_SetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	delay_us(5);
#if 1
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
		delay_us(4);
	}
	//current value didnt equal prev value ,so some key has been pressed or released
	if(oldvalue[0] != value[0] || oldvalue[1] != value[1] || oldvalue[2] != value[2])
	{
		if(oldvalue[0] != value[0])//key1 -- key16 has been pressed or released
		{
			changed = oldvalue[0] ^ value[0];//bit value of key
			isPressed = changed & value[0];
			chip_index = 1;		
		}else if(oldvalue[1] != value[1]){ //key17 -- key32 has been pressed or released
			changed = oldvalue[1] ^ value[1];
			isPressed = changed & value[1];
			chip_index = 2;
		}else{	//key33 -- key48 has been pressed or released
			changed = oldvalue[2] ^ value[2];
			isPressed = changed & value[2];
			chip_index = 3;
		}
		for(key_count=0;key_count<DATA_WIDTH_165;key_count++)
		{
			if((changed & key_index[key_count]) != 0)
			{
				if(isPressed)
				{
					key_state[DATA_WIDTH_165*(chip_index-1)+key_count].key_status = KEY_PRESSED;
					*state =KEY_PRESSED;
				}else{
					key_state[DATA_WIDTH_165*(chip_index-1)+key_count].key_status = KEY_RELEASED;
					*state =KEY_RELEASED;
				}
				*index =DATA_WIDTH_165*(chip_index-1)+key_count;
				// MSG("key%d = %d\n",*index,*state);
			}
		}
		 
		ret = 1;
	}
	//if *value != 0, so one or more keys is holding
	if(value[0] || value[1] || value[2])
	{
		for(chip_count=0;chip_count<NUMBER_OF_CHIPS;chip_count++)
		{
			for(key_count=0;key_count<DATA_WIDTH_165;key_count++)
			{
				if((value[chip_count] & key_index[key_count]) != 0)
				{
					key_state[chip_count*DATA_WIDTH_165+key_count].key_status = KEY_HOLDING;
				}
			}
		}
	}
	oldvalue[0] = value[0];
	oldvalue[1] = value[1];
	oldvalue[2] = value[2];

#endif
	return ret;
}



void read_shift_init(void)
{
	GPIO_SetBits(MASTER_RESET_165_GPIO,MASTER_RESET_165);
	GPIO_ResetBits(SPI_CLK_1_GPIO,SPI_CLK_1);
	GPIO_ResetBits(SPI_CLK_2_GPIO,SPI_CLK_2);
	GPIO_ResetBits(SPI_CLK_3_GPIO,SPI_CLK_3);
}































