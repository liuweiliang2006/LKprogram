

#include "stm32f10x.h"

#ifndef MAIN_H
#define MAIN_H


typedef struct{
//	uint8_t ucCarDoor;		//����״̬
	struct{
		uint8_t LFDOOR	 :1;	//��λ
		uint8_t LBDOOR	 :1;
		uint8_t RBDOOR	 :1;
		uint8_t RFDOOR	 :1;		
		uint8_t DOOR		 :4;	//��λ		    
	}DOORBITS;
	uint8_t ucYear;				//��
	uint8_t ucMonth;			//��
	uint8_t ucDay;				//��
	uint8_t ucHour;				//ʱ
	uint8_t ucMinute;			//��
	uint8_t ucSecond;			//��
}CarDoorStateInfo_TYPE;

//RightFront RF ��ǰ
//RightBack  RB �Һ�
//LeftFront  LF ��ǰ
//LeftBack   LB ���
typedef struct{
	uint8_t ucRFDoorThresholdHi;				//��ǰ���Ÿ��ֽ�
	uint8_t ucRFDoorThresholdLo;		//��ǰ���ŵ��ֽ�
	
	uint8_t ucRBDoorThresholdHi;				//�Һ��Ÿ��ֽ�
	uint8_t ucRBDoorThresholdLo;				//�Һ��ŵ��ֽ�
	
	uint8_t ucLFDoorThresholdHi;				//��ǰ���Ÿ��ֽ�
	uint8_t ucLFDoorThresholdLo;				//��ǰ���ŵ��ֽ�
	
	uint8_t ucLBDoorThresholdHi;				//����Ÿ��ֽ�
	uint8_t ucLBDoorThresholdLo;				//����ŵ��ֽ�
	
	uint8_t ucLFBumperThresholdHi;			//��ǰ���ոܸ��ֽ�
	uint8_t ucLFBumperThresholdLo;			//��ǰ���ոܵ��ֽ�
	
	uint8_t ucRFBumperThresholdHi;			//��ǰ���ոܸ��ֽ�
	uint8_t ucRFBumperThresholdLo;			//��ǰ���ոܵ��ֽ�
	
	uint8_t ucLBBumperThresholdHi;			//����ոܸ��ֽ�
	uint8_t ucLBBumperThresholdLo;			//����ոܵ��ֽ�
	
	uint8_t ucRBBumperThresholdHi;			//�Һ��ոܸ��ֽ�
	uint8_t ucRBBumperThresholdLo;			//�Һ��ոܵ��ֽ�
	
	uint8_t ucEngineThreshold;					//�����
	
}CarThresholdInfo_TYPE;


typedef struct{
	struct{
		uint8_t LFDOOR			:1;  
		uint8_t LBDOOR			:1;
		uint8_t RBDOOR			:1;
		uint8_t RFDOOR			:1;	
		uint8_t DOOR				:4;		
	}DOORBITS;
	
	struct{
		uint8_t LFCARLOCK		:1;
		uint8_t LBCARLOCK		:1;
		uint8_t RBCARLOCK		:1;
		uint8_t RFCARLOCK		:1;		
		uint8_t CARLOCK  		:4;		
	}CARLOCKBITS;
	
	struct{
		uint8_t LFCARWIN		:1;
		uint8_t LBCARWIN		:1;
		uint8_t RBCARWIN		:1;
		uint8_t RFCARWIN		:1;
		uint8_t CARWIN  		:4;		
	}CARWINBITS;
	
	uint8_t ucTrunk; //����
	uint8_t ucACC;
	uint8_t ucSpeed;
	
}CarStateInfo_TYPE;



extern uint8_t g_ucCarDoorFlag;								//����״̬��ʶ 1��ʾ�� 0��ʾ��
extern uint8_t g_ucADCTriggerFlag;						//ADC��ֵ������ʶ
extern uint8_t g_ucAlgorithmFlag;							//�㷨�����ʶ
extern uint8_t g_ucMasterSendFlag;						//���ͱ�ʶ  0���߱��������� 1�߱���������
extern uint8_t g_ucCarLockFlag;								//������״̬��ʶ 0��ʾ�޶��� 1��ʾ�ж���
extern uint8_t g_ucCarWinFlag;								//����״̬��ʶ 0��ʾ�޶��� 1��ʾ�ж���
extern uint8_t g_ucCarTrunkFlag;							//����״̬��ʶ 0��ʾ�޶��� 1��ʾ�ж���
extern uint8_t g_ucCarACCFlag;								//ACC�ϵ�״̬ 0��ʾACC�ϵ� 1��ʾACC�ϵ�
extern uint8_t g_ucThresholdFlag;							//��ֵ���ñ�ʶ�� 1��ʾ��ֵ�и��� 0��ʾ��ֵû�и���
extern uint8_t g_ucGetCarStateresponse; 			//�ӻ��Ƿ�����ӦgetCarStaterָ��
extern uint8_t ucSetThreshold[18]; 

extern CarThresholdInfo_TYPE  g_tCarThreshold;
extern CarDoorStateInfo_TYPE  g_tCarDoorState;
extern CarStateInfo_TYPE g_tCarState;

#endif

