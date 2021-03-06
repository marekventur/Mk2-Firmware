#include <Debounce.h>
#include <FreeRTOS_ARM.h>

// timer handle for use by pin interrrupt 
TimerHandle_t xLightFadeTimer;

// time to fade lights after in ms
#define LIGHT_FADE_AFTER   3000     // how long should the LED's be on for
#define LIGHT_FADE_STEP    5        // how much to decune the PWM by each time (starts at 255)
#define LIGHT_FADE_DELAY   10       // how much to delay at each level


void setup() {
     
  // PMIC to CHARGE
  pinMode(PMIC_ENOTG, OUTPUT);
  digitalWrite(PMIC_ENOTG, LOW);
  pinMode(SRF_SLEEP, OUTPUT);
  digitalWrite(SRF_SLEEP, LOW); 
  Serial.begin(115200);
  delay(500);
  Serial.println("TiLDA RTOS task test");
  
  
  BaseType_t t1;
  
  Serial.println("LED's OFF");
  // LED's off by defualt
  analogWrite(LED1_RED, 0);
  analogWrite(LED1_GREEN, 0);
  analogWrite(LED1_BLUE, 0);
  analogWrite(LED2_RED, 0);
  analogWrite(LED2_GREEN, 0);
  analogWrite(LED2_BLUE, 0);

  Serial.println("Setup timmer");
  // create Ligth Timmer
  xLightFadeTimer = xTimerCreate(NULL,
                                 (LIGHT_FADE_AFTER / portTICK_PERIOD_MS),
                                 pdFALSE,
                                 NULL,
                                 vLightFadeCallback);
                                 
  Serial.println("Setup BUTTON_LIGHT");
  // setup Light button, set debounce, attachInterrupt 
  pinMode(BUTTON_LIGHT, INPUT);
  digitalWrite(BUTTON_LIGHT, HIGH);
  setDebounce(BUTTON_LIGHT);
  attachInterrupt(BUTTON_LIGHT, vLightOnCallback, FALLING);
  
  Serial.println("Change IRQ priority");
  // reset pin interrupt handler IRQn priority levels to allow use of FreeRTOS API calls
  NVIC_DisableIRQ(PIOA_IRQn);
  NVIC_ClearPendingIRQ(PIOA_IRQn);
  NVIC_SetPriority(PIOA_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ(PIOA_IRQn);
  
  NVIC_DisableIRQ(PIOB_IRQn);
  NVIC_ClearPendingIRQ(PIOB_IRQn);
  NVIC_SetPriority(PIOB_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ(PIOB_IRQn);
  
  NVIC_DisableIRQ(PIOC_IRQn);
  NVIC_ClearPendingIRQ(PIOC_IRQn);
  NVIC_SetPriority(PIOC_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ(PIOC_IRQn);

  NVIC_DisableIRQ(PIOD_IRQn);
  NVIC_ClearPendingIRQ(PIOD_IRQn);
  NVIC_SetPriority(PIOD_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
  NVIC_EnableIRQ(PIOD_IRQn);
  
  Serial.println("Setup blink Task");
  // setup blink task
  t1 = xTaskCreate(vBlinkTask, 
                   NULL, 
                   configMINIMAL_STACK_SIZE, 
                   NULL, 
                   1, 
                   NULL);
  
  if (t1 != pdPASS) {
    // tasked didn't get created
    Serial.println("Failed to create task");
    while(1);
  }
  Serial.println("Start Scheduler");
  // start scheduler
  vTaskStartScheduler();
  Serial.println("Insufficient RAM");
  while(1);
}

// in FreeRTOS loop is use as the vApplicationIdleHook
void loop() {
}

/* 
 * A BUTTON_LIGHT press will call the ISR and turn on the LED's to white
 * this will then start the timer to turn them back off after LIGHT_FADE_AFTER 
 * Another button press befor the LIGHT_FADE_AFTER time has expired or during the LightFadeCallback
 * will set the RGB's back to White and reset the time out period
 */
void vLightOnCallback() {
  // turn on LED's to White
  analogWrite(LED1_RED, 255);
  analogWrite(LED1_GREEN, 255);
  analogWrite(LED1_BLUE, 255);
  analogWrite(LED2_RED, 255);
  analogWrite(LED2_GREEN, 255);
  analogWrite(LED2_BLUE, 255);
  
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  // start the timer task to turn off the LED's
  xTimerStartFromISR(xLightFadeTimer, &xHigherPriorityTaskWoken);
  
  /* If xHigherPriorityTaskWoken equals pdTRUE, then a context switch
    should be performed.  The syntax required to perform a context switch
    from inside an ISR varies from port to port, and from compiler to
    compiler.  Inspect the demos for the port you are using to find the
    actual syntax required. */
  if( xHigherPriorityTaskWoken != pdFALSE )
  {
    /* Call the interrupt safe yield function here (actual function
       depends on the FreeRTOS port being used). */
       portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }

}
  
/* This should get run LIGHT_FADE_AFTER ms after a call to vLightOnCallback
 * evemtulay we will fade the RGB's to off, for testing we are using digitalWrite LOW
 */
void vLightFadeCallback(TimerHandle_t xTimer) {
  // just turn off ligts for now, will work out fade later as not sure we want to do it in ISR
  Serial.println("LED fade timeout");
  for(int i=255; i>=0; i-=LIGHT_FADE_STEP){
    analogWrite(LED1_RED, i);
    analogWrite(LED1_GREEN, i);
    analogWrite(LED1_BLUE, i);
    analogWrite(LED2_RED, i);
    analogWrite(LED2_GREEN, i);
    analogWrite(LED2_BLUE, i);
    delay(LIGHT_FADE_DELAY);
  } 
}


void vBlinkTask(void *pvParameters) {
  Serial.println("Blink Task start");
  // int to hold led state
  uint8_t state = 0;
  // enabled pin 13 led
  pinMode(PIN_LED_TXL, OUTPUT);
  while(1) {
   digitalWrite(PIN_LED_TXL, state);
   state = !state; 
    
   vTaskDelay((100/portTICK_PERIOD_MS));
  }
  
}
