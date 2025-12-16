#include "OrderBook.h"

void OrderBook::addOrder(int id, double price, int quantity, Side side)
{
    Order newOrder = {id, price, quantity, side};

    matchOrder(newOrder);

    if (newOrder.quantity > 0)
    {
        if (side == Side::BUY)
        {
            bids[price].push_back(newOrder);
        }
        else
        {
            asks[price].push_back(newOrder);
        }
    }
}

void OrderBook::printOrder()
{
    std::cout << "\n\n\n-----Order Book-----\n\n";
    std::cout << "Asks:" << std::endl;

    for (auto it = asks.rbegin(); it != asks.rend(); ++it)
    {
        double price = it->first;
        for (const auto &order : it->second)
        {
            std::cout << "price:" << price << " & quantity: " << order.quantity << std::endl;
        }
    }

    std::cout << "-------SPREAD-------" << std::endl;

    std::cout << "Bids:" << std::endl;
    for (const auto &[price, orderlist] : bids) // Can use iterator as well like in 'asks' above instead of structural binding
    {
        for (const auto &order : orderlist)
        {
            std::cout << "price:" << price << " & quantity: " << order.quantity << std::endl;
        }
    }

    std::cout << "------------------\n";
}

bool OrderBook::matchOrder(Order &incomingOrder)
{
    if (incomingOrder.side == Side::BUY)
    {
        auto it = asks.begin();

        while (it != asks.end() && incomingOrder.quantity > 0)
        {
            double bestAskPrice = it->first;
            std::list<Order> &ordersAtPrice = it->second;

            if (incomingOrder.price < bestAskPrice)
            {
                break;
            }

            auto orderIt = ordersAtPrice.begin();
            while (orderIt != ordersAtPrice.end() && incomingOrder.quantity > 0)
            {
                Order &bookOrder = *orderIt;

                int tradeQty = std::min(incomingOrder.quantity, bookOrder.quantity);

                incomingOrder.quantity -= tradeQty;
                bookOrder.quantity -= tradeQty;

                if (bookOrder.quantity == 0)
                {
                    orderIt = ordersAtPrice.erase(orderIt); // returns the next iterator
                }
                else
                {
                    ++orderIt;
                }
            }
            if (ordersAtPrice.empty())
            {
                it = asks.erase(it); // returns the next iterator
            }
            else
            {
                ++it;
            }
        }
    }
    else
    {
        auto it = bids.begin();
        while (it != bids.end() && incomingOrder.quantity > 0)
        {
            double bestBuyPrice = it->first;
            std::list<Order> &orderAtPrice = it->second;

            if (incomingOrder.price > bestBuyPrice)
            {
                break;
            }

            auto orderIt = orderAtPrice.begin();
            while (orderIt != orderAtPrice.end() && incomingOrder.quantity > 0)
            {
                Order &bookOrder = *orderIt;
                int tradeQty = std::min(incomingOrder.quantity, bookOrder.quantity);
                incomingOrder.quantity -= tradeQty;
                bookOrder.quantity -= tradeQty;

                if (bookOrder.quantity == 0)
                {
                    orderIt = orderAtPrice.erase(orderIt); // returns the next iterator
                }
                else
                {
                    ++orderIt;
                }
            }
            if (orderAtPrice.empty())
            {
                it = bids.erase(it); // return the next iterator
            }
            else
            {
                ++it;
            }
        }
    }
    return incomingOrder.quantity == 0;
}