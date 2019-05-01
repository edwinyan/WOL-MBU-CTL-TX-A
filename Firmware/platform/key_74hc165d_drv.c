#include "common.h"
#include "stm32f4xx_gpio.h"
#include "key_74hc165d_drv.h"
#include "gpio_drv.h"
#include "protocol.h"
#include "bsp.h"
#include "bsp_os.h"
//#include "adc_drv.h"


key_state_t key_state[NUMBER_OF_KEY]={{0,0,0,0},{0,1,0,0},{0,2,0,0},{0,3,0,0},{0,4,0,0},{0,5,0,0},{0,6,0,0},{0,7,0,0},
									  {0,8,0,0},{0,9,0,0},{0,10,0,0},{0,11,0,0},{0,12,0,0},{0,13,0,0},{0,14,0,0},{0,15,0,0},
									  {1,0,0,0},{1,1,0,0},{1,2,0,0},{1,3,0,0},{1,4,0,0},{1,5,0,0},{1,6,0,0},{1,7,0,0},
									  {1,8,0,0},{1,9,0,0},{1,10,0,0},{1,11,0,0},{1,12,0,0},{1,13,0,0},{1,14,0,0},{1,15,0,0},
									  {2,0,0,0},{2,1,0,0},{2,2,0,0},{2,3,0,0},{2,4,0,0},{2,5,0,0},{2,6,0,0},{2,7,0,0},
									  {2,8,0,0},{2,9,0,0},{2,10,0,0},{2,11,0,0},{2,12,0,0},{2,13,0,0},{2,14,0,0},{2,15,0,0}};

const u16 key_index[16] = {KEY1,KEY2,KEY3,KEY4,KEY5,KEY6,KEY7,KEY8,KEY9,KEY10,KEY11,KEY12,KEY13,KEY14,KEY15,KEY16};

aircraft_state_t aircraft_state[4]={{0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0}};

extern packet_t dataToSend;


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
u8 get_selected_aircraft(void)
{
	u8 ret=0;
	
	if(key_state[KEY_SRC_CTLOPT_AIRCRAFT1].key_status == KEY_RELEASED){
		ret=1;
	}
	if(key_state[KEY_SRC_CTLOPT_AIRCRAFT2].key_status == KEY_RELEASED){
		ret=2;
	}
	if(key_state[KEY_SRC_CTLOPT_AIRCRAFT3].key_status == KEY_RELEASED){
		ret=3;
	}
	if(key_state[KEY_SRC_CTLOPT_AIRCRAFT4].key_status == KEY_RELEASED){
		ret=4;
	}

	return ret;
}

