#include <string>
#include "gmock/gmock.h"
#include "stockbroker.cpp"

using namespace std;
using namespace testing;

class MockStockBroker : public StockBroker {

public:
	MOCK_METHOD(bool, login, (string ID, string password), (override));
	MOCK_METHOD(void, buy, (string stockCode, int count, int price), (override));
	MOCK_METHOD(void, sell, (string stockCode, int count, int price), (override));
	MOCK_METHOD(int, currentPrice, (string stockCode), (override));

};

class AutoTradingFixture : public Test {
public:
	AutoTradingSystem app;
	AutoTradingSystem mockApp{ &mockStockBroker };
	const string INVALID_STOCK_BROCKER = "Invalid";
	const string KIWER_STOCK_BROCKER = "Kiwer";
	const string NEMO_STOCK_BROCKER = "Nemo";

	const string NO_ID = "";
	const string NOT_IMPORTANT_ID = "ID123";
	const string INVALID_PASSWORD = "";
	const string VALID_PASSWORD = "ValidPassword";

	const string INVALID_STOCK_CODE= "INVD";
	const string VALID_STOCK_CODE = "NVDA";

	const int MINIMUM_PRICE = 5000;
	const int MAXIMUM_PRICE = 5900;

	const int DECREASE_PRICES[3] = { 5900, 5500, 5000 };
	const int INCREASE_PRICES[3] = { 5000, 5500, 5900 };

	const int STOCK_BUDGET = 500000;
	const int TARGET_STOCK_COUNT = 10;

	NiceMock<MockStockBroker> mockStockBroker;
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
		app.login(NO_ID, VALID_PASSWORD);
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
		app.login(NO_ID, VALID_PASSWORD);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid ID" });
	}
}

TEST_F(AutoTradingFixture, KiwerLogin) {
	app.selectStockBrocker(KIWER_STOCK_BROCKER);

	bool result = app.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_EQ(true, result);
}

TEST_F(AutoTradingFixture, NemoLoginSuccess) {
	app.selectStockBrocker(NEMO_STOCK_BROCKER);

	bool result = app.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_EQ(true, result);
}

TEST_F(AutoTradingFixture, MockLoginFail) {
	EXPECT_CALL(mockStockBroker, login)
		.Times(1)
		.WillOnce(Return(false));

	bool result = mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_EQ(false, result);
}

TEST_F(AutoTradingFixture, MockLoginFailWithInvalidPassward) {
	bool result = mockApp.login(NOT_IMPORTANT_ID, INVALID_PASSWORD);

	EXPECT_EQ(false, result);
}

TEST_F(AutoTradingFixture, MockLoginSuccess) {
	EXPECT_CALL(mockStockBroker, login)
		.Times(1)
		.WillOnce(Return(true));

	bool result = mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_EQ(true, result);
}

TEST_F(AutoTradingFixture, ThrowInvalidStockCode) {
	try {
		mockApp.currentPrice(INVALID_STOCK_CODE);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid Stock Code" });
	}
}

TEST_F(AutoTradingFixture, ThrowInvalidBuySequence) {
	try {
		mockApp.buy(VALID_STOCK_CODE, 1, 100);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid Sequence - Login First" });
	}
}

TEST_F(AutoTradingFixture, ThrowInvalidSellSequence) {
	try {
		mockApp.sell(VALID_STOCK_CODE, 1, 100);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid Sequence - Login First" });
	}
}

TEST_F(AutoTradingFixture, NiceBuyTestPriceThreeTimes) {
	mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_CALL(mockStockBroker, currentPrice)
		.Times(3);

	mockApp.buyNiceTiming(VALID_STOCK_CODE, STOCK_BUDGET);
}

TEST_F(AutoTradingFixture, NiceBuyTestRepeatSamePrices) {
	mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_CALL(mockStockBroker, currentPrice)
		.WillRepeatedly(Return(MINIMUM_PRICE));

	EXPECT_CALL(mockStockBroker, buy)
		.Times(0);

	mockApp.buyNiceTiming(VALID_STOCK_CODE, STOCK_BUDGET);
}

TEST_F(AutoTradingFixture, NiceBuyTestDecreasePrices) {
	mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_CALL(mockStockBroker, currentPrice)
		.WillOnce(Return(DECREASE_PRICES[0]))
		.WillOnce(Return(DECREASE_PRICES[1]))
		.WillRepeatedly(Return(DECREASE_PRICES[2]));

	EXPECT_CALL(mockStockBroker, buy)
		.Times(0);

	mockApp.buyNiceTiming(VALID_STOCK_CODE, STOCK_BUDGET);
}

TEST_F(AutoTradingFixture, NiceBuyTestIncreasePrices) {
	mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_CALL(mockStockBroker, currentPrice)
		.WillOnce(Return(INCREASE_PRICES[0]))
		.WillOnce(Return(INCREASE_PRICES[1]))
		.WillRepeatedly(Return(INCREASE_PRICES[2]));

	EXPECT_CALL(mockStockBroker, buy)
		.Times(1);

	mockApp.buyNiceTiming(VALID_STOCK_CODE, STOCK_BUDGET);
}

TEST_F(AutoTradingFixture, NiceSellTestPriceThreeTimes) {
	mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_CALL(mockStockBroker, currentPrice)
		.Times(3);

	mockApp.sellNiceTiming(VALID_STOCK_CODE, TARGET_STOCK_COUNT);
}

TEST_F(AutoTradingFixture, NiceSellTestRepeatSamePrices) {
	mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_CALL(mockStockBroker, currentPrice)
		.WillRepeatedly(Return(MAXIMUM_PRICE));

	EXPECT_CALL(mockStockBroker, sell)
		.Times(0);

	mockApp.sellNiceTiming(VALID_STOCK_CODE, TARGET_STOCK_COUNT);
}

TEST_F(AutoTradingFixture, NiceSellTestIncreasePrices) {
	mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_CALL(mockStockBroker, currentPrice)
		.WillOnce(Return(INCREASE_PRICES[0]))
		.WillOnce(Return(INCREASE_PRICES[1]))
		.WillRepeatedly(Return(INCREASE_PRICES[2]));

	EXPECT_CALL(mockStockBroker, sell)
		.Times(0);

	mockApp.sellNiceTiming(VALID_STOCK_CODE, TARGET_STOCK_COUNT);
}

TEST_F(AutoTradingFixture, NiceSellTestDecreasePrices) {
	mockApp.login(NOT_IMPORTANT_ID, VALID_PASSWORD);

	EXPECT_CALL(mockStockBroker, currentPrice)
		.WillOnce(Return(DECREASE_PRICES[0]))
		.WillOnce(Return(DECREASE_PRICES[1]))
		.WillRepeatedly(Return(DECREASE_PRICES[2]));

	EXPECT_CALL(mockStockBroker, sell)
		.Times(1);

	mockApp.sellNiceTiming(VALID_STOCK_CODE, TARGET_STOCK_COUNT);
}
