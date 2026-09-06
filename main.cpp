#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Course rule: "no header files, one class per file" -- so each class file
// is pulled into this single translation unit with #include, in dependency
// order (a class must be #included before anything that uses it).
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "03_Screen.cpp"
#include "04_Cinema.cpp"
#include "06_ShowSeat.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "08_Booking.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
#include "13_BookingService.cpp"

// Splits "A1,B2" into {"A1","B2"} -- a small helper so main() stays readable.
vector<string> splitSeats(const string &line) {
    vector<string> result;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ',')) {
        while (!token.empty() && token.front() == ' ') token.erase(token.begin());
        while (!token.empty() && token.back() == ' ') token.pop_back();
        if (!token.empty()) result.push_back(token);
    }
    return result;
}

Screen buildScreen1() {
    Screen screen(1);
    screen.addSeat(Seat("A1", SILVER));
    screen.addSeat(Seat("A2", SILVER));
    screen.addSeat(Seat("A3", SILVER));
    screen.addSeat(Seat("A4", SILVER));
    screen.addSeat(Seat("B1", GOLD));
    screen.addSeat(Seat("B2", GOLD));
    screen.addSeat(Seat("B3", GOLD));
    screen.addSeat(Seat("C1", PLATINUM));
    screen.addSeat(Seat("C2", PLATINUM));
    return screen;
}

Screen buildScreen2() {
    Screen screen(2);
    screen.addSeat(Seat("A1", SILVER));
    screen.addSeat(Seat("A2", SILVER));
    screen.addSeat(Seat("B1", GOLD));
    screen.addSeat(Seat("B2", GOLD));
    screen.addSeat(Seat("C1", PLATINUM));
    return screen;
}

int main() {
    // ---- seed data -------------------------------------------------------
    Cinema cinema("PVR Dehradun");

    Movie m1("Bleach", "Japanese", 105);
    Movie m2("Haikyuu!!", "Japanese", 85);
    Movie m3("Project Hail Mary", "English", 132);
    Movie m4("The Thing", "English", 109);
    cinema.addMovie(m1);
    cinema.addMovie(m2);
    cinema.addMovie(m3);
    cinema.addMovie(m4);

    Screen s1 = buildScreen1();
    Screen s2 = buildScreen2();
    cinema.addScreen(s1);
    cinema.addScreen(s2);

    vector<Movie> &movies = cinema.getMovies();
    vector<Screen> &screens = cinema.getScreens();

    vector<Show> shows;
    shows.push_back(Show(&movies[0], &screens[0], "06:00 PM"));  // Show 0: Bleach, Screen-1
    shows.push_back(Show(&movies[1], &screens[1], "06:30 PM"));  // Show 1: Haikyuu!!, Screen-2
    shows.push_back(Show(&movies[2], &screens[0], "09:00 PM"));  // Show 2: Project Hail Mary, Screen-1
    shows.push_back(Show(&movies[3], &screens[1], "09:30 PM"));  // Show 3: The Thing, Screen-2

    BookingService bookingService;
    Customer customer("Guest", "9999999999");

    cout << "===== MOVIE TICKET BOOKING =====" << endl;

    bool running = true;
    while (running) {
        cout << endl << "1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit" << endl;
        cout << "Choose: ";
        string choiceStr;
        if (!(cin >> choiceStr)) break;

        if (choiceStr == "1") {
            cinema.listMovies();

        } else if (choiceStr == "2") {
            cinema.listMovies();
            cout << endl << "Choose movie: ";
            int movieChoice;
            if (!(cin >> movieChoice) || movieChoice < 1 || (size_t)movieChoice > movies.size()) {
                cout << "  Invalid choice." << endl;
                cin.clear(); cin.ignore(1000, '\n');
                continue;
            }
            Movie *chosenMovie = &movies[movieChoice - 1];

            // F2 - list shows for the chosen movie
            vector<Show *> matchingShows;
            for (size_t i = 0; i < shows.size(); i++) {
                if (shows[i].getMovie() == chosenMovie) matchingShows.push_back(&shows[i]);
            }
            if (matchingShows.empty()) {
                cout << "  No shows currently scheduled for this movie." << endl;
                continue;
            }
            for (size_t i = 0; i < matchingShows.size(); i++) {
                cout << "  [" << (i + 1) << "] Screen-" << matchingShows[i]->getScreen()->getScreenNumber()
                     << "\t" << matchingShows[i]->getStartTime() << endl;
            }
            cout << "Choose show: ";
            int showChoice;
            if (!(cin >> showChoice) || showChoice < 1 || (size_t)showChoice > matchingShows.size()) {
                cout << "  Invalid choice." << endl;
                cin.clear(); cin.ignore(1000, '\n');
                continue;
            }
            Show *chosenShow = matchingShows[showChoice - 1];

            // F3 - seat layout
            chosenShow->printSeatLayout();

            cout << endl << "Seats (e.g. A1,B2): ";
            cin.ignore();
            string seatLine;
            getline(cin, seatLine);
            vector<string> seatNumbers = splitSeats(seatLine);
            if (seatNumbers.empty()) {
                cout << "  No seats entered." << endl;
                continue;
            }

            // F4 - book seats (rejects if any is already booked or invalid)
            Booking *booking = bookingService.bookSeats(chosenShow, seatNumbers, &customer);
            if (booking == nullptr) continue;

            // F6 - payment
            cout << "Pay by: 1.UPI  2.Card  3.Cash > ";
            int payChoice;
            if (!(cin >> payChoice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

            Payment *payment = nullptr;
            if (payChoice == 1) payment = new UpiPayment();
            else if (payChoice == 2) payment = new CardPayment();
            else if (payChoice == 3) payment = new CashPayment();
            else {
                cout << "  Invalid payment choice. Booking left FAILED." << endl;
                booking->fail();
                continue;
            }

            bookingService.makePayment(booking, payment);   // F5 (pricing) + F7 (ticket) happen inside
            delete payment;

        } else if (choiceStr == "3") {
            cout << "Enter Booking ID to cancel: ";
            string bookingId;
            cin >> bookingId;
            bookingService.cancelBooking(bookingId);        // F8

        } else if (choiceStr == "4") {
            bookingService.listBookingsFor(&customer);

        } else if (choiceStr == "0") {
            running = false;
            cout << "Thank you for visiting " << cinema.getName() << "!" << endl;

        } else {
            cout << "  Invalid menu choice. Please choose 0-4." << endl;
        }
    }

    return 0;
}
