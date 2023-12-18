#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef uint8_t (*CheckCB)(uint8_t*, uint8_t);

#define MOVE 0xA5
#define STOP 0x00
#define HELLO 0xFF
#define ALERT 0xBB
#define FINE 0xCC

#define MS_TERMINAL 0x5A

/**
 * @brief 
 * 
 * @param message_id 
 * @param button 
 * @param power 
 * @param movedir 
 * @param senddata 
 */
void MakeSendData(uint8_t message_id, uint8_t button, uint16_t power, uint16_t movedir, uint8_t hijou, uint8_t* senddata);



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