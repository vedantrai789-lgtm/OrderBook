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
    std::cout << "--- ASKS ---\n";
    int asksSeen = 0;
    // Scan from 1000 ticks above the best ask to the best ask to find depth
    for (int p = minAskPrice + 1000; p >= minAskPrice; p--)
    {
        if (p >= MAX_PRICE)
            break;
        if (asksSeen >= 5)
            break; // Stop after printing top 5 levels

        if (!asks[p].empty())
        {
            int totalQty = 0;
            for (const auto &o : asks[p])
            {
                totalQty += o.quantity;
            }
            std::cout << "Price: " << p << " | Total Qty: " << totalQty << " (" << asks[p].size() << " orders)\n";
            asksSeen++;
        }
    }

    std::cout << "--- BIDS ---\n";
    int bidsSeen = 0;
    // Scan down to 1000 ticks below the best bid
    for (int p = maxBidPrice; p >= maxBidPrice - 1000; p--)
    {
        if (p < 0)
            break;
        if (bidsSeen >= 5)
            break; // Stop after printing top 5 levels

        if (!bids[p].empty())
        {
            int totalQty = 0;
            for (const auto &o : bids[p])
            {
                totalQty += o.quantity;
            }
            std::cout << "Price: " << p << " | Total Qty: " << totalQty << " (" << bids[p].size() << " orders)\n";
            bidsSeen++;
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