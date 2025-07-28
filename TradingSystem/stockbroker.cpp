#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include <exception>
#include <sstream>

interface StockBroker {
	virtual bool login(std::string ID, std::string password) = 0;
	virtual void buy(std::string stockCode, int count, int price) = 0;
	virtual void sell(std::string stockCode, int count, int price) = 0;
	virtual int currentPrice(std::string stockCode) = 0;
	virtual bool getLoggedIn(void) = 0;
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
			return true;
		} else {
			std::cout.rdbuf(oldCoutStreamBuf);
			std::cout << "Kiwer login failed\n";
			return false;
		}
	}

	void buy(std::string stockCode, int count, int price) override {
		buy(stockCode, count, price);
	}

	void sell(std::string stockCode, int count, int price) override {
		sell(stockCode, count, price);
	}

	int currentPrice(std::string stockCode) override {
		return currentPrice(stockCode);
	}

	bool getLoggedIn(void) override {
		return loggedIn;
	}

private:
	void setLoggedIn(void) override {
		loggedIn = true;
	}
	bool loggedIn;
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
			return true;
		} else {
			std::cout.rdbuf(oldCoutStreamBuf);
			std::cout << "Nemo login failed\n";
			return false;
		}
		setLoggedIn();
	}

	void buy(std::string stockCode, int count, int price) override {
		buy(stockCode, count, price);
	}

	void sell(std::string stockCode, int count, int price) override {
		sell(stockCode, count, price);
	}

	int currentPrice(std::string stockCode) override {
		return currentPrice(stockCode);
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

	bool loggedIn;
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

	void setStockBroker(StockBroker* broker) {
		if (stockBroker != nullptr) {
			delete stockBroker;
		}
		stockBroker = broker;
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
	int currentPrice(std::string stockCode)
	{
		int price = 0;
		if (isValidRequestWithoutLogin(stockCode) == false)
		{
			return price;
		}
		stockBroker->currentPrice(stockCode);
		return price;
	}
	void buy(std::string stockCode, int count, int price)
	{
		if (isValidRequestWithLogin(stockCode) == false) {
			return;
		}
		stockBroker->buy(stockCode, count, price);
		return;
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
};
