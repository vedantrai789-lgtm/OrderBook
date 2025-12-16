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
    double price;
    int quantity;
    Side side;
};

class OrderBook
{
private:
    std::map<double, std::list<Order>, std::greater<double>> bids; // buy side
    std::map<double, std::list<Order>> asks;                       // sell side
    bool matchOrder(Order &order);

public:
    void addOrder(int id, double price, int quantity, Side side);
    void cancelOrder(int id);
    void printOrder();
};
