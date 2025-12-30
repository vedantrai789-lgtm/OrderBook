#pragma once
#include "OrderDefs.h"
#include "OrderPool.h"
#include <vector>

struct level
{
    int head = -1;
    int tail = -1;
};

class OrderBook
{
private:
    std::vector<level> bids;
    std::vector<level> asks;
    OrderPool orderpool; // custom memory

    int maxBidPrice;
    int minAskPrice;

public:
    OrderBook() : orderpool(100000), maxBidPrice(0), minAskPrice(MAX_PRICE) // constructor
    {
        bids.resize(MAX_PRICE + 1);
        asks.resize(MAX_PRICE + 1);
    };
    void addOrder(int id, int price, int quantity, Side side);
    bool matchOrder(Order &order);
    // void cancelOrder(int id);
    void printOrder();

    bool isAskEmpty(int price) const { return asks[price].head == -1; }
    bool isBidEmpty(int price) const { return bids[price].head == -1; }
    int getAskCount(int price); // Need to count manually now
};
