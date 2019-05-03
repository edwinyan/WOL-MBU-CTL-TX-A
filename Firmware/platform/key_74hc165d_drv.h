#ifndef _KEY_74HC165D_DRV_H_
#define _KEY_74HC165D_DRV_H_


#define KEY1				0X0001
#define KEY2				0X0002
#define KEY3				0X0004
#define KEY4				0X0008
#define KEY5				0X0010
#define KEY6				0X0020
#define KEY7				0X0040
#define KEY8				0X0080
#define KEY9				0X0100
#define KEY10				0X0200
#define KEY11				0X0400
#define KEY12				0X0800
#define KEY13				0X1000
#define KEY14				0X2000
#define KEY15				0X4000
#define KEY16				0X8000

#define KEY_RELEASE			0x00
#define KEY_PRESSED			0x01
#define KEY_HOLDING			0x02
#define KEY_RELEASED		0x04

#define NUMBER_OF_SHIFT_CHIPS	2
#define NUMBER_OF_CHIPS			3
#define DATA_WIDTH_165			NUMBER_OF_SHIFT_CHIPS*8
#define NUMBER_OF_KEY			48

typedef enum{
	//display video to center screen
	KEY_SRC_DISPLAY_AIRCRAFT1 = 0,
	KEY_SRC_DISPLAY_AIRCRAFT2,
	KEY_SRC_DISPLAY_AIRCRAFT3,
	KEY_SRC_DISPLAY_AIRCRAFT4,
	KEY_SRC_DISPLAY_RECORDER1,
	KEY_SRC_DISPLAY_RECORDER2,
	KEY_SRC_DISPLAY_CONTROLBALL1,
	KEY_SRC_DISPLAY_CONTROLBALL2,
	//video streaming to control center
	KEY_SRC_STREAMING_AIRCRAFT1,
	KEY_SRC_STREAMING_AIRCRAFT2,
	KEY_SRC_STREAMING_AIRCRAFT3,
	KEY_SRC_STREAMING_AIRCRAFT4,
	KEY_SRC_STREAMING_RECORDER1,
	KEY_SRC_STREAMING_RECORDER2,
	KEY_SRC_STREAMING_CONTROLBALL1,
	KEY_SRC_STREAMING_CONTROLBALL2,
	//control option select
	KEY_SRC_CTLOPT_AIRCRAFT1,
	KEY_SRC_CTLOPT_AIRCRAFT2,
	KEY_SRC_CTLOPT_AIRCRAFT3,
	KEY_SRC_CTLOPT_AIRCRAFT4,
	KEY_SRC_CTLOPT_RECORDER1,
	KEY_SRC_CTLOPT_RECORDER2,
	KEY_SRC_CTLOPT_CONTROLBALL1,
	KEY_SRC_CTLOPT_CONTROLBALL2,
	//control motion select
	KEY_SRC_CTLMOTION_AIRCRAFT,
	KEY_SRC_CTLMOTION_PTZ,
	KEY_SRC_CTLMOTION_PTZRESET,
	KEY_SRC_CTLMOTION_ZOOMIN,
	KEY_SRC_CTLMOTION_ZOOMOUT,
	KEY_SRC_CTLMOTION_ZOOMRESET,
	KEY_SRC_CTLMOTION_CRASHSTOP,
	KEY_SRC_CTLMOTION_RETURNHOME,
	//voice and light control
	KEY_SRC_ALARM_LAMP,
	KEY_SRC_ALARM_WHISTLE1,
	KEY_SRC_ALARM_WHISTLE2,
	KEY_SRC_SHOUTING,
	KEY_SRC_BACKUP1,
	KEY_SRC_BACKUP2,
	KEY_SRC_BACKUP3,
	KEY_SRC_BACKUP4,
	//backup
	KEY_SRC_BACKUP5,
	KEY_SRC_BACKUP6,
	KEY_SRC_BACKUP7,
	KEY_SRC_BACKUP8,
	KEY_SRC_BACKUP9,
	KEY_SRC_BACKUP10,
	KEY_SRC_BACKUP11,
	KEY_SRC_BACKUP12,
	
	KEY_SRC_NUM
}key_src_enum;

typedef struct{
	u8 chip_index;	//0 -- 2 chip index
	u8 key_index;	//0 -- 15 key index 	
	u8 key_status;	//0:released 1:hold 2:pressed
	u16 key_set;		//count of key has been pressed
}key_state_t;

typedef struct{
	u8 aircraft_selected;
	u8 alarm_lamp;
	u8 alarm_whistle1;
	u8 alarm_whistle2;
	u8 shouting;
}aircraft_state_t;

typedef struct{
	u8 ptz_reset;
	u8 zoom_in;
	u8 zoom_out;
	u8 zoom_reset;
}controlball_state_t;


void read_shift_init(void);
u8 read_shift_regs(u8 *index);
void key_state_remap_handle(u8 index);
void handle_display_exchange(u8 count);

#endif
