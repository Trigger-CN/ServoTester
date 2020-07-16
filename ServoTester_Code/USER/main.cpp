//2019.7.3  V2.0.1 	修改Dshot的Bug
//2019.6.29 V2.0 	添加对Dshot协议的支持，并支持PWM与Dshot切换
//2019.6.19 v1.0 	OLED 64*32超高分辨率屏幕，绚丽滑动logo，精密电位器，支持PWM

#include "FileGroup.h"

SCREEN_CLASS screen(Reset_Pin);

Servo sign;

void Init_Display()
{
	__LoopExecute(Wire.begin(), 50);
    screen.begin(SSD1306_SWITCHCAPVCC);
	screen.fillScreen(screen.Black);
	screen.display();
    screen.setTextSize(1);
    screen.setTextColor(screen.White, screen.Black);
}

float LinearMap(float val, float omin, float omax, float min, float max)
{
	float k = 0;
	if(omin != omax)
	{
		k = (val - omin) / (omax - omin);
	}
	return min - min * k + max * k;
}

float ReadKnobADC()
{
    //return (ADC_MaxValue - dfKnob_ADC.getSmooth(analogRead(Knob_Pin)));
	float val = analogRead(Knob_Pin);
	val = constrain(val, DeadSpace_MIN, DeadSpace_MAX);
	return KnobFilter.Next(val);
}

bool HadLongPressed()
{
	static bool longstate = false;
	static uint32_t lastMS = millis();
	if(digitalRead(KEY1_Pin))
	{
		lastMS = millis();
		longstate = false;
	}
	if(millis() - lastMS > 1000)
	{
		if(!longstate)
		{
			longstate = true;
			return true;
		}
	}
	return false;
}

void setup()
{
	 float By;
    int p0y = 0, p1y = 50, p2y = 250;
    pinMode(KEY1_Pin, INPUT_PULLUP);
    pinMode(KEY2_Pin, INPUT_PULLUP);
    pinMode(Knob_Pin, INPUT_ANALOG);
	PWM_Init(LED_Pin, 180, 1000);
	PWM_Init(SignOut_Pin, 20000, 50);
    //sign.attach(SignOut_Pin);
	analogWrite(LED_Pin, 180);
	Init_Display();
	for(int i=65;i>0;i--)
	{
		
		By = ((1 - (i * 0.01)) * (1 - (i * 0.01)) * p0y) + (2 * i * (1 - (i * 0.01)) * p1y) + i * i * p2y;
		screen.clearDisplay();
		screen.drawBitmap(0,map(By, 0, 1000000, 0, 65),logo_bmp,64,32,screen.White);
		screen.display();
	}
	delay(500);
	for(int i=65;i>0;i--)
	{
		
		By = ((1 - (i * 0.01)) * (1 - (i * 0.01)) * p0y) + (2 * i * (1 - (i * 0.01)) * p1y) + i * i * p2y;
		screen.clearDisplay();
		screen.drawBitmap(0,(68-map(By, 0, 1000000, 0, 65)),logo_bmp,64,32,screen.White);
		screen.display();
	}
	screen.clearDisplay();
}

void loop()
{
	while(1)
	{
		PWMMode();
		DShotMode();
	}
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    Delay_Init();
    ADCx_Init(ADC1);
//	ADCx_DMA_Init();
    setup();
    for(;;)loop();
}
