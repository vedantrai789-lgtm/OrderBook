#include "OrderBook.h"

int main()
{
    OrderBook book;
    // 1. Add a Seller (Ask): 100 shares @ $100
    std::cout << "Adding Ask: 100 @ 100\n";
    book.addOrder(1, 100.00, 100, Side::SELL);
    book.printOrder();

    // 2. Add an Aggressive Buyer: 50 shares @ $101
    // Matches because $101 (willing to pay) >= $100 (asking price)
    std::cout << "\nAdding Aggressive Buy: 50 @ 101\n";
    book.addOrder(2, 101.00, 50, Side::BUY);
    book.printOrder();

    // 3. Add another Aggressive Buyer: 60 shares @ $100
    // Matches remaining 50 shares @ 100. 10 shares left over -> goes to book.
    std::cout << "\nAdding Aggressive Buy: 60 @ 100\n";
    book.addOrder(3, 100.00, 60, Side::BUY);
    book.printOrder();

    return 0;
}