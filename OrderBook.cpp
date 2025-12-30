#include "OrderBook.h"
#include <algorithm>
#include <iostream>

void OrderBook::addOrder(int id, int price, int quantity, Side side)
{
    Order newOrder = {id, price, quantity, side};

    if (matchOrder(newOrder))
        return;

    if (newOrder.quantity > 0)
    {
        // allocate new memory
        int idx = orderpool.allocate();
        OrderNode &node = orderpool.get(idx);
        node.order = newOrder;

        level *lvl = (side == Side::BUY) ? &bids[price] : &asks[price];

        if (lvl->head == -1)
        {
            // first order in this price
            lvl->head = idx;
            lvl->tail = idx;
        }
        else
        {
            // append to tail
            OrderNode &Tailnode = orderpool.get(lvl->tail);
            Tailnode.next = idx;
            node.prev = lvl->tail;
            lvl->tail = idx;
        }

        if (side == Side::BUY)
        {
            if (price > maxBidPrice)
                maxBidPrice = price;
        }
        else
        {
            if (price < minAskPrice)
                minAskPrice = price;
        }
    }
}

void OrderBook::printOrder()
{
    /*
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
    */
}

bool OrderBook::matchOrder(Order &incomingOrder)
{
    if (incomingOrder.side == Side::BUY)
    {
        while (minAskPrice < MAX_PRICE && incomingOrder.quantity > 0)
        {
            if (incomingOrder.price < minAskPrice)
            {
                return false;
            }
            level &lvl = asks[minAskPrice];

            int curr = lvl.head;
            while (curr != -1 && incomingOrder.quantity > 0)
            {
                OrderNode &node = orderpool.get(curr);
                Order &bookOrder = node.order;

                int tradeQty = std::min(incomingOrder.quantity, bookOrder.quantity);
                incomingOrder.quantity -= tradeQty;
                bookOrder.quantity -= tradeQty;

                int nextNodeInt = node.next;

                if (bookOrder.quantity == 0)
                {
                    // unlink node (remove from doubly linked list)
                    if (node.prev != -1)
                    {
                        // if a prev node exists
                        orderpool.get(node.prev).next = node.next;
                    }
                    else
                    {
                        lvl.head = node.next; // just change head of list
                    }

                    if (node.next != -1)
                    {
                        orderpool.get(node.next).prev = node.prev;
                    }
                    else
                    {
                        lvl.tail = node.prev; // change tail of list
                    }
                }
                curr = nextNodeInt;
            }
            if (lvl.head == -1)
            {
                minAskPrice++;
            }
        }
    }
    else
    {
        while (maxBidPrice > 0 && incomingOrder.quantity > 0)
        {

            if (incomingOrder.price > maxBidPrice)
            {
                return false;
            }

            level &lvl = bids[maxBidPrice];
            int curr = lvl.head;
            while (curr != -1 && incomingOrder.quantity > 0)
            {
                OrderNode &node = orderpool.get(curr);
                Order &bookOrder = node.order;

                int tradeQty = std::min(incomingOrder.quantity, bookOrder.quantity);

                incomingOrder.quantity -= tradeQty;
                bookOrder.quantity -= tradeQty;

                int nextNodeInt = node.next;
                if (bookOrder.quantity == 0)
                {
                    // unlink node (remove from doubly linked list)
                    if (node.prev != -1)
                    {
                        // if a prev node exists
                        orderpool.get(node.prev).next = node.next;
                    }
                    else
                    {
                        lvl.head = node.next; // just change head of list
                    }

                    if (node.next != -1)
                    {
                        orderpool.get(node.next).prev = node.prev;
                    }
                    else
                    {
                        lvl.tail = node.prev; // change tail of list
                    }
                }
                curr = nextNodeInt;
            }
            if (lvl.head == -1)
            {
                maxBidPrice--;
            }
        }
    }
    return incomingOrder.quantity == 0;
}
int OrderBook::getAskCount(int price)
{
    int count = 0;
    int curr = asks[minAskPrice].head;
    while (curr != -1)
    {
        count++;
        curr = orderpool.get(curr).next;
    }
    return count;
}