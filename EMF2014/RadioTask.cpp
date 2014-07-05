/*
 TiLDA Mk2

 RadioTask

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

#include "RadioTask.h"
#include "DebugTask.h"

namespace radio {
    void task(void *pvParameters) {
        debug::log("Starting radio task");

        char buffer [16];
        char bufferPosition = 0;

        while(true) {

            while (Serial.available() > 0) {
                buffer[bufferPosition] = Serial.read();
                bufferPosition++;
                debug::log("char: " + String(buffer[bufferPosition]));
                if (bufferPosition == 16) {
                    Serial.print("PING BACK: " + String(buffer));
                    debug::log(String(buffer));
                }
            }
            vTaskDelay( 1200 );
        }
    }

    void initializeTask() {
        pinMode(8, OUTPUT);
        digitalWrite(8, HIGH);
        Serial.begin(115200);

        BaseType_t taskHolder;
        taskHolder = xTaskCreate(task, "radioTask", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        if (taskHolder != pdPASS) {
            debug::log("Failed to create radio task");
            while(1);
        }
    }
}
