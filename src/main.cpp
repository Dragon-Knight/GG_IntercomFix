#include <Arduino.h>
#include "Control.h"
#include "utils.h"

static constexpr uint16_t CFG_TICK = 16;
static constexpr uint8_t CFG_READ_COUNT = 8;
static constexpr uint8_t CFG_RUN_COUNT = 2;
static constexpr uint16_t CFG_MIN_DELAY = 50;
static constexpr uint16_t CFG_MAX_DELAY = 1000;
static constexpr uint16_t CFG_MIN_VOLTAGE = 10000;

static constexpr uint8_t PIN_LED = PIN_PB0;			// PB0
static constexpr uint8_t PIN_IN_INT = PIN_PB1;		// PB1, INT0
static constexpr analog_pin_t PIN_VRAW_ADC = A1;	// PB2, A1
static constexpr uint8_t PIN_LOAD_EN = PIN_PB3;		// PB3
static constexpr analog_pin_t PIN_POT_ADC = A2;		// PB4, A2


Control load(PIN_LOAD_EN);
Control led(PIN_LED);
static constexpr uint32_t adc_coefficient = GetCoefficient(5000, 51000, 10000, 1024);
volatile bool is_interrupt = false;
uint8_t read_count = 0;
uint8_t run_count = 0;
uint32_t time;


// Чтение входа
bool GetInState()
{
	return ((digitalRead(PIN_IN_INT) == HIGH) ? false : true);
}

// Получить напряжение питания (до диода) в мВ
uint16_t GetInVoltage()
{
	return (uint16_t)(analogRead(PIN_VRAW_ADC) * adc_coefficient);
}

// Получить значение потанциометра от 0 до 511
uint16_t GetPotValue()
{
	uint16_t adc = analogRead(PIN_POT_ADC);
	if(adc < 64) adc = 256;
	
	return (adc / 2);
	//return map(adc, (uint16_t)0, (uint16_t)1023, CFG_MIN_DELAY, CFG_MAX_DELAY);
}


void ResetRun()
{
	is_interrupt = false;
	read_count = 0;
	run_count = 0;

	return;
}

void setup()
{
	led.Init();
	pinMode(PIN_IN_INT, INPUT);
	pinMode(PIN_VRAW_ADC, INPUT);
	load.Init();
	pinMode(PIN_POT_ADC, INPUT);

	attachInterrupt(PIN_IN_INT, [](){ is_interrupt = true; return; }, FALLING);

	analogReference(EXTERNAL);
	
	return;
}

void loop()
{
	time = millis();
	load.Processing(time);
	led.Processing(time);
	
	static uint32_t last_tick = 0;
	//time = millis();
	if(time - last_tick > CFG_TICK)
	{
		last_tick = time;
		
		if(is_interrupt == true && load.Get() == LOW)
		{
			if(GetInVoltage() >= CFG_MIN_VOLTAGE && GetInState() == true)
			{
				if(++read_count == CFG_READ_COUNT)
				{
					if(++run_count == CFG_RUN_COUNT)
					{
						ResetRun();
					}
					
					uint16_t timeout = GetPotValue();
					
					load.On();
					led.On();
					delay(timeout);
					load.Off();
					led.Off();

					read_count = 0;
				}
			}
			else
			{
				ResetRun();
			}
		}
	}
	
	return;
}
