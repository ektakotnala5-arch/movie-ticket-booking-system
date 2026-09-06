#include <string>
using namespace std;

// ---------------------------------------------------------------------------
// Customer
// Knows   : name and phone number, nothing else
// Does    : exposes read-only access to that data
// Must NOT: hold bookings itself, call BookingService, or know about seats/
//           payments -- Customer is passive data, BookingService drives the
//           flow (ASSOCIATION: Customer <-> BookingService, neither owns
//           the other's lifetime)
// ---------------------------------------------------------------------------
class Customer {
private:
    string name;
    string phone;

public:
    Customer(string name, string phone) : name(name), phone(phone) {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
};
