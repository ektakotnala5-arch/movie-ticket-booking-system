#include <iostream>
#include <string>
using namespace std;

// Small shared helper so all three payment types confirm the same way
// without duplicating the prompt-and-read logic (DRY).
bool confirmWithUser(const string &methodLabel, double amount) {
    cout << "  [" << methodLabel << "] Rs." << amount
         << " -- confirm payment? (y/n): ";
    string input;
    cin >> input;
    return (input == "y" || input == "Y");
}

// ---------------------------------------------------------------------------
// UpiPayment / CardPayment / CashPayment (INHERITANCE from Payment)
// Knows   : nothing extra beyond Payment
// Does    : implements pay() its own way; a real system would call a UPI
//           gateway, a card processor, or simply accept cash on the spot --
//           here each is simulated with a user confirmation so both the
//           success path and the "failed payment" edge case (F6) are
//           demonstrable in the console demo
// Must NOT: be referenced anywhere by concrete type inside BookingService --
//           BookingService only ever holds a Payment* (Dependency Inversion)
// ---------------------------------------------------------------------------
class UpiPayment : public Payment {
public:
    bool pay(double amount) override { return confirmWithUser("UPI", amount); }
    string getMethodName() override { return "UPI"; }
};

class CardPayment : public Payment {
public:
    bool pay(double amount) override { return confirmWithUser("Card", amount); }
    string getMethodName() override { return "Card"; }
};

class CashPayment : public Payment {
public:
    // Cash is handed over in person, so it never "fails" the way an
    // online gateway can -- this asymmetry is intentional, not an oversight.
    bool pay(double amount) override {
        cout << "  [Cash] Rs." << amount << " received at counter." << endl;
        return true;
    }
    string getMethodName() override { return "Cash"; }
};
