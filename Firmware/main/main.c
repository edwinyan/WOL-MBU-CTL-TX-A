#include "common.h"
#include "bsp.h"
#include "bsp_os.h"
#include "led_drv.h"
#include "tc_common.h"
#include "uart_drv.h"
#include "key_74hc165d_drv.h"
#include "led_74hc595d_drv.h"

OS_MUTEX	TX_MUTEX;		//uart tx mutex
OS_MUTEX	RX_MUTEX;		//uart rx mutex

OS_MUTEX	FIFO_MUTEX;

extern key_state_t key_state[NUMBER_OF_KEY];
extern const u16 led_index[16];
//static FIFO_T stFiFo;

const u16 led_test[16]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};

static u16 led_value[3]={0};	//led display value for each two 74hc595 input

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

/*----------------------------------------------------------------------------*/
//local function
STATIC void app_tx_task(void *p_arg)
{
	OS_ERR      err;

	(void)p_arg;
	
	while (DEF_TRUE) 
    {   
		


		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}

STATIC void app_key_scan_task(void *p_arg)
{
	OS_ERR      err;
	u8 index=0;
	u8 state=0;

	(void)p_arg;
	

	while (DEF_TRUE) 
    {   
    	if(read_shift_regs(&index,&state))
		{
			MSG("key%d =%d\n",index,state);
		}
		
		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}

STATIC void app_led_display_task(void *p_arg)
{
	OS_ERR      err;
	u8 chip_index;
	u8 key_index;
	//u16 led_value[3]={0};

	(void)p_arg;
	

	while (DEF_TRUE) 
    {   
    	//get led display values
    	for(chip_index=0;chip_index<3;chip_index++)
    	{
    		for(key_index=0;key_index<16;key_index++)
    		{
				if(key_state[chip_index*DATA_WIDTH_165+key_index].key_status == KEY_RELEASED)
				{
					led_value[chip_index] |= led_index[key_index];
				}
    		}
		}
		for(chip_index=0;chip_index<3;chip_index++)
		{
			write_shift_regs(led_value[chip_index],chip_index);
		}
		OSTimeDlyHMSM(0, 0, 0, 5, OS_OPT_TIME_HMSM_STRICT, &err);
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
	//u8 i,j;


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

    while (DEF_TRUE) 
    {   
        //tc_run_all();
       // MSG("------=------loop-------=------\r\n");
//       	for(i=0;i<3;i++)
//		{
//			for(j=0;j<16;j++)
//			{
//				write_shift_regs(led_test[j],i);
//				
//			}
//       	}
		OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);	
		
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

