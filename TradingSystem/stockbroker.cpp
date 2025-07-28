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
		kiwerAPI.login(ID, password);
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

class NemoStockBroker : public StockBroker, public NemoAPI {
public:
	void login(std::string ID, std::string password) override {
		nemoAPI.certification(ID, password);
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
		stockBroker->login(ID, password);
		return true;
	}
	void buy(std::string stockCode, int count, int price);
	int currentPrice(std::string stockCode)
	{
		return 0;
	}

protected:
	StockBroker* stockBroker;
};
