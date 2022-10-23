/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "MPU9250.h"

#ifndef BUFFER_LENGTH
// band-aid fix for platforms without Wire-defined BUFFER_LENGTH (removed from some official implementations)
#define BUFFER_LENGTH 32
#endif

/** Specific address constructor.
 * @param address I2C address, uses default I2C address if none is specified
 * @see MPU9250_DEFAULT_ADDRESS
 * @see MPU9250_ADDRESS_AD0_LOW
 * @see MPU9250_ADDRESS_AD0_HIGH
 */
MPU9250::MPU9250(uint8_t address):devAddr(address) {
}

/** Power on and prepare for general usage.
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to their most sensitive settings, namely +/- 2g and +/- 250 degrees/sec, and sets
 * the clock source to use the X Gyro for reference, which is slightly better than
 * the default internal clock source.
 */
void MPU9250::initialize() {
    setClockSource( (MPU9250_IMU::MPU9250_CLOCK_PLL_XGYRO));
    setFullScaleGyroRange( (MPU9250_IMU::MPU9250_GYRO_FS_250));
    setFullScaleAccelRange( (MPU9250_IMU::MPU9250_ACCEL_FS_2));
    setSleepEnabled(false); // thanks to Jack Elston for pointing this one out!
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool MPU9250::testConnection() {
    uint8_t deviceId = getDeviceID();
    return (deviceId == 0x34) || (deviceId == 0xC);
}

// GYRO_CONFIG register

/** Get full-scale gyroscope range.
 * The FS_SEL parameter allows setting the full-scale range of the gyro sensors,
 * as described in the table below.
 *
 * <pre>
 * 0 = +/- 250 degrees/sec
 * 1 = +/- 500 degrees/sec
 * 2 = +/- 1000 degrees/sec
 * 3 = +/- 2000 degrees/sec
 * </pre>
 *
 * @return Current full-scale gyroscope range setting
 * @see MPU9250_GYRO_FS_250
 * @see MPU9250_RA_GYRO_CONFIG
 * @see MPU9250_GCONFIG_FS_SEL_BIT
 * @see (MPU9250_IMU::MPU9250_GCONFIG_FS_SEL_LENGTH)
 */
uint8_t MPU9250::getFullScaleGyroRange() {
    I2Cdev::readBits(devAddr,  (MPU9250_IMU::MPU9250_RA_GYRO_CONFIG),  (MPU9250_IMU::MPU9250_GCONFIG_FS_SEL_BIT), (MPU9250_IMU::MPU9250_GCONFIG_FS_SEL_LENGTH), buffer);
    return buffer[0];
}
/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU9250_GYRO_FS_250
 * @see MPU9250_RA_GYRO_CONFIG
 * @see MPU9250_GCONFIG_FS_SEL_BIT
 * @see (MPU9250_IMU::MPU9250_GCONFIG_FS_SEL_LENGTH)
 */
void MPU9250::setFullScaleGyroRange(uint8_t range) {
    I2Cdev::writeBits(devAddr,  (MPU9250_IMU::MPU9250_RA_GYRO_CONFIG),  (MPU9250_IMU::MPU9250_GCONFIG_FS_SEL_BIT), (MPU9250_IMU::MPU9250_GCONFIG_FS_SEL_LENGTH), range);
}