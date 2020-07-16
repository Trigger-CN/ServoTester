#include "FileGroup.h"

static uint16_t L_position;
void DShotMode_setup()
{
	pinMode(SignOut_Pin, OUTPUT);
	digitalWrite(SignOut_Pin, HIGH);
	screen.clearDisplay();
	screen.display();
}
int DShotMode_loop()
{
	static uint16_t adc = ReadKnobADC();
	__IntervalExecute(adc = ReadKnobADC(), 10);
	
	uint16_t pulse = LinearMap(adc, DeadSpace_MAX, DeadSpace_MIN, 0, 2047);
	uint16_t bright = LinearMap(adc, DeadSpace_MAX, DeadSpace_MIN, 200, 0);
  float angle = LinearMap(adc, DeadSpace_MAX, DeadSpace_MIN, 0, 100.0);
	
	analogWrite(LED_Pin, bright);
	
	uint16_t position = LinearMap(SliderFilter.Next(adc), DeadSpace_MAX, DeadSpace_MIN, 0, 60);
	delay(1);
	DSHOT_Write(SignOut_Pin, pulse, 0, 1);
	screen.drawRect(L_position,31,4,1,screen.Black);
	screen.drawRect(position,31,4,1,screen.White);
	screen.setCursor(0, 0);
	screen.printf("M:DShot600",pulse);
	screen.setCursor(0, 10);
	screen.printf("Thr: %04d  ",pulse);
	screen.setCursor(0, 20);
	screen.printf("Pos:%5.1f%%",angle);
	screen.display();
	L_position=position;
	
	if(HadLongPressed())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void DShotMode()
{
	DShotMode_setup();
	for(;;)
	{
		if(DShotMode_loop())
		{
			break;
		}
	}
}
