#include "OrderBook.h"
#include <algorithm>

OrderBook::OrderBook() : maxBidPrice(0), minAskPrice(MAX_PRICE)
{
    // resize vectors
    bids.resize(MAX_PRICE + 1);
    asks.resize(MAX_PRICE + 1);
}

void OrderBook::addOrder(int id, int price, int quantity, Side side)
{
    Order newOrder = {id, price, quantity, side};

    if (matchOrder(newOrder))
        return;

    if (newOrder.quantity > 0)
    {
        if (side == Side::BUY)
        {
            bids[price].push_back(newOrder);
            if (price > maxBidPrice)
                maxBidPrice = price;
        }
        else
        {
            asks[price].push_back(newOrder);
            if (price < minAskPrice)
                minAskPrice = price;
        }
    }
}

void OrderBook::printOrder()
{
    // Only print relevant range for visual clarity
    // (Printing 100,000 lines would be bad)
    std::cout << "--- ASKS ---\n";
    for (int p = minAskPrice + 5; p >= minAskPrice; p--)
    {
        if (p >= MAX_PRICE)
            continue;
        if (!asks[p].empty())
        {
            std::cout << "Price: " << p << " Qty: " << asks[p].size() << "\n";
        }
    }
    std::cout << "--- BIDS ---\n";
    for (int p = maxBidPrice; p >= maxBidPrice - 5; p--)
    {
        if (p < 0)
            continue;
        if (!bids[p].empty())
        {
            std::cout << "Price: " << p << " Qty: " << bids[p].size() << "\n";
        }
    }
}

bool OrderBook::matchOrder(Order &incomingOrder)
{
    if (incomingOrder.side == Side::BUY)
    {
        // auto it = asks.begin();

        while (minAskPrice < MAX_PRICE && incomingOrder.quantity > 0)
        {
            // double bestAskPrice = it->first;
            std::list<Order> &ordersAtPrice = asks[minAskPrice];

            if (incomingOrder.price < minAskPrice)
            {
                return false;
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
                ++minAskPrice; // moving inside vector
            }
        }
    }
    else
    {
        // auto it = bids.begin();
        while (maxBidPrice > 0 && incomingOrder.quantity > 0)
        {
            // double bestBuyPrice = it->first;
            std::list<Order> &orderAtPrice = bids[maxBidPrice];

            if (incomingOrder.price > maxBidPrice)
            {
                return false;
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
                --maxBidPrice;
            }
        }
    }
    return incomingOrder.quantity == 0;
}