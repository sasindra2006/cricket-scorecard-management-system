#include "cricket.h"

int main() {
    cout << "=========================================\n";
    cout << "  REAL-TIME CRICKET SCORING (Console)    \n";
    cout << "=========================================\n\n";
    try {
        Match match;
        match.startMatch();
    } catch (const exception &e) {
        cerr << "\nUnexpected error occurred: " << e.what() << "\n";
    } catch (...) {
        cerr << "\nUnknown error occurred.\n";
    }
    cout << "\n=========================================\n";
    cout << "        Thank you for using!     \n";
    cout << "=========================================\n";
    return 0;
}


