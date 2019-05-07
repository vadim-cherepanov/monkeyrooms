#include <iostream>
#include <vector>

#include "monkeyrooms.h"
#include "timer.h"

using namespace std;

vector<MonkeyRooms> vpMonkeyRooms;

void printMonkeyRoomsStatistics(const MonkeyRooms &monkeyRooms, const string& sAdditionalInfo = "") {
    uint32_t iChecksNumber = monkeyRooms.getChecksNumber();
    cout << "Rooms # " << monkeyRooms.getRoomsNumber()
         << " Days # " << (iChecksNumber == -1u ? "+oo" : to_string(iChecksNumber))
         << " Solutions # " << monkeyRooms.getSolutionsNumber()
         << "\t" << sAdditionalInfo
         << endl;
}

uint32_t askNumber(const string &sMessage) {
    uint32_t iAnswer;
    cout << sMessage;
    cin >> iAnswer;
    return iAnswer;
}

bool askNY(const string &sMessage) {
    string sAnswer;
    cout << sMessage;
    cin >> sAnswer;
    return (sAnswer == "y" || sAnswer == "Y");
}

int main() {
    uint32_t iChecksNumber = askNumber("Maximum number of room checks per day: ");
    if (!iChecksNumber) return 0;
    bool bAnyMoves = iChecksNumber > 1u && !askNY("Require all checked rooms to be different? ");
    bool bCircular = askNY("Circular? ");
    uint32_t iMax = 1u;
    Timer timer;
    while (true) {
        double dStart = timer.getSeconds();
        vpMonkeyRooms.emplace_back(MonkeyRooms(iMax, bCircular, iChecksNumber, bAnyMoves));
        double dSpan = timer.getSeconds() - dStart;
        printMonkeyRoomsStatistics(vpMonkeyRooms[iMax - 1], "| " + timer.secondsToString(dSpan) + " sec ");
        if (dSpan > 3.3) break;
        ++iMax;
    }
    cout << "Total time: " << timer.getSecondsString() << " sec" << endl;
    //*
    while (true) {
        uint32_t n = askNumber("Show all solutions for Rooms # ");
        if (--n >= iMax) break;
        MonkeyRooms &monkeyRooms = vpMonkeyRooms.at(n);
        printMonkeyRoomsStatistics(monkeyRooms);
        monkeyRooms.printAllSolutions();
    }
    /**/
    return 0;
}
