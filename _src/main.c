#include <stdio.h>

#include "serial.h"
#include "controller.h"
#include "command.h"

int main()
{
    const char _dev_cnt[] = "/dev/input/js0";
    const char _dev_bord[] = "/dev/tty0";
    
    uint8_t senddata[100];
    ssize_t count = 0;

    controllerPac cPac;
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    do{
        ControllerRead(_handle_con, &cPac);
        if(cPac.button == PS)
        {
            MakeSendData(HELLO, 0, 0, 0, senddata, 0);
            SerialWrite(_handle, senddata, 2);
            break;
        }
    }while(cPac.button != PS);

    while(1)
    {
        while(count = (ControllerRead(_handle_con, &cPac)) > 0);

        if(count == -1)
        {
            perror("failed!!\nSTOP SYSTEM\n");
            break;
        }

        MakeSendData(MOVE, cPac.button, cPac.stick_value, cPac.stick_angle, senddata, 7);
        SerialWrite(_handle, senddata, 7);
    }

    ControllerClose(_handle_con);
    SerialClose(_handle);

    return 0;
}