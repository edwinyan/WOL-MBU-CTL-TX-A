#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


//protocol for send message to gcs by UART
#define HEAD1		0xAA
#define HEAD2		0xAA
#define TAIL1		0x55
#define TAIL2		0x55

#define ROCKER_MIDDLE	0
#define ROCKER_UP		1
#define ROCKER_DOWN		2

//���������
typedef enum{
	IDNEX_SRC_DRONE1=1,
	INDEX_SRC_DRONE2,
	INDEX_SRC_DRONE3,
	INDEX_SRC_DRONE4,
	INDEX_SRC_PWRKEY1,
	INDEX_SRC_PWRKEY2,
	INDEX_SRC_PWRKEY3,
	INDEX_SRC_PWRKEY4
}index_src_enum;

//��������
typedef enum{
	CONTROL_SRC_CLOSE=0,
	CONTROL_SRC_OPEN,
	CONTROL_SRC_DRONE,
	CONTROL_SRC_PTZ,
	CONTROL_SRC_LAMP,
	CONTROL_SRC_WHISTLE1,
	CONTROL_SRC_WHISTLE2,
	CONTROL_SRC_SHOUTING,

	CONTROL_SRC_NUM
}control_src_enum;

//�����¼�
typedef enum{
	EVENT_SRC_CLOSE=0,
	EVENT_SRC_OPEN,
	EVENT_SRC_JOYSTICK,
	EVENT_SRC_CRASHSTOP,
	EVENT_SRC_RTNHOME,
	EVENT_SRC_PTZRESET,
	EVENT_SRC_ZOOMIN,
	EVENT_SRC_ZOOMOUT,
	EVENT_SRC_ZOOMRESET,

	EVENT_SRC_NUM
}event_src_enum;


typedef struct{
	u8 packet_enable;	//�����¼�����ʹ��
	u8 power_enable;	//��Դ�¼�����ʹ��
	u8 power_trig;	
	u8 event_index;
	u8 repeat;
	u8 index_src;		//���Ʊ��
	u8 control_type;	//��������
	u8 control_event;	//�����¼�
	u8 adc_value[8];	//ҡ������
}packet_t;

void data_packet(void);
void adc_packet(u8 index);

#endif
