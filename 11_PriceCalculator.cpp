#include <vector>
using namespace std;

// ---------------------------------------------------------------------------
// PriceCalculator
// Knows   : the fixed price per seat type (constants, not magic numbers)
// Does    : turns a list of ShowSeats into a total amount (F5)
// Must NOT: know about Booking, Payment, or printing -- purely arithmetic
// ---------------------------------------------------------------------------
class PriceCalculator {
public:
    static const int SILVER_PRICE = 150;
    static const int GOLD_PRICE = 250;
    static const int PLATINUM_PRICE = 400;

    double priceFor(SeatType type) const {
        if (type == SILVER) return SILVER_PRICE;
        if (type == GOLD) return GOLD_PRICE;
        return PLATINUM_PRICE;
    }

    double calculateTotal(vector<ShowSeat *> &seats) const {
        double total = 0;
        for (size_t i = 0; i < seats.size(); i++) {
            total += priceFor(seats[i]->getSeat()->getType());
        }
        return total;
    }
};
