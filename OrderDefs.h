#pragma once

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