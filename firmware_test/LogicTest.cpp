#include "gtest/gtest.h"
#include "fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include "Logik.h"
#include <mh_z19.h>
#include "ModuleHandler.h"
#include "SensorReading.h"


}


DEFINE_FFF_GLOBALS
FAKE_VOID_FUNC(humChecker);
FAKE_VOID_FUNC(tempChecker);
FAKE_VOID_FUNC(co2Checker);
FAKE_VOID_FUNC(humidifier_Lower);

logik_obj logikObj;

FAKE_VALUE_FUNC(float, sensor_getHum);



class LogicTest_test : public ::testing::Test
{

protected:
	void SetUp() override
	{
	RESET_FAKE(humChecker);
    RESET_FAKE(tempChecker);
    RESET_FAKE(co2Checker);
    RESET_FAKE(sensor_getHum);

		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};
TEST(HumCheckerTest, HumOutsideValidRange) {
    
    // Arrange
    logikObj.hum_Lower = 40;  // Set the lower threshold
    logikObj.hum_Upper = 60;  // Set the upper threshold
    sensor_getHum_fake.return_val=30;  // Set the humidity reading below the lower threshold

    // Act
    humChecker(NULL);  // Call the function

    // Assert
    EXPECT_EQ(humidifier_Lower_fake.call_count, 0);
   // EXPECT_EQ(mock_humidifier_raise_called, 0);  // Verify that humidifier_Raise was not called
    //EXPECT_EQ(mock_humidifier_lower_called, 0);  // Verify that humidifier_Lower was not called
}

