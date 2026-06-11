#ifndef CRICKET_H
#define CRICKET_H

#include <bits/stdc++.h>

using namespace std;

// utility helpers
string toUpper(const string &s);
string trim(const string &s);
bool askYesNo(const string &question);
int getValidCatchRating();
bool isValidWicketType(const string &wt);

// ========== inheritance demo ==========
class Cricketer {
protected:
    string name;
public:
    Cricketer(const string &n = "") : name(n) {}
    virtual ~Cricketer() = default;
    string getName() const { return name; }
    virtual void displayRole(ostream &os) const { (void)os; }
};

// ---------------- Player ----------------
class Player : public Cricketer {
private:
    // Batting
    int runs;
    int ballsFaced;
    int fours;
    int sixes;
    bool isOut;
    string howOut;

    // Bowling
    int ballsBowled;
    int runsConceded;
    int wickets;
    int dotBalls;
    int maidenOvers;

    // Fielding
    int catches;
    int runouts;
    int bestCatchRating;

    // Other
    bool injured;
    bool retiredHurt;
    bool hasEntered;
    double rating;
    double fantasyPoints;

public:
    Player(const string &n = "");
    // batting
    void addRuns(int r);
    void addBallFaced();
    void addFour();
    void addSix();
    void setOut(const string &how);
    bool getIsOut() const;
    string getHowOut() const;
    string getName() const; // override

    // bowling
    void addBowlingBall(int runsConcededOnBall, bool legalBall, bool wicketTaken, bool isDotBall = false);
    void addPartialBowlingBalls(int balls);
    void addBowlerDotBall();
    void addMaidenOver();

    // fielding
    void addCatch(int rating);
    void addRunout();
    void addBatsmanDotBall();

    // stats
    void computeRating(bool underPressure);
    void computeFantasyPoints(int oversPerInnings);
    void addFantasyPoints(double pts);

    // health / entry
    bool isInjured() const;
    void setInjured(bool v);
    void setRetiredHurt();
    bool isRetiredHurt() const;
    void setEntered(bool v);
    bool isEntered() const;

    // getters
    int getRuns() const;
    int getBallsFaced() const;
    int getFours() const;
    int getSixes() const;
    int getBallsBowled() const;
    int getRunsConceded() const;
    int getWickets() const;
    int getCatches() const;
    int getRunouts() const;
    int getBestCatchRating() const;
    double getRating() const;
    double getFantasyPoints() const;
    int getDotBalls() const;
    int getMaidenOvers() const;

    // display
    void displayBattingRow() const;
    void displayBowlingRow() const;

    // operator overloads
    bool operator>(const Player &other) const { return runs > other.runs; }
    friend ostream& operator<<(ostream &os, const Player &p);
    void displayRole(ostream &os) const override;
};

// ---------------- Team ----------------
class Team {
private:
    string name;
    vector<Player> players;
    int totalRuns;
    int totalWickets;
    int legalBallsBowled;
    int wicketKeeperIndex;
    int captainIndex;
    int extras;
    int wides;
    int noBalls;
    int byes;
    int legByes;

    vector<string> fallOfWickets;

public:
    Team(const string &n = "");
    void inputTeamDetails();

    string getName() const;
    vector<Player>& getPlayers();
    Player& getPlayer(int idx);
    int getTotalRuns() const;
    int getTotalWickets() const;
    int getLegalBalls() const;
    int getExtras() const;
    int getWides() const;
    int getNoBalls() const;
    int getByes() const;
    int getLegByes() const;

    void addRunsToTeam(int r);
    void addWicketToTeam();
    void addLegalBallToTeam();
    void addExtras(int e);
    void addWides(int w);
    void addNoBalls(int nb);
    void addByes(int b);
    void addLegByes(int lb);

    string getExtrasBreakdown() const;

    void displayBattingCard() const;
    void displayBowlingCard() const;

    int getWicketKeeperIndex() const;
    void setWicketKeeperIndex(int idx);

    int getCaptainIndex() const;
    void setCaptainIndex(int idx);

    void addFallOfWicket(int teamScore, const string& batsmanName);
    const vector<string>& getFallOfWickets() const;
};

// ---------------- Match ----------------
class Match {
private:
    Team team1;
    Team team2;

    string tossWinner;
    string tossDecision;
    string pitchType;
    int oversPerInnings;

public:
    struct Snapshot {
        int totalRuns;
        int totalWickets;
        int legalBallsBowledInInnings;
        int strikerIndex;
        int nonStrikerIndex;
        int currentBowlerIndex;
        vector<int> bowlerBallsCount;
        vector<int> bowlerRunsInCurrentOver;
        vector<int> playerRuns;
        vector<int> playerBallsFaced;
        vector<bool> playerIsOut;
        vector<string> playerHowOut;
        vector<int> bowlerBallsBowled;
        vector<int> bowlerRunsConceded;
        vector<int> bowlerWickets;
        vector<int> teamExtras;
    };

    struct InningsState {
        Team *battingTeam;
        Team *bowlingTeam;
        int strikerIndex;
        int nonStrikerIndex;
        int nextBatsmanIndex;
        int currentBowlerIndex;
        int legalBallsBowledInInnings;
        int totalRuns;
        int totalWickets;
        int highestCatchRating;
        string highestCatchPlayer;
        bool freeHitActive;
        bool inningsEndedEarly;
        int lastBowlerIndex;
        vector<int> bowlerBallsCount;
        vector<int> bowlerRunsInCurrentOver;
        vector<int> bowlerStartBalls; // snapshot at over start - used for robust maiden detection
        int partnershipRuns;
        int lastMilestone;
        vector<Snapshot> history;
    };

private:
    InningsState innings1;
    InningsState innings2;

    vector<int> globalConsecWickets;
    vector<bool> globalHattrickMarked;

    int userTeamNumber;

    int localBowlerIndex(Team *teamPtr, int localIdx) const;
    void resetGlobalConsecForTeam(Team *teamPtr);
    int maxOversPerBowler() const;
    bool bowlerCanBeSelected(InningsState &state, int bowlerIdx) const;

    void saveSnapshot(InningsState &state);
    bool undoLastBall(InningsState &state);

public:
    Match();
    void conductToss();
    void selectPitchType();
    void startMatch();

    void playInnings(InningsState &state);

    template<typename T>
    void processBall(InningsState &state, const T &input, bool countFantasy = true);

    void showPostOverMenu(InningsState &state);
    //void showPerBallMenu(InningsState &state);
    void changeBattingOrder(InningsState &state);
    void displayFinalResult();
    void runSuperOver();

    void checkMilestones(InningsState &state, int runs);
    void checkPartnership(InningsState &state);
    void selectNextBatsman(InningsState &state, int positionToFill = -1);
    string getCurrentOverString(const InningsState &state) const;
    void checkMaidenOver(InningsState &state);

    Team& getTeam1();
    Team& getTeam2();
    int getOversPerInnings() const;
};

// ---------------- Templates / Utilities ----------------
namespace utils {
    template <typename T>
    const T& getTopPerformer(const vector<T> &vec) {
        if (vec.empty()) throw runtime_error("Empty vector in getTopPerformer");
        return *max_element(vec.begin(), vec.end(), [](const T &a, const T &b){ return !(a > b); });
    }
}

#endif // FUN_H
