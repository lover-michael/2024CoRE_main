#include "command.h"

void MakeSendMotarData(uint8_t motnum, uint8_t message_id, uint8_t power, bool roledir, uint8_t movedir, uint8_t* senddata)
{
    *senddata = (motnum << 5) + (message_id << 1) + roledir;
    *(senddata + 1) = power;
    *(senddata + 2) = movedir;
}