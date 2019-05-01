#ifndef _ADC_DRV_H_
#define _ADC_DRV_H_

#define ADC1_IN0_PIN	GPIO_Pin_0
#define ADC1_IN1_PIN	GPIO_Pin_1
#define ADC1_IN4_PIN	GPIO_Pin_4


#define ADC1_IN10_PIN	GPIO_Pin_0
#define ADC1_IN11_PIN	GPIO_Pin_1
#define ADC1_IN12_PIN	GPIO_Pin_2
#define ADC1_IN13_PIN	GPIO_Pin_3

//V -- vertical    ˙÷·
//H -- horizontal ∫·÷·

#define ADC_NUM_CHANNELS	7

#define ADC_CHANNEL_JS_ROLL		0		//joystick right channel1
#define	ADC_CHANNEL_JS_THROTTLE	1		//joystick right channel2
#define ADC_CHANNEL_VIN			2		//camera adc channel
#define ADC_CHANNEL_PTZ_H		3		//‘∆Ã®∫·÷·
#define ADC_CHANNEL_PTZ_V		4		//‘∆Ã® ˙÷·

#define ADC_CHANNEL_JS_YAW		5		//joystick left channel1
#define	ADC_CHANNEL_JS_PITCH	6		//joystick left channel2

void  Adc_Init(void);
u16 adc_getvalue(u8 num);



#endif