void key_state_remap_handle(u8 index)
{
	u8 count,count_ch;
	static u8 prev_index;//上一次触发的按键
	static u8 aircraftFlag=0;//飞机控制选择flag，如果此状态不为1，则无法选择控制动作
	static u8 aircraft_selected=0; //被选中的飞行器
	//static u8 controlballFalg=0;//布控球选择flag
	//key0 -- key7 关联处理(互斥)

	if(index<8)
	{
		for(count=KEY_SRC_DISPLAY_AIRCRAFT1;count<KEY_SRC_STREAMING_AIRCRAFT1;count++)
		{
			if(key_state[count].key_status == KEY_RELEASED && count != index)
			{
				key_state[count].key_status = KEY_RELEASE;
			}
			//发送视频显示切换事件
			//MSG("display selected--%d\n",index);
		}
	}
	else if(index < KEY_SRC_CTLOPT_AIRCRAFT1 && index >=KEY_SRC_STREAMING_AIRCRAFT1){//key8 -- key15关联处理(独立可复用，自身开关)
		if(key_state[index].key_set == 1)
		{
			key_state[index].key_status = KEY_RELEASE;
			key_state[index].key_set = 0;
			//关闭推流事件
			//MSG("close push flow channel %d\n",(index-8));
		}else{
			//发送推流事件
			//MSG("open push flow channel %d\n",(index-8));
		}
		
		key_state[index].key_set++;
		
	}else if(index < KEY_SRC_CTLMOTION_AIRCRAFT && index >= KEY_SRC_CTLOPT_AIRCRAFT1){//key16 -- key23关联处理(互斥)
		for(count=KEY_SRC_CTLOPT_AIRCRAFT1;count<KEY_SRC_CTLMOTION_AIRCRAFT;count++)
		{
			if(key_state[count].key_status == KEY_RELEASED && count != index)
			{
				key_state[count].key_status = KEY_RELEASE;
			}
			//如果当前的按键不是上一次选中的按键
			if(prev_index != index){
				for(count_ch=KEY_SRC_CTLMOTION_AIRCRAFT;count_ch<KEY_SRC_BACKUP1;count_ch++)
				{
					key_state[count_ch].key_status = KEY_RELEASE;
				}
			}
			
			aircraftFlag = (key_state[KEY_SRC_CTLOPT_AIRCRAFT1].key_status == KEY_RELEASED)
						|| (key_state[KEY_SRC_CTLOPT_AIRCRAFT2].key_status == KEY_RELEASED)
						|| (key_state[KEY_SRC_CTLOPT_AIRCRAFT3].key_status == KEY_RELEASED)
						|| (key_state[KEY_SRC_CTLOPT_AIRCRAFT4].key_status == KEY_RELEASED);
			//MSG("aircraftFlag=%d\n",aircraftFlag);
			
			aircraft_selected = get_selected_aircraft();
			//MSG("aircraft_selected= %d\n",aircraft_selected);
			dataToSend.packet_enable &= aircraftFlag;
			dataToSend.packet_enable &= (key_state[KEY_SRC_CTLMOTION_AIRCRAFT].key_status==KEY_RELEASED || key_state[KEY_SRC_CTLMOTION_PTZ].key_status==KEY_RELEASED);
			prev_index = index;
		}
		//如果选择的是飞行器1到4，则需要设置相应的状态位
		if(index <= KEY_SRC_CTLOPT_AIRCRAFT4){
			if(aircraft_selected !=0){
				aircraft_state[aircraft_selected-1].aircraft_selected = 1;
				//显示当前飞行器的警灯警笛状态
				if(aircraft_state[aircraft_selected-1].alarm_lamp == 1)
				{
					key_state[KEY_SRC_ALARM_LAMP].key_status = KEY_RELEASED;
				}
				if(aircraft_state[aircraft_selected-1].alarm_whistle1 == 1)
				{
					key_state[KEY_SRC_ALARM_WHISTLE1].key_status = KEY_RELEASED;
				}
				if(aircraft_state[aircraft_selected-1].alarm_whistle2 == 1)
				{
					key_state[KEY_SRC_ALARM_WHISTLE2].key_status = KEY_RELEASED;
				}
				if(aircraft_state[aircraft_selected-1].shouting== 1)
				{
					key_state[KEY_SRC_SHOUTING].key_status = KEY_RELEASED;
				}
				
			}
			for(count_ch=0;count_ch<4;count_ch++)
			{
				if(count_ch != aircraft_selected-1){
					aircraft_state[count_ch].aircraft_selected = 0;
				}
			}
			
		}
	}
	else if(index < 32 && index >= 24){ //key24 -- key31 关联处理
		//key17 -- key20中任意一个按键被按下，key24才起作用
		if(index == KEY_SRC_CTLMOTION_AIRCRAFT || index == KEY_SRC_CTLMOTION_PTZ)
		{
			if(aircraftFlag == 0){
				key_state[index].key_status = KEY_RELEASE;
			}else{
				if(index == KEY_SRC_CTLMOTION_AIRCRAFT)
				{
					if(key_state[KEY_SRC_CTLMOTION_PTZ].key_status == KEY_RELEASED)
						key_state[KEY_SRC_CTLMOTION_PTZ].key_status = KEY_RELEASE;
					dataToSend.index_src = aircraft_selected;
					dataToSend.control_type= CONTROL_SRC_DRONE;
					dataToSend.control_event = EVENT_SRC_JOYSTICK;
					dataToSend.packet_enable = 1;
					dataToSend.repeat = 0xFE;
					dataToSend.event_index = 1;
					//adc_packet(0);
					MSG("send message,index=%d\n",index);
					
				}else if(index == KEY_SRC_CTLMOTION_PTZ){
					if(key_state[KEY_SRC_CTLMOTION_AIRCRAFT].key_status == KEY_RELEASED)
						key_state[KEY_SRC_CTLMOTION_AIRCRAFT].key_status = KEY_RELEASE;
					dataToSend.index_src = aircraft_selected;
					dataToSend.control_type= CONTROL_SRC_PTZ;
					dataToSend.control_event = EVENT_SRC_JOYSTICK;
					dataToSend.packet_enable = 1;
					dataToSend.repeat = 0xFE;
					dataToSend.event_index = 2;
					//adc_packet(1);
					MSG("send message,index=%d\n",index);
				}
			}
		}else if(index == KEY_SRC_CTLMOTION_PTZRESET || index == KEY_SRC_CTLMOTION_ZOOMIN || index == KEY_SRC_CTLMOTION_ZOOMOUT || index == KEY_SRC_CTLMOTION_ZOOMRESET){
			if(key_state[KEY_SRC_CTLMOTION_PTZ].key_status == KEY_RELEASED)
			{
				//产生飞行器云台控制事件
				dataToSend.index_src = aircraft_selected;			
				dataToSend.control_type= CONTROL_SRC_PTZ;
				if(index == KEY_SRC_CTLMOTION_PTZRESET){
					dataToSend.control_event = EVENT_SRC_PTZRESET;
				}else if(index == KEY_SRC_CTLMOTION_ZOOMIN){
					dataToSend.control_event = EVENT_SRC_ZOOMIN;
				}else if(index == KEY_SRC_CTLMOTION_ZOOMOUT){
					dataToSend.control_event = EVENT_SRC_ZOOMOUT;
				}else if(index == KEY_SRC_CTLMOTION_ZOOMRESET){
					dataToSend.control_event = EVENT_SRC_ZOOMRESET;
				}
				dataToSend.packet_enable = 1;
				dataToSend.repeat = 1;
				dataToSend.event_index = 255;
				//adc_packet(255);
				MSG("send message,index=%d\n",index);
			}
			key_state[index].key_status = KEY_RELEASE;
		}else if(index == KEY_SRC_CTLMOTION_CRASHSTOP || index ==KEY_SRC_CTLMOTION_RETURNHOME){
			if(key_state[KEY_SRC_CTLMOTION_AIRCRAFT].key_status == KEY_RELEASED){
				//产生返航或者急停事件
				dataToSend.index_src = aircraft_selected;
				dataToSend.control_type= CONTROL_SRC_DRONE;
				if(index == KEY_SRC_CTLMOTION_CRASHSTOP){
					dataToSend.control_event = EVENT_SRC_CRASHSTOP;
				}else{
					dataToSend.control_event = EVENT_SRC_RTNHOME;
				}
				dataToSend.packet_enable = 1;
				dataToSend.repeat = 1;
				adc_packet(255);
				MSG("send message,index=%d\n",index);				
			}
			key_state[index].key_status = KEY_RELEASE;
		}
	}
	else if(index < KEY_SRC_BACKUP1 && index >=KEY_SRC_ALARM_LAMP){//key32 -- key 35 关联处理
		if(aircraft_selected == 0){
			key_state[index].key_status = KEY_RELEASE;
		}else{
			//更新aircraft_state数组
			switch(index)
			{
				case KEY_SRC_ALARM_LAMP:
					if(aircraft_state[aircraft_selected-1].alarm_lamp == 1){
						aircraft_state[aircraft_selected-1].alarm_lamp = 0;
						key_state[index].key_status = KEY_RELEASE;
					}else{
						aircraft_state[aircraft_selected-1].alarm_lamp = 1;
					}
					break;
				case KEY_SRC_ALARM_WHISTLE1:
					if(aircraft_state[aircraft_selected-1].alarm_whistle1 == 1){
						aircraft_state[aircraft_selected-1].alarm_whistle1 = 0;
						key_state[index].key_status = KEY_RELEASE;
					}else{
						aircraft_state[aircraft_selected-1].alarm_whistle1 = 1;
					}
					break;
				case KEY_SRC_ALARM_WHISTLE2:
					if(aircraft_state[aircraft_selected-1].alarm_whistle2 == 1){
						aircraft_state[aircraft_selected-1].alarm_whistle2 = 0;
						key_state[index].key_status = KEY_RELEASE;
					}else{
						aircraft_state[aircraft_selected-1].alarm_whistle2 = 1;
					}
					break;
				case KEY_SRC_SHOUTING:
					if(aircraft_state[aircraft_selected-1].shouting == 1)
					{
						aircraft_state[aircraft_selected-1].shouting = 0;
						key_state[index].key_status = KEY_RELEASE;
					}else{
						aircraft_state[aircraft_selected-1].shouting = 1;
					}
					break;
			}
			//key_state[index].key_set++;
		}
		
	}
	else if(index >= KEY_SRC_BACKUP1){
		key_state[index].key_status = KEY_RELEASE;
	}
	//dataToSend.packet_enable = aircraftFlag;
}


u8 read_shift_regs(u8 *Index)
{
	u8 num;
	u16 value[3]={0};
	static u16 oldvalue[3];
	u8 ret=0;
	u8 bit[3];
	u16 changed=0;
	u8 chip_index=0;
	//u8 index=0;
	u16 isPressed = 0;
	u8 chip_count,key_count;
	
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
				*Index = DATA_WIDTH_165*(chip_index-1)+key_count;
				if(isPressed)
				{
					key_state[*Index].key_status = KEY_PRESSED;			
					ret = 1;
				}else{
					key_state[*Index].key_status = KEY_RELEASED;
					//key_state[*Index].key_set++;
					ret=4;
				}
			}
		}
		 
	//	ret = 1;
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
					ret=2;
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






























