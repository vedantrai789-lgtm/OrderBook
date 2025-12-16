#include "OrderBook.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
int main()
{
    OrderBook book;
    const int NUM_ORDERS = 100000;

    std::vector<Order> orders;
    orders.reserve(NUM_ORDERS);
    std::mt19937 gen(42);                               // fixed seed for reproducibility
    std::uniform_int_distribution<> priceDist(90, 110); // price range between 90 and 110
    std::uniform_int_distribution<> qtyDist(1, 100);
    std::uniform_int_distribution<> sideDist(0, 1);

    for (int i = 0; i < NUM_ORDERS; ++i)
    {
        Side side = (sideDist(gen) == 0) ? Side::BUY : Side::SELL;
        orders.push_back({i, (double)priceDist(gen), qtyDist(gen), side});
    }

    std::cout << "processing" << NUM_ORDERS << " orders \n";
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &order : orders)
    {
        book.addOrder(order.id, order.price, order.quantity, order.side);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "total time: " << duration.count() << " microseconds\n";
    std::cout << "latency per order: " << duration.count() / (double)NUM_ORDERS << " microseconds\n";

    return 0;
}