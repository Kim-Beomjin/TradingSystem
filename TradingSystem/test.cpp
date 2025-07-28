#include "gmock/gmock.h"

using namespace std;
using namespace testing;

class AutoTradingFixture : public Test {
public:
	AutoTradingSystem app;
};

TEST_F(AutoTradingFixture, ThrowInvalidSelection) {
	try {
		app.selectStockBrocker("Invalid");
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid stock broker selection" });
	}
}