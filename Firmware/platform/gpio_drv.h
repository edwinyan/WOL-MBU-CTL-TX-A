#ifndef _GPIO_DRV_H_
#define _GPIO_DRV_H_

 // 74HC165D pin define
#define MASTER_RESET_165	GPIO_Pin_15		//PA15
#define SPI_CLK_1			GPIO_Pin_12		//PB12
#define OUT_165_1			GPIO_Pin_13		//PB13

#define SPI_CLK_2			GPIO_Pin_14		//PB14
#define OUT_165_2			GPIO_Pin_15		//PB15

#define SPI_CLK_3			GPIO_Pin_8		//PC8
#define OUT_165_3			GPIO_Pin_9		//PC9

// 74HC165D GPIO define
#define MASTER_RESET_165_GPIO	GPIOA		//PA15
#define SPI_CLK_1_GPIO			GPIOB		//PB12
#define OUT_165_1_GPIO			GPIOB		//PB13

#define SPI_CLK_2_GPIO			GPIOB		//PB14
#define OUT_165_2_GPIO			GPIOB		//PB15

#define SPI_CLK_3_GPIO			GPIOC		//PC8
#define OUT_165_3_GPIO			GPIOC		//PC9


//74HC595D pin defines
#define IN_595_1	GPIO_Pin_5		//PA5
#define STCP_CLK_1	GPIO_Pin_6		//PA6
#define	SHCP_CLK_1	GPIO_Pin_7		//PA7

#define IN_595_2	GPIO_Pin_4		//PC4
#define STCP_CLK_2	GPIO_Pin_5		//PC5
#define	SHCP_CLK_2	GPIO_Pin_0		//PB0

#define IN_595_3	GPIO_Pin_8		//PA8
#define STCP_CLK_3	GPIO_Pin_11		//PA11
#define	SHCP_CLK_3	GPIO_Pin_12		//PA12

#define IN_595_4	GPIO_Pin_4		//PB4
#define STCP_CLK_4	GPIO_Pin_6		//PB5
#define	SHCP_CLK_4	GPIO_Pin_5		//PB6

//74HC595D GPIO defines
#define IN_595_1_GPIO		GPIOA	//PA5
#define STCP_CLK_1_GPIO		GPIOA		//PA6
#define	SHCP_CLK_1_GPIO		GPIOA		//PA7

#define IN_595_2_GPIO		GPIOC		//PC4
#define STCP_CLK_2_GPIO		GPIOC		//PC5
#define	SHCP_CLK_2_GPIO		GPIOB		//PB0

#define IN_595_3_GPIO		GPIOA		//PA8
#define STCP_CLK_3_GPIO		GPIOA		//PA11
#define	SHCP_CLK_3_GPIO		GPIOA		//PA12

#define IN_595_4_GPIO		GPIOB		//PB4
#define STCP_CLK_4_GPIO		GPIOB		//PB5
#define	SHCP_CLK_4_GPIO		GPIOB		//PB6

#define MASTER_RESET_595		GPIO_Pin_3	//PB3
#define MASTER_RESET_595_GPIO	GPIOB	//PB3

typedef enum{
    GPIO_SRC_START_OUT = 0,
	GPIO_SRC_RESET165 =GPIO_SRC_START_OUT,
	GPIO_SRC_SPICLK1,
	GPIO_SRC_SPICLK2,
	GPIO_SRC_SPICLK3,
	
	GPIO_SRC_RESET595,
	
	GPIO_SRC_STCPCLK1,
	GPIO_SRC_STCPCLK2,
	GPIO_SRC_STCPCLK3,
	GPIO_SRC_STCPCLK4,
	
	GPIO_SRC_SHCPCLK1,
	GPIO_SRC_SHCPCLK2,
	GPIO_SRC_SHCPCLK3,
	GPIO_SRC_SHCPCLK4,
	
	GPIO_SRC_OUT595_1,
	GPIO_SRC_OUT595_2,
	GPIO_SRC_OUT595_3,
	GPIO_SRC_OUT595_4,

	GPIO_SRC_TX2,		//gpio for control exchange video display
	
	GPIO_SRC_NUM_OUT

}gpio_src_out_enum;

typedef enum{
    GPIO_SRC_START_IN = 0,
	GPIO_SRC_OUT165_1 =GPIO_SRC_START_IN,
	GPIO_SRC_OUT165_2,
	GPIO_SRC_OUT165_3,
	
	GPIO_SRC_NUM_IN

}gpio_src_in_enum;

void gpio_drv_init(void);
void gpio_value_set(u8 src);
void gpio_value_reset(u8 src);



#endif
