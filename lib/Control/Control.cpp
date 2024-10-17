#include <Control.h>

void Control::Init()
{
	pinMode(_pin, OUTPUT);
	Off();

	return;
}

void Control::On()
{
	digitalWrite(_pin, HIGH);
	_mode = MODE_ON;
	
	return;
}

void Control::Off()
{
	digitalWrite(_pin, LOW);
	_mode = MODE_OFF;
	_delay = 0;
	
	return;
}

// Включить на указанное время
void Control::On(uint16_t delay)
{
	if(_delay > 0) return;
	
	On();
	_mode = MODE_DELAY_OFF;
	_delay = delay / TICK;
}

bool Control::Get()
{
	return digitalRead(_pin);
}

void Control::Processing(uint32_t time)
{
	if(time - _last_time < TICK) return;
	_last_time = time;

		switch(_mode)
		{
			case MODE_DELAY_OFF:
			{
				if(--_delay == 0)
				{
					Off();
				}

				break;
			}
			default:
			{
				break;
			}
		}
	

	return;
}
