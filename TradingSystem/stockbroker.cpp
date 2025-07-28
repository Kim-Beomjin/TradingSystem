#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include <exception>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <string>


interface StockBroker {
	StockBroker() : currentID(""), stockPriceMapForID{} {}
	virtual bool login(std::string ID, std::string password) = 0;
	virtual bool buy(std::string stockCode, int count, int price) = 0;
	virtual void sell(std::string stockCode, int count, int price) = 0;
	virtual int currentPrice(std::string stockCode) = 0;
	virtual bool getLoggedIn(void) = 0;
protected:
	std::unordered_map<std::string, std::pair<int, int>> getStockPriceMapForID(std::string ID) {
		if (stockPriceMapForID.find(ID) == stockPriceMapForID.end()) {
			stockPriceMapForID[ID] = {};
		}
		return stockPriceMapForID[ID];
	}

	std::pair<int, int> getCountAndPriceForStock(std::string stockCode) {
		if (stockPriceMapForID[currentID].find(stockCode) == stockPriceMapForID[currentID].end()) {
			return std::make_pair(0, 0);
		}
		return stockPriceMapForID[currentID][stockCode];
	}

	std::string currentID;
	std::unordered_map<std::string, std::unordered_map<std::string, std::pair<int, int>>> stockPriceMapForID;
	virtual void setLoggedIn(void) = 0;
};

class KiwerStockBroker : public StockBroker {
public:
	bool login(std::string ID, std::string password) override {
		std::ostringstream oss = {};
		auto oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		kiwerAPI.login(ID, password);
		if (oss.str().find("login success") != std::string::npos) {
			std::cout.rdbuf(oldCoutStreamBuf);
			setLoggedIn();
			std::cout << "Kiwer login success\n";
			currentID = ID;
			return true;
		} else {
			std::cout.rdbuf(oldCoutStreamBuf);
			std::cout << "Kiwer login failed\n";
			currentID = "";
			return false;
		}
	}

	bool buy(std::string stockCode, int count, int price) override {
		if (currentID.empty()) {
			std::cout << "Login First Please\n";
			return false;
		}

		std::unordered_map<std::string, std::pair<int, int>> stockPriceMap = getStockPriceMapForID(currentID);
		std::pair<int, int> countAndPriceForStock = getCountAndPriceForStock(stockCode);

		std::cout << "current stock is " << stockCode << " : Having stock info is ( " << countAndPriceForStock.first << " * " << countAndPriceForStock.second << ")\n";
		std::cout << "buy stock ( " << price << " * " << count << ")\n";

		kiwerAPI.buy(stockCode, count, price);

		countAndPriceForStock.first += count;
		countAndPriceForStock.second += price;
		stockPriceMap[stockCode] = std::make_pair(count, price);
		stockPriceMapForID[currentID] = stockPriceMap;
		std::cout << "[KIWER] " << stockCode << " buy result ( price : " << countAndPriceForStock.second << " ) * ( count : " << countAndPriceForStock.first << ")\n";
		return true;
	}

	void sell(std::string stockCode, int count, int price) override {
		kiwerAPI.sell(stockCode, count, price);
	}

	int currentPrice(std::string stockCode) override {
		Sleep(200);
		return kiwerAPI.currentPrice(stockCode);
	}

	bool getLoggedIn(void) override {
		return loggedIn;
	}

private:
	void setLoggedIn(void) override {
		loggedIn = true;
	}
	bool loggedIn = false;
	KiwerAPI kiwerAPI;
};

class NemoStockBroker : public StockBroker {
public:
	bool login(std::string ID, std::string password) override {
		std::ostringstream oss = {};
		auto oldCoutStreamBuf = std::cout.rdbuf();
		std::cout.rdbuf(oss.rdbuf());

		nemoAPI.certification(ID, password);
		if (oss.str().find("login GOOD") != std::string::npos) {
			std::cout.rdbuf(oldCoutStreamBuf);
			std::cout << "Nemo login success\n";
			setLoggedIn();
			currentID = ID;
			return true;
		} else {
			std::cout.rdbuf(oldCoutStreamBuf);
			std::cout << "Nemo login failed\n";
			currentID = "";
			return false;
		}
	}

	bool buy(std::string stockCode, int count, int price) override {
		if (currentID.empty()) {
			std::cout << "Login First Please\n";
			return false;
		}

		std::unordered_map<std::string, std::pair<int, int>> stockPriceMap = getStockPriceMapForID(currentID);
		std::pair<int, int> countAndPriceForStock = getCountAndPriceForStock(stockCode);

		std::cout << "current stock is " << stockCode << " : Having stock info is ( " << countAndPriceForStock.first << " * " << countAndPriceForStock.second << ")\n";
		std::cout << "buy stock ( " << price << " * " << count << ")\n";

		nemoAPI.purchasingStock(stockCode, count, price);

		countAndPriceForStock.first += count;
		countAndPriceForStock.second += price;
		stockPriceMap[stockCode] = std::make_pair(count, price);
		stockPriceMapForID[currentID] = stockPriceMap;
		std::cout << "[NEMO] " << stockCode << " buy result ( price : " << countAndPriceForStock.second << " ) * ( count : " << countAndPriceForStock.first << ")\n";
		return true;
	}

