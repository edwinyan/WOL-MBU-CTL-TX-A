#include "common.h"
#include "bsp.h"
#include "bsp_os.h"
#include "led_drv.h"
#include "tc_common.h"
#include "uart_drv.h"
#include "key_74hc165d_drv.h"
#include "led_74hc595d_drv.h"
#include "protocol.h"
#include "gpio_drv.h"
#include "can_drv.h"
#include "button_drv.h"
#include "adc_drv.h"
#include "flash_drv.h"
#include "buzzer_drv.h"

OS_MUTEX	TX_MUTEX;		//uart tx mutex
OS_MUTEX	RX_MUTEX;		//uart rx mutex

OS_MUTEX	FIFO_MUTEX;

//extern key_state_t key_state[NUMBER_OF_KEY];
extern app_t app;
extern const u16 led_index[16];
//static FIFO_T stFiFo;
//extern u32 test_flag;
//const u16 led_test[16]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};

//static u16 led_value[3]={0};	//led display value for each two 74hc595 input

//u8 needSaved[10] = {1,2,3,4,5,6,7,8,9,10};

/*----------------------------------------------------------------------------*/
//macro and variables
#define  APP_CFG_TASK_START_STK_SIZE             256u
#define  APP_CFG_TASK_START_PRIO                 2u

#define  APP_TX_TASK_STK_SIZE                    256u
#define  APP_TX_TASK_PRIO                        5u

#define  APP_KEY_TASK_STK_SIZE                   256u
#define  APP_KEY_TASK_PRIO                       3u

#define  APP_LED_TASK_STK_SIZE                   256u
#define  APP_LED_TASK_PRIO                       4u

static  OS_TCB   app_task_start_tcb;
static  CPU_STK  app_task_start_stk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   app_tx_task_tcb;
static  CPU_STK  app_tx_task_stk[APP_TX_TASK_STK_SIZE];

static  OS_TCB   app_key_task_tcb;
static  CPU_STK  app_key_task_stk[APP_KEY_TASK_STK_SIZE];

static  OS_TCB   app_led_task_tcb;
static  CPU_STK  app_led_task_stk[APP_LED_TASK_STK_SIZE];


extern volatile packet_t dataToSend;
/*----------------------------------------------------------------------------*/
//u8 read_enable=0;
//extern u8 press_count;
//extern u8 exchange_enable;
//extern u8 prev_display;

void power_in_status(void)
{	
	static u8 flash=0;
	float vddIn=0.0f;
	
	vddIn = adc_getvalue(ADC_CHANNEL_VIN)*3.3f*11/4095;
	//MSG("vddIn =%f\n",vddIn);
	if(vddIn < 23.0f)
	{
		if((flash++)%2 == 0){
			write_shift_regs(0x0001,3);
			buzzer(1);
		}else{
			write_shift_regs(0x0000,3);
			buzzer(0);
		}
	}else{
		write_shift_regs(0x0001,3);
		buzzer(0);
	}
}

