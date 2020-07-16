#ifndef __FILEGROUP_H
#define	__FILEGROUP_H

/** 
  *@Illustrate: A Cross-platform migration Project | Arduino for STM32F0xx 
  *@Author: FASTSHIFT
  *@Website: https://github.com/FASTSHIFT/Arduino-For-Keil
  */

//************* Arduino Library **************//
#include "Arduino.h"
#include "HardwareSerial.h"
#include "Stream.h"
#include "SPI.h"
#include "Tone.h"
#include "WString.h"
#include "Wire.h"
#include "WMath.h"

//************* STM32 Core Library **************//
#include "adc.h"
#include "delay.h"
#include "exti.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"

//************* STM32 Core Library **************//
#include "Servo.h"
#include "CommonMacro.h"
#include "Adafruit_SSD1306.h"
#include "filters.h"
//************* USER Library **************//
#include "bitmap.h"
#include "softdshot.h"

//************* Value define **************//
#define SCREEN_CLASS Adafruit_SSD1306

#define LED_Pin PA7
#define KEY1_Pin PA6
#define KEY2_Pin PA5
#define Knob_Pin PA0
#define SignOut_Pin	PA4
#define SignOut_CCR	(TIM14->CCR1)
#define Reset_Pin PB1

#define ADC_MaxValue 4095
#define DeadSpace_MIN 100
#define DeadSpace_MAX 4000
//************* project define **************//
 extern SCREEN_CLASS screen;
 extern	Servo sign;
  //static DigitalFilter<int> dfKnob_ADC(15);
static PT1Filter KnobFilter(0.01, 5);
static PT1Filter SliderFilter(0.01, 2);

 //*************function define **************//
void Init_Display(void);
float LinearMap(float val, float omin, float omax, float min, float max);
float ReadKnobADC(void);
void PWMMode(void);
void DShotMode();
bool HadLongPressed();

#endif
