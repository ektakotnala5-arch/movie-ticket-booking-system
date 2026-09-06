#include <string>
#include <vector>
#include <iostream>
using namespace std;

// ---------------------------------------------------------------------------
// Show
// Knows   : which Movie is playing (aggregation), on which Screen
//           (aggregation), at what start time, and owns one ShowSeat per
//           physical seat on that screen (composition)
// Does    : builds its ShowSeat list from the Screen's seats; prints the seat
//           layout (F3); finds a ShowSeat by seat number for booking
// Must NOT: know about Bookings, Payments or pricing -- a Show only tracks
//           seat availability, not who booked what or how much they paid
// ---------------------------------------------------------------------------
class Show {
private:
    static int nextShowId;
    int showId;
    Movie *movie;                 // AGGREGATION: Show borrows a Movie
    Screen *screen;                // AGGREGATION: Show borrows a Screen
    string startTime;
    vector<ShowSeat> showSeats;    // COMPOSITION: created here, die with Show

public:
    Show(Movie *movie, Screen *screen, string startTime)
        : movie(movie), screen(screen), startTime(startTime) {
        showId = nextShowId++;
        // Build one ShowSeat per physical seat on the screen, all AVAILABLE
        vector<Seat> &seats = screen->getSeats();
        for (size_t i = 0; i < seats.size(); i++) {
            showSeats.push_back(ShowSeat(&seats[i]));
        }
    }

    Movie *getMovie() const { return movie; }
    Screen *getScreen() const { return screen; }
    string getStartTime() const { return startTime; }
    int getShowId() const { return showId; }
    vector<ShowSeat> &getShowSeats() { return showSeats; }

    // Returns nullptr if seatNumber does not exist on this show
    ShowSeat *findShowSeat(const string &seatNumber) {
        for (size_t i = 0; i < showSeats.size(); i++) {
            if (showSeats[i].getSeat()->getSeatNumber() == seatNumber) {
                return &showSeats[i];
            }
        }
        return nullptr;
    }

    // F3 - display the seat layout with AVAILABLE / BOOKED status,
    // grouped by seat type exactly like the expected demo output.
    void printSeatLayout() {
        cout << endl << "  SCREEN-" << screen->getScreenNumber() << "  "
             << startTime << "  |  " << movie->getTitle() << endl;

        SeatType typesInOrder[3] = {SILVER, GOLD, PLATINUM};
        for (int t = 0; t < 3; t++) {
            SeatType type = typesInOrder[t];
            bool any = false;
            for (size_t i = 0; i < showSeats.size(); i++) {
                if (showSeats[i].getSeat()->getType() == type) any = true;
            }
            if (!any) continue;

            cout << "  " << seatTypeToString(type);
            for (size_t i = 0; i < showSeats.size(); i++) {
                if (showSeats[i].getSeat()->getType() != type) continue;
                string mark = showSeats[i].isAvailable() ? " " : "X";
                cout << "\t" << showSeats[i].getSeat()->getSeatNumber()
                     << "[" << mark << "]";
            }
            cout << endl;
        }
        cout << "  ( [ ] = available   [X] = booked )" << endl;
    }
};

int Show::nextShowId = 1;
