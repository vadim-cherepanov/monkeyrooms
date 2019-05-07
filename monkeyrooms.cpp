#include "monkeyrooms.h"

#include <deque>

#define OFFSET(x) ((x) >> 5u)
#define VALUE(x) (1u << ((x) & 31u))

/////// Different update rules ///////

inline TROOMSINFO updateRoomsInfoLinear(TROOMSINFO iRoomsInfo, TROOMSINFO iMask) {
    return ((iRoomsInfo << (TROOMSINFO)1u) | (iRoomsInfo >> (TROOMSINFO)1u)) & iMask;
}

inline TROOMSINFO updateRoomsInfoCircular(TROOMSINFO iRoomsInfo, TROOMSINFO iMask, TROOMSINFO iRoomsNumber_1) {
    return ((iRoomsInfo << (TROOMSINFO)1u) | (iRoomsInfo >> (TROOMSINFO)1u)
            | ((iRoomsInfo & (TROOMSINFO)1u) << iRoomsNumber_1)
            | ((iRoomsInfo >> iRoomsNumber_1) & (TROOMSINFO)1u)) & iMask;
}

/////// Class methods and helper functions ///////

void MonkeyRooms::addPossibleMoves_(TROOMSINFO iMove, TROOMSINFO iFrom, uint32_t iChecksNumber) {
    if ((iFrom << (TROOMSINFO)iChecksNumber) > iRoomsInfoNumber_) return;
    if (!iChecksNumber) {
        vPossibleMoves_.push_back(iMove); return;
    }
    else {
        for (TROOMSINFO b = iFrom; b < iRoomsInfoNumber_; b <<= (TROOMSINFO)1u) {
            addPossibleMoves_(iMove | b, b << (TROOMSINFO) 1u, iChecksNumber - (TROOMSINFO) 1u);
        }
    }
}

inline void setLevel(TROOMSINFO iRoomsInfo, uint32_t iLevel, vector<uint32_t> &vFound,
                     vector<vector<TROOMSINFO>> &vvRoomsInfoByLevel) {
    vFound[OFFSET(iRoomsInfo)] |= VALUE(iRoomsInfo);
    if (iLevel >= vvRoomsInfoByLevel.size()) vvRoomsInfoByLevel.resize(vvRoomsInfoByLevel.size() * 2u);
    vvRoomsInfoByLevel[iLevel].push_back(iRoomsInfo);
}

/////// Different cases for Solve_() for efficiency ( never do that in production code :) ) ///////

#define solveCheckMove(iRoomsInfo, updateRoomsInfo, ...) \
    TROOMSINFO iRoomsInfoAfter = updateRoomsInfo((iRoomsInfo), __VA_ARGS__); \
    if (!(vFound[OFFSET(iRoomsInfoAfter)] & VALUE(iRoomsInfoAfter))) { \
        setLevel(iRoomsInfoAfter, iLevel, vFound, vvRoomsInfoByLevel); \
        if (__builtin_popcountl(iRoomsInfoAfter) <= (int)iChecksNumber_ && iLevelIncrease >= 0) { \
            qQueue[iLevelIncrease] = iRoomsInfoAfter; \
        } \
        else { \
            if (iLevelIncrease < 0) iLevelIncrease = qQueue.size(); \
            qQueue.push_back(iRoomsInfoAfter); \
        } \
    }

#define solveCycleInner1(updateRoomsInfo, ...) \
    TROOMSINFO iRoomsInfoTopBits = iRoomsInfo; \
    while (iRoomsInfoTopBits) { \
        TROOMSINFO iRoomsInfo1 = iRoomsInfo ^ (iRoomsInfoTopBits & -iRoomsInfoTopBits); \
        iRoomsInfoTopBits &= iRoomsInfo1; \
        solveCheckMove(iRoomsInfo1, updateRoomsInfo, __VA_ARGS__) \
    }

#define solveCycleInner2(updateRoomsInfo, ...) \
    TROOMSINFO aRoomsInfo[iRoomsNumber_]; \
    TROOMSINFO *pRoomsInfoEnd = aRoomsInfo; \
    TROOMSINFO iRoomsInfoTopBits = iRoomsInfo; \
    while (iRoomsInfoTopBits) { \
        TROOMSINFO iRoomsInfo1 = iRoomsInfo ^ (iRoomsInfoTopBits & -iRoomsInfoTopBits); \
        iRoomsInfoTopBits &= iRoomsInfo1; \
        *pRoomsInfoEnd++ = iRoomsInfo1; \
        for (TROOMSINFO *pRoomsInfo = aRoomsInfo; pRoomsInfo != pRoomsInfoEnd; ++pRoomsInfo) { \
            solveCheckMove(iRoomsInfo1 & *pRoomsInfo, updateRoomsInfo, __VA_ARGS__) \
        } \
    }

