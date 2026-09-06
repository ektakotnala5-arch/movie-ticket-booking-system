#include <string>
using namespace std;

// SeatType is shared by Seat, ShowSeat and PriceCalculator.
enum SeatType { SILVER, GOLD, PLATINUM };

string seatTypeToString(SeatType type) {
    if (type == SILVER) return "SILVER";
    if (type == GOLD) return "GOLD";
    return "PLATINUM";
}

// ---------------------------------------------------------------------------
// Seat
// Knows   : its physical seat number and its type (SILVER/GOLD/PLATINUM)
// Does    : exposes read-only access to that data
// Must NOT: know whether it is booked for any particular show
//           (that is ShowSeat's job -- a Seat is a physical chair, not a
//            booking record)
// ---------------------------------------------------------------------------
class Seat {
private:
    string seatNumber;
    SeatType type;

public:
    Seat(string seatNumber, SeatType type)
        : seatNumber(seatNumber), type(type) {}

    string getSeatNumber() const { return seatNumber; }
    SeatType getType() const { return type; }
};
