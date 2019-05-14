#include "common.h"
#include "stm32f4xx_gpio.h"
#include "led_74hc595d_drv.h"
#include "gpio_drv.h"

#define NUMBER_OF_CHIPS 2
#define DATA_WIDTH_595	NUMBER_OF_CHIPS*8

const u16 led_index[16]={LED1,LED2,LED3,LED4,LED5,LED6,LED7,LED8,LED9,LED10,LED11,LED12,LED13,LED14,LED15,LED16};

/*----------------------------------------------------------------------------*/
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
#if 0
void write_shift_regs(u16 value,u8 index)
{
	u8 num;
	u8 bit;


	//clear all outputs
	GPIO_ResetBits(MASTER_RESET_595_GPIO,MASTER_RESET_595);
	delay_us(5);
	GPIO_SetBits(MASTER_RESET_595_GPIO,MASTER_RESET_595);
	
	switch(index)
	{
		case 0:
			for(num=0;num<DATA_WIDTH_595;num++)
			{
				bit = (value >> (DATA_WIDTH_595-1-num)) & 0x01;
				/*push data into register*/
				if(bit !=0)
				{
					GPIO_SetBits(IN_595_1_GPIO,IN_595_1);
				}else{
					GPIO_ResetBits(IN_595_1_GPIO,IN_595_1);
				}
				GPIO_SetBits(SHCP_CLK_1_GPIO,SHCP_CLK_1);
				delay_us(5);
				GPIO_ResetBits(SHCP_CLK_1_GPIO,SHCP_CLK_1);
				
			}
			/*store bits*/
			GPIO_SetBits(SHCP_CLK_1_GPIO,STCP_CLK_1);
			delay_us(5);
			GPIO_ResetBits(SHCP_CLK_1_GPIO,STCP_CLK_1);
			delay_us(5);
			break;
		case 1:
			for(num=0;num<DATA_WIDTH_595;num++)
			{
				bit = (value >> (DATA_WIDTH_595-1-num)) & 0x01;
				/*push data into register*/
				if(bit !=0)
				{
					GPIO_SetBits(IN_595_2_GPIO,IN_595_2);
				}else{
					GPIO_ResetBits(IN_595_2_GPIO,IN_595_2);
				}
				GPIO_SetBits(SHCP_CLK_2_GPIO,SHCP_CLK_2);
				delay_us(5);
				GPIO_ResetBits(SHCP_CLK_2_GPIO,SHCP_CLK_2);
				
			}
			
			/*store bits*/
			GPIO_SetBits(STCP_CLK_2_GPIO,STCP_CLK_2);
			delay_us(5);
			GPIO_ResetBits(STCP_CLK_2_GPIO,STCP_CLK_2);
			delay_us(5);
			break;
		case 2:
			for(num=0;num<DATA_WIDTH_595;num++)
			{
				bit = (value >> (DATA_WIDTH_595-1-num)) & 0x01;
				/*push data into register*/
				if(bit !=0)
				{
					GPIO_SetBits(IN_595_3_GPIO,IN_595_3);
				}else{
					GPIO_ResetBits(IN_595_3_GPIO,IN_595_3);
				}
				GPIO_SetBits(SHCP_CLK_3_GPIO,SHCP_CLK_3);
				delay_us(5);
				GPIO_ResetBits(SHCP_CLK_3_GPIO,SHCP_CLK_3);
				
			}
			
			/*store bits*/
			GPIO_SetBits(STCP_CLK_3_GPIO,STCP_CLK_3);
			delay_us(5);
			GPIO_ResetBits(STCP_CLK_3_GPIO,STCP_CLK_3);
			delay_us(5);
			break;
		case 3:
			for(num=0;num<DATA_WIDTH_595;num++)
			{
				bit = (value >> (DATA_WIDTH_595-1-num)) & 0x01;
				/*push data into register*/
				if(bit !=0)
				{
					GPIO_SetBits(IN_595_4_GPIO,IN_595_4);
				}else{
					GPIO_ResetBits(IN_595_4_GPIO,IN_595_4);
				}
				GPIO_SetBits(SHCP_CLK_4_GPIO,SHCP_CLK_4);
				delay_us(5);
				GPIO_ResetBits(SHCP_CLK_4_GPIO,SHCP_CLK_4);
				
			}
			
			/*store bits*/
			GPIO_SetBits(STCP_CLK_4_GPIO,STCP_CLK_4);
			delay_us(5);
			GPIO_ResetBits(STCP_CLK_4_GPIO,STCP_CLK_4);
			delay_us(5);
			break;
		default:
			break;
	}
}
#else
void write_shift_regs(u16 value,u8 index)
{
	u8 num;
	u8 bit;


	//clear all outputs
	GPIO_ResetBits(MASTER_RESET_595_GPIO,MASTER_RESET_595);
	delay_us(5);
	GPIO_SetBits(MASTER_RESET_595_GPIO,MASTER_RESET_595);
	
	switch(index)
	{
		case 0:
			for(num=0;num<DATA_WIDTH_595;num++)
			{
				bit = (value >> (DATA_WIDTH_595-1-num)) & 0x01;
				/*push data into register*/
				if(bit !=0)
				{
					GPIO_SetBits(IN_595_1_GPIO,IN_595_1);
				}else{
					GPIO_ResetBits(IN_595_1_GPIO,IN_595_1);
				}
				GPIO_SetBits(SHCP_CLK_1_GPIO,SHCP_CLK_1);
				delay_us(5);
				GPIO_ResetBits(SHCP_CLK_1_GPIO,SHCP_CLK_1);
				
			}
			/*store bits*/
			GPIO_SetBits(STCP_CLK_1_GPIO,STCP_CLK_1);
			delay_us(5);
			GPIO_ResetBits(STCP_CLK_1_GPIO,STCP_CLK_1);
			delay_us(5);
			break;
		case 1:
			for(num=0;num<DATA_WIDTH_595;num++)
			{
				bit = (value >> (DATA_WIDTH_595-1-num)) & 0x01;
				/*push data into register*/
				if(bit !=0)
				{
					GPIO_SetBits(IN_595_2_GPIO,IN_595_2);
				}else{
					GPIO_ResetBits(IN_595_2_GPIO,IN_595_2);
				}
				GPIO_SetBits(SHCP_CLK_2_GPIO,SHCP_CLK_2);
				delay_us(5);
				GPIO_ResetBits(SHCP_CLK_2_GPIO,SHCP_CLK_2);
				
			}
			
			/*store bits*/
			GPIO_SetBits(STCP_CLK_2_GPIO,STCP_CLK_2);
			delay_us(5);
			GPIO_ResetBits(STCP_CLK_2_GPIO,STCP_CLK_2);
			delay_us(5);
			break;
		case 2:
			for(num=0;num<DATA_WIDTH_595;num++)
			{
				bit = (value >> (DATA_WIDTH_595-1-num)) & 0x01;
				/*push data into register*/
				if(bit !=0)
				{
					GPIO_SetBits(IN_595_3_GPIO,IN_595_3);
				}else{
					GPIO_ResetBits(IN_595_3_GPIO,IN_595_3);
				}
				GPIO_SetBits(SHCP_CLK_3_GPIO,SHCP_CLK_3);
				delay_us(5);
				GPIO_ResetBits(SHCP_CLK_3_GPIO,SHCP_CLK_3);
				
			}
			
			/*store bits*/
			GPIO_SetBits(STCP_CLK_3_GPIO,STCP_CLK_3);
			delay_us(5);
			GPIO_ResetBits(STCP_CLK_3_GPIO,STCP_CLK_3);
			delay_us(5);
			break;
		case 3:
			for(num=0;num<DATA_WIDTH_595;num++)
			{
				bit = (value >> (DATA_WIDTH_595-1-num)) & 0x01;
				/*push data into register*/
				if(bit !=0)
				{
					GPIO_SetBits(IN_595_4_GPIO,IN_595_4);
				}else{
					GPIO_ResetBits(IN_595_4_GPIO,IN_595_4);
				}
				GPIO_SetBits(SHCP_CLK_4_GPIO,SHCP_CLK_4);
				delay_us(5);
				GPIO_ResetBits(SHCP_CLK_4_GPIO,SHCP_CLK_4);
				
			}
			
			/*store bits*/
			GPIO_SetBits(STCP_CLK_4_GPIO,STCP_CLK_4);
			delay_us(5);
			GPIO_ResetBits(STCP_CLK_4_GPIO,STCP_CLK_4);
			delay_us(5);
			break;
		default:
			break;
	}
}

#endif

void write_shift_init(void)
{
	GPIO_SetBits(MASTER_RESET_595_GPIO,MASTER_RESET_595);
	GPIO_ResetBits(STCP_CLK_1_GPIO,STCP_CLK_1);
	GPIO_ResetBits(STCP_CLK_2_GPIO,STCP_CLK_2);
	GPIO_ResetBits(STCP_CLK_3_GPIO,STCP_CLK_3);
	GPIO_ResetBits(STCP_CLK_4_GPIO,STCP_CLK_4);

	GPIO_ResetBits(SHCP_CLK_1_GPIO,SHCP_CLK_1);
	GPIO_ResetBits(SHCP_CLK_2_GPIO,SHCP_CLK_2);
	GPIO_ResetBits(SHCP_CLK_3_GPIO,SHCP_CLK_3);
	GPIO_ResetBits(SHCP_CLK_4_GPIO,SHCP_CLK_4);

	//GPIO_ResetBits(GPIOB,MASTER_RESET_595);
	
}






















