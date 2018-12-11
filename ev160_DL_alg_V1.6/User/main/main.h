

#include "stm32f10x.h"

#ifndef MAIN_H
#define MAIN_H


typedef struct{
//	uint8_t ucCarDoor;		//车门状态
	struct{
		uint8_t LFDOOR	 :1;	//低位
		uint8_t LBDOOR	 :1;
		uint8_t RBDOOR	 :1;
		uint8_t RFDOOR	 :1;		
		uint8_t DOOR		 :4;	//高位		    
	}DOORBITS;
	uint8_t ucYear;				//年
	uint8_t ucMonth;			//月
	uint8_t ucDay;				//天
	uint8_t ucHour;				//时
	uint8_t ucMinute;			//分
	uint8_t ucSecond;			//秒
}CarDoorStateInfo_TYPE;

//RightFront RF 右前
//RightBack  RB 右后
//LeftFront  LF 左前
//LeftBack   LB 左后
typedef struct{
	uint8_t ucRFDoorThresholdHi;				//右前车门高字节
	uint8_t ucRFDoorThresholdLo;		//右前车门低字节
	
	uint8_t ucRBDoorThresholdHi;				//右后车门高字节
	uint8_t ucRBDoorThresholdLo;				//右后车门低字节
	
	uint8_t ucLFDoorThresholdHi;				//左前车门高字节
	uint8_t ucLFDoorThresholdLo;				//左前车门低字节
	
	uint8_t ucLBDoorThresholdHi;				//左后车门高字节
	uint8_t ucLBDoorThresholdLo;				//左后车门低字节
	
	uint8_t ucLFBumperThresholdHi;			//左前保险杠高字节
	uint8_t ucLFBumperThresholdLo;			//左前保险杠低字节
	
	uint8_t ucRFBumperThresholdHi;			//右前保险杠高字节
	uint8_t ucRFBumperThresholdLo;			//右前保险杠低字节
	
	uint8_t ucLBBumperThresholdHi;			//左后保险杠高字节
	uint8_t ucLBBumperThresholdLo;			//左后保险杠低字节
	
	uint8_t ucRBBumperThresholdHi;			//右后保险杠高字节
	uint8_t ucRBBumperThresholdLo;			//右后保险杠低字节
	
	uint8_t ucEngineThreshold;					//引擎盖
	
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
	
	uint8_t ucTrunk; //后备箱
	uint8_t ucACC;
	uint8_t ucSpeed;
	
}CarStateInfo_TYPE;



extern uint8_t g_ucCarDoorFlag;								//车门状态标识 1表示开 0表示关
extern uint8_t g_ucADCTriggerFlag;						//ADC阈值触发标识
extern uint8_t g_ucAlgorithmFlag;							//算法处理标识
extern uint8_t g_ucMasterSendFlag;						//发送标识  0不具备发送条件 1具备发送条件
extern uint8_t g_ucCarLockFlag;								//车门锁状态标识 0表示无动作 1表示有动作
extern uint8_t g_ucCarWinFlag;								//车窗状态标识 0表示无动作 1表示有动作
extern uint8_t g_ucCarTrunkFlag;							//后备箱状态标识 0表示无动作 1表示有动作
extern uint8_t g_ucCarACCFlag;								//ACC上电状态 0表示ACC断电 1表示ACC上电
extern uint8_t g_ucThresholdFlag;							//阈值设置标识符 1表示阈值有更新 0表示阈值没有更新
extern uint8_t g_ucGetCarStateresponse; 			//从机是否有响应getCarStater指令
extern uint8_t ucSetThreshold[18]; 

extern CarThresholdInfo_TYPE  g_tCarThreshold;
extern CarDoorStateInfo_TYPE  g_tCarDoorState;
extern CarStateInfo_TYPE g_tCarState;

#endif