	void sell(std::string stockCode, int count, int price) override {
		nemoAPI.sellingStock(stockCode, price, count);
	}

	int currentPrice(std::string stockCode) override {
		return nemoAPI.getMarketPrice(stockCode, 200);
	}

	bool getLoggedIn(void) override
	{
		return loggedIn;
	}

private:
	void setLoggedIn(void) override
	{
		loggedIn = true;
	}

	bool loggedIn = false;
	NemoAPI nemoAPI;
};

class AutoTradingSystem {
public:
	AutoTradingSystem(): stockBroker(nullptr) {}
	AutoTradingSystem(StockBroker* broker) : stockBroker(broker) {}

	void selectStockBrocker(std::string brokerName) {
		if (brokerName == "Kiwer") {
			stockBroker = new KiwerStockBroker();
		} else if (brokerName == "Nemo") {
			stockBroker = new NemoStockBroker();
		} else {
			throw std::runtime_error("Invalid stock broker selection");
		}
	}

	bool login(std::string ID, std::string password) {
		if (stockBroker == nullptr) {
			throw std::runtime_error("Stock broker not selected");
		}
		if (ID.empty()) {
			throw std::runtime_error("Invalid ID");
		}
		if (password.empty()) {
			return false;
		}

		return stockBroker->login(ID, password);
	}

	bool buy(std::string stockCode, int count, int price) {
		if (stockCode.empty()) {
			throw std::runtime_error("Empty Stock Code");
		}

		if (stockBroker == nullptr) {
			throw std::runtime_error("Stock broker not selected");
		}

		return stockBroker->buy(stockCode, count, price);
	}

	int currentPrice(std::string stockCode)
	{
		int price = 0;
		if (isValidRequestWithoutLogin(stockCode) == false)
		{
			return price;
		}
		price = stockBroker->currentPrice(stockCode);
		return price;
	}
	void sell(std::string stockCode, int count, int price)
	{
		if (isValidRequestWithLogin(stockCode) == false)
		{
			return;
		}
		stockBroker->sell(stockCode, count, price);
		return;
	}

	void buyNiceTiming(std::string stockCode, int budget)
	{
		if (stockBroker == nullptr) {
			throw std::runtime_error("Stock broker not selected");
		}
		if (stockCode.empty()) {
			throw std::runtime_error("Invalid stock code");
		}
		if (budget <= 0) {
			throw std::runtime_error("Invalid budget");
		}
		std::vector<int> historyPrice;
		int timeStampMs = 0;
		int curPrice = 0, prevPrice = 0, increaseCnt = 0;;
		for (int i = 0; i < PRICE_CHECK_COUNT; i++) {
			curPrice = currentPrice(stockCode);
			historyPrice.push_back(curPrice);
		}
		for (int i = 1; i < PRICE_CHECK_COUNT; i++) {
			if (historyPrice[i] <= historyPrice[i - 1]) {
				return;
			}
		}
		stockBroker->buy(stockCode, budget / curPrice, curPrice);
	}
	void sellNiceTiming(std::string stockCode, int stockCount)
	{
		if (stockBroker == nullptr) {
			throw std::runtime_error("Stock broker not selected");
		}
		if (stockCode.empty()) {
			throw std::runtime_error("Invalid stock code");
		}
		if (stockCount <= 0) {
			throw std::runtime_error("Invalid stock count");
		}
		std::vector<int> historyPrice;
		int curPrice = 0;
		for (int i = 0; i < PRICE_CHECK_COUNT; i++)
		{
			curPrice = stockBroker->currentPrice(stockCode);
			historyPrice.push_back(curPrice);
		}
		for (int i = 1; i < PRICE_CHECK_COUNT; i++) {
			if (historyPrice[i] >= historyPrice[i - 1]) {
				return;
			}
		}
		stockBroker->sell(stockCode, stockCount, curPrice);
	}
private:
	bool isValidRequestWithoutLogin(std::string stockCode)
	{
		// 추후 구현에 맞춰 수정할 예정
		if (stockCode == "INVD")
		{
			throw std::runtime_error("Invalid Stock Code");
			return false;
		}

		return true;
	}

	bool isValidRequestWithLogin(std::string stockCode)
	{
		if (isValidRequestWithoutLogin(stockCode) == false) {
			return false;
		}

		if (stockBroker->getLoggedIn() == false)
		{
			throw std::runtime_error("Invalid Sequence - Login First");
			return false;
		}

		return true;
	}
protected:
	StockBroker* stockBroker;
	const int PRICE_CHECK_COUNT = 3;
};
