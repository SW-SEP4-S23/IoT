#include "gtest/gtest.h"
#include "fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
#include "ModuleHandler.h"
}

TEST(SimpleTest, enLigmedEn)
{
EXPECT_EQ(1,1);
}

FAKE_VOID_FUNC(display_7seg_display);
FAKE_VOID_FUNC(display_7seg_powerDown);
FAKE_VOID_FUNC(display_7seg_powerUp);


//FAKE_VOID_FUNC(startCo2Generator);

class startCo2Generator_test : public ::testing::Test
{
protected:
	void SetUp() override
	{
	RESET_FAKE(display_7seg_display);
    RESET_FAKE(display_7seg_powerUp);
    RESET_FAKE(display_7seg_powerDown);
    //RESET_FAKE(startCo2Generator);
    
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST(startCo2Generator_test, init_co2Generator){
startCo2Generator();
EXPECT_EQ(display_7seg_powerDown_fake.call_count,1);
EXPECT_EQ(display_7seg_powerUp_fake.call_count,1);
EXPECT_EQ(display_7seg_display_fake.call_count,1);

}

