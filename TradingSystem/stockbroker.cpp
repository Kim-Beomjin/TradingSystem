#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include <exception>

interface StockBroker {
	virtual void login(std::string ID, std::string password) = 0;
	virtual void buy(std::string stockCode, int count, int price) = 0;
	virtual void sell(std::string stockCode, int count, int price) = 0;
	virtual int currentPrice(std::string stockCode) = 0;
};

class KiwerStockBroker : public StockBroker, public KiwerAPI {
public:
	void login(std::string ID, std::string password) override {

	}

	void buy(std::string stockCode, int count, int price) override {

	}

	void sell(std::string stockCode, int count, int price) override {

	}

	int currentPrice(std::string stockCode) override {
		return 0;
	}
};

class NemoStockBroker : public StockBroker, public NemoAPI {
public:
	void login(std::string ID, std::string password) override {

	}

	void buy(std::string stockCode, int count, int price) override {

	}

	void sell(std::string stockCode, int count, int price) override {

	}

	int currentPrice(std::string stockCode) override {
		return 0;
	}
};

class AutoTradingSystem {
public:
	void selectStockBrocker(std::string brokerName) {
		if (brokerName == "Kiwer") {
			stockBroker = new KiwerStockBroker();
		} else if (brokerName == "Nemo") {
			stockBroker = new NemoStockBroker();
		} else {
			throw std::runtime_error("Invalid stock broker selection");
		}
		stockBroker = nullptr;
	}

	void setStockBroker(StockBroker* broker) {
		if (stockBroker != nullptr) {
			delete stockBroker;
		}
		stockBroker = broker;
	}

private:
	StockBroker* stockBroker;
};
