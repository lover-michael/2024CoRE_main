#include "controller.h"

typedef struct{
    int ps3_serial;
}PS3_controllertable;

#define X_AXIS 0
#define Y_AXIS 1 

#define LEFT 1
#define RIGHT 0

PS3_HANDLE ConttrollerOpen(const char const* dev)
{
    PS3_controllertable* _table = (PS3_controllertable*)malloc(sizeof(PS3_controllertable));
    memset(_table, 0, sizeof(PS3_controllertable));

    _table->ps3_serial = open(dev, O_RDONLY);

    if(_table->ps3_serial < 0)
    {
        perror("Could not open joystick");
        _table = NULL;
    }

    return _table; 
}

ssize_t ControllerRead(PS3_HANDLE handle, controllerPac* _cntPkt)
{
    js_event EVENT;
    PS3_controllertable* _table = handle;
    uint8_t count = 0;
    static double axis[2][2] = { {0, 0}, {0, 0} };
    static bool flag_axis[2] = {0, 0};

    read(_table->ps3_serial, (&EVENT), sizeof(EVENT));
    switch (EVENT.type & ~JS_EVENT_INIT)
    {
        case JS_EVENT_BUTTON:
        {
            if((EVENT.value ? "pressed" : "released") == "pressed")
                _cntPkt->button = EVENT.number;
            else
                _cntPkt->button = EVENT.number + 30;
        }
        case JS_EVENT_AXIS:
        {
            if(EVENT.number == 1 || EVENT.number == 0)
            {
                axis[LEFT][EVENT.number ? X_AXIS : Y_AXIS] = EVENT.value;

                if(flag_axis[LEFT] == true)
                {
                    _cntPkt->stick_value[LEFT] = (sqrt(axis[LEFT][X_AXIS] * axis[LEFT][X_AXIS] + axis[LEFT][Y_AXIS] * axis[LEFT][Y_AXIS]));
                    
                    if(_cntPkt->stick_value[LEFT]  <= 2500)
                        _cntPkt->stick_value[LEFT] = 0;  

                    double ang = 180 * (atan2(axis[LEFT][Y_AXIS], -axis[LEFT][X_AXIS]) / M_PI);
                    
                    if(ang < 0)
                        ang += 360;
                    
                    _cntPkt->stick_angle[LEFT] = ang;

                    memset(axis, 0, 4);
                    flag_axis[LEFT] = false;
                }
                else
                    flag_axis[LEFT] = true;
            }

            if(EVENT.number == 3 || EVENT.number == 4)
            {
                axis[RIGHT][(EVENT.number - 3) ? X_AXIS : Y_AXIS ] = EVENT.value;

                if(flag_axis[RIGHT] == true)
                {
                    _cntPkt->stick_value[RIGHT] = (sqrt(axis[RIGHT][X_AXIS] * axis[RIGHT][X_AXIS] + axis[RIGHT][Y_AXIS] * axis[RIGHT][Y_AXIS]));

                    if(_cntPkt->stick_value[RIGHT]  <= 2500)
                        _cntPkt->stick_value[RIGHT] = 0;  

                    double ang = 180 * (atan2(axis[RIGHT][Y_AXIS], -axis[RIGHT][X_AXIS]) / M_PI);
                    
                    if(ang < 0)
                        ang += 360;
                    
                    _cntPkt->stick_angle[RIGHT] = ang;
                    
                    memset(axis, 0, 4);
                    flag_axis[RIGHT] = false;
                }
                else
                    flag_axis[RIGHT] = true;
            }

            if(EVENT.number == 2 || EVENT.number > 4)
            {
                switch (EVENT.number)
                {
                    case 2:
                    {
                        if(EVENT.value > -30000)
                            _cntPkt->trigger_value[LEFT] = EVENT.value + 32767;
                        else
                            _cntPkt->trigger_value[LEFT] = 0;
                    }break;
                    case 5:
                    {
                        if(EVENT.value > -30000)
                            _cntPkt->trigger_value[RIGHT] = EVENT.value + 32767;
                        else
                            _cntPkt->trigger_value[RIGHT] = 0;
                    }break;
                    case 6:
                    {
                        if(EVENT.value != 0 && _cntPkt->button > 30)
                            _cntPkt->button = ((EVENT.value < 0) ? LEFT_B : RIGHT_B);
                        else
                            _cntPkt->button = 31;
                    }break;
                    case 7:
                    {
                        if(EVENT.value != 0 && _cntPkt->button > 30)
                            _cntPkt->button = ((EVENT.value < 0) ? UP_B : DOWN_B);
                        else
                            _cntPkt->button = 31;
                    }break;
                    default:
                        break;
                }
            }
        }
        default:
            break;
    }
}

void ControllerClose(PS3_HANDLE handle)
{
    PS3_controllertable* _table = handle;

    close(_table->ps3_serial);
    free(_table);
}

void GetButtonNum(PS3_HANDLE handle, uint8_t* buttonnum)
{
    PS3_controllertable* _table = handle;

    if(ioctl(_table->ps3_serial, JSIOCGBUTTONS, buttonnum) == -1)
    {
        perror("Failed!");
        return;
    }
}

void GetAxisNum(PS3_HANDLE handle, uint8_t* axisnum)
{
    PS3_controllertable* _table = handle;

    if(ioctl(_table->ps3_serial, JSIOCGAXES, axisnum) == -1)
    {
        perror("Failed!");
        return;
    }
}