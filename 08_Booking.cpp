#include <string>
#include <vector>
using namespace std;

// ---------------------------------------------------------------------------
// Booking
// Knows   : a unique booking id, which Show, which ShowSeats, total amount,
//           status ("PENDING"/"CONFIRMED"/"FAILED"/"CANCELLED"), and which
//           Customer it belongs to
// Does    : transitions its own status via confirm()/fail()/cancel(); the
//           static counter generates unique ids (Static Member + this)
// Must NOT: print itself (TicketPrinter's job -- Single Responsibility),
//           calculate its own price (PriceCalculator's job), or talk to
//           Payment directly (BookingService orchestrates that)
// ---------------------------------------------------------------------------
class Booking {
private:
    static int nextBookingId;          // STATIC MEMBER: shared by all Bookings
    string bookingId;
    Show *show;                         // ASSOCIATION: Booking uses a Show,
                                         // does not own its lifetime
    vector<ShowSeat *> bookedSeats;     // AGGREGATION: borrowed ShowSeats --
                                         // they keep existing after cancellation
    Customer *customer;                 // ASSOCIATION
    double totalAmount;
    string status;

public:
    Booking(Show *show, vector<ShowSeat *> bookedSeats, Customer *customer)
        : show(show), bookedSeats(bookedSeats), customer(customer),
          totalAmount(0.0), status("PENDING") {
        // `this` used explicitly here to disambiguate from the static counter
        this->bookingId = "BK" + to_string(nextBookingId++);
    }

    void confirm(double amount) {
        totalAmount = amount;
        status = "CONFIRMED";
        for (size_t i = 0; i < bookedSeats.size(); i++) bookedSeats[i]->book();
    }

    void fail() {
        status = "FAILED";
        // Seats were never marked BOOKED during a pending booking (see
        // BookingService::bookSeats), so nothing needs to be released here --
        // this keeps FR4's "no seat changes state" guarantee airtight.
    }

    // F8 - cancel a CONFIRMED booking; seats become AVAILABLE again
    bool cancel() {
        if (status != "CONFIRMED") return false;
        for (size_t i = 0; i < bookedSeats.size(); i++) bookedSeats[i]->release();
        status = "CANCELLED";
        return true;
    }

    string getBookingId() const { return bookingId; }
    Show *getShow() const { return show; }
    vector<ShowSeat *> &getBookedSeats() { return bookedSeats; }
    Customer *getCustomer() const { return customer; }
    double getTotalAmount() const { return totalAmount; }
    string getStatus() const { return status; }
};

int Booking::nextBookingId = 1001;
