#include <display_7seg.h>

void return_temp_raised()
{
    display_7seg_displayHex("1");
}

void return_temp_lowered()
{
    display_7seg_displayHex("2");
}

void return_co2_raised()
{
    display_7seg_displayHex("3");
}

void return_co2_lowered()
{
    display_7seg_displayHex("4");
}

void sending_data()
{
    display_7seg_displayHex("5");
}

void reciving_data()
{
    display_7seg_displayHex("6");
}