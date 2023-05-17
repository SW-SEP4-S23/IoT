#include "gtest/gtest.h"
#include "fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

// Include interfaces and define global variables
// defined by the production code
extern "C"
{
#include "appconfig.h"
#include "co2.h"
#include <mh_z19.h>

struct sensorData_st iotThing;
EventGroupHandle_t meassureEventGroup;
EventGroupHandle_t dataReadyEventGroup;
extern SemaphoreHandle_t printMutex;
void co2_taskRun();
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
		RESET_FAKE(xTaskCreate);
		RESET_FAKE(xEventGroupWaitBits);
		RESET_FAKE(xSemaphoreTake);
		RESET_FAKE(xSemaphoreGive);
		RESET_FAKE(mh_z19_initialise);
		RESET_FAKE(mh_z19_injectCallBack);
		RESET_FAKE(mh_z19_takeMeassuring);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

// Test that the task is created correct
TEST_F(Sep4ThingTest_co2, Test_co2_createTask)
{
	// Create the co2 task
	co2_createTask();

	// Is xTaskCreate called?
	ASSERT_EQ(xTaskCreate_fake.call_count, 1);
	// Check all arguments to xTaskCreate
	ASSERT_EQ(xTaskCreate_fake.arg0_val, &co2_task);
	ASSERT_EQ(strncmp(xTaskCreate_fake.arg1_val, "co2_task", 8), 0);
	ASSERT_EQ(xTaskCreate_fake.arg2_val, configMINIMAL_STACK_SIZE);
	ASSERT_EQ(xTaskCreate_fake.arg3_val, nullptr);
	ASSERT_EQ(xTaskCreate_fake.arg4_val, 1);
	ASSERT_EQ(xTaskCreate_fake.arg5_val, nullptr);
}

// Test if co2 driver is initialized by co2_createTask()
TEST_F(Sep4ThingTest_co2, mh_z19_initialise)
{
	// Act
	co2_createTask();

	// Test if initialize functions are called
	ASSERT_EQ(mh_z19_initialise_fake.call_count, 1);
	ASSERT_EQ(mh_z19_initialise_fake.arg0_val, ser_USART3);
	ASSERT_EQ(mh_z19_injectCallBack_fake.call_count, 1);
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

TEST_F(Sep4ThingTest_co2, Test_co2_inject_callback) 
{
	// Prepare expected output
	testing::internal::CaptureStdout();
	char strbuff[30];
	sprintf_s(strbuff, sizeof(strbuff)-1, "CO2: ppm = %d\n", _ppm);
	// Set custom mock functionality to simulate call back from driver
	mh_z19_injectCallBack_fake.custom_fake = injectCallBack_custom_fake;

	// Act
	co2_createTask();

	// Is mh_z19_injectCallBack called?
	ASSERT_EQ(mh_z19_injectCallBack_fake.call_count, 1);
	// Did the callback function print out the CO2 value?
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_EQ(strbuff, output);
	// Does call back update co2 value?
	ASSERT_EQ(iotThing.co2, _ppm);
}

TEST_F(Sep4ThingTest_co2, Test_co2_taskRun_eventGroup)
{
	// Act
	co2_taskRun();

	// Test
	ASSERT_EQ(xEventGroupWaitBits_fake.call_count, 1);
	ASSERT_EQ(xEventGroupWaitBits_fake.arg0_val, meassureEventGroup);
	ASSERT_EQ(xEventGroupWaitBits_fake.arg1_val, CO2_MEASURE_BIT);
	ASSERT_EQ(xEventGroupWaitBits_fake.arg2_val, pdTRUE);
	ASSERT_EQ(xEventGroupWaitBits_fake.arg3_val, pdTRUE);
	ASSERT_EQ(xEventGroupWaitBits_fake.arg4_val, portMAX_DELAY);
}

TEST_F(Sep4ThingTest_co2, Test_mh_z19_takeMeassuring_ok)
{
	// Prepare
	mh_z19_takeMeassuring_fake.return_val = MHZ19_OK;

	// Act
	co2_taskRun();

	// Test if mh_z19_takeMeassuring is called 
	ASSERT_EQ(mh_z19_takeMeassuring_fake.call_count, 1);

	// Verify that no message is printed to the console
	ASSERT_EQ(xSemaphoreTake_fake.call_count, 0);
	ASSERT_EQ(xSemaphoreGive_fake.call_count, 0);
}

TEST_F(Sep4ThingTest_co2, Test_mh_z19_takeMeassuring_no_measuring_available)
{
	// Prepare expected output
	testing::internal::CaptureStdout();

	mh_z19_takeMeassuring_fake.return_val = MHZ19_NO_MEASSURING_AVAILABLE;

	// Act
	co2_taskRun();

	// Test if mh_z19_takeMeassuring is called 
	EXPECT_EQ(mh_z19_takeMeassuring_fake.call_count, 1);

	// Test if error message is printed
	EXPECT_EQ(xSemaphoreTake_fake.call_count, 1);
	EXPECT_EQ(xSemaphoreTake_fake.arg0_val, printMutex);
	EXPECT_EQ(xSemaphoreTake_fake.arg1_val, portMAX_DELAY);
	EXPECT_EQ(xSemaphoreGive_fake.call_count, 1);
	EXPECT_EQ(xSemaphoreGive_fake.arg0_val, printMutex);

	// Test error message
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_EQ("co2 failure\n", output);
}