#define solveCycleInnerOther(updateRoomsInfo, ...) \
    const TROOMSINFO *pPossibleMoves_ = &vPossibleMoves_[0]; \
    const TROOMSINFO *pPossibleMovesEnd_ = &*vPossibleMoves_.end(); \
    for (const TROOMSINFO *pMove = pPossibleMoves_; pMove != pPossibleMovesEnd_; ++pMove) { \
        TROOMSINFO iMove = *pMove; \
        if (iMove & ~iRoomsInfo) continue; \
        solveCheckMove(iRoomsInfo & ~iMove, updateRoomsInfo, __VA_ARGS__) \
    }

#define solveCycle(solveCycleInner) \
    deque<TROOMSINFO> qQueue {iRoomsInfoRoot}; \
    uint32_t iLevel = 0u; \
    int iLevelIncrease = 0; \
    while (!qQueue.empty()) { \
        if (!iLevelIncrease--) ++iLevel; \
        TROOMSINFO iRoomsInfo = qQueue.front(); \
        qQueue.pop_front(); \
        if (__builtin_popcountl(iRoomsInfo) <= (int)iChecksNumber_) { \
            iLevel0 = iLevel; \
            goto TADA; \
        } \
        solveCycleInner \
    }

void MonkeyRooms::Solve_() {
    vector<uint32_t> vFound(OFFSET(iRoomsInfoNumber_ - (TROOMSINFO)1u) + 1u, 0u);
    vector<vector<TROOMSINFO>> vvRoomsInfoByLevel(1u);
    TROOMSINFO iRoomsInfoRoot = iRoomsInfoNumber_ - (TROOMSINFO)1u;
    setLevel(iRoomsInfoRoot, 0u, vFound, vvRoomsInfoByLevel);
    uint32_t iLevel0 = -1u;
    if (!iRoomsNumber_) {
        iLevel0 = 0u;
    }
    else {
        const TROOMSINFO iRoomsNumber_1 = iRoomsNumber_ - (TROOMSINFO)1u;
        if (iRoomsNumber_ <= iChecksNumber_) {
            iLevel0 = 1u;
        }
        else if (iChecksNumber_ == 1u) {
            if (!bCircular_) { solveCycle(solveCycleInner1(updateRoomsInfoLinear, iRoomsInfoRoot)) }
            else { solveCycle(solveCycleInner1(updateRoomsInfoCircular, iRoomsInfoRoot, iRoomsNumber_1)) }
        }
        else if (iChecksNumber_ == 2u) {
            if (!bCircular_) { solveCycle(solveCycleInner2(updateRoomsInfoLinear, iRoomsInfoRoot)) }
            else { solveCycle(solveCycleInner2(updateRoomsInfoCircular, iRoomsInfoRoot, iRoomsNumber_1)) }
        }
        else {
            if (!bCircular_) { solveCycle(solveCycleInnerOther(updateRoomsInfoLinear, iRoomsInfoRoot)) }
            else { solveCycle(solveCycleInnerOther(updateRoomsInfoCircular, iRoomsInfoRoot, iRoomsNumber_1)) }
        }
        TADA: if (iLevel0 != -1u) setLevel((TROOMSINFO)0u, iLevel0, vFound, vvRoomsInfoByLevel);
    }
    mvSolutions_.insert({iRoomsInfoRoot, vector<ParentInfo> {}});
    constructSolutions_((TROOMSINFO)0u, iLevel0, vPossibleMoves_, vvRoomsInfoByLevel);
}

