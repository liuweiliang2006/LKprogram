#include "stm32f10x.h"

#ifndef API_PROTOCOL_H
#define API_PROTOCOL_H

#define CMD_HEARTBEAT							0xF1
#define CMD_ALARM									0xF2
#define CMD_GETSET								0xF3
#define CMD_SENDDATA							0xF4
#define CMD_GETCARSTATE						0xF5
#define CMD_GETIAP								0xF6

#define RECO_CHAR1								0x5A
#define RECO_CHAR2								0xA5


#endif
