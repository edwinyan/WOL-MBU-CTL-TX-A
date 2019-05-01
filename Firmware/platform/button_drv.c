#include "common.h"
#include "stm32f4xx_gpio.h"
#include "button_drv.h"
#include "protocol.h"
#include "can_drv.h"

/*----------------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *port;
    u16         pin;
	//GPIOPuPd_TypeDef pupd;
}button_config_t;


STATIC button_config_t button_config_array[BUTTON_SRC_NUM] = {
	{GPIOC, GPIO_Pin_13},
    {GPIOC, GPIO_Pin_14},
	{GPIOC, GPIO_Pin_15},
	{GPIOB, GPIO_Pin_7},
};

extern volatile packet_t dataToSend;
/*----------------------------------------------------------------------------*/
//global functions
void button_drv_init(void)
{
    GPIO_InitTypeDef  gpio_init;
    button_src_enum src;
    
    gpio_init.GPIO_Mode  = GPIO_Mode_IN;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;

    for(src = BUTTON_SRC_START; src < BUTTON_SRC_NUM; src++)
    {
        gpio_init.GPIO_Pin = button_config_array[src].pin;
		//gpio_init.GPIO_PuPd = button_config_array[src].pupd;
        GPIO_Init(button_config_array[src].port, &gpio_init);
    }
}


u8 button_value_get(button_src_enum src)
{
    ASSERT_D(src < BUTTON_SRC_NUM);
    return GPIO_ReadInputDataBit(button_config_array[src].port, button_config_array[src].pin);
}

void pwr_key_scan(void)
{
	button_src_enum i;
	static u8 enable_count=0;
	u8 value[BUTTON_SRC_NUM];
//	u8 send_buf[16];
	static u8 prev_value[BUTTON_SRC_NUM]={0};

	for(i=0;i<4;i++)
	{
		value[i] = button_value_get(i);
		if(value[i] != prev_value[i])	
		{
			if(value[i] == 1) //pwr key pressed
			{
				dataToSend.index_src = i+5;
				dataToSend.control_type= CONTROL_SRC_OPEN;
				dataToSend.control_event = EVENT_SRC_OPEN;
				enable_count++;
				//dataToSend.repeat = 5;
				dataToSend.power_enable = 1;
				dataToSend.power_trig = 1;
				//data_packet();
				//MSG("pwr key%d pressed\n",i);
			}else{
				dataToSend.index_src = i+5;
				dataToSend.control_type= CONTROL_SRC_CLOSE;
				dataToSend.control_event = EVENT_SRC_CLOSE;
				enable_count--;
				if(enable_count == 0){
					dataToSend.power_enable = 0;					
				}
				//dataToSend.repeat = 5;
				dataToSend.power_trig = 1;
				//data_packet();
				//CAN1_Send_Msg(send_buf,16);
				//MSG("pwr key%d released\n",i);
			}
			prev_value[i] = value[i];
			
		}
		
	}
}


/*----------------------------------------------------------------------------*/



