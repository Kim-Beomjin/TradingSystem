#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include <exception>
#include <vector>

interface StockBroker {
	virtual void login(std::string ID, std::string password) = 0;
	virtual void buy(std::string stockCode, int count, int price) = 0;
	virtual void sell(std::string stockCode, int count, int price) = 0;
	virtual int currentPrice(std::string stockCode) = 0;
};

class KiwerStockBroker : public StockBroker, public KiwerAPI {
public:
	void login(std::string ID, std::string password) override {
		login(ID, password);
	}	

	void buy(std::string stockCode, int count, int price) override {

	}

	void sell(std::string stockCode, int count, int price) override {

	}

	int currentPrice(std::string stockCode) override {
		Sleep(200);
		return KiwerAPI::currentPrice(stockCode);
	}
};

class NemoStockBroker : public StockBroker, public NemoAPI {
public:
	void login(std::string ID, std::string password) override {
		certification(ID, password);
	}

	void buy(std::string stockCode, int count, int price) override {

	}

	void sell(std::string stockCode, int count, int price) override {

	}

	int currentPrice(std::string stockCode) override {
		return getMarketPrice(stockCode, 200);
	}
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
	void buy(std::string stockCode, int count, int price)
	{
		stockBroker->buy(stockCode, count, price);
	}
	void sell(std::string stockCode, int count, int price)
	{
		stockBroker->sell(stockCode, count, price);
	}

	int currentPrice(std::string stockCode)
	{
		return stockBroker->currentPrice(stockCode);
	}

	void buyNiceTiming(std::string stockCode, int budget)
	{
		int timeStampMs = 0;

		do {
			int curPrice = currentPrice(stockCode);

			timeStampMs += 200;
			historyPrice.push_back(curPrice);

			if (historyPrice.size() < 3)
				continue;

			if (historyPrice.size() > 3)
				historyPrice.erase(historyPrice.begin());

			int prevPrice = 0, increaseCnt = 0;
			for (int price : historyPrice) {
				if (price > prevPrice) {
					prevPrice = price;
					increaseCnt++;
				}
				else
					break;
			}

			if (increaseCnt == 3) {
				buy(stockCode, budget / curPrice, curPrice);
			}
		} while (timeStampMs < 600);
	}
protected:
	StockBroker* stockBroker;
	std::vector<int> historyPrice;
};
