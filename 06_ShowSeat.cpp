// SeatStatus is specific to a Show, not to a physical Seat.
enum SeatStatus { AVAILABLE, BOOKED };

// ---------------------------------------------------------------------------
// ShowSeat
// Why this class exists (not just reusing Seat):
//   Physical seat "A1" exists once on the Screen, but its BOOKED/AVAILABLE
//   status is different for every Show. A1 may be booked for the 6 PM show
//   and free for the 9 PM show. Status belongs to the (Show, Seat) pair,
//   not to the physical chair.
//
// Knows   : which physical Seat it represents (aggregation -- it borrows the
//           Seat, does not own/create it) and its own booking status
// Does    : reports availability; transitions status on book()/release()
// Must NOT: calculate price or know which Booking booked it -- Booking holds
//           that link, not the other way around, so ShowSeat stays reusable
// ---------------------------------------------------------------------------
class ShowSeat {
private:
    Seat *seat;             // AGGREGATION: borrowed pointer, ShowSeat does not
                             // create or destroy the underlying Seat
    SeatStatus status;

public:
    ShowSeat(Seat *seat) : seat(seat), status(AVAILABLE) {}

    bool isAvailable() const { return status == AVAILABLE; }

    void book() { status = BOOKED; }
    void release() { status = AVAILABLE; }

    Seat *getSeat() const { return seat; }
    SeatStatus getStatus() const { return status; }
};
