#include <string>
#include "gmock/gmock.h"

using namespace std;
using namespace testing;

class AutoTradingFixture : public Test {
public:
	AutoTradingSystem app;

	const string INVALID_STOCK_BROCKER = "Invalid";
	const string KIWER_STOCK_BROCKER = "Kiwer";
	const string NEMO_STOCK_BROCKER = "Nemo";

	const string NO_ID = "";
	const string NOT_IMPORTANT_ID = "ID123";


};

TEST_F(AutoTradingFixture, ThrowInvalidSelection) {
	try {
		app.selectStockBrocker(INVALID_STOCK_BROCKER);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid stock broker selection" });
	}
}

TEST_F(AutoTradingFixture, ThrowKiwerBlankID) {
	app.selectStockBrocker(KIWER_STOCK_BROCKER);

	try {
		app.login(NO_ID, 0);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid ID" });
	}
}

TEST_F(AutoTradingFixture, ThrowNemoBlankID) {
	app.selectStockBrocker(NEMO_STOCK_BROCKER);

	try {
		app.login(NO_ID, 0);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid ID" });
	}
}
