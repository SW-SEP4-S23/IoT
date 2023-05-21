/*
 * ModuleHandler.c
 *
 * Created: 19/05/2023 12.42.20
 *  Author: jacob
 */ 
#include <display_7seg.h>
#include "../Headers/ModuleHandler.h"
#include <util/delay.h>


void startCo2Generator(){
	display_7seg_powerDown();
	_delay_ms(1000);
	display_7seg_powerUp();
	display_7seg_displayHex("1");
}

void startVentilation(){
	display_7seg_powerDown();
	_delay_ms(1000);
	display_7seg_powerUp();
	display_7seg_displayHex("2");
}

void humidifier_Lower(){
	display_7seg_powerDown();
	_delay_ms(1000);
	display_7seg_powerUp();
	display_7seg_displayHex("3");	
}

void humidifier_Raise(){
	display_7seg_powerDown();
	_delay_ms(1000);
	display_7seg_powerUp();
	display_7seg_displayHex("4");
}

void ac_Raise(){
	display_7seg_powerDown();
	_delay_ms(1000);
	display_7seg_powerUp();
	display_7seg_displayHex("5");
}

void ac_Lower(){
	display_7seg_powerDown();
	_delay_ms(1000);
	display_7seg_powerUp();
	display_7seg_displayHex("6");
}