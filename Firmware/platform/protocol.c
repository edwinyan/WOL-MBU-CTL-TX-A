#include "common.h"
#include "stm32f4xx_gpio.h"
#include "protocol.h"
#include "can_drv.h"
#include "uart_drv.h"
#include "adc_drv.h"

//u8 send_buf[16]={0};

volatile packet_t dataToSend={0};


u8 checksum(u8 *data,u8 size)
{
	u8 ret;
	u16 sum=0;

	while(size--)
	{
		sum += *(data++);
	}

	ret = sum %256;

	return ret;
}

void adc_packet(u8 index)
{
	u8 i;
	
	if(index == 1)
	{
		dataToSend.adc_value[0] = adc_getvalue(ADC_CHANNEL_JS_THROTTLE) >> 8;
		dataToSend.adc_value[1] = adc_getvalue(ADC_CHANNEL_JS_THROTTLE);
		dataToSend.adc_value[2] = adc_getvalue(ADC_CHANNEL_JS_ROLL) >> 8;
		dataToSend.adc_value[3] = adc_getvalue(ADC_CHANNEL_JS_ROLL);
		dataToSend.adc_value[4] = adc_getvalue(ADC_CHANNEL_JS_PITCH) >> 8;
		dataToSend.adc_value[5] = adc_getvalue(ADC_CHANNEL_JS_PITCH);
		dataToSend.adc_value[6] = adc_getvalue(ADC_CHANNEL_JS_YAW) >> 8;
		dataToSend.adc_value[7] = adc_getvalue(ADC_CHANNEL_JS_YAW);
	}else if(index==2){
		dataToSend.adc_value[0] = adc_getvalue(ADC_CHANNEL_PTZ_V) >> 8;
		dataToSend.adc_value[1] = adc_getvalue(ADC_CHANNEL_PTZ_V);
		dataToSend.adc_value[2] = adc_getvalue(ADC_CHANNEL_PTZ_H) >> 8;
		dataToSend.adc_value[3] = adc_getvalue(ADC_CHANNEL_PTZ_H);
		dataToSend.adc_value[4] = 0x00;
		dataToSend.adc_value[5] = 0x00;
		dataToSend.adc_value[6] = 0x00;
		dataToSend.adc_value[7] = 0x00;
	}else{
		for(i=0;i<8;i++)
		{
			dataToSend.adc_value[i] = 0x00;
		}
	}
}



void data_packet(void)
{
	u8 index;
	u8 can_buf[16]={0};

	
	can_buf[0] = HEAD1;
	can_buf[1] = HEAD2;

	can_buf[2] = dataToSend.index_src;
	can_buf[3] = dataToSend.control_type;
	can_buf[4] = dataToSend.control_event;
	//send_buf[2] = src++;
	//send_buf[3] = 8;
	//send_buf[4] = 5;
	for(index=0;index<8;index++)
	{
		can_buf[5+index] = dataToSend.adc_value[index];
	}
	can_buf[13] = checksum(can_buf+2,11);
	can_buf[14] = TAIL1;
	can_buf[15] = TAIL2;
//	MSG("----pwr key pressed----1\n");
#if 1
	//send buffer by can bus
//	MSG("----pwr key pressed----2\n");
	CAN1_Send_Msg(can_buf,16);
#else
	uart_drv_comm_send(can_buf,16);
#endif
}




