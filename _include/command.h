#ifndef COMMAND_H
#define COMMAND_H
#define PS5

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#ifdef PS5
    #include "dualsense.h"
#elif PS3
    #include "dualshock3.h"
#endif

typedef uint8_t (*CheckCB)(uint8_t*, uint8_t);

#define MOVE 0xA5
#define STOP 0x00
#define HELLO 0xFF
#define ALERT 0xEE
#define FINE 0xEF
#define TURN 0x55
#define SHOT_S 0xAA
#define SHOT_L 0xAB

/**
 * @brief 
 * 
 * @param button 
 * @param power 
 * @param movedir 
 * @param senddata You should prepere larger number of senddata than the data that you want to send
 * @param dataSize is that the data you wanna send is added 2
 */
uint8_t MakeDataCobs(uint8_t button, uint16_t power, uint16_t movedir, uint8_t* senddata, uint8_t dataSize);

/**
 * @brief 
 * 
 * @param num 
 * @return uint8_t 
 */
uint8_t ReturnMessage(uint8_t num);

/**
 * @brief 
 * 
 * @param [out] senddata Must prepare the pointer which is larger byte than arduments
 * @param [in] arg_1 Everything after this argument will be stored in senddata
 * @param ... 
 */
void MakeByte(uint8_t* senddata, uint16_t arg_1, ...);

#endif