

#include "stm32f10x.h"

#ifndef API_DOOR_H
#define API_DOOR_H

typedef enum {DOOR_OPEN = 0, DOOR_CLOSE = 1} DOOR_STATE;

void door_state_init(void);
bool is_door_allclosed(void);
DOOR_STATE get_door_state(void);
void detect_door(void);
DOOR_STATE detect_door2(void);
void door_detect_test(void);

#endif








