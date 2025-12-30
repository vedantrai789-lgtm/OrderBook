#include <gtest/gtest.h>
#include "../OrderBook.h"
#include "../OrderDefs.h"

// test that orderbook initializes correctly
TEST(OrderBookTests, Initialization)
{
    OrderBook book;
    SUCCEED();
}

// test adding an order
TEST(OrderBookTests, AddOrder)
{
    OrderBook book;
    book.addOrder(1, 100, 10, Side::BUY);
    book.addOrder(2, 100, 10, Side::SELL);

    SUCCEED();
}

TEST(OrderBookTests, OrderMatching)
{
    OrderBook book;
    book.addOrder(1, 100, 100, Side::SELL);

    ASSERT_FALSE(book.isAskEmpty(100));
    ASSERT_EQ(book.getAskCount(100), 1); // should have 1 order

    book.addOrder(2, 100, 50, Side::BUY);

    ASSERT_FALSE(book.isAskEmpty(100));
    ASSERT_TRUE(book.isBidEmpty(100)); // buy order should be gone

    book.addOrder(3, 100, 50, Side::BUY);
    ASSERT_TRUE(book.isAskEmpty(100));
}

TEST(OrderBookTests, MarketSweep)
{
    OrderBook book;
    // create market(provide liquidity)
    book.addOrder(1, 100, 10, Side::SELL);
    book.addOrder(2, 101, 20, Side::SELL);
    book.addOrder(2, 102, 30, Side::SELL);

    ASSERT_EQ(book.getAskCount(100), 1);
    ASSERT_EQ(book.getAskCount(101), 1);
    ASSERT_EQ(book.getAskCount(102), 1);

    // the whale (eats up the sell orders)
    book.addOrder(4, 105, 100, Side::BUY);

    ASSERT_TRUE(book.isAskEmpty(100));
    ASSERT_TRUE(book.isAskEmpty(101));
    ASSERT_TRUE(book.isAskEmpty(102));

    ASSERT_FALSE(book.isBidEmpty(105)); // bid exists

    // ASSERT_EQ(book.getBidCount(105), 1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
