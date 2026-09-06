// ---------------------------------------------------------------------------
// Payment (ABSTRACT)
// Knows   : nothing about seats or bookings -- only the payment contract
// Does    : declares pay(amount) as pure virtual (ABSTRACTION); provides
//           getMethodName() so a receipt can print which method was used
// Must NOT: know about Booking, Show, or Customer -- keeping Payment generic
//           is what lets NetBanking be added later with zero changes here
//           (Open/Closed Principle)
// ---------------------------------------------------------------------------
class Payment {
public:
    // Runtime polymorphism: BookingService calls pay() through a Payment*
    // and the correct override (UPI/Card/Cash) runs automatically.
    virtual bool pay(double amount) = 0;
    virtual string getMethodName() = 0;
    virtual ~Payment() {}
};
