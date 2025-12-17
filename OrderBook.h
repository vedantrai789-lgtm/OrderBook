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
};
