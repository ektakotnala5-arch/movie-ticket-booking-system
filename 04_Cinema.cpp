#include <string>
#include <vector>
#include <iostream>
using namespace std;

// ---------------------------------------------------------------------------
// Cinema
// Knows   : its name, the Screens it owns, and the Movies it plays
// Does    : owns/creates its Screens (composition); lists movies (F1)
// Must NOT: manage bookings, payments, ticket printing, or shows directly --
//           Show objects are held by main()/BookingService, not by Cinema,
//           keeping Cinema a simple catalog class with ONE responsibility.
// ---------------------------------------------------------------------------
class Cinema {
private:
    string name;
    vector<Screen> screens;   // COMPOSITION: screens belong only to this cinema
    vector<Movie> movies;

public:
    Cinema(string name) : name(name) {}

    void addScreen(const Screen &screen) { screens.push_back(screen); }
    void addMovie(const Movie &movie) { movies.push_back(movie); }

    string getName() const { return name; }
    vector<Screen> &getScreens() { return screens; }
    vector<Movie> &getMovies() { return movies; }

    // F1 - list all movies currently playing
    void listMovies() const {
        cout << endl;
        for (size_t i = 0; i < movies.size(); i++) {
            cout << "  [" << (i + 1) << "] " << movies[i].getTitle()
                 << "\t" << movies[i].getLanguage()
                 << "\t" << movies[i].getDurationMin() << " min" << endl;
        }
    }
};
