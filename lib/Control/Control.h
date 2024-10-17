#pragma once
#include <inttypes.h>
#include <Arduino.h>

class Control
{
	static constexpr uint16_t TICK = 32;
	
	enum mode_t : uint8_t { MODE_NONE, MODE_OFF, MODE_ON, MODE_DELAY_OFF };
	
	public:

		Control(uint8_t pin) : _pin(pin), _last_time(0){}

		void Init();

		void On();

		void Off();

		// Включить на указанное время
		void On(uint16_t delay);

		bool Get();

		void Processing(uint32_t time);
		
	private:
		
		uint8_t _pin;
		uint16_t _delay;
		mode_t _mode;
		uint32_t _last_time;

};
