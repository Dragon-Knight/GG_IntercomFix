#pragma once
#include <inttypes.h>
#include <Arduino.h>

class Control
{
	static constexpr uint16_t TICK = 8;
	
	enum mode_t : uint8_t { MODE_NONE, MODE_OFF, MODE_ON, MODE_DELAY_OFF };
	
	public:

		Control(uint8_t pin) : _pin(pin), _last_time(0)
		{

		}

		void Init()
		{
			pinMode(_pin, OUTPUT);
			Off();

			return;
		}

		void On()
		{
			digitalWrite(_pin, HIGH);
			_mode = MODE_ON;
			
			return;
		}
		
		void Off()
		{
			digitalWrite(_pin, LOW);
			_mode = MODE_OFF;
			_delay = 0;
			
			return;
		}
		
		// Включить на указанное время
		void On(uint16_t delay)
		{
			if(_delay > 0) return;
			
			On();
			_mode = MODE_DELAY_OFF;
			_delay = delay / TICK;
		}

		bool Get()
		{
			return digitalRead(_pin);
		}
		
		void Processing(uint32_t time)
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
		
	private:
		
		uint8_t _pin;
		uint16_t _delay;
		mode_t _mode;
		uint32_t _last_time;

};
