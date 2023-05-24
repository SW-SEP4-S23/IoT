/*
 * ModuleHandler.c
 *
 * Created: 19/05/2023 12.42.20
 *  Author: jacob
 */ 
#include <display_7seg.h>
#include "../Headers/ModuleHandler.h"
#include <util/delay.h>

void write_7Segment(char value[]);

void startCo2Generator(){
	write_7Segment("1");
}

void startVentilation(){
	write_7Segment("2");
}

void humidifier_Lower(){
	write_7Segment("3");
}

void humidifier_Raise(){
	write_7Segment("4");
}

void ac_Raise(){
	write_7Segment("5");
}

void ac_Lower(){
	write_7Segment("6");
}

void write_7Segment(char value[]) {
	display_7seg_powerDown();
	_delay_ms(1000);
	display_7seg_powerUp();
	display_7seg_displayHex(value);
}