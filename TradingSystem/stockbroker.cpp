#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include <exception>
#include <sstream>

interface StockBroker {
	virtual bool login(std::string ID, std::string password) = 0;
	virtual void buy(std::string stockCode, int count, int price) = 0;
	virtual void sell(std::string stockCode, int count, int price) = 0;
	virtual int currentPrice(std::string stockCode) = 0;
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
			std::cout << "Kiwer login success\n";
			return true;
		} else {
			std::cout.rdbuf(oldCoutStreamBuf);
			std::cout << "Kiwer login failed\n";
			return false;
		}
	}

	void buy(std::string stockCode, int count, int price) override {

	}

	void sell(std::string stockCode, int count, int price) override {

	}

	int currentPrice(std::string stockCode) override {
		return 0;
	}
private:
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
			return true;
		} else {
			std::cout.rdbuf(oldCoutStreamBuf);
			std::cout << "Nemo login failed\n";
			return false;
		}
	}

	void buy(std::string stockCode, int count, int price) override {

	}

	void sell(std::string stockCode, int count, int price) override {

	}

	int currentPrice(std::string stockCode) override {
		return 0;
	}
private:
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
	void buy(std::string stockCode, int count, int price);
	int currentPrice(std::string stockCode)
	{
		return 0;
	}

protected:
	StockBroker* stockBroker;
};
