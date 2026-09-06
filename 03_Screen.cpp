#include <string>
#include <vector>
using namespace std;

// ---------------------------------------------------------------------------
// Screen
// Knows   : its screen number and the fixed physical Seats inside it
// Does    : owns/creates its Seats (composition), lets other classes read them
// Must NOT: know which movie is playing or what time (that is Show's job)
// ---------------------------------------------------------------------------
class Screen {
private:
    int screenNumber;
    vector<Seat> seats;   // COMPOSITION: seats are created here and die with
                           // this Screen -- they have no existence of their own

public:
    Screen(int screenNumber) : screenNumber(screenNumber) {}

    void addSeat(const Seat &seat) { seats.push_back(seat); }

    int getScreenNumber() const { return screenNumber; }
    vector<Seat> &getSeats() { return seats; }
};
