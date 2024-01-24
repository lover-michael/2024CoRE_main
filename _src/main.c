#include <stdio.h>

#include "serial.h"
#include "controller.h"
#include "command.h"

int main()
{
    const char _dev_cnt[] = "/dev/input/js0";
    const char _dev_bord[] = "/dev/ttyUSB0";
    
    uint8_t senddata[100];
    uint8_t MessID_H = 0;
    uint8_t hijou = 0;
    bool flagHijou = true;
    ssize_t count = 0;

    controllerPac cPac;
    PS3_HANDLE _handle_con = ConttrollerOpen(_dev_cnt);
    SERIAL_HANDLE _handle = SerialOpen(_dev_bord, B115200);

    // while (1)
    // {
    //     senddata[0] =2;
    //     senddata[1] =0xff;
    //     senddata[2] =0;
    //     count = SerialWrite(_handle, senddata, 3);
    //     printf("Send %x %x %x \n",senddata[0], senddata[1], senddata[2]);//, senddata[3], senddata[4], senddata[5]);
    //     sleep(1);
    // }
    

    //コントローラーの接続を確認する
    do{
        ControllerRead(_handle_con, &cPac);
        if(cPac.button == 10)
        {
            MakeDataCobs(0, 0, 0, senddata, 6);
            count = SerialWrite(_handle, senddata, 6);
            printf("%ld\n", count);
            for(int i = 0;i < 10000;i++)
            {
                for(int j = 0;j < 1000;)
                {
                    j++;
                }
            }
        }
    }while(cPac.button != 10);

    memset(&cPac, 0, sizeof(cPac));

    while(1)
    {
        count = ControllerRead(_handle_con, &cPac);

        if(count < 0)
        {
            MessID_H = ALERT;
            MakeDataCobs(cPac.button, cPac.stick_value[0], cPac.stick_angle[0], senddata, 6);
            count = SerialWrite(_handle, senddata, 6);
            printf("%ld\n", count);
            hijou++;
            if(hijou > 50)
                break;
        }
        else
        {    
            //遠隔非常停止等を指示する通信
            // senddata[0] =2;
            // senddata[1] =0xff;
            // senddata[2] =0;
            // MakeDataCobs(cPac.button, cPac.stick_value[1], cPac.stick_angle[1], senddata, 3);
            // count = SerialWrite(_handle, senddata, 3);
            // printf("%x %x %x %d %d %d\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5]);

            //足回り用の通信
            MakeDataCobs(MOVE, cPac.stick_value[1], cPac.stick_angle[1], senddata, 6);
            count = SerialWrite(_handle, senddata, 6);
            printf("%x %x %x %d %d %d\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5]);

            //放蕩浅海用の通信
            // MakeDataCobs(TURN, cPac.stick_value[0], cPac.stick_angle[0], senddata, 6);
            // count = SerialWrite(_handle, senddata, 6);
            // printf("%x %x %x %d %d %d\n",senddata[0], senddata[1], senddata[2], senddata[3], senddata[4], senddata[5]);
        }

            
        // memset(&cPac, 0, sizeof(cPac));
    }

    ControllerClose(_handle_con);
    SerialClose(_handle);

    return 0;
}
