#include <stdint.h>
#include <stdbool.h>

#ifndef COMMAND_H
#define COMMAND_H

void MakeSendMotarData(uint8_t address, uint8_t message_id, uint8_t power, bool roledir, uint8_t movedir, uint8_t* senddata);

#define MOVE 0b1000
#define STOP 0b0000
#define BRAKE 0b1111
#define HELLO 0b1010

//送信データ構造//
/*
|1byte        |2byte        |3byte       |4byte       |
|address + mid|power        |movedir     |roledir     |
|             |Motar's power|forw / back |
*/

#endif