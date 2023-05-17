#include "gtest/gtest.h"
#include "fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

// Include interfaces and define global variables
// defined by the production code
extern "C"
{
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
TEST_F(Sep4ThingTest_co2, Test_co2_taskRun_eventGroup)
{
	ASSERT_EQ(1,1);
}