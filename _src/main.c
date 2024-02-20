#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "serial.h"
#include "controller.h"
#include "command.h"

int main()
{
    const char _dev_cnt[] = "/dev/input/js0";
    const char _dev_bord[] = "/dev/ttyUSB0";
    
    uint8_t senddata[100];
    uint8_t hijou = 0;
    uint8_t numM = 0;
    ssize_t count = 0;

    controllerPac cPac;
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    // while(1)
    // {
    //     ControllerRead(_handle_con, &cPac);
    //     printf("button -> %d  ", cPac.button);
    //     printf("Lstick -> %d  ", cPac.stick_value[1]);
    //     printf("Rstick -> %d  ", cPac.stick_value[0]);
    //     printf("LTvakue -> %d  ", cPac.trigger_value[1]);
    //     printf("RTvakue -> %d\n", cPac.trigger_value[0]);
    // }

    //コントローラーの接続を確認する
    do{
        ControllerRead(_handle_con, &cPac);
        if(cPac.button == PS)
        {
            numM = MakeDataCobs(cPac.button, 0, 0, senddata, 1);
            count = SerialWrite(_handle, senddata, 3);
            printf("%ld\n", count);
            for(int i = 0;i < 10000;i++)
            {
                for(int j = 0;j < 1000;)
                {
                    j++;
                }
            }
        }
    }while(cPac.button != PS);

    memset(&cPac, 0, sizeof(cPac));

    while(1)
    {
        count = ControllerRead(_handle_con, &cPac);

        if(count < 0)
        {
            numM = MakeDataCobs(START, cPac.stick_value[0], cPac.stick_angle[0], senddata, 12);
            count = SerialWrite(_handle, senddata, 12);
            if(count < 0)
                break;
            hijou++;
            if(hijou > 30)
                break;
        }
        //非常停止等用
        numM = MakeDataCobs(cPac.button, 0, 0, senddata, 1);
        if(numM == 0)
        {
            count = SerialWrite(_handle, senddata, 3);
            printf("%x %x %x\n",senddata[0], senddata[1], senddata[2]);
            if(count < 0)
                hijou++;            
        }

        // 足回り用の通信
        senddata[7] = 0xAA;
        numM = MakeDataCobs(cPac.button, cPac.stick_value[1], cPac.stick_angle[1], senddata, 10);
        if(numM == 1)
        {
            printf("%d %x %d %d %d %d %d %d %d %d %d %x\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5], senddata[6], senddata[7], senddata[8], senddata[9], senddata[10], senddata[11]);
            count = SerialWrite(_handle, senddata, 12);
            if(count < 0)
                hijou++;
        }
        // memset(senddata, 0, 12);

        //放蕩浅海用の通信
        // senddata[7] = 0xAB;
        // numM = MakeDataCobs(cPac.button, cPac.stick_value[0], cPac.stick_angle[0], senddata, 8);
        // if(numM == 1)
        // {
        //     count = SerialWrite(_handle, senddata, 10);
        //     printf("%x %x %d %d %x %x %x %x\n",senddata[0], senddata[1], senddata[2] + senddata[3], senddata[4] + senddata[5], senddata[6], senddata[7], senddata[8], senddata[9]);
            
        // }
    }

    ControllerClose(_handle_con);
    SerialClose(_handle);

    return 0;
}
