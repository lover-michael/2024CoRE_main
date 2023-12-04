#include <stdio.h>

#include "serial.h"
#include "controller.h"
#include "command.h"

int main()
{
    char _dev_cnt[] = "/dev/input/js0";
    char _dev_bord[] = "/dev/tty0";
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    while(1)
    {

    }

    ControllerClose(_handle_con);
    SerialClose(_handle);

    return 0;
}