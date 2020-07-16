#include "FileGroup.h"

static uint16_t L_position;
void PWMMode_setup()
{
	sign.attach(SignOut_Pin);
	screen.clearDisplay();
	screen.display();
}
int PWMMode_loop()
{
	static uint16_t adc = ReadKnobADC();
	__IntervalExecute(adc = ReadKnobADC(), 10);
	
    float angle = LinearMap(adc, DeadSpace_MAX, DeadSpace_MIN, 0.0, 180.0);
	
	uint16_t position = LinearMap(SliderFilter.Next(adc), DeadSpace_MAX, DeadSpace_MIN, 0, 60);
	sign.write(angle);
	float pulse = sign.readMicroseconds();
	screen.drawRect(L_position,31,4,1,screen.Black);
	screen.drawRect(position,31,4,1,screen.White);
	screen.setCursor(0, 0);
	screen.printf("M:PPM",pulse);
	screen.setCursor(0, 10);
	screen.printf("Pulse:%0.0f  ",pulse);
	screen.setCursor(0, 20);
	screen.printf("Deg:%0.0f  ",angle);
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

void PWMMode()
{
	PWMMode_setup();
	for(;;)
	{
		if(PWMMode_loop())
		{
			return;
		}
	}
}
