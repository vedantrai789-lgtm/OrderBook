#pragma once
#include <iostream>
#include <map>
#include <list>

enum class Side
{
    BUY,
    SELL
};

struct Order
{
    int id;
    int price;
    int quantity;
    Side side;
};

const int MAX_PRICE = 100000;

class OrderBook
{
private:
    std::vector<std::list<Order>> bids;
    std::vector<std::list<Order>> asks;

    int maxBidPrice;
    int minAskPrice;

public:
    OrderBook(); // constructor
    void addOrder(int id, int price, int quantity, Side side);
    bool matchOrder(Order &order);
    void cancelOrder(int id);
    void printOrder();

    bool isAskEmpty(int price)
    {
        if (price >= MAX_PRICE || price < 0)
            return true;
        return asks[price].empty();
    }

    bool isBidEmpty(int price)
    {
        if (price >= MAX_PRICE || price < 0)
            return true;
        return bids[price].empty();
    }

    int getAskCount(int price)
    {
        if (price >= MAX_PRICE || price < 0)
            return 0;
        return asks[price].size();
    }

    int getBidCount(int price)
    {
        if (price >= MAX_PRICE || price < 0)
            return 0;
        return bids[price].size();
    }
};