//local function
STATIC void app_tx_task(void *p_arg)
{
	OS_ERR      err;
	
	(void)p_arg;
	
	while (DEF_TRUE) 
    {   
		if(dataToSend.repeat)
		{
			if(dataToSend.repeat != 0xFE)
				dataToSend.repeat--;
			if(dataToSend.power_enable && dataToSend.packet_enable)
			{
				adc_packet(dataToSend.event_index);
				data_packet();
			}
		}
		if(dataToSend.power_trig == 1)
		{
			data_packet();
			dataToSend.power_trig = 0;
		}

		OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}

STATIC void app_key_scan_task(void *p_arg)
{
	OS_ERR      err;
	u8 index=0;
//	u8 Index=0;
//u8 display[10]={0};

//u8 i;
	

	(void)p_arg;
	

	while (DEF_TRUE) 
    { 
    	if(read_shift_regs(&index) == 4){
//			if(index == 47)
//			{
//				gpio_value_reset(GPIO_SRC_TX2);
//				OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &err);
//				gpio_value_set(GPIO_SRC_TX2);
//				needSaved[0]++;
//				FLASH_Write(FLASH_SAVE_ADDR,(u32*)needSaved,10);
//				
//				//FLASH_Read(FLASH_SAVE_ADDR,(u32 *)display,1);
//				//read_enable =1;
//				for(i=0;i<10;i++){
//					if(display[i] != 0)
//					{
//						//MSG("display0 = %d\n",display[0]);
//					}
//				}
//				//MSG("set value to flash\n");
//			}
			key_state_remap_handle(index);
    	}
		pwr_key_scan();
		OSTimeDlyHMSM(0, 0, 0, 15, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}

STATIC void app_led_display_task(void *p_arg)
{
	OS_ERR      err;
	u8 chip_index;
	u8 key_index;
	u16 led_value[4]={0,0,0,0};

	(void)p_arg;
	

	while (DEF_TRUE) 
    {   
    	led_value[0]=0;
		led_value[1]=0;
		led_value[2]=0;
    	//get led display values
    	for(chip_index=0;chip_index<3;chip_index++)
    	{
    		for(key_index=0;key_index<16;key_index++)
    		{
				if(app.key_state[chip_index*DATA_WIDTH_165+key_index].key_status == KEY_RELEASED)
				{
					led_value[chip_index] |= led_index[key_index];
				}
    		}
		}
		for(chip_index=0;chip_index<3;chip_index++)
		{
			write_shift_regs(led_value[chip_index],chip_index);
		}
		OSTimeDlyHMSM(0, 0, 0, 20, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}


STATIC void app_task_create(void)
{

	OS_ERR      err;
#if 1	
    OSTaskCreate((OS_TCB       *)&app_tx_task_tcb,              
                 (CPU_CHAR     *)"App tx Task",
                 (OS_TASK_PTR   )app_tx_task, 
                 (void         *)0,
                 (OS_PRIO       )APP_TX_TASK_PRIO,
                 (CPU_STK      *)&app_tx_task_stk[0],
                 (CPU_STK_SIZE  )app_tx_task_stk[APP_TX_TASK_STK_SIZE / 10],
                 (CPU_STK_SIZE  )APP_TX_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
#endif
#if 1
    OSTaskCreate((OS_TCB       *)&app_key_task_tcb,              
                 (CPU_CHAR     *)"App key scan Task",
                 (OS_TASK_PTR   )app_key_scan_task, 
                 (void         *)0,
                 (OS_PRIO       )APP_KEY_TASK_PRIO,
                 (CPU_STK      *)&app_key_task_stk[0],
                 (CPU_STK_SIZE  )app_key_task_stk[APP_KEY_TASK_STK_SIZE / 10],
                 (CPU_STK_SIZE  )APP_KEY_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
#endif

#if 1	
    OSTaskCreate((OS_TCB       *)&app_led_task_tcb,              
                 (CPU_CHAR     *)"App led display Task",
                 (OS_TASK_PTR   )app_led_display_task, 
                 (void         *)0,
                 (OS_PRIO       )APP_LED_TASK_PRIO,
                 (CPU_STK      *)&app_led_task_stk[0],
                 (CPU_STK_SIZE  )app_led_task_stk[APP_LED_TASK_STK_SIZE / 10],
                 (CPU_STK_SIZE  )APP_LED_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
#endif

}

STATIC void app_task_start(void *p_arg)
{
    OS_ERR      err;
	//u32 i =0;
//	u8 i;
//	u16 adcValue[7]={0};
	u8 display[2]={0};
	u8 index;

   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */
    
    BSP_Tick_Init();                                            /* Start Tick Initialization                            */
    
    Mem_Init();                                                 /* Initialize Memory Management Module                  */
    Math_Init();                                                /* Initialize Mathematical Module                       */

    BSP_Peripheral_Init();

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    MSG("Creating Application Tasks: %d\r\n",__FPU_USED);
	OSMutexCreate((OS_MUTEX*	)&FIFO_MUTEX,
				  (CPU_CHAR*	)"UART FIFO_MUTEX",
				  (OS_ERR*		)&err);
	OSMutexCreate((OS_MUTEX*	)&TX_MUTEX,
				  (CPU_CHAR*	)"UART TX_MUTEX",
				  (OS_ERR*		)&err);
	OSMutexCreate((OS_MUTEX*	)&RX_MUTEX,
				  (CPU_CHAR*	)"UART RX_MUTEX",
				  (OS_ERR*		)&err);

    app_task_create();                                            
	gpio_value_set(GPIO_SRC_TX2);
	//for exchange init
	//handle_display_exchange(8);

	FLASH_Read(FLASH_SAVE_ADDR,(u32 *)display,2);
	index = display[0];
	if(index <8){
		app.key_state[index].key_status = KEY_RELEASED;
		app.display_state.prev_display = index;
		//prev_display = index;
	}

	//write_shift_regs(0x8421,3);
    while (DEF_TRUE) 
    {   
  //      tc_run_all();
//		for(i=0;i<7;i++)
//		{
//				adcValue[i] = adc_getvalue(i);
//			MSG("%d,",adcValue[i]);
//		}
		//MSG("%d\n",test_flag);
		//MSG("\n");
		power_in_status();
#if 1
		if(app.display_state.exchange_event == TRUE)
		{
			handle_display_exchange(app.display_state.press_count);
			app.display_state.exchange_event = FALSE;
		}
 #else 
		if(exchange_enable ==1)
		{
			handle_display_exchange(press_count);
			exchange_enable=0;Q
		}
#endif
		//MSG("-----------------------------------------------\n");
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err);	
		
    }
}


/*----------------------------------------------------------------------------*/
int main(void)
{
    OS_ERR err;

    //BSP_IntDisAll();
    OSInit(&err);

    OSTaskCreate((OS_TCB       *)&app_task_start_tcb,              
                 (CPU_CHAR     *)"App Task Start",
                 (OS_TASK_PTR   )app_task_start, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,
                 (CPU_STK      *)&app_task_start_stk[0],
                 (CPU_STK_SIZE  )app_task_start_stk[APP_CFG_TASK_START_STK_SIZE / 10],
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

    OSStart(&err);                                              
    
    (void)&err;
    
    return 0;
}

