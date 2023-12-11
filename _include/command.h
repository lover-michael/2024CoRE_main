#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef uint8_t (*CheckCB)(uint8_t*, uint8_t);

#define MOVE 0xAA
#define STOP 0x00
#define HELLO 0xFF

#define MS_TERMINAL 0x5A

/**
 * @brief 
 * 
 * @param [in] message_id 
 * @param [in] power 
 * @param [in] movedir 
 * @param [out] senddata 
 */
void MakeSendMotarData(uint8_t message_id, uint16_t power, uint16_t movedir, uint8_t* senddata, CheckCB checkSum);

/**
 * @brief 
 * 
 * @param senddata 
 * @param sizeData 
 * @return uint8_t 
 */
uint8_t CheckSum(uint8_t* senddata, uint8_t sizeData);

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