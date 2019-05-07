#ifndef MONKEYROOMS_MONKEYROOMS_H
#define MONKEYROOMS_MONKEYROOMS_H

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

static_assert(sizeof(int) == 4u, "int should be 4 bytes.");
static_assert(sizeof(long) == 8u, "long should be 8 bytes.");

typedef unsigned long TROOMSINFO;

class MonkeyRooms {
public:
    struct ParentInfo {
        TROOMSINFO iParent;
        TROOMSINFO iMove;
    };
protected:
    uint32_t iRoomsNumber_;
    bool bCircular_;
    uint32_t iChecksNumber_;
    bool bAnyMoves_;
    TROOMSINFO iRoomsInfoNumber_;
    vector<TROOMSINFO> vPossibleMoves_;
    unordered_map<TROOMSINFO, vector<ParentInfo>> mvSolutions_;

    void addPossibleMoves_(TROOMSINFO iMove, TROOMSINFO iFrom, uint32_t iChecksNumber);
    void Solve_();
    void constructSolutions_(TROOMSINFO iRoomsInfo, uint32_t iLevel,
            const vector<TROOMSINFO> &vPossibleMoves, const vector<vector<TROOMSINFO>> &vvRoomsInfoByLevel);
    uint32_t getSolutionsNumber_(TROOMSINFO iRoomsInfo) const;
    uint32_t printAllSolutions_(TROOMSINFO iRoomsInfo, uint32_t iSolutionNumber,
                                    const string &sSuffix, bool bNewLine) const;
public:
    explicit MonkeyRooms(uint32_t iRoomsNumber, bool bCircular, uint32_t iChecksNumber, bool bAnyMoves = true) :
            iRoomsNumber_(iRoomsNumber), bCircular_(bCircular), iChecksNumber_(iChecksNumber), bAnyMoves_(bAnyMoves),
            iRoomsInfoNumber_((TROOMSINFO)1u << iRoomsNumber) {
        if (bAnyMoves_) for (auto i = 1u; i < iChecksNumber_; ++i) addPossibleMoves_((TROOMSINFO)0u, (TROOMSINFO)1u, i);
        addPossibleMoves_((TROOMSINFO)0u, (TROOMSINFO)1u, iChecksNumber_);
        Solve_();
    }
    inline uint32_t getRoomsNumber() const { return iRoomsNumber_; };
    uint32_t getChecksNumber() const;
    inline uint32_t getSolutionsNumber() const { return getSolutionsNumber_((TROOMSINFO)0u); };
    inline uint32_t printAllSolutions() const { return printAllSolutions_((TROOMSINFO)0u, 1, "", true); };
};

#endif //MONKEYROOMS_MONKEYROOMS_H
