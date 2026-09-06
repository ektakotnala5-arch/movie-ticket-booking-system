#include <string>
using namespace std;

// ---------------------------------------------------------------------------
// Movie
// Knows   : its own title, language and duration only
// Does    : exposes read-only access to that data
// Must NOT: know about screens, shows, seats, or pricing
// ---------------------------------------------------------------------------
class Movie {
private:
    string title;
    string language;
    int durationMin;

public:
    // Compile-time polymorphism: an overloaded constructor with a default
    // duration, alongside the fully-specified one below.
    Movie(string title, string language)
        : title(title), language(language), durationMin(120) {}

    Movie(string title, string language, int durationMin)
        : title(title), language(language), durationMin(durationMin) {}

    string getTitle() const { return title; }
    string getLanguage() const { return language; }
    int getDurationMin() const { return durationMin; }
};
