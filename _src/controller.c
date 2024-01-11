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

    while(read(_table->ps3_serial, (&EVENT), sizeof(EVENT)) < 7);
    switch (EVENT.type & ~JS_EVENT_INIT)
    {
        case JS_EVENT_BUTTON:
        {
            if(EVENT.value >= 1){
                _cntPkt->button = EVENT.number;
                if(_cntPkt->button == RIGHT_T || _cntPkt->button == LEFT_T)
                    _cntPkt->trigger_value[(_cntPkt->button - 8) ? RIGHT : LEFT] = EVENT.value;
            }
        }
        case JS_EVENT_AXIS:
        {
            if(EVENT.number == 1 || EVENT.number == 0)
            {
                axis[LEFT][EVENT.number ? X_AXIS : Y_AXIS] = EVENT.value / 4.0;

                if(flag_axis[LEFT] == true)
                {
                    _cntPkt->stick_value[LEFT] = (sqrt(axis[LEFT][X_AXIS] * axis[LEFT][X_AXIS] + axis[LEFT][Y_AXIS] * axis[LEFT][Y_AXIS]))*2 - 100;

                    if(_cntPkt->stick_value[LEFT] > 16200)
                        _cntPkt->stick_value[LEFT] = 16200;

                    double ang = 180 * (atan2(axis[LEFT][Y_AXIS], -axis[LEFT][X_AXIS]) / M_PI);
                    
                    if(ang < 0)
                        ang += 360;
                    
                    _cntPkt->stick_angle[LEFT] = ang;

                    memset(axis, 0, 2);
                    flag_axis[LEFT] = false;
                }
                else
                    flag_axis[LEFT] = true;
            }

            if(EVENT.number == 2 || EVENT.number == 3)
            {
                axis[RIGHT][(EVENT.number - 2) ? X_AXIS : Y_AXIS ] = EVENT.value / 4.0;

                if(flag_axis[RIGHT] == true)
                {
                    _cntPkt->stick_value[RIGHT] = (sqrt(axis[RIGHT][X_AXIS] * axis[RIGHT][X_AXIS] + axis[RIGHT][Y_AXIS] * axis[RIGHT][Y_AXIS]))*2 - 100;

                    if(_cntPkt->stick_value[RIGHT] > 16200)
                        _cntPkt->stick_value[RIGHT] = 16200;

                    double ang = 180 * (atan2(axis[RIGHT][Y_AXIS], -axis[RIGHT][X_AXIS]) / M_PI);
                    
                    if(ang < 0)
                        ang += 360;
                    
                    _cntPkt->stick_angle[RIGHT] = ang;

                    // printf("v: %d a: %f\n", _cntPkt->stick_value[RIGHT], ang);

                    memset(axis, 0, 2);
                    flag_axis[RIGHT] = false;
                }
                else
                    flag_axis[RIGHT] = true;
            }
        }
        default:
            break;
    }

    return count;
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