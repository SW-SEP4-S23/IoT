#include "gtest/gtest.h"
#include "fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

// Include interfaces and define global variables
// defined by the production code
extern "C"
{
#include <mh_z19.h>

struct sensorData_st iotThing;
EventGroupHandle_t meassureEventGroup;
EventGroupHandle_t dataReadyEventGroup;
extern SemaphoreHandle_t printMutex;
}

// Create Fake Driver functions
FAKE_VOID_FUNC(mh_z19_initialise, serial_comPort_t);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_takeMeassuring);

// Trick to specify a function pointer as argument in mock
typedef void (*mh_z19_cb)(uint16_t);
FAKE_VOID_FUNC(mh_z19_injectCallBack, mh_z19_cb);

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
uint16_t _ppm = 1055;
void injectCallBack_custom_fake(void(*mh_z19_callBack)(uint16_t arg))
{
	if (mh_z19_callBack)
		mh_z19_callBack(_ppm);
}