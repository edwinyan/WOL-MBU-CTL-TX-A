#include "common.h"
#include "stm32f4xx_gpio.h"
#include "gpio_drv.h"

/*----------------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *port;
    u16         pin;
}gpio_config_t;


STATIC gpio_config_t gpio_config_out_array[GPIO_SRC_NUM_OUT] = {
    {MASTER_RESET_165_GPIO, MASTER_RESET_165},  //74hc595 reset 
    {SPI_CLK_1_GPIO, SPI_CLK_1},	//74hc165 clk1
    {SPI_CLK_2_GPIO, SPI_CLK_2},	//74hc165 clk2
    {SPI_CLK_3_GPIO, SPI_CLK_3}, //74hc165 clk3

    {MASTER_RESET_595_GPIO, MASTER_RESET_595}, 	//74hc595 reset
	{STCP_CLK_1_GPIO, STCP_CLK_1}, 	//74hc595	 stcp clk1
	{STCP_CLK_2_GPIO, STCP_CLK_2}, 	//74hc595	 stcp clk2
	{STCP_CLK_3_GPIO, STCP_CLK_3}, 	//74hc595	 stcp clk3
	{STCP_CLK_4_GPIO, STCP_CLK_4}, 	//74hc595	 stcp clk4
	{SHCP_CLK_1_GPIO, SHCP_CLK_1}, 	//74hc595 shcp clk1
	{SHCP_CLK_2_GPIO, SHCP_CLK_2}, 	//74hc595 shcp clk2
	{SHCP_CLK_3_GPIO, SHCP_CLK_3}, 	//74hc595 shcp clk3
	{SHCP_CLK_4_GPIO, SHCP_CLK_4}, 	//74hc595 shcp clk4

	{IN_595_1_GPIO, IN_595_1}, 	
	{IN_595_2_GPIO, IN_595_2}, 	
	{IN_595_3_GPIO, IN_595_3}, 	
	{IN_595_4_GPIO, IN_595_4},

    {GPIOA,GPIO_Pin_2},

};

STATIC gpio_config_t gpio_config_in_array[GPIO_SRC_NUM_IN] = {
    {OUT_165_1_GPIO, OUT_165_1}, //74hc165 data out1
    {OUT_165_2_GPIO, OUT_165_2}, //74hc165 data out2
    {OUT_165_3_GPIO, OUT_165_3}, //74hc165 data out3
    
};
/*----------------------------------------------------------------------------*/
//global functions
void gpio_drv_init(void)
{
    GPIO_InitTypeDef  gpio_init;
	gpio_src_out_enum src1;
    gpio_src_in_enum src2;
    
    gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;

    for(src1 = GPIO_SRC_START_OUT; src1 < GPIO_SRC_NUM_OUT; src1++)
    {
        gpio_init.GPIO_Pin = gpio_config_out_array[src1].pin;
        GPIO_Init(gpio_config_out_array[src1].port, &gpio_init);
    }

	gpio_init.GPIO_Mode  = GPIO_Mode_IN;
	gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	for(src2 = GPIO_SRC_START_IN; src2 < GPIO_SRC_NUM_IN; src2++)
    {
        gpio_init.GPIO_Pin = gpio_config_in_array[src2].pin;
        GPIO_Init(gpio_config_in_array[src2].port, &gpio_init);
    }

}


void gpio_value_set(u8 src)
{
    ASSERT_D(src < GPIO_SRC_NUM_OUT);
    GPIO_SetBits(gpio_config_out_array[src].port, gpio_config_out_array[src].pin);
}

void gpio_value_reset(u8 src)
{
    ASSERT_D(src < GPIO_SRC_NUM_OUT);
    GPIO_ResetBits(gpio_config_out_array[src].port, gpio_config_out_array[src].pin);
}

