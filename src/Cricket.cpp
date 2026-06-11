// fun.cpp
#include "cricket.h"

// helpers
string toUpper(const string &s) {
    string t = s;
    for (auto &c : t) c = toupper((unsigned char)c);
    return t;
}
string trim(const string &s) {
    string res = s;
    while (!res.empty() && isspace((unsigned char)res.back())) res.pop_back();
    while (!res.empty() && isspace((unsigned char)res.front())) res.erase(res.begin());
    return res;
}
bool askYesNo(const string &question) {
    while (true) {
        cout << question << " (y/n): ";
        string response;
        getline(cin, response);
        response = toUpper(trim(response));
        if (response == "Y" || response == "YES") return true;
        if (response == "N" || response == "NO") return false;
        cout << " Invalid input. Please enter 'y' or 'n'.\n";
    }
}
int getValidCatchRating() {
    int cr;
    while (true) {
        if (!(cin >> cr)) {
            cout << "Enter integer 1-10: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (cr >= 1 && cr <= 10) break;
        cout << "Enter integer 1-10: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return cr;
}
bool isValidWicketType(const string &wt) {
    vector<string> validTypes = {"C", "B", "R", "L", "S", "H", "T", "O", "RI"};
    return find(validTypes.begin(), validTypes.end(), wt) != validTypes.end();
}

// ---------------- Player ----------------
Player::Player(const string &n) : Cricketer(n) {
    runs = ballsFaced = fours = sixes = 0;
    isOut = false; howOut = "";
    ballsBowled = runsConceded = wickets = dotBalls = maidenOvers = 0;
    catches = runouts = bestCatchRating = 0;
    injured = false; retiredHurt = false; hasEntered = false;
    rating = 0.0; fantasyPoints = 0.0;
}
void Player::addRuns(int r) { runs += r; if (r==4) fours++; if (r==6) sixes++; }
void Player::addBallFaced() { ballsFaced++; }
void Player::addFour() { fours++; }
void Player::addSix() { sixes++; }
void Player::addBatsmanDotBall() {}
void Player::addBowlerDotBall() { dotBalls++; }
void Player::addMaidenOver() { maidenOvers++; }
void Player::setOut(const string &how) { isOut = true; howOut = how; }
bool Player::getIsOut() const { return isOut; }
string Player::getHowOut() const { return howOut; }
void Player::addBowlingBall(int runsConcededOnBall, bool legalBall, bool wicketTaken, bool isDotBall) {
    runsConceded += runsConcededOnBall;
    if (legalBall) ballsBowled++;
    if (wicketTaken) wickets++;
    if (isDotBall) dotBalls++;
}
void Player::addPartialBowlingBalls(int balls) { ballsBowled += balls; }
void Player::addCatch(int rating) { catches++; if (rating > bestCatchRating) bestCatchRating = rating; }
void Player::addRunout() { runouts++; }
void Player::setInjured(bool v) { injured = v; }
bool Player::isInjured() const { return injured; }
void Player::addFantasyPoints(double v) { fantasyPoints += v; }

void Player::setRetiredHurt() { retiredHurt = true; injured = true; howOut = "retired hurt"; }
bool Player::isRetiredHurt() const { return retiredHurt; }

void Player::setEntered(bool v) { hasEntered = v; }
bool Player::isEntered() const { return hasEntered; }

void Player::computeRating(bool underPressure) {
    double batPart = (ballsFaced > 0) ? ((double)runs / max(1, ballsFaced) * 100.0 / 30.0) : 0.0;
    double bowlPart = (ballsBowled > 0) ? ((double)wickets * 3.0 + (double)ballsBowled / 6.0) : 0.0;
    double fieldPart = catches * 0.8 + bestCatchRating * 0.2;
    rating = batPart * 0.5 + bowlPart * 0.4 + fieldPart * 0.1;
    if (underPressure) rating += 1.0;
    if (rating > 10.0) rating = 10.0;
}

void Player::computeFantasyPoints(int oversPerInnings) {
    double fp = 0.0;
    fp += 4.0;
    fp += runs * 1.0;
    fp += fours * 6.0;
    fp += sixes * 12.0;
    if (runs >= 100) fp += 100.0; else if (runs >= 50) fp += 25.0;
    fp += wickets * 24.0;
    fp += catches * 12.0;
    fp += runouts * 15.0;
    if (isOut && runs == 0) fp -= 2.0;
    fp += dotBalls * 0.5;
    fp += maidenOvers * 12.0;
    if (ballsBowled > 0) {
        double overs = (double)ballsBowled / 6.0;
        double econ = (runsConceded) / max(0.000001, overs);
        if (econ > 12.0) fp -= 2.0;
    }
    fantasyPoints = fp;
}

string Player::getName() const { return name; }
int Player::getRuns() const { return runs; }
int Player::getBallsFaced() const { return ballsFaced; }
int Player::getFours() const { return fours; }
int Player::getSixes() const { return sixes; }
int Player::getBallsBowled() const { return ballsBowled; }
int Player::getRunsConceded() const { return runsConceded; }
int Player::getWickets() const { return wickets; }
int Player::getCatches() const { return catches; }
int Player::getRunouts() const { return runouts; }
int Player::getBestCatchRating() const { return bestCatchRating; }
double Player::getRating() const { return rating; }
double Player::getFantasyPoints() const { return fantasyPoints; }
int Player::getDotBalls() const { return dotBalls; }
int Player::getMaidenOvers() const { return maidenOvers; }

void Player::displayBattingRow() const {
    double strikeRate = (ballsFaced > 0) ? (100.0 * runs / ballsFaced) : 0.0;
    cout << left << setw(18) << name
         << setw(6) << runs
         << setw(7) << ballsFaced
         << setw(8) << fixed << setprecision(1) << strikeRate
         << setw(6) << fours
         << setw(6) << sixes
         << setw(16) << (isOut ? howOut : (retiredHurt ? howOut : "not out")) << "\n";
}
void Player::displayBowlingRow() const {
    if (ballsBowled == 0) return;
    int overs = ballsBowled / 6;
    int balls = ballsBowled % 6;
    stringstream ss; ss << overs << "." << balls;
    double eco = (ballsBowled > 0) ? ((double)runsConceded * 6.0 / ballsBowled) : 0.0;
    cout << left << setw(18) << name
         << setw(8) << ss.str()
         << setw(8) << runsConceded
         << setw(8) << wickets
         << setw(8) << dotBalls
         << setw(8) << maidenOvers
         << setw(8) << fixed << setprecision(2) << eco << "\n";
}

void Player::displayRole(ostream &os) const { (void)os; }

ostream& operator<<(ostream &os, const Player &p) {
    os << left << setw(12) << p.getName()
      << " R:" << setw(3) << p.getRuns()
      << " B:" << setw(3) << p.getBallsFaced()
      << " 4s:" << setw(2) << p.getFours()
      << " 6s:" << setw(2) << p.getSixes();
    return os;
}

// ---------------- Team ----------------
Team::Team(const string &n) {
    name = n;
    players.clear();
    totalRuns = totalWickets = legalBallsBowled = 0;
    wicketKeeperIndex = -1;
    captainIndex = -1;
    extras = wides = noBalls = byes = legByes = 0;
    fallOfWickets.clear();
}
void Team::inputTeamDetails() {
    cout << "Enter Team Name: ";
    string t; getline(cin, t);
    while (t.size() == 0) { cout << "Team name cannot be empty. Enter Team Name: "; getline(cin, t); }
    name = t;
    players.clear();
    cout << "Enter 11 player names for " << name << " (press Enter after each):\n";
    for (int i = 0; i < 11; ) {
        cout << "Player " << (i + 1) << ": ";
        string p; getline(cin, p);
        if (p.size() == 0) { cout << "Player name cannot be empty. Re-enter.\n"; continue; }
        players.emplace_back(Player(p)); i++;
    }
    cout << "Select wicket keeper for " << name << " by number (1-11):\n";
    for (int i = 0; i < 11; ++i) cout << i + 1 << ". " << players[i].getName() << "\n";
    int kidx;
    while (!(cin >> kidx) || kidx < 1 || kidx > 11) { cout << "Invalid. Enter 1-11: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    wicketKeeperIndex = kidx - 1;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << " Keeper set to: " << players[wicketKeeperIndex].getName() << "\n";

    cout << "Select captain for " << name << " by number (1-11):\n";
    for (int i = 0; i < 11; ++i) {
        cout << i + 1 << ". " << players[i].getName();
        if (i == wicketKeeperIndex) cout << " (Keeper)";
        cout << "\n";
    }
    int cidx;
    while (!(cin >> cidx) || cidx < 1 || cidx > 11) { cout << "Invalid. Enter 1-11: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    captainIndex = cidx - 1;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Captain set to: " << players[captainIndex].getName() << "\n";
}
string Team::getName() const { return name; }
vector<Player>& Team::getPlayers() { return players; }
Player& Team::getPlayer(int idx) { return players[idx]; }
int Team::getTotalRuns() const { return totalRuns; }
int Team::getTotalWickets() const { return totalWickets; }
int Team::getLegalBalls() const { return legalBallsBowled; }
int Team::getWicketKeeperIndex() const { return wicketKeeperIndex; }
int Team::getCaptainIndex() const { return captainIndex; }
int Team::getExtras() const { return extras; }
int Team::getWides() const { return wides; }
int Team::getNoBalls() const { return noBalls; }
int Team::getByes() const { return byes; }
int Team::getLegByes() const { return legByes; }

void Team::setWicketKeeperIndex(int idx) { wicketKeeperIndex = idx; }
void Team::setCaptainIndex(int idx) { captainIndex = idx; }
void Team::addRunsToTeam(int r) { totalRuns += r; }
void Team::addWicketToTeam() { totalWickets++; }
void Team::addLegalBallToTeam() { legalBallsBowled++; }
void Team::addExtras(int e) { extras += e; }
void Team::addWides(int w) { wides += w; extras += w; }
void Team::addNoBalls(int nb) { noBalls += nb; extras += nb; }
void Team::addByes(int b) { byes += b; extras += b; }
void Team::addLegByes(int lb) { legByes += lb; extras += lb; }

string Team::getExtrasBreakdown() const {
    vector<string> parts;
    if (wides > 0) parts.push_back(to_string(wides) + "wd");
    if (noBalls > 0) parts.push_back(to_string(noBalls) + "nb");
    if (byes > 0) parts.push_back(to_string(byes) + "b");
    if (legByes > 0) parts.push_back(to_string(legByes) + "lb");
    if (parts.empty()) return "0";
    string result;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) result += ", ";
        result += parts[i];
    }
    return result;
}

void Team::addFallOfWicket(int teamScore, const string& batsmanName) {
    stringstream ss;
    ss << to_string(totalWickets) << "-" << teamScore << " (" << batsmanName << ")";
    fallOfWickets.push_back(ss.str());
}
const vector<string>& Team::getFallOfWickets() const { return fallOfWickets; }

void Team::displayBattingCard() const {
    cout << "\n🏏 Batting Scorecard - " << name << "\n";
    cout << left << setw(18) << "Name" << setw(6) << "Runs" << setw(7) << "Balls" << setw(8) << "SR"
         << setw(6) << "4s" << setw(6) << "6s" << setw(16) << "HowOut" << "\n";
    cout << string(110, '-') << "\n";
    vector<bool> hasBatted(11,false);
    for (int i = 0; i < (int)players.size(); ++i) {
        const auto &p = players[i];
        if (p.getBallsFaced() > 0 || p.getIsOut() || p.isEntered() || p.isRetiredHurt()) {
            string dname = p.getName();
            if (i == captainIndex) dname += " (C)";
            if (i == wicketKeeperIndex) dname += " (W)";
            double strikeRate = (p.getBallsFaced() > 0) ? (100.0 * p.getRuns() / p.getBallsFaced()) : 0.0;
            cout << left << setw(18) << dname
                 << setw(6) << p.getRuns()
                 << setw(7) << p.getBallsFaced()
                 << setw(8) << fixed << setprecision(1) << strikeRate
                 << setw(6) << p.getFours()
                 << setw(6) << p.getSixes()
                 << setw(16) << (p.getIsOut() ? p.getHowOut() : (p.isRetiredHurt() ? p.getHowOut() : "not out")) << "\n";
            hasBatted[i] = true;
        }
    }
    cout << string(110, '-') << "\n";
    bool anyYetToBat = false;
    for (int i=0;i<11;++i) {
        if (!hasBatted[i] && !players[i].isInjured()) {
            if (!anyYetToBat) { cout << "YET TO BAT\n"; anyYetToBat = true; }
            string dname = players[i].getName();
            if (i == captainIndex) dname += " (C)";
            if (i == wicketKeeperIndex) dname += " (W)";
            cout << dname;
            if (i<10) {
                bool nextAlsoYetToBat=false;
                for (int j=i+1;j<11;++j) if (!hasBatted[j] && !players[j].isInjured()) { nextAlsoYetToBat=true; break; }
                if (nextAlsoYetToBat) cout << ", ";
            }
        }
    }
    if (anyYetToBat) cout << "\n";
    cout << string(110, '-') << "\n";

    int batRuns = totalRuns - extras;
    stringstream tb;
    tb << "Total: " << totalRuns << " (Bat: " << batRuns << ", Extras: " << extras << " [" << getExtrasBreakdown() << "])";
    cout << left << setw(60) << tb.str() << setw(16) << ("Wkts: " + to_string(totalWickets)) << "\n";
    cout << "Overs: " << (legalBallsBowled / 6) << "." << (legalBallsBowled % 6) << "\n";
    if (!fallOfWickets.empty()) {
        cout << "Fall of Wickets: ";
        for (size_t i=0;i<fallOfWickets.size();++i) {
            if (i>0) cout << ", ";
            cout << fallOfWickets[i];
        }
        cout << "\n";
    }
}

void Team::displayBowlingCard() const {
    cout << "\n Bowling Figures for " << name << "\n";
    cout << left << setw(18) << "Name" << setw(8) << "Overs" << setw(8) << "Runs" << setw(8) << "Wkts"
         << setw(8) << "Dots" << setw(8) << "Maidens" << setw(8) << "Eco" << "\n";
    cout << string(106,'-') << "\n";
    for (int i = 0; i < (int)players.size(); ++i) {
        const auto &p = players[i];
        if (p.getBallsBowled() > 0) {
            string dname = p.getName();
            if (i == captainIndex) dname += " (C)";
            if (i == wicketKeeperIndex) dname += " (W)";
            int ballsBowled = p.getBallsBowled();
            int overs = ballsBowled / 6;
            int balls = ballsBowled % 6;
            stringstream ss; ss << overs << "." << balls;
            double eco = (ballsBowled > 0) ? ((double)p.getRunsConceded() * 6.0 / ballsBowled) : 0.0;
            cout << left << setw(18) << dname
                 << setw(8) << ss.str()
                 << setw(8) << p.getRunsConceded()
                 << setw(8) << p.getWickets()
                 << setw(8) << p.getDotBalls()
                 << setw(8) << p.getMaidenOvers()
                 << setw(8) << fixed << setprecision(2) << eco << "\n";
        }
    }
    cout << string(106,'-') << "\n";
}

// ---------------- Match ----------------
Match::Match() {
    srand((unsigned)time(nullptr));
    oversPerInnings = 0;
    tossWinner = ""; tossDecision = ""; pitchType = "Batting"; userTeamNumber = 1;
    globalConsecWickets.assign(22,0);
    globalHattrickMarked.assign(22,false);
}

int Match::localBowlerIndex(Team *teamPtr, int localIdx) const {
    int teamIndex = (teamPtr == &team1) ? 0 : 1;
    return teamIndex * 11 + localIdx;
}
void Match::resetGlobalConsecForTeam(Team *teamPtr) {
    int base = (teamPtr == &team1) ? 0 : 11;
    for (int i=0;i<11;++i) { globalConsecWickets[base+i]=0; globalHattrickMarked[base+i]=false; }
}

void Match::conductToss() {
    cout << "\n--- Toss ---\n";
    cout << "Which team is yours? Enter 1 for " << team1.getName() << " or 2 for " << team2.getName() << ": ";
    while (!(cin >> userTeamNumber) || (userTeamNumber != 1 && userTeamNumber != 2)) { cout << "Enter 1 or 2: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Call the coin: (H)eads or (T)ails: ";
    char call;
    while (true) { cin >> call; call = toupper(call); if (call == 'H' || call == 'T') break; cout << "Invalid. Enter H or T: "; }
    int flip = rand() % 2;
    char actual = (flip == 0) ? 'H' : 'T';
    cout << "Coin shows: " << (actual=='H'?"Heads":"Tails") << "\n";
    if (call == actual) { tossWinner = (userTeamNumber==1)? team1.getName() : team2.getName(); cout << " " << tossWinner << " (your team) won the toss.\n"; }
    else { tossWinner = (userTeamNumber==1)? team2.getName() : team1.getName(); cout << tossWinner << " won the toss.\n"; }
    cout << tossWinner << ", choose to (B)at or (L) Bowl (enter B or L): ";
    char dec;
    while (true) { cin >> dec; dec = toupper(dec); if (dec=='B' || dec=='L') break; cout << "Invalid. Enter B (bat) or L (bowl): "; }
    tossDecision = (dec=='B') ? "bat" : "bowl";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << tossWinner << " chose to " << tossDecision << " first.\n";
}

void Match::selectPitchType() {
    cout << "Select pitch type: (1) Batting friendly (2) Bowling friendly. Enter 1 or 2: ";
    int p;
    while (!(cin >> p) || (p!=1 && p!=2)) { cout << "Enter 1 or 2: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    pitchType = (p==1) ? "Batting" : "Bowling";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Pitch set to " << pitchType << " friendly.\n";
}

int Match::maxOversPerBowler() const {
    if (oversPerInnings < 5) return 1;
    if (oversPerInnings < 10) return 2;
    if (oversPerInnings <= 20) return 4;
    return max(1, oversPerInnings / 5);
}

bool Match::bowlerCanBeSelected(InningsState &state, int bowlerIdx) const {
    if (bowlerIdx < 0 || bowlerIdx >= 11) return false;
    Team *bw = state.bowlingTeam;
    if (bw->getPlayers()[bowlerIdx].isInjured()) return false;
    if (state.lastBowlerIndex == bowlerIdx) return false; // no consecutive overs by same bowler
    int ballsBowledBy = (bowlerIdx < (int)state.bowlerBallsCount.size()) ? state.bowlerBallsCount[bowlerIdx] : 0;
    int maxBalls = maxOversPerBowler() * 6;
    if (ballsBowledBy >= maxBalls) return false;
    if (bowlerIdx == bw->getWicketKeeperIndex()) return false;
    return true;
}

string Match::getCurrentOverString(const InningsState &state) const {
    int overs = state.legalBallsBowledInInnings / 6;
    int balls = state.legalBallsBowledInInnings % 6;
    stringstream ss; ss << overs << "." << balls;
    return ss.str();
}

void Match::checkMilestones(InningsState &state, int runs) {
    Player &striker = state.battingTeam->getPlayers()[state.strikerIndex];
    if (striker.getRuns() >= 50 && striker.getRuns() < 100 && (striker.getRuns() - runs) < 50) {
        cout << " HALF CENTURY! " << striker.getName() << " scores 50 runs! \n";
    } else if (striker.getRuns() >= 100 && (striker.getRuns() - runs) < 100) {
        cout << " CENTURY! " << striker.getName() << " scores 100 runs! \n";
    }
}

void Match::checkPartnership(InningsState &state) {
    if (state.partnershipRuns >= 50 && state.partnershipRuns < 100 && state.lastMilestone < 50) {
        cout << " 50-RUN PARTNERSHIP! \n";
        cout << "Partnership: " << state.partnershipRuns << " runs\n";
        cout << state.battingTeam->getPlayers()[state.strikerIndex].getName() << ": " 
             << state.battingTeam->getPlayers()[state.strikerIndex].getRuns() << " (" 
             << state.battingTeam->getPlayers()[state.strikerIndex].getBallsFaced() << " balls)\n";
        cout << state.battingTeam->getPlayers()[state.nonStrikerIndex].getName() << ": " 
             << state.battingTeam->getPlayers()[state.nonStrikerIndex].getRuns() << " (" 
             << state.battingTeam->getPlayers()[state.nonStrikerIndex].getBallsFaced() << " balls)\n";
        state.lastMilestone = 50;
    } else if (state.partnershipRuns >= 100 && state.lastMilestone < 100) {
        cout << " 100-RUN PARTNERSHIP! \n";
        cout << "Partnership: " << state.partnershipRuns << " runs\n";
        cout << state.battingTeam->getPlayers()[state.strikerIndex].getName() << ": " 
             << state.battingTeam->getPlayers()[state.strikerIndex].getRuns() << " (" 
             << state.battingTeam->getPlayers()[state.strikerIndex].getBallsFaced() << " balls)\n";
        cout << state.battingTeam->getPlayers()[state.nonStrikerIndex].getName() << ": " 
             << state.battingTeam->getPlayers()[state.nonStrikerIndex].getRuns() << " (" 
             << state.battingTeam->getPlayers()[state.nonStrikerIndex].getBallsFaced() << " balls)\n";
        state.lastMilestone = 100;
    }
}

void Match::selectNextBatsman(InningsState &state, int positionToFill) {
    Team &bat = *state.battingTeam;
    cout << "\n--- Select Next Batsman ---\n";
    cout << "Available batsmen (yet to bat or retired-hurt who can return):\n";
    vector<int> available;
    for (int i = 0; i < 11; ++i) {
        if ((i != state.strikerIndex && i != state.nonStrikerIndex) &&
            !bat.getPlayers()[i].getIsOut() &&
            (bat.getPlayers()[i].getBallsFaced() == 0 || bat.getPlayers()[i].isRetiredHurt())) {
            cout << i + 1 << ". " << bat.getPlayers()[i].getName();
            if (bat.getPlayers()[i].isRetiredHurt()) cout << " (retired hurt)";
            cout << "\n";
            available.push_back(i);
        }
    }
    if (available.empty()) {
        cout << "No more batsmen available! Innings ended.\n";
        state.inningsEndedEarly = true;
        return;
    }
    cout << "Enter batsman number: ";
    int choice;
    while (!(cin >> choice) || find(available.begin(), available.end(), choice - 1) == available.end()) {
        cout << "Invalid choice. Enter a valid number from the list: ";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int chosenIndex = choice - 1;
    if (positionToFill >= 0) {
        if (positionToFill == state.strikerIndex) state.strikerIndex = chosenIndex;
        else if (positionToFill == state.nonStrikerIndex) state.nonStrikerIndex = chosenIndex;
        else state.strikerIndex = chosenIndex;
    } else state.strikerIndex = chosenIndex;
    bat.getPlayers()[chosenIndex].setEntered(true);
    state.nextBatsmanIndex = max(state.nextBatsmanIndex, chosenIndex + 1);
    cout << bat.getPlayers()[chosenIndex].getName() << " comes to bat.\n";
    state.partnershipRuns = 0; state.lastMilestone = 0;
}

void Match::checkMaidenOver(InningsState &state) {
    // Robust maiden detection using snapshot of bowler's balls at over-start
    int idx = state.lastBowlerIndex; // the bowler who completed the over
    if (idx < 0 || idx >= 11) return;
    Player &bowler = state.bowlingTeam->getPlayers()[idx];
    if (idx >= (int)state.bowlerStartBalls.size()) return;
    int startBalls = state.bowlerStartBalls[idx];
    int ballsBowledThisOver = state.bowlerBallsCount[idx] - startBalls;
    // Maiden only if bowler bowled exactly 6 legal deliveries in that over and conceded 0 runs in that over
    if (ballsBowledThisOver == 6 && state.bowlerRunsInCurrentOver[idx] == 0) {
        bowler.addMaidenOver();
        cout << " MAIDEN OVER! " << bowler.getName() << " bowls a maiden over!\n";
    }
}

// Snapshot save and undo (approximate reconstruction)
void Match::saveSnapshot(InningsState &state) {
    Snapshot s;
    s.totalRuns = state.totalRuns; s.totalWickets = state.totalWickets;
    s.legalBallsBowledInInnings = state.legalBallsBowledInInnings;
    s.strikerIndex = state.strikerIndex; s.nonStrikerIndex = state.nonStrikerIndex;
    s.currentBowlerIndex = state.currentBowlerIndex;
    s.bowlerBallsCount = state.bowlerBallsCount;
    s.bowlerRunsInCurrentOver = state.bowlerRunsInCurrentOver;
    s.playerRuns.resize(11); s.playerBallsFaced.resize(11); s.playerIsOut.resize(11); s.playerHowOut.resize(11);
    for (int i=0;i<11;++i) { const Player &p = state.battingTeam->getPlayers()[i]; s.playerRuns[i]=p.getRuns(); s.playerBallsFaced[i]=p.getBallsFaced(); s.playerIsOut[i]=p.getIsOut(); s.playerHowOut[i]=p.getHowOut(); }
    s.bowlerBallsBowled.resize(11); s.bowlerRunsConceded.resize(11); s.bowlerWickets.resize(11);
    for (int i=0;i<11;++i) { const Player &p = state.bowlingTeam->getPlayers()[i]; s.bowlerBallsBowled[i]=p.getBallsBowled(); s.bowlerRunsConceded[i]=p.getRunsConceded(); s.bowlerWickets[i]=p.getWickets(); }
    s.teamExtras = { state.battingTeam->getWides(), state.battingTeam->getNoBalls(), state.battingTeam->getByes(), state.battingTeam->getLegByes() };
    state.history.push_back(std::move(s));
}

bool Match::undoLastBall(InningsState &state) {
    if (state.history.empty()) { cout << "No history to undo.\n"; return false; }
    Snapshot s = state.history.back(); state.history.pop_back();
    state.totalRuns = s.totalRuns; state.totalWickets = s.totalWickets;
    state.legalBallsBowledInInnings = s.legalBallsBowledInInnings;
    state.strikerIndex = s.strikerIndex; state.nonStrikerIndex = s.nonStrikerIndex;
    state.currentBowlerIndex = s.currentBowlerIndex;
    state.bowlerBallsCount = s.bowlerBallsCount;
    state.bowlerRunsInCurrentOver = s.bowlerRunsInCurrentOver;
    vector<Player> newBat;
    for (int i=0;i<11;++i) {
        string nm = state.battingTeam->getPlayers()[i].getName();
        Player p(nm);
        if (s.playerRuns[i]>0) p.addRuns(s.playerRuns[i]);
        for (int k=0;k<s.playerBallsFaced[i];++k) p.addBallFaced();
        if (s.playerIsOut[i]) p.setOut(s.playerHowOut[i]);
        newBat.push_back(p);
    }
    state.battingTeam->getPlayers() = newBat;
    vector<Player> newBowl;
    for (int i=0;i<11;++i) {
        string nm = state.bowlingTeam->getPlayers()[i].getName();
        Player p(nm);
        int balls = s.bowlerBallsBowled[i];
        int runsConceded = s.bowlerRunsConceded[i];
        int wickets = s.bowlerWickets[i];
        int rpb = (balls>0)?(runsConceded/balls):0;
        for (int b=0;b<balls;++b) p.addBowlingBall(rpb, true, false, rpb==0);
        for (int w=0;w<wickets;++w) p.addBowlingBall(0, true, true, false);
        newBowl.push_back(p);
    }
    state.bowlingTeam->getPlayers() = newBowl;
    cout << "Last ball undone. Score restored.\n";
    return true;
}

void Match::startMatch() {
    cout << "=== Team 1 Input ===\n"; team1.inputTeamDetails();
    cout << "=== Team 2 Input ===\n"; team2.inputTeamDetails();
    cout << "Enter number of overs per innings (e.g., 20): ";
    while (!(cin >> oversPerInnings) || oversPerInnings <= 0) { cout << "Enter a positive integer for overs: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    conductToss(); selectPitchType();

    Team *firstBat=nullptr,*firstBowl=nullptr;
    if (tossWinner == team1.getName()) { if (tossDecision=="bat") { firstBat=&team1; firstBowl=&team2; } else { firstBat=&team2; firstBowl=&team1; } }
    else { if (tossDecision=="bat") { firstBat=&team2; firstBowl=&team1; } else { firstBat=&team1; firstBowl=&team2; } }

    innings1.battingTeam = firstBat; innings1.bowlingTeam = firstBowl;
    innings1.strikerIndex=0; innings1.nonStrikerIndex=1; innings1.nextBatsmanIndex=2;
    innings1.currentBowlerIndex=-1; innings1.legalBallsBowledInInnings=0; innings1.totalRuns=0; innings1.totalWickets=0;
    innings1.highestCatchRating=0; innings1.highestCatchPlayer=""; innings1.inningsEndedEarly=false; innings1.lastBowlerIndex=-1;
    innings1.bowlerBallsCount.assign(11,0); innings1.bowlerRunsInCurrentOver.assign(11,0); innings1.bowlerStartBalls.assign(11,0); innings1.freeHitActive=false;
    innings1.partnershipRuns=0; innings1.lastMilestone=0; innings1.history.clear();
    resetGlobalConsecForTeam(innings1.battingTeam); resetGlobalConsecForTeam(innings1.bowlingTeam);

    innings1.battingTeam->getPlayers()[0].setEntered(true);
    innings1.battingTeam->getPlayers()[1].setEntered(true);

    cout << "\n=== Innings 1: " << innings1.battingTeam->getName() << " batting ===\n";
    playInnings(innings1);

    innings2.battingTeam = (innings1.battingTeam==&team1)? &team2 : &team1;
    innings2.bowlingTeam = (innings1.bowlingTeam==&team1)? &team2 : &team1;
    innings2.strikerIndex=0; innings2.nonStrikerIndex=1; innings2.nextBatsmanIndex=2;
    innings2.currentBowlerIndex=-1; innings2.legalBallsBowledInInnings=0; innings2.totalRuns=0; innings2.totalWickets=0;
    innings2.highestCatchRating=0; innings2.highestCatchPlayer=""; innings2.inningsEndedEarly=false; innings2.lastBowlerIndex=-1;
    innings2.bowlerBallsCount.assign(11,0); innings2.bowlerRunsInCurrentOver.assign(11,0); innings2.bowlerStartBalls.assign(11,0); innings2.freeHitActive=false;
    innings2.partnershipRuns=0; innings2.lastMilestone=0; innings2.history.clear();
    resetGlobalConsecForTeam(innings2.battingTeam); resetGlobalConsecForTeam(innings2.bowlingTeam);

    innings2.battingTeam->getPlayers()[0].setEntered(true);
    innings2.battingTeam->getPlayers()[1].setEntered(true);

    cout << "\n=== Innings 2: " << innings2.battingTeam->getName() << " batting ===\n";
    playInnings(innings2);

    displayFinalResult();
}

void Match::playInnings(InningsState &state) {
    Team &bat = *state.battingTeam; Team &bowl = *state.bowlingTeam;
    int totalLegalBallsAllowed = oversPerInnings * 6;

    while (state.legalBallsBowledInInnings < totalLegalBallsAllowed && state.totalWickets < 10 && !state.inningsEndedEarly) {
        cout << "\nChoose bowler for next over from bowling team (" << bowl.getName() << "):\n";
for (int i=0;i<(int)bowl.getPlayers().size();++i)
{
    if (i == bowl.getWicketKeeperIndex())
        continue;

    if (bowl.getPlayers()[i].isInjured())
        continue;

    cout << i+1 << ". "
         << bowl.getPlayers()[i].getName()
         << " (Balls: "
         << state.bowlerBallsCount[i]
         << ")\n";
}
        cout << "Enter bowler number (1-11): ";
        int bidx;
        while (!(cin >> bidx) || bidx < 1 || bidx > 11) { cout << "Invalid. Enter 1-11: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        bidx--;
        if (bidx == bowl.getWicketKeeperIndex()) { cout << "Keeper cannot bowl. Choose another bowler.\n"; cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        if (!bowlerCanBeSelected(state, bidx)) { cout << "Bowler selection invalid / exhausted overs or injured or same as last over. Re-select next over.\n"; cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        state.currentBowlerIndex = bidx; state.lastBowlerIndex = bidx;
        // snapshot start balls for robust maiden detection
        state.bowlerStartBalls[state.currentBowlerIndex] = state.bowlerBallsCount[state.currentBowlerIndex];
        state.bowlerRunsInCurrentOver[state.currentBowlerIndex] = 0;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "🏁 Over starts. Striker: " << bat.getPlayers()[state.strikerIndex].getName()
             << "  Non-Striker: " << bat.getPlayers()[state.nonStrikerIndex].getName()
             << "  Bowler: " << bowl.getPlayers()[state.currentBowlerIndex].getName() << "\n";

        int ballsThisOver = 0; bool overInterruptedByInjury = false;

        while (ballsThisOver < 6 && state.totalWickets < 10 && state.legalBallsBowledInInnings < totalLegalBallsAllowed && !state.inningsEndedEarly) {
            string currentOver = getCurrentOverString(state);
            cout << "\nBall " << currentOver << " | " <<bowl.getPlayers()[state.currentBowlerIndex].getName()
                 << " to " << bat.getPlayers()[state.strikerIndex].getName()<< ": ";
            if (askYesNo("Ball instructions")) {
                cout << "Enter: 0-6 for runs, W for wicket, WD for wide, NB for no-ball, B for byes, LB for leg byes, INJ for injury";
            }
            string raw; getline(cin, raw); raw = trim(raw);
            if (raw.size() == 0) { cout << "Empty input, re-enter.\n"; continue; }
            string up = toUpper(raw);
            vector<string> validInputs = {"W","WD","NB","B","LB","INJ"};
            bool isValidInput=false;
            if (find(validInputs.begin(), validInputs.end(), up) != validInputs.end()) isValidInput = true;
            else {
                try { int runs = stoi(raw); if (runs >= 0 && runs <= 6) isValidInput = true; else { cout << " Invalid runs input. Acceptable range is 0 to 6.\n"; continue; } }
                catch(...) { cout << " Invalid input. Please enter 0-6, W, WD, NB, B, LB, INJ"; continue; }
            }
            if (!isValidInput) { cout << "Invalid input. Please enter 0-6, W, WD, NB, B, LB, INJ\n"; continue; }



            if (up == "INJ") {
                cout << "Is this (B)owler injury or (A)batsman injury? (enter B or A): ";
                char type; while (true) { cin >> type; type = toupper(type); if (type=='B'||type=='A') break; cout<<"Enter B or A: "; }
                cout << "Before (B) or after (A) delivering the ball? (enter B or A): ";
                char when; while (true) { cin >> when; when = toupper(when); if (when=='B'||when=='A') break; cout<<"Enter B or A: "; }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (type=='B') {
                    if (when=='B') {
                        cout << " Bowler " << bowl.getPlayers()[state.currentBowlerIndex].getName() << " injured before delivery.\n";
                        bowl.getPlayers()[state.currentBowlerIndex].setInjured(true);
                        cout << "Select replacement bowler:\n"; 
                        for (int i=0;i<11;++i) 
                        { 
                            if (i==bowl.getWicketKeeperIndex()||i==state.currentBowlerIndex) 
                            {continue; 
                            }
                        cout<<i+1<<". "<<bowl.getPlayers()[i].getName()<<" (Balls: "<<state.bowlerBallsCount[i]<<")\n"; 
                            if (bowl.getPlayers()[i].isInjured())
                             continue;
                        }
                        int ridx; while (!(cin>>ridx)||ridx<1||ridx>11) { cout<<"Invalid. Enter 1-11: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
                        ridx--; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (!bowlerCanBeSelected(state,ridx)) { cout<<"Replacement invalid. This over will be re-selected next cycle.\n"; overInterruptedByInjury=true; break; }
                        state.currentBowlerIndex = ridx; state.bowlerRunsInCurrentOver[state.currentBowlerIndex]=0;
                        // snapshot replacement bowler's start balls (he will finish remainder of over)
                        state.bowlerStartBalls[state.currentBowlerIndex] = state.bowlerBallsCount[state.currentBowlerIndex];
                        cout << "Replacement " << bowl.getPlayers()[ridx].getName() << " will bowl now.\n"; continue;
                    } else {
                        cout << " Bowler injured after delivery.\n"; bowl.getPlayers()[state.currentBowlerIndex].setInjured(true);
                        cout << "Enter result of the ball just delivered (0-6, W, WD, NB, B, LB): ";
                        string res; getline(cin, res); res=trim(res); if (res.size()==0) { res="0"; cout<<"Empty result, treating as 0.\n"; }
                        //saveSnapshot(state); 
                        processBall(state, res, true);
                        string upres = toUpper(res);
                        bool legalBall = !(upres=="WD"||upres=="NB");
                        if (legalBall) { ballsThisOver++; state.legalBallsBowledInInnings++; state.bowlerBallsCount[state.currentBowlerIndex]++; bat.addLegalBallToTeam(); state.freeHitActive=false; }
                        cout << "Select replacement bowler to finish the over:\n"; for (int i=0;i<11;++i) { if (i==bowl.getWicketKeeperIndex()||i==state.currentBowlerIndex) continue; cout<<i+1<<". "<<bowl.getPlayers()[i].getName()<<" (Balls: "<<state.bowlerBallsCount[i]<<")\n"; }
                        int ridx; while (!(cin>>ridx)||ridx<1||ridx>11) { cout<<"Invalid. Enter 1-11: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
                        ridx--; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (!bowlerCanBeSelected(state,ridx)) { cout<<"Replacement invalid. This over will be re-selected next cycle.\n"; overInterruptedByInjury=true; break; }
                        state.currentBowlerIndex = ridx; state.bowlerRunsInCurrentOver[state.currentBowlerIndex]=0; state.bowlerStartBalls[state.currentBowlerIndex] = state.bowlerBallsCount[state.currentBowlerIndex]; cout<<"Replacement "<<bowl.getPlayers()[ridx].getName()<<" will finish the over.\n"; continue;
                    }
                } else {
                    if (when=='B') { cout<<"Batsman "<<bat.getPlayers()[state.strikerIndex].getName()<<" injured before delivery (Retired Hurt).\n"; bat.getPlayers()[state.strikerIndex].setRetiredHurt(); bat.getPlayers()[state.strikerIndex].setEntered(true); selectNextBatsman(state); continue; }
                    else { cout<<" Batsman injured after delivery (Retired Hurt).\n"; bat.getPlayers()[state.strikerIndex].setRetiredHurt(); bat.getPlayers()[state.strikerIndex].setEntered(true);
                        cout<<"Enter result of the ball just delivered (0-6, W, WD, NB, B, LB): "; string res; getline(cin,res); res=trim(res); if (res.size()==0){ res="0"; cout<<"Empty result, treating as 0.\n"; }
                        saveSnapshot(state); processBall(state,res,true);
                        string upres=toUpper(res); bool legalBall=!(upres=="WD"||upres=="NB");
                        if (legalBall) { ballsThisOver++; state.legalBallsBowledInInnings++; state.bowlerBallsCount[state.currentBowlerIndex]++; bat.addLegalBallToTeam(); state.freeHitActive=false; }
                        selectNextBatsman(state); continue;
                    }
                }
            } // INJ end

            saveSnapshot(state);
            processBall(state, raw, true);

            string s = toUpper(raw);
            bool legalBall = !(s=="WD"||s=="NB");
            if (legalBall) {
                ballsThisOver++; state.legalBallsBowledInInnings++; state.bowlerBallsCount[state.currentBowlerIndex]++; bat.addLegalBallToTeam(); state.freeHitActive=false;
            } else {
                int base = (bowl.getName() == team1.getName()) ? 0 : 11;
                globalConsecWickets[base + state.currentBowlerIndex] = 0;
            }

            cout << "Current: " << state.totalRuns << "/" << state.totalWickets << "  (" << getCurrentOverString(state) << " overs)\n";
          //  if (askYesNo("Show per-ball options")) showPerBallMenu(state);

            if (&state == &innings2) {
                int target = innings1.totalRuns + 1;
                int runsNeeded = max(0, target - state.totalRuns);
                int ballsLeft = max(0, oversPerInnings*6 - state.legalBallsBowledInInnings);
                double reqRR = 0.0;
                if (ballsLeft > 0) { double oversLeft = (double)ballsLeft/6.0; reqRR = (double)runsNeeded/oversLeft; } else reqRR = (runsNeeded>0)?9999.0:0.0;
                cout << "Required: " << runsNeeded << " runs from " << ballsLeft << " balls. Required RR: ";
                if (reqRR > 999.0) cout << "N/A\n"; else cout << fixed << setprecision(2) << reqRR << "\n";
            }

            if (&state == &innings2 && state.totalRuns > innings1.totalRuns) { cout << "\n Target achieved! " << state.battingTeam->getName() << " have chased down the target.\n"; state.inningsEndedEarly = true; break; }
        } // ball loop

        if (overInterruptedByInjury) continue;
        if (state.inningsEndedEarly) break;

        // After over completes, check maiden using robust method
        checkMaidenOver(state);

        swap(state.strikerIndex, state.nonStrikerIndex);

        cout << "\nOver completed. Score: " << state.totalRuns << "/" << state.totalWickets << "  Overs: " << getCurrentOverString(state) << "\n";

        if (&state == &innings1) {
            double oversCompleted = (double)state.legalBallsBowledInInnings / 6.0;
            double runRate = (oversCompleted>0.0)? (double)state.totalRuns/oversCompleted : 0.0;
            double adjustment = 1.0 - 0.05 * state.totalWickets; if (adjustment < 0.5) adjustment = 0.5;
            double projected = runRate * (double)oversPerInnings * adjustment; int projectedInt = (int)lrint(projected);
            cout << "Projected Score (adjusted by wickets): " << projectedInt << "\n";
        }

        bool underPressure=false;
        for (auto &p : state.battingTeam->getPlayers()) p.computeRating(underPressure);
        for (auto &p : state.bowlingTeam->getPlayers()) p.computeRating(underPressure);
        for (auto &p : state.battingTeam->getPlayers()) p.computeFantasyPoints(oversPerInnings);
        for (auto &p : state.bowlingTeam->getPlayers()) p.computeFantasyPoints(oversPerInnings);

        cout << "\n--- Batting card (" << state.battingTeam->getName() << ") ---\n"; state.battingTeam->displayBattingCard();
        cout << "\n--- Bowling card (" << state.bowlingTeam->getName() << ") ---\n"; state.bowlingTeam->displayBowlingCard();

        showPostOverMenu(state);
    } // innings loop

    bool underPressure=false;
    for (auto &p : state.battingTeam->getPlayers()) { p.computeRating(underPressure); p.computeFantasyPoints(oversPerInnings); }
    for (auto &p : state.bowlingTeam->getPlayers()) { p.computeRating(underPressure); p.computeFantasyPoints(oversPerInnings); }

    cout << "\nInnings ended for " << state.battingTeam->getName() << ".\n";
    state.battingTeam->displayBattingCard();
}

// processBall implementation
template<typename T>
void Match::processBall(InningsState &state, const T &input, bool countFantasy) {
    Team &bat = *state.battingTeam; Team &bowl = *state.bowlingTeam;
    if (state.currentBowlerIndex < 0 || state.currentBowlerIndex >= 11) { cout << "️ No bowler selected. Please select a bowler for the over.\n"; return; }
    Player &striker = bat.getPlayers()[state.strikerIndex];
    Player &bowler = bowl.getPlayers()[state.currentBowlerIndex];
ostringstream oss;
oss << input;
string s = oss.str();
    s = trim(s); string up = toUpper(s);
    int gidx = localBowlerIndex(&bowl, state.currentBowlerIndex);

    if (up == "B") {
        cout << " Byes - enter runs 0-6: ";
        int runs; while (!(cin >> runs) || runs < 0 || runs > 6) { cout << "Enter runs 0-6: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        bat.addRunsToTeam(runs); bat.addByes(runs); state.totalRuns += runs; state.partnershipRuns += runs; checkPartnership(state);
        bowler.addBowlingBall(0, true, false, runs==0); state.bowlerRunsInCurrentOver[state.currentBowlerIndex]+=0;
        // byes: legal delivery, batsman faced the ball -> counts as ball faced
        striker.addBallFaced();
        if (runs % 2 == 1) swap(state.strikerIndex, state.nonStrikerIndex);
        if (runs==0) cout<<"Dot ball - byes.\n"; else cout<<runs<<" bye(s) taken. Added to extras.\n";
        return;
    }
    if (up == "LB") {
        cout << " Leg Byes - enter runs 0-6: ";
        int runs; while (!(cin >> runs) || runs < 0 || runs > 6) { cout << "Enter runs 0-6: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        bat.addRunsToTeam(runs); bat.addLegByes(runs); state.totalRuns += runs; state.partnershipRuns += runs; checkPartnership(state);
        bowler.addBowlingBall(0, true, false, runs==0); state.bowlerRunsInCurrentOver[state.currentBowlerIndex]+=0;
        // leg byes: counts as batsman faced the ball
        striker.addBallFaced();
        if (runs % 2 == 1) swap(state.strikerIndex, state.nonStrikerIndex);
        if (runs==0) cout<<"Dot ball - leg byes.\n"; else cout<<runs<<" leg bye(s) taken. Added to extras.\n";
        return;
    }

    if (up == "W") {
        string wt;
        if (state.freeHitActive) {
            cout << " Free Hit active — normal dismissals (except run-out) are not out.\n";
cout << "Allowed dismissals on Free Hit:\n";
cout << "R  = Run Out\n";
cout << "T  = Timed Out\n";
cout << "O  = Obstructing the Field\n";
cout << "RI = Retired Out\n";

cout << "Enter dismissal type or N for Not Out: ";

string choice;

getline(cin, choice);

choice = toUpper(trim(choice));

if(choice=="N")
{
    cout<<"Not out due to Free Hit.\n";
    return;
}

if(choice!="R" &&
   choice!="T" &&
   choice!="O" &&
   choice!="RI")
{
    cout<<"Not out due to Free Hit.\n";
    return;
}
wt = choice;
        }
        else
{
            if (askYesNo("Wicket type instructions")) {
            cout << "Wicket type codes:\n C (Caught)  B (Bowled)  R (RunOut)  L (LBW)  S (Stumped)  H (HitWicket)  T (TimedOut)  O (Obstructing)  RI (RetiredOut)\n";
        }
      
    while(true)
    {
        cout<<"Enter code: ";

        getline(cin, wt);

        wt = toUpper(trim(wt));

        if(isValidWicketType(wt))
            break;

        cout<<" Invalid wicket type.\n";
    }
}

       
        bool wicketCreditedToBowler = false; string how = "out";
        if (wt == "C") {
            cout << "Caught by which fielder? (enter number 1-11 or exact name):\n";
            for (int i=0;i<11;++i) cout<<i+1<<". "<<bowl.getPlayers()[i].getName()<<"\n";
            string f; getline(cin,f); f = trim(f); int finder=-1;
            try { int idx = stoi(f)-1; if (idx>=0 && idx<11) finder = idx; } catch(...) { for (int i=0;i<11;++i) if (bowl.getPlayers()[i].getName()==f) { finder=i; break; } }
            if (finder != -1) { cout << "Enter catch rating (1-10): "; int cr = getValidCatchRating(); bowl.getPlayers()[finder].addCatch(cr); if (cr > state.highestCatchRating) { state.highestCatchRating = cr; state.highestCatchPlayer = bowl.getPlayers()[finder].getName(); } how = "c "+bowl.getPlayers()[finder].getName()+" b "+bowler.getName(); if (countFantasy) bowl.getPlayers()[finder].computeFantasyPoints(oversPerInnings); }
            else { cout << "Invalid fielder selection. Recorded as c (unknown).\n"; how = "c (unknown) b " + bowler.getName(); }
            wicketCreditedToBowler = true;
        } else if (wt=="B") { how = "b "+bowler.getName(); wicketCreditedToBowler=true; }
        else if (wt=="R") {
            cout << "Run out: How many runs were completed before the wicket? (0,1,2): ";
            int runsCompleted; while (!(cin>>runsCompleted) || runsCompleted<0 || runsCompleted>2) { cout<<"Enter 0 or 1 or 2: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Run out at which end? (S for striker end, N for non-striker end): ";
            char endAt; while (true) { cin>>endAt; endAt=toupper(endAt); if (endAt=='S'||endAt=='N') break; cout<<"Enter S or N: "; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Run out by which fielder? (1-11 or name):\n"; for (int i=0;i<11;++i) cout<<i+1<<". "<<bowl.getPlayers()[i].getName()<<"\n";
            string f; getline(cin,f); f=trim(f); int finder=-1; try { int idx = stoi(f)-1; if (idx>=0 && idx<11) finder=idx; } catch(...) { for (int i=0;i<11;++i) if (bowl.getPlayers()[i].getName()==f) { finder=i; break; } }
            if (runsCompleted > 0) {
                bat.addRunsToTeam(runsCompleted); striker.addRuns(runsCompleted); striker.addBallFaced();
                //if (runsCompleted==4) striker.addFour(); if (runsCompleted==6) striker.addSix();
                bowler.addBowlingBall(0, true, false, false);
                //state.bowlerRunsInCurrentOver[state.currentBowlerIndex]+=runsCompleted;
                if (countFantasy) striker.addFantasyPoints(runsCompleted + (runsCompleted==4?6:(runsCompleted==6?12:0)));
                state.partnershipRuns += runsCompleted; checkPartnership(state);
            } else {
                striker.addBallFaced(); bowler.addBowlingBall(0, true, false, true);
            }
            int outIndex = (endAt=='S')? state.strikerIndex : state.nonStrikerIndex;
            string outName = bat.getPlayers()[outIndex].getName();
            if (finder != -1) { bowl.getPlayers()[finder].addRunout(); if (countFantasy) bowl.getPlayers()[finder].computeFantasyPoints(oversPerInnings); how = "run out " + bowl.getPlayers()[finder].getName(); }
            else how = "run out";
            bat.getPlayers()[outIndex].setOut(how); bat.addWicketToTeam(); state.totalWickets++;
            bat.addFallOfWicket(state.totalRuns, outName);
            state.partnershipRuns = 0; state.lastMilestone = 0;
            selectNextBatsman(state,outIndex);
            cout << " WICKET (Run Out)! " << outName << " " << how << "\n";
            int base = (bowl.getName()==team1.getName())?0:11; globalConsecWickets[base + state.currentBowlerIndex] = 0;
            return;
        } else if (wt=="L") { how = "lbw " + bowler.getName(); wicketCreditedToBowler = true; }
        else if (wt=="S") { int keeper = bowl.getWicketKeeperIndex(); if (keeper>=0) { how = "stumped " + bowl.getPlayers()[keeper].getName(); 
        //if (countFantasy) bowl.getPlayers()[keeper].addRunout();
        } else how="stumped"; wicketCreditedToBowler = true; }
        else if (wt=="H") { how="hit wicket b " + bowler.getName(); wicketCreditedToBowler = true; }
        else if (wt=="T") { how="timed out"; wicketCreditedToBowler = false; }
        else if (wt=="O") { how="obstructing the field"; wicketCreditedToBowler = false; }
        else if (wt=="RI") { how="retired out"; wicketCreditedToBowler = false; }
        else { how="out"; wicketCreditedToBowler = true; }

        if (wt != "R") {
            striker.addBallFaced();
            string outName = striker.getName();
            bat.getPlayers()[state.strikerIndex].setOut(how); bat.addWicketToTeam(); state.totalWickets++;
            bat.addFallOfWicket(state.totalRuns, outName);

            if (wicketCreditedToBowler) {
                bowler.addBowlingBall(0, true, true, false);
                int base = (bowl.getName()==team1.getName())?0:11;
                globalConsecWickets[gidx] += 1;
                for (int i=0;i<11;++i) if (base + i != base + gidx) globalConsecWickets[base + i] = 0;
                if (globalConsecWickets[gidx] == 3 && !globalHattrickMarked[gidx]) {
                    cout << "\n HAT-TRICK! by " << bowler.getName() << " 🎯\n";
                    if (countFantasy) bowler.addFantasyPoints(30.0);
                    globalHattrickMarked[gidx] = true;
                }
            } else {
                int base = (bowl.getName()==team1.getName())?0:11; globalConsecWickets[base + state.currentBowlerIndex] = 0;
            }
            state.partnershipRuns = 0; state.lastMilestone = 0;
            selectNextBatsman(state, state.strikerIndex);
            cout << "⚡ WICKET! " << outName << " " << how << "\n";
            return;
        }
    }

    if (up == "WD") {
        cout << " Wide ball. Base +1 extra. Enter additional runs scored on this delivery (0 if none): ";
        int extra=0; while (!(cin>>extra)||extra<0) { cout<<"Enter non-negative integer: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        int totalExtra = 1 + extra; bat.addRunsToTeam(totalExtra); bat.addWides(totalExtra); state.totalRuns += totalExtra; state.partnershipRuns += totalExtra; checkPartnership(state);
        bowler.addBowlingBall(totalExtra, false, false, false); state.bowlerRunsInCurrentOver[state.currentBowlerIndex] += totalExtra;
        if (extra % 2 == 1) swap(state.strikerIndex, state.nonStrikerIndex);
        int base = (bowl.getName()==team1.getName())?0:11; globalConsecWickets[base + state.currentBowlerIndex] = 0;
        if (state.freeHitActive) cout << "Free Hit continues (wide on free-hit). Free hit remains active until a legal delivery.\n";
        return;
    }

if (up == "NB") 
    {
  cout << "Type of no-ball?\n";
cout << "1. Runs off bat\n";
cout << "2. Byes\n";
cout << "3. Leg Byes\n";

int nbType;

while(!(cin>>nbType) || nbType<1 || nbType>3)
{
    cout<<"Enter 1-3: ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

cout<<"Enter additional runs (0-6): ";

int runs;

while(!(cin>>runs) || runs<0 || runs>6)
{
    cout<<"Enter 0-6: ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

cin.ignore(numeric_limits<streamsize>::max(),'\n');
bat.addRunsToTeam(1 + runs);
state.totalRuns += (1 + runs);
state.partnershipRuns += (1 + runs);

checkPartnership(state);

bowler.addBowlingBall(1,false,false,false);
state.bowlerRunsInCurrentOver[state.currentBowlerIndex] += 1;
if(nbType==1)
{
    bat.addNoBalls(1);

    striker.addRuns(runs);
    striker.addBallFaced();
    if(runs%2==1)
        swap(state.strikerIndex,state.nonStrikerIndex);
}
else if(nbType==2)
{
    bat.addNoBalls(1);
    bat.addByes(runs);

    //striker.addBallFaced();

    if(runs%2==1)
        swap(state.strikerIndex,state.nonStrikerIndex);
}
else
{
    bat.addNoBalls(1);
    bat.addLegByes(runs);

   // striker.addBallFaced();

    if(runs%2==1)
        swap(state.strikerIndex,state.nonStrikerIndex);
}
state.freeHitActive = true;

int base = (bowl.getName()==team1.getName())?0:11;
globalConsecWickets[base + state.currentBowlerIndex] = 0;

cout << "Free Hit loaded for next delivery.\n";
return;
}

    // numeric runs
    try {
        int runs = stoi(s);
        if (runs < 0) runs = 0;
        if (runs > 6) { cout << " Invalid runs > 6 detected. Ignoring this input.\n"; return; }
        bat.addRunsToTeam(runs); striker.addRuns(runs); striker.addBallFaced();
        bool isDotBall = (runs==0);
        bowler.addBowlingBall(runs, true, false, isDotBall);
        state.bowlerRunsInCurrentOver[state.currentBowlerIndex] += runs; state.totalRuns += runs;
        checkMilestones(state, runs);
        state.partnershipRuns += runs; checkPartnership(state);
        if (countFantasy) { striker.addFantasyPoints(runs); if (runs==4) striker.addFantasyPoints(6); if (runs==6) striker.addFantasyPoints(12); }
        if (runs % 2 == 1) swap(state.strikerIndex, state.nonStrikerIndex);
        if (runs==4) cout<<" FOUR!\n"; if (runs==6) cout<<" SIX!\n"; if (isDotBall) cout<<"Dot ball.\n";
        state.freeHitActive = false;
    } catch(...) { cout << " Invalid ball input. Ignored.\n"; }
}
template void Match::processBall<string>(InningsState &state, const string &input, bool countFantasy);

// post over menu
void Match::showPostOverMenu(InningsState &state) {
    if (state.inningsEndedEarly || state.legalBallsBowledInInnings >= oversPerInnings*6 || state.totalWickets >= 10) return;
    while (true) {
        if (askYesNo("Show post-over menu")) {
            cout << "Options: (1) Continue (2) View Batting Card (3) View Bowling Card (4) View Fantasy Points (5) Quit Match\n";
        }
        cout << "Enter choice: ";
        int c;
        if (!(cin >> c)) { cout << "Invalid input. Please enter a number 1-5.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (c==1) break;
        else if (c==2) state.battingTeam->displayBattingCard();
        else if (c==3) state.bowlingTeam->displayBowlingCard();
        else if (c==4) {
            cout << "\n Fantasy Points - " << state.battingTeam->getName() << "\n";
            for (const auto &p : state.battingTeam->getPlayers()) cout << left << setw(20) << p.getName() << " : " << fixed << setprecision(1) << p.getFantasyPoints() << "\n";
            cout << "\n Fantasy Points - " << state.bowlingTeam->getName() << "\n";
            for (const auto &p : state.bowlingTeam->getPlayers()) cout << left << setw(20) << p.getName() << " : " << fixed << setprecision(1) << p.getFantasyPoints() << "\n";
        } else if (c==5) { if (askYesNo("Are you sure you want to quit the match?")) { state.inningsEndedEarly = true; break; } }
        else cout << "Invalid choice. Enter a number 1-5.\n";
    }
}

void Match::changeBattingOrder(InningsState &state) { cout << "Change batting order feature not implemented.\n"; }

void Match::displayFinalResult() {
    cout << "\n=== Match Result ===\n";
    cout << team1.getName() << ": " << team1.getTotalRuns() << "/" << team1.getTotalWickets() << " (" << team1.getLegalBalls()/6 << "." << team1.getLegalBalls()%6 << ")\n";
    cout << team2.getName() << ": " << team2.getTotalRuns() << "/" << team2.getTotalWickets() << " (" << team2.getLegalBalls()/6 << "." << team2.getLegalBalls()%6 << ")\n";
    if (team1.getTotalRuns() > team2.getTotalRuns()) cout << "Winner: " << team1.getName() << " by " << team1.getTotalRuns() - team2.getTotalRuns() << " runs.\n";
    else if (team2.getTotalRuns() > team1.getTotalRuns()) cout << "Winner: " << team2.getName() << " by " << 10 - team2.getTotalWickets() << " wickets.\n";
    else cout << "Match tied.\n";
Team *winnerTeam = nullptr;
Team *loserTeam = nullptr;

if (team1.getTotalRuns() > team2.getTotalRuns())
{
    winnerTeam = &team1;
    loserTeam = &team2;
}
else if (team2.getTotalRuns() > team1.getTotalRuns())
{
    winnerTeam = &team2;
    loserTeam = &team1;
}

double winnerBestPts = -1;
string winnerBestName;

double loserBestPts = -1;
string loserBestName;

if (winnerTeam != nullptr)
{
    for (auto &p : winnerTeam->getPlayers())
    {
        if (p.getFantasyPoints() > winnerBestPts)
        {
            winnerBestPts = p.getFantasyPoints();
            winnerBestName = p.getName();
        }
    }

    for (auto &p : loserTeam->getPlayers())
    {
        if (p.getFantasyPoints() > loserBestPts)
        {
            loserBestPts = p.getFantasyPoints();
            loserBestName = p.getName();
        }
    }

    string momName;
    double momPoints;

    if (loserBestPts - winnerBestPts >=  (oversPerInnings * 6))
    {
        momName = loserBestName;
        momPoints = loserBestPts;
    }
    else
    {
        momName = winnerBestName;
        momPoints = winnerBestPts;
    }

    cout << "\nMan of the Match\n";
    cout << momName
         << " (Fantasy Points: "
         << fixed << setprecision(1)
         << momPoints
         << ")\n";
}
else
{
    cout << "\nMan of the Match: Match Tied\n";
}
string bestCatchPlayer;
int bestCatchRating = 0;


if(innings1.highestCatchRating > bestCatchRating)
{
    bestCatchRating = innings1.highestCatchRating;
    bestCatchPlayer = innings1.highestCatchPlayer;
}

if(innings2.highestCatchRating > bestCatchRating)
{
    bestCatchRating = innings2.highestCatchRating;
    bestCatchPlayer = innings2.highestCatchPlayer;
}


cout << "\nBest Catch of the Match:\n";

if(bestCatchRating > 0)
{
    cout << bestCatchPlayer
         << " (Rating: "
         << bestCatchRating
         << "/10)\n";
}
else
{
    cout << "No catches taken.\n";
}

cout << "\n====================================\n";
cout << "INNINGS SUMMARIES\n";
cout << "====================================\n";
vector<Player> team1Batters = team1.getPlayers();

sort(team1Batters.begin(), team1Batters.end(),
[](const Player &a, const Player &b)
{
    return a.getRuns() > b.getRuns();
});

cout << "\n " << team1.getName() << " Top Batters\n";

for(int i=0;i<2;i++)
{
    cout << i+1 << ". "
         << team1Batters[i].getName()
         << " - "
         << team1Batters[i].getRuns()
         << " ("
         << team1Batters[i].getBallsFaced()
         << ")\n";
}
vector<Player> team2Bowlers;

for(auto &p : team2.getPlayers())
{
    if(p.getBallsBowled() > 0)
        team2Bowlers.push_back(p);
}


sort(team2Bowlers.begin(), team2Bowlers.end(),
[](const Player &a, const Player &b)
{
    if(a.getWickets() != b.getWickets())
        return a.getWickets() > b.getWickets();

    return a.getRunsConceded() < b.getRunsConceded();
});

cout << "\n " << team2.getName() << " Top Bowlers\n";

for(int i=0;i<2;i++)
{
    cout << i+1 << ". "
         << team2Bowlers[i].getName()
         << " - "
         << team2Bowlers[i].getWickets()
         << "/"
         << team2Bowlers[i].getRunsConceded()
         << "\n";
}
vector<Player> team2Batters = team2.getPlayers();

sort(team2Batters.begin(), team2Batters.end(),
[](const Player &a, const Player &b)
{
    return a.getRuns() > b.getRuns();
});

cout << "\n" << team2.getName() << " Top Batters\n";

for(int i=0;i<2;i++)
{
    cout << i+1 << ". "
         << team2Batters[i].getName()
         << " - "
         << team2Batters[i].getRuns()
         << " ("
         << team2Batters[i].getBallsFaced()
         << ")\n";
}
vector<Player> team1Bowlers;

for(auto &p : team1.getPlayers())
{
    if(p.getBallsBowled() > 0)
        team1Bowlers.push_back(p);
}

sort(team1Bowlers.begin(), team1Bowlers.end(),
[](const Player &a, const Player &b)
{
    if(a.getWickets() != b.getWickets())
        return a.getWickets() > b.getWickets();

    return a.getRunsConceded() < b.getRunsConceded();
});

cout << "\n" << team1.getName() << " Top Bowlers\n";

for(int i=0;i<2;i++)
{
    cout << i+1 << ". "
         << team1Bowlers[i].getName()
         << " - "
         << team1Bowlers[i].getWickets()
         << "/"
         << team1Bowlers[i].getRunsConceded()
         << "\n";
}
    try {
        const Player &top1 = utils::getTopPerformer(team1.getPlayers());
        const Player &top2 = utils::getTopPerformer(team2.getPlayers());
        cout << "\nTop scorers (team-wise):\n";
        cout << team1.getName() << " : " << top1 << "\n";
        cout << team2.getName() << " : " << top2 << "\n";
    } catch(...) {
        // ignore if not available
    }
}