#define constructSolutionsCycle(updateRoomsInfo, ...) \
    deque<TROOMSINFO> qQueue {iRoomsInfo}; \
    int iLevelDecrease = 0; \
    while (!qQueue.empty()) { \
        if (!iLevelDecrease--) --iLevel; \
        iRoomsInfo = qQueue.front(); \
        qQueue.pop_front(); \
        if (mvSolutions_.count(iRoomsInfo)) continue; \
        mvSolutions_.insert({iRoomsInfo, vector<ParentInfo> {}}); \
        if (iLevel >= vvRoomsInfoByLevel.size()) return; \
        const TROOMSINFO iMask = iRoomsInfoNumber_ - (TROOMSINFO)1u; \
        vector<ParentInfo> &vParents = mvSolutions_.at(iRoomsInfo); \
        for (const auto &iParent : vvRoomsInfoByLevel.at(iLevel)) { \
            TROOMSINFO iExtraBitsRoomsInfo = updateRoomsInfo(iParent, iMask, ##__VA_ARGS__) ^ iRoomsInfo; \
            if (iExtraBitsRoomsInfo & iRoomsInfo) continue; \
            TROOMSINFO iExtraBitsParent = updateRoomsInfo(iExtraBitsRoomsInfo, iParent, ##__VA_ARGS__); \
            if (__builtin_popcountl(iExtraBitsParent) > (int)iChecksNumber_ \
                    || (~updateRoomsInfo(iParent & ~iExtraBitsParent, iMask, ##__VA_ARGS__) & iRoomsInfo)) continue; \
            for (const auto b : vPossibleMoves) { \
                if (!(iExtraBitsParent & ~b) && updateRoomsInfo(iParent & ~b, iMask, ##__VA_ARGS__) == iRoomsInfo) { \
                    vParents.push_back({ .iParent = iParent, .iMove = b }); \
                } \
            } \
        } \
        for (const auto &sParent : vParents) { \
            constructSolutions_(sParent.iParent, iLevel, vPossibleMoves, vvRoomsInfoByLevel); \
        } \
    }

void MonkeyRooms::constructSolutions_(TROOMSINFO iRoomsInfo, uint32_t iLevel,
        const vector<TROOMSINFO> &vPossibleMoves, const vector<vector<TROOMSINFO>> &vvRoomsInfoByLevel) {
    const TROOMSINFO iRoomsNumber_1 = iRoomsNumber_ - (TROOMSINFO)1u; \
    if (!bCircular_) { constructSolutionsCycle(updateRoomsInfoLinear) }
    else { constructSolutionsCycle(updateRoomsInfoCircular, iRoomsNumber_1) }
}

uint32_t MonkeyRooms::getSolutionsNumber_(TROOMSINFO iRoomsInfo) const {
    const vector<ParentInfo> &vParents = mvSolutions_.at(iRoomsInfo);
    if (vParents.empty()) return (iRoomsInfo == iRoomsInfoNumber_ - (TROOMSINFO)1u ? 1u : 0u);
    uint32_t iCount = 0u;
    for (auto sParent : vParents) iCount += getSolutionsNumber_(sParent.iParent);
    return iCount;
}

inline string PrintSetBits(TROOMSINFO b) {
    string res;
    for (uint32_t i = 1u; b; ++i, b >>= 1u) if (b & (TROOMSINFO)1u) res += to_string(i) + ",";
    return !res.length() ? "-" : res.substr(0u, res.length() - 1u);
}

uint32_t MonkeyRooms::printAllSolutions_(TROOMSINFO iRoomsInfo, uint32_t iSolutionNumber,
                                             const string &sSuffix, bool bNewLine) const {
    const vector<ParentInfo> &vParents = mvSolutions_.at(iRoomsInfo);
    if (vParents.empty()) {
        if (iRoomsInfo == iRoomsInfoNumber_ - (TROOMSINFO)1u) cout << "[" << iSolutionNumber << "] " << sSuffix << " ";
        else cout << "-";
        ++iSolutionNumber;
    }
    else {
        for (auto sParent : vParents) iSolutionNumber = printAllSolutions_(sParent.iParent, iSolutionNumber,
                                                                PrintSetBits(sParent.iMove) + " " + sSuffix, false);
    }
    if (bNewLine) cout << endl;
    return iSolutionNumber;
}

uint32_t MonkeyRooms::getChecksNumber() const {
    uint32_t iLevel = 0u;
    auto iRoomsInfo = (TROOMSINFO)0u;
    while (true) {
        const vector<ParentInfo> &vParents = mvSolutions_.at(iRoomsInfo);
        if (vParents.empty()) break;
        iRoomsInfo = vParents[(TROOMSINFO)0u].iParent;
        ++iLevel;
    }
    return iRoomsInfo == iRoomsInfoNumber_ - (TROOMSINFO)1u ? iLevel : -1u;
}
