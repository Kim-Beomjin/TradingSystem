#include "gmock/gmock.h"
#include "stockbroker.cpp"

using namespace std;
using namespace testing;

class AutoTradingFixture : public Test {
public:
	AutoTradingSystem app;
};

class MockStockBroker : public StockBroker {
	MOCK_METHOD(void, login, (string ID, string password), (override));
	MOCK_METHOD(void, buy, (string stockCode, int count, int price), (override));
	MOCK_METHOD(int, currentPrice, (string stockCode), (override));
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

TEST_F(AutoTradingFixture, SelectKiwerStockBrokerAndBuyStock) {
	// TODO: Implement KiwerStockBroker's buy method
	app.selectStockBrocker("Kiwer");
	std::string stockCode = "test";
	int count = 0;
	int price = 0;

	app.setStockBroker(&mockBroker);

	app.buyNiceTiming(stockCode, count, price);

	EXPECT_EQ(app.stockBroker->currentPrice(stockCode), price);
}
