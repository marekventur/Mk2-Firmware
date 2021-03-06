/*
 TiLDA Mk2

 FlashLightTask
 Torch Mode - Press the light button and both RGB LEDs light up.

 The MIT License (MIT)

 Copyright (c) 2014 Electromagnetic Field LTD

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef _FLASH_LIGHT_TASK_H_
#define _FLASH_LIGHT_TASK_H_

#include <Arduino.h>
#include <FreeRTOS_ARM.h>
#include "EMF2014Config.h"
#include "Task.h"
#include "RGBTask.h"
#include "ButtonTask.h"

class FlashLightTask: public Task {
public:
	FlashLightTask(RGBTask rgbTask): _rgbTask(rgbTask) {};
	String getName();
protected:
    void task();
private:
	RGBTask _rgbTask;
};

#endif // _FLASH_LIGHT_TASK_H_