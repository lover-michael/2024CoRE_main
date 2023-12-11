#include <stdio.h>

#include "serial.h"
#include "controller.h"
#include "command.h"

int main()
{
    const char _dev_cnt[] = "/dev/input/js0";
    const char _dev_bord[] = "/dev/tty0";
    uint8_t senddata[100];

    controllerPac cPac;
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    do{
        ControllerRead(_handle_con, &cPac);
        if(cPac.button == PS)
        {
            MakeSendMotarData(HELLO, 0, 0, senddata, 0);
            SerialWrite(_handle, senddata, 2);
        }
    }while(cPac.button != PS);

    while(1)
    {

    }

    ControllerClose(_handle_con);
    SerialClose(_handle);

    return 0;
}