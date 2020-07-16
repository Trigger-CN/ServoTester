#include "filters.h"


ButterworthFilter::ButterworthFilter(float dt, float s_1, float a_2_1, float b_2_1, float a_3_1) : DigitalFilter(dt)
{
	Rate_a_2_1 = a_2_1;
	Rate_b_2_1 = b_2_1;
	Rate_a_3_1 = a_3_1;
	Rate_s_1 = s_1;
}

float ButterworthFilter::Next(float value)
{
	static float last1 = 0;
	static float last2 = 0;
	
	float s_1_out = value * Rate_s_1;
	float intg1_out = s_1_out - last1 * Rate_a_2_1;
	float intg2_out = intg1_out - last2 * Rate_a_3_1;
	float intg3_out = intg2_out + last1 * Rate_b_2_1;
	float intg4_out = intg3_out + last2;
	
	last2 = last1;
	last1 = intg2_out;
	
	return intg4_out;
}

CustomFilter::CustomFilter(float dt, int pole, const float *rates) : DigitalFilter(dt)
{
	dT = dt;
	Pole = pole + 1;
	Rates = rates;
	Data = new float[pole];
	
	for(int i = 0; i < pole; i++)
	{
		Data[pole] = 0;
	}
}

float CustomFilter::Next(float value)
{
	Data[CrtPos] = value;
	float val = 0;
	for(int i = Pole; i >= 0; i--)
	{
		val += Data[(CrtPos + Pole + i) % Pole] * Rates[Pole - i];
	}
	CrtPos = (CrtPos + 1) % Pole;
	return val;
}

PT1Filter::PT1Filter(float dt, float cutoff) : DigitalFilter(dt)
{
	IsFirst = true;
	dT = dt;
	float RC = 1 / (2 * 3.141592653 * cutoff);
	RCRate = dT / (RC + dT);
}
float PT1Filter::Next(float value)
{
	if(IsFirst)
	{
		IsFirst = false;
		return LastValue = value;
	}
	else
	{
		return LastValue = (LastValue + (value - LastValue) * RCRate);
	}
}

SlewFilter::SlewFilter(float dt, float dPerSecond) : DigitalFilter(dt)
{
	IsFirst = true;
	dT = dt;
	delta = dPerSecond * dt;
}
float SlewFilter::Next(float value)
{
	if(IsFirst)
	{
		IsFirst = false;
		return LastValue = value;
	}
	else
	{
		if(value - LastValue > delta)
		{
			return LastValue = (LastValue + delta);
		}
		else if(LastValue - value > delta)
		{
			return LastValue = (LastValue - delta);
		}
		else
		{
			return LastValue = value;
		}
	}
}

FIRFilter::FIRFilter(float dt, int cnt) : DigitalFilter(dt)
{
	IsFirst = true;
	dT = dt;
	Buffer = new float[cnt];
	CrtPos = 0;
	Total = 0;
	Count = cnt;
}
FIRFilter::FIRFilter(float dt, int cnt, float *buf) : DigitalFilter(dt)
{
	IsFirst = true;
	dT = dt;
	Buffer = buf;
	CrtPos = 0;
	Total = 0;
	Count = cnt;
}
float FIRFilter::Next(float value)
{
	if(IsFirst)
	{
		IsFirst = false;
		for(int i = 0; i < Count; i++)
		{
			Buffer[i] = value;
			Total += value;
		}
		return value;
	}
	else
	{
		Total -= Buffer[CrtPos = ((CrtPos + 1) % Count)];
		Total += (Buffer[CrtPos] = value);
		return Total / Count;
	}
}

