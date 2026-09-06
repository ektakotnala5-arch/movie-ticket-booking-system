#include <iostream>
using namespace std;

// ---------------------------------------------------------------------------
// TicketPrinter
// Knows   : nothing (stateless service)
// Does    : formats and prints a ticket for a CONFIRMED Booking (F7) --
//           printing only
// Must NOT: calculate price, change booking status, or touch seats -- a
//           layout change here must never require editing Booking or
//           PriceCalculator (Single Responsibility)
// ---------------------------------------------------------------------------
class TicketPrinter {
public:
    void printTicket(Booking *booking) {
        cout << endl << "  ================ TICKET ================" << endl;
        cout << "  Booking ID : " << booking->getBookingId() << endl;
        cout << "  Movie      : " << booking->getShow()->getMovie()->getTitle() << endl;
        cout << "  Screen     : Screen-" << booking->getShow()->getScreen()->getScreenNumber()
             << "   " << booking->getShow()->getStartTime() << endl;

        cout << "  Seats      : ";
        vector<ShowSeat *> &seats = booking->getBookedSeats();
        for (size_t i = 0; i < seats.size(); i++) {
            cout << seats[i]->getSeat()->getSeatNumber();
            if (i + 1 < seats.size()) cout << ", ";
        }
        cout << endl;

        cout << "  Amount     : Rs." << booking->getTotalAmount()
             << "\tStatus: " << booking->getStatus() << endl;
        cout << "  =========================================" << endl;
    }
};
