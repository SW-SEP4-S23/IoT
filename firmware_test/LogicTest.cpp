#include "gtest/gtest.h"
#include "fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "../Headers/Logik.h"
#include "../Headers/SensorReading.h"

SemaphoreHandle_t Mutex;

logik_obj logikObj;

}


DEFINE_FFF_GLOBALS
FAKE_VOID_FUNC(humChecker);
/*FAKE_VOID_FUNC(tempChecker);
FAKE_VOID_FUNC(co2Checker);

FAKE_VOID_FUNC(humidifier_Lower);
FAKE_VOID_FUNC(humidifier_Raise);

FAKE_VOID_FUNC(startCo2Generator);
FAKE_VOID_FUNC(startVentilation);
FAKE_VOID_FUNC(ac_Lower);
FAKE_VOID_FUNC(ac_Raise);
*/


FAKE_VALUE_FUNC(float, sensor_getHum);
FAKE_VALUE_FUNC(float, sensor_getTemp);
FAKE_VALUE_FUNC(float, sensor_getCo2);



class LogicTest_test : public ::testing::Test
{

protected:
	void SetUp() override
	{
    RESET_FAKE(xTaskCreate);
	RESET_FAKE(humChecker);
    /*RESET_FAKE(co2Checker);
    RESET_FAKE(sensor_getHum);
    RESET_FAKE(sensor_getTemp);
    RESET_FAKE(sensor_getCo2);
    RESET_FAKE(humidifier_Lower);
    RESET_FAKE(humidifier_Raise);
    RESET_FAKE(startCo2Generator);
    RESET_FAKE(startVentilation);
    RESET_FAKE(ac_Lower);
    RESET_FAKE(ac_Raise);
    */

		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};
TEST(HumCheckerTest, HumOutsideLowValidRange) {
    
    
    // Arrange
   // logikObj.hum_Lower = 0;  // Set the lower threshold
   // logikObj.hum_Upper = 100;  // Set the upper threshold
   // sensor_getHum_fake.return_val=-1;  // Set the humidity reading below the lower threshold

    // Act
    humChecker();// Call the function

    ASSERT_EQ(xTaskCreate_fake.call_count, 1);// Check to see if the task is created
}
/*
    // Assert
    EXPECT_EQ(humidifier_Lower_fake.call_count, 0);// Verify that humidifier_Lower was not called
    EXPECT_EQ(humidifier_Raise_fake.call_count, 0); // Verify that humidifier_Raise was not called
}
TEST(HumCheckerTest, HumOutsideHighValidRange) {
    logikObj.hum_Lower = 0;  // Set the lower threshold
    logikObj.hum_Upper = 100;  // Set the upper threshold
    sensor_getHum_fake.return_val=101;  // Set the humidity reading above the lower threshold
  
     humChecker();// Call the function

    // Assert
    EXPECT_EQ(humidifier_Lower_fake.call_count, 0);// Verify that humidifier_Lower was not called
    EXPECT_EQ(humidifier_Raise_fake.call_count, 0); // Verify that humidifier_Raise was not called
}

TEST(TempCheckerTest, TempOutsideLowValidRange) {
    
    // Arrange
    logikObj.temp_Lower = 0;  // Set the lower threshold
    logikObj.temp_Upper = 50;  // Set the upper threshold
    sensor_getTemp_fake.return_val=-1;  // Set the temp reading below the lower threshold

    // Act
    tempChecker();// Call the function

    // Assert
    EXPECT_EQ(ac_Raise_fake.call_count, 0);// Verify that ac_Raise was not called
    EXPECT_EQ(ac_Lower_fake.call_count, 0); // Verify that ac_Lower was not called
}
TEST(TempCheckerTest, TempOutsideHighValidRange) {
    logikObj.temp_Lower = 0;  // Set the lower threshold
    logikObj.temp_Upper = 50;  // Set the upper threshold
    sensor_getTemp_fake.return_val=61;  // Set the temp reading above the lower threshold
  
    tempChecker();// Call the function

    // Assert
    EXPECT_EQ(ac_Raise_fake.call_count, 0);// Verify that ac_Raise was not called
    EXPECT_EQ(ac_Lower_fake.call_count, 0); // Verify that ac_Lower was not called
}

TEST(Co2CheckerTest, Co2OutsideLowvalidRange){
    logikObj.co2_Lower = 0; // Set the lower threshold
    logikObj.co2_Upper = 2001; // Set the upper threshold
    sensor_getCo2_fake.return_val=-1; // Set the co2 reading below the lower threshold

    co2Checker(); // Call the function

    EXPECT_EQ(startCo2Generator_fake.call_count, 0); // Verify that startCo2Generator was not called
    EXPECT_EQ(startVentilation_fake.call_count, 0); // Verify that startVentilation was not called
}

TEST(Co2CheckerTest, Co2OutsideHighvalidRange){
    logikObj.co2_Lower = 0; // Set the lower threshold
    logikObj.co2_Upper = 2001; // Set the upper threshold
    sensor_getCo2_fake.return_val=-1; // Set the co2 reading above the lower threshold

    co2Checker(); // Call the function

    EXPECT_EQ(startCo2Generator_fake.call_count, 0); // Verify that startCo2Generator was not called
    EXPECT_EQ(startVentilation_fake.call_count, 0); // Verify that startVentilation was not called
}


*/


