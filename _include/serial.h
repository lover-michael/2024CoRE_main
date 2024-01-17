#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* SERIAL_HANDLE;

/*
linuxでシリアル通信を行う手順

1.シリアルポートに対応するデバイスファイルをオープンする
↓
2.termios構造体を使って通信設定する
↓
3.termios構造体の設定値をポートに反映させる
↓
4.read/write関数を使って通信する
*/


/**
 * @brief 
 * 
 * @param [in] dev device path
 * @param [in] baudrate when you put into the baudrate, add a 'B' to the head of baudrate's number
 * @return SERIAL_HANDLE 
 */
SERIAL_HANDLE SerialOpen(const char* const dev, uint64_t baudrate);

/**
 * @brief 
 * 
 * @param [in] handle what is used for Serial connection  
 */
void SerialClose(SERIAL_HANDLE handle);

/**
 * @brief 
 * 
 * @param [in] handle what is used for Serial connection
 * @param [out] buff send buff
 * @param [in] size buff size (unit : byte)
 * @return ssize_t 
 */
ssize_t SerialWrite(SERIAL_HANDLE handle, unsigned char* buff, uint8_t size);

/**
 * @brief 
 * 
 * @param [in] handle what is used for Serial connection
 * @param [in] buff recieve buff
 * @param [in] size buff size
 * @return ssize_t 
 */
ssize_t SerialRead(SERIAL_HANDLE handle, unsigned char* buff, uint8_t size);