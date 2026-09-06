#include <string>
#include <vector>
#include <iostream>
using namespace std;

// ---------------------------------------------------------------------------
// BookingService
// Knows   : the running list of all Bookings ever created (AGGREGATION --
//           Bookings are meant to persist/be looked up after this call
//           returns, not die with a single service invocation)
// Does    : the ONE orchestrator -- runs "validate seats -> create Booking ->
//           take payment -> confirm/fail -> print ticket" end to end (F4,F6)
// Must NOT: calculate price itself, print tickets itself, or know about any
//           *concrete* payment class -- it only ever holds a Payment*
//           (Dependency Inversion), so adding NetBanking means adding one
//           new class and NOT touching this file (Open/Closed Principle)
// ---------------------------------------------------------------------------
class BookingService {
private:
    PriceCalculator priceCalculator;
    TicketPrinter ticketPrinter;
    vector<Booking *> bookings;

public:
    // F4 - book one or more seats for a show; reject an already-booked seat
    // and change NOTHING if any requested seat is unavailable or invalid.
    Booking *bookSeats(Show *show, vector<string> seatNumbers, Customer *customer) {
        vector<ShowSeat *> chosen;

        for (size_t i = 0; i < seatNumbers.size(); i++) {
            ShowSeat *showSeat = show->findShowSeat(seatNumbers[i]);
            if (showSeat == nullptr) {
                cout << "  Error: seat " << seatNumbers[i] << " does not exist on this show." << endl;
                return nullptr;
            }
            if (!showSeat->isAvailable()) {
                cout << "  Error: seat " << seatNumbers[i] << " is already BOOKED. Booking rejected." << endl;
                return nullptr;   // whole booking rejected, no state changed
            }
            chosen.push_back(showSeat);
        }

        Booking *booking = new Booking(show, chosen, customer);
        bookings.push_back(booking);
        return booking;
    }

    // F6 - pay by UPI/Card/Cash; a failed payment must NOT confirm the booking
    bool makePayment(Booking *booking, Payment *payment) {
        double total = priceCalculator.calculateTotal(booking->getBookedSeats());

        cout << endl;
        for (size_t i = 0; i < booking->getBookedSeats().size(); i++) {
            ShowSeat *s = booking->getBookedSeats()[i];
            cout << "  " << s->getSeat()->getSeatNumber() << " "
                 << seatTypeToString(s->getSeat()->getType()) << "  Rs."
                 << priceCalculator.priceFor(s->getSeat()->getType()) << endl;
        }
        cout << "  TOTAL\t\tRs." << total << endl << endl;

        bool success = payment->pay(total);
        if (success) {
            booking->confirm(total);
            cout << "  [" << payment->getMethodName() << "] Rs." << total << " paid successfully" << endl;
            ticketPrinter.printTicket(booking);
        } else {
            booking->fail();
            cout << "  Payment FAILED. Booking NOT confirmed; seats remain AVAILABLE." << endl;
        }
        return success;
    }

    // F8 - cancel an existing booking by id
    bool cancelBooking(const string &bookingId) {
        Booking *booking = findBooking(bookingId);
        if (booking == nullptr) {
            cout << "  Error: no booking found with id " << bookingId << endl;
            return false;
        }
        if (booking->cancel()) {
            cout << "  Booking " << bookingId << " cancelled. Seats are now AVAILABLE." << endl;
            return true;
        }
        cout << "  Error: booking " << bookingId << " cannot be cancelled (status: "
             << booking->getStatus() << ")." << endl;
        return false;
    }

    Booking *findBooking(const string &bookingId) {
        for (size_t i = 0; i < bookings.size(); i++) {
            if (bookings[i]->getBookingId() == bookingId) return bookings[i];
        }
        return nullptr;
    }

    // F.. "My tickets" - list every booking made this session, for the customer
    void listBookingsFor(Customer *customer) {
        bool any = false;
        for (size_t i = 0; i < bookings.size(); i++) {
            if (bookings[i]->getCustomer() == customer) {
                any = true;
                cout << "  " << bookings[i]->getBookingId() << "  "
                     << bookings[i]->getShow()->getMovie()->getTitle()
                     << "  Status: " << bookings[i]->getStatus() << endl;
            }
        }
        if (!any) cout << "  No bookings yet." << endl;
    }
};
