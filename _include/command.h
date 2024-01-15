#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef uint8_t (*CheckCB)(uint8_t*, uint8_t);

#define MOVE 0xA5
#define STOP 0x00
#define HELLO 0xFF
#define ALERT 0xEE
#define FINE 0xEF
#define TURN 0x55

#define MARU_B 13
#define SANKAKU_B 12
#define BATU_B 14
#define SIKAKU_B 15
#define UP_B 4
#define LEFT_B 7
#define DOWN_B 6
#define RIGHT_B 5
#define LEFT_1 10
#define LEFT_T 8
#define RIGHT_1 11
#define RIGHT_T 9
#define LEFT_S_PUSH 1
#define RIGHT_S_PUSH 2
#define START 3
#define SELECT 0
#define PS 16

#define MS_TERMINAL 0x5A

/**
 * @brief 
 * 
 * @param button 
 * @param power 
 * @param movedir 
 * @param senddata You should prepere larger number of senddata than the data that you want to send
 * @param dataSize is that the data you wanna send is added 2
 */
void MakeDataCobs(uint8_t button, uint16_t power, uint16_t movedir, uint8_t* senddata, uint8_t dataSize);

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

//送信データ構造(USBtoCANと同じにします...)//
/*
 *                                              Packet
 * *************************|*************************|**************************|**************************|
 *                          |                         |                          |                          |
 *      MassageID:8bit      |    PayloadSize:8bit     | Payload:8bit*PayloadSize |      ChecKSum:8bit       |
 *                          |         (Max12)         |                          | (Only when have Payload) |
 * *************************|*************************|**************************|**************************|
 * 
 */

#endif