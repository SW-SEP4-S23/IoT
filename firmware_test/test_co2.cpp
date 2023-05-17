#include "gtest/gtest.h"
#include "fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

// Include interfaces and define global variables
// defined by the production code
extern "C"
{

struct sensorData_st iotThing;
EventGroupHandle_t meassureEventGroup;
EventGroupHandle_t dataReadyEventGroup;
extern SemaphoreHandle_t printMutex;
}

// Create Test fixture and Reset all Mocks before each test
class Sep4ThingTest_co2 : public ::testing::Test
{
protected:
	void SetUp() override
	{
	}
	void TearDown() override
	{}
};

// Test if co2 driver is initialized by co2_createTask()
TEST_F()
{
	ASSERT_EQ(1, 1);
}

// Test if co2 driver is initialized with a call back function.
// Insert a custom designed mock to act as co2 driver that calls call back function.
// When call back is run it receives the value in _ppm from co2 driver