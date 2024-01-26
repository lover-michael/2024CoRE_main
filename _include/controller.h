#ifndef CONTROLLER_H
#define CONTROLLER_H
#define PS5                     //コントローラーの製品をここで指定してください

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <linux/joystick.h>
#include <math.h>

#ifdef PS5
    #include "dualsense.h"
#elif PS3
    #include "dualshock3.h"
#endif

#define M_PI 3.14159265358979323846

typedef void* PS3_HANDLE;
typedef struct js_event js_event;

typedef struct{
    uint8_t button;
    uint16_t trigger_value[2];
    uint16_t stick_value[2];
    uint16_t stick_angle[2];
}controllerPac;

/**
 * @brief 
 * 
 * @param [in] dev Only use /dev/input/js*(if use one controller, * set to 0)
 * @return PS3_HANDLE 
 */
PS3_HANDLE ConttrollerOpen(const char const* dev);

/**
 * @brief 
 * 
 * @param [in] handle Please use what is made by ControllerOpen();
 * @return ssize_t 
 */
ssize_t ControllerRead(PS3_HANDLE handle, controllerPac* _cntPkt);

/**
 * @brief 
 * 
 * @param [in] handle 
 */
void ControllerClose(PS3_HANDLE handle);

/**
 * @brief Get the Button Num object
 * 
 * @param [in] handle 
 * @param [in] buttonnum 
 */
void GetButtonNum(PS3_HANDLE handle, uint8_t* buttonnum);

/**
 * @brief Get the Axis Num object
 * 
 * @param [in] handle 
 * @param [in] axisnum 
 */
void GetAxisNum(PS3_HANDLE handle, uint8_t* axisnum);

#endif