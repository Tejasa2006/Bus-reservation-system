/*#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

const string SEAT_FILE = "seats.dat";
const string BOOKING_FILE = "bookings.csv";
const int TOTAL_SEATS = 20;

vector<int> loadSeats() {
    vector<int> seats(TOTAL_SEATS+1, 0);
    ifstream in(SEAT_FILE);
    if (!in.is_open()) {
        // initialize file
        ofstream out(SEAT_FILE);
        for (int i = 1; i <= TOTAL_SEATS; ++i) {
            out << 0 << (i==TOTAL_SEATS ? "" : ",");
        }
        out.close();
        return seats;
    }
    string line;
    if (!getline(in, line)) return seats;
    in.close();
    stringstream ss(line);
    string token;
    int idx = 1;
    while (getline(ss, token, ',') && idx <= TOTAL_SEATS) {
        seats[idx++] = stoi(token);
    }
    return seats;
}

bool saveSeats(const vector<int>& seats) {
    ofstream out(SEAT_FILE, ios::trunc);
    if (!out.is_open()) return false;
    for (int i = 1; i <= TOTAL_SEATS; ++i) {
        out << seats[i];
        if (i != TOTAL_SEATS) out << ",";
    }
    out.close();
    return true;
}

string jsonEscape(const string &s){
    string out;
    for (char c: s) {
        if (c == '\"') out += "\\\\\"";
        else if (c == '\\\\') out += "\\\\\\\\";
        else out.push_back(c);
    }
    return out;
}

string nowIso() {
    time_t t = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", localtime(&t));
    return string(buf);
}

void appendBooking(const string &id, const string &name, int seat) {
    ofstream out(BOOKING_FILE, ios::app);
    if (!out.is_open()) return;
    out << id << "," << name << "," << seat << "," << nowIso() << "\n";
    out.close();
}

string generateId() {
    long long x = time(nullptr);
    return string("BK") + to_string(x % 10000000);
}

int main(int argc, char* argv[]) {
    // Simple commands:
    // available            -> {"available":[1,2,...],"booked":[...]}
    // list                 -> {"seats":[0,1,0,...]}
    // book <seat> <name>   -> {"success":true,"id":"BK...","seat":5}
    // cancel <seat>        -> {"success":true,"seat":5}
    // bookings             -> outputs CSV lines as simple JSON array of objects
    vector<int> seats = loadSeats();

    if (argc < 2) {
        cout << "{\"error\":\"no command\"}";
        return 0;
    }

    string cmd = argv[1];

    if (cmd == "available") {
        cout << "{";
        cout << "\"available\":[";
        bool first = true;
        for (int i = 1; i <= TOTAL_SEATS; ++i) if (seats[i] == 0) {
            if (!first) cout << ",";
            cout << i;
            first = false;
        }
        cout << "],\"booked\":[";
        first = true;
        for (int i = 1; i <= TOTAL_SEATS; ++i) if (seats[i] == 1) {
            if (!first) cout << ",";
            cout << i;
            first = false;
        }
        cout << "]}";
        return 0;
    } else if (cmd == "list") {
        cout << "{\"seats\":[";
        for (int i = 1; i <= TOTAL_SEATS; ++i) {
            cout << seats[i];
            if (i != TOTAL_SEATS) cout << ",";
        }
        cout << "]}";
        return 0;
    } else if (cmd == "book") {
        if (argc < 4) {
            cout << "{\"error\":\"usage: book <seat> <name>\"}";
            return 0;
        }
        int seat = stoi(argv[2]);
        string name = argv[3];
        if (seat < 1 || seat > TOTAL_SEATS) {
            cout << "{\"success\":false,\"error\":\"invalid seat\"}";
            return 0;
        }
        if (seats[seat] == 1) {
            cout << "{\"success\":false,\"error\":\"already booked\"}";
            return 0;
        }
        seats[seat] = 1;
        if (!saveSeats(seats)) {
            cout << "{\"success\":false,\"error\":\"save failed\"}";
            return 0;
        }
        string id = generateId();
        appendBooking(id, name, seat);
        cout << "{\"success\":true,\"id\":\"" << id << "\",\"seat\":" << seat << "}";
        return 0;
    } else if (cmd == "cancel") {
        if (argc < 3) {
            cout << "{\"error\":\"usage: cancel <seat>\"}";
            return 0;
        }
        int seat = stoi(argv[2]);
        if (seat < 1 || seat > TOTAL_SEATS) {
            cout << "{\"success\":false,\"error\":\"invalid seat\"}";
            return 0;
        }
        if (seats[seat] == 0) {
            cout << "{\"success\":false,\"error\":\"seat not booked\"}";
            return 0;
        }
        seats[seat] = 0;
        if (!saveSeats(seats)) {
            cout << "{\"success\":false,\"error\":\"save failed\"}";
            return 0;
        }
        cout << "{\"success\":true,\"seat\":" << seat << "}";
        return 0;
    } else if (cmd == "bookings") {
        // read bookings.csv and output JSON array
        ifstream in(BOOKING_FILE);
        cout << "{\"bookings\":[";
        bool first = true;
        if (in.is_open()) {
            string line;
            while (getline(in, line)) {
                if (line.empty()) continue;
                // CSV: id,name,seat,date
                stringstream ss(line);
                string id, name, seat, date;
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, seat, ',');
                getline(ss, date, ',');
                if (!first) cout << ",";
                cout << "{\"id\":\"" << jsonEscape(id) << "\",\"name\":\"" << jsonEscape(name) << "\",\"seat\":" << stoi(seat) << ",\"date\":\"" << jsonEscape(date) << "\"}";
                first = false;
            }
            in.close();
        }
        cout << "]}";
        return 0;
    }

    cout << "{\"error\":\"unknown command\"}";
    return 0;
}
*/

// reservation.cpp
// Advanced file-based Bus Reservation System (single-file implementation)
// Compile: g++ reservation.cpp -o reservation -std=c++17

#include <bits/stdc++.h>
using namespace std;
const string BUSES_FILE = "buses.json";
const string BOOKINGS_FILE = "bookings.csv";
const string SEAT_FILE_PREFIX = "seats_"; // seats_<busid>.dat

// -------------------- Utilities --------------------
string nowIso() {
    time_t t = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", localtime(&t));
    return string(buf);
}
string escapeJson(const string &s) {
    string out;
    for (char c: s) {
        if (c == '\"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out.push_back(c);
    }
    return out;
}
vector<string> split(const string &s, char delim) {
    vector<string> out; string cur; stringstream ss(s);
    while (getline(ss, cur, delim)) out.push_back(cur);
    return out;
}

// -------------------- Models --------------------
struct Bus {
    string id;          // machine id (unique)
    string number;      // bus number/plate
    string from;
    string to;
    string date;        // YYYY-MM-DD
    string depart_time; // HH:MM
    string type;        // AC/Non-AC/Sleeper
    int seats;          // total seats
    double fare;        // base fare
};

struct Booking {
    string pnr;
    string name;
    string phone;
    string busid;
    int seat;
    double fare;
    string datetime;
};

// -------------------- Storage helpers --------------------
vector<Bus> loadBuses() {
    vector<Bus> res;
    ifstream in(BUSES_FILE);
    if (!in.is_open()) return res;
    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();
    // very small JSON parser tailored for our format: array of objects with simple fields
    // expecting format: [{"id":"...","number":"...","from":"...","to":"...","date":"...","depart_time":"...","type":"...","seats":20,"fare":150.0}, ...]
    size_t pos = 0;
    while ((pos = content.find("{", pos)) != string::npos) {
        size_t end = content.find("}", pos);
        if (end == string::npos) break;
        string obj = content.substr(pos+1, end - pos - 1);
        Bus b;
        vector<string> parts = split(obj, ',');
        for (auto &p: parts) {
            size_t colon = p.find(':');
            if (colon == string::npos) continue;
            string key = p.substr(0, colon);
            string val = p.substr(colon+1);
            // trim quotes/spaces
            auto trim = [](string s){ while(!s.empty() && isspace(s.front())) s.erase(s.begin()); while(!s.empty() && isspace(s.back())) s.pop_back(); if (!s.empty() && s.front()=='\"') s.erase(s.begin()); if (!s.empty() && s.back()=='\"') s.pop_back(); return s; };
            key = trim(key); val = trim(val);
            if (key == "\"id\"" || key == "id") b.id = val;
            else if (key == "\"number\"" || key == "number") b.number = val;
            else if (key == "\"from\"" || key == "from") b.from = val;
            else if (key == "\"to\"" || key == "to") b.to = val;
            else if (key == "\"date\"" || key == "date") b.date = val;
            else if (key == "\"depart_time\"" || key == "depart_time") b.depart_time = val;
            else if (key == "\"type\"" || key == "type") b.type = val;
            else if (key == "\"seats\"" || key == "seats") b.seats = stoi(val);
            else if (key == "\"fare\"" || key == "fare") b.fare = stod(val);
        }
        res.push_back(b);
        pos = end + 1;
    }
    return res;
}

bool saveBuses(const vector<Bus> &buses) {
    ofstream out(BUSES_FILE, ios::trunc);
    if (!out.is_open()) return false;
    out << "[";
    for (size_t i=0;i<buses.size();++i) {
        const Bus &b = buses[i];
        out << "{";
        out << "\"id\":\"" << escapeJson(b.id) << "\",";
        out << "\"number\":\"" << escapeJson(b.number) << "\",";
        out << "\"from\":\"" << escapeJson(b.from) << "\",";
        out << "\"to\":\"" << escapeJson(b.to) << "\",";
        out << "\"date\":\"" << escapeJson(b.date) << "\",";
        out << "\"depart_time\":\"" << escapeJson(b.depart_time) << "\",";
        out << "\"type\":\"" << escapeJson(b.type) << "\",";
        out << "\"seats\":" << b.seats << ",";
        out << "\"fare\":" << b.fare;
        out << "}";
        if (i+1<buses.size()) out << ",";
    }
    out << "]";
    out.close();
    return true;
}

string seatsFilename(const string &busid) {
    return SEAT_FILE_PREFIX + busid + ".dat";
}

vector<int> loadSeatsFor(const Bus &b) {
    vector<int> seats(b.seats+1,0);
    ifstream in(seatsFilename(b.id));
    if (!in.is_open()) {
        // initialize file
        ofstream out(seatsFilename(b.id), ios::trunc);
        for (int i=1;i<=b.seats;++i) {
            out << 0;
            if (i!=b.seats) out << ",";
        }
        out.close();
        return seats;
    }
    string line;
    if (!getline(in, line)) return seats;
    in.close();
    vector<string> toks = split(line, ',');
    for (size_t i=0;i<toks.size() && i+1<seats.size(); ++i) seats[i+1] = stoi(toks[i]);
    return seats;
}

bool saveSeatsFor(const Bus &b, const vector<int> &seats) {
    ofstream out(seatsFilename(b.id), ios::trunc);
    if (!out.is_open()) return false;
    for (int i=1;i<=b.seats;++i) {
        out << seats[i];
        if (i!=b.seats) out << ",";
    }
    out.close();
    return true;
}

// -------------------- Booking management --------------------
string genId() {
    // simple id using time + rand
    long long x = time(nullptr);
    int r = rand() % 1000;
    return "BK" + to_string(x%10000000) + to_string(r);
}
string genPnr() {
    long long x = time(nullptr);
    int r = rand() % 900 + 100;
    stringstream ss;
    ss << "PNR" << (x % 10000000) << r;
    return ss.str();
}

bool appendBookingCsv(const Booking &bk) {
    bool exist = ifstream(BOOKINGS_FILE).good();
    ofstream out(BOOKINGS_FILE, ios::app);
    if (!out.is_open()) return false;
    // CSV header if new
    if (!exist) out << "pnr,name,phone,busid,seat,fare,datetime\n";
    out << bk.pnr << "," << "\"" << bk.name << "\"," << "\"" << bk.phone << "\"," << bk.busid << "," << bk.seat << "," << bk.fare << "," << bk.datetime << "\n";
    out.close();
    return true;
}

// -------------------- Commands --------------------
void cmd_list_buses() {
    auto buses = loadBuses();
    cout << "{\"buses\":[";
    for (size_t i=0;i<buses.size();++i) {
        auto &b = buses[i];
        cout << "{\"id\":\"" << escapeJson(b.id) << "\",\"number\":\"" << escapeJson(b.number) << "\",\"from\":\"" << escapeJson(b.from) << "\",\"to\":\"" << escapeJson(b.to) << "\",\"date\":\"" << escapeJson(b.date) << "\",\"depart_time\":\"" << escapeJson(b.depart_time) << "\",\"type\":\"" << escapeJson(b.type) << "\",\"seats\":" << b.seats << ",\"fare\":" << b.fare << "}";
        if (i+1<buses.size()) cout << ",";
    }
    cout << "]}";
}

void cmd_search(int argc, char* argv[]) {
    // search <from> <to> [date]
    if (argc < 4) { cout << "{\"error\":\"usage: search <from> <to> [date]\"}"; return; }
    string from = argv[2]; string to = argv[3]; string date = (argc>=5? argv[4] : "");
    auto buses = loadBuses();
    cout << "{\"buses\":[";
    bool first = true;
    for (auto &b: buses) {
        bool m = (b.from == from || b.from.find(from) != string::npos) && (b.to == to || b.to.find(to) != string::npos);
        if (!date.empty()) m = m && (b.date == date);
        if (m) {
            if (!first) cout << ",";
            cout << "{\"id\":\"" << escapeJson(b.id) << "\",\"number\":\"" << escapeJson(b.number) << "\",\"from\":\"" << escapeJson(b.from) << "\",\"to\":\"" << escapeJson(b.to) << "\",\"date\":\"" << escapeJson(b.date) << "\",\"depart_time\":\"" << escapeJson(b.depart_time) << "\",\"type\":\"" << escapeJson(b.type) << "\",\"seats\":" << b.seats << ",\"fare\":" << b.fare << "}";
            first = false;
        }
    }
    cout << "]}";
}

void cmd_seats(int argc, char* argv[]) {
    // seats <busid>
    if (argc < 3) { cout << "{\"error\":\"usage: seats <busid>\"}"; return; }
    string busid = argv[2];
    auto buses = loadBuses();
    for (auto &b: buses) if (b.id == busid) {
        auto seats = loadSeatsFor(b);
        cout << "{\"bus\":{\"id\":\"" << escapeJson(b.id) << "\",\"number\":\"" << escapeJson(b.number) << "\"},\"seats\":[";
        for (int i=1;i<=b.seats;++i) {
            cout << seats[i];
            if (i!=b.seats) cout << ",";
        }
        cout << "],\"fare\":" << b.fare << "}";
        return;
    }
    cout << "{\"error\":\"bus not found\"}";
}

void cmd_book(int argc, char* argv[]) {
    // book <busid> <seat> <name> <phone>
    if (argc < 6) { cout << "{\"error\":\"usage: book <busid> <seat> <name> <phone>\"}"; return; }
    string busid = argv[2];
    int seat = stoi(argv[3]);
    string name = argv[4];
    string phone = argv[5];
    auto buses = loadBuses();
    for (auto &b: buses) if (b.id == busid) {
        if (seat < 1 || seat > b.seats) { cout << "{\"success\":false,\"error\":\"invalid seat\"}"; return; }
        auto seats = loadSeatsFor(b);
        if (seats[seat] == 1) { cout << "{\"success\":false,\"error\":\"already booked\"}"; return; }
        seats[seat] = 1;
        if (!saveSeatsFor(b, seats)) { cout << "{\"success\":false,\"error\":\"failed to save seats\"}"; return; }
        Booking bk;
        bk.pnr = genPnr();
        bk.name = name;
        bk.phone = phone;
        bk.busid = b.id;
        bk.seat = seat;
        bk.fare = b.fare;
        bk.datetime = nowIso();
        appendBookingCsv(bk);
        cout << "{\"success\":true,\"pnr\":\"" << escapeJson(bk.pnr) << "\",\"seat\":" << seat << ",\"fare\":" << bk.fare << "}";
        return;
    }
    cout << "{\"success\":false,\"error\":\"bus not found\"}";
}

void cmd_cancel(int argc, char* argv[]) {
    // cancel <pnr>
    if (argc < 3) { cout << "{\"error\":\"usage: cancel <pnr>\"}"; return; }
    string pnr = argv[2];
    // read bookings, find pnr
    ifstream in(BOOKINGS_FILE);
    if (!in.is_open()) { cout << "{\"error\":\"no bookings\"}"; return; }
    vector<string> lines; string hdr;
    getline(in, hdr);
    string line;
    bool found=false;
    string busid; int seat=0;
    while (getline(in, line)) {
        if (line.empty()) continue;
        // pnr,name,phone,busid,seat,fare,datetime
        vector<string> cols;
        // naive CSV parse (we assume commas inside quotes not used except in name/phone but we used quotes)
        // split respecting quotes - simple approach:
        string cur; bool inq=false;
        for (size_t i=0;i<line.size();++i) {
            char c = line[i];
            if (c=='\"') inq = !inq;
            else if (c==',' && !inq) { cols.push_back(cur); cur.clear(); }
            else cur.push_back(c);
        }
        cols.push_back(cur);
        if (!cols.empty() && cols[0]==pnr) {
            found=true;
            busid = cols[3];
            seat = stoi(cols[4]);
            // mark this line as removed by skipping it in written file
        } else {
            lines.push_back(line);
        }
    }
    in.close();
    if (!found) { cout << "{\"success\":false,\"error\":\"pnr not found\"}"; return; }
    // write back bookings (without canceled)
    ofstream out(BOOKINGS_FILE, ios::trunc);
    out << hdr << "\n";
    for (auto &l: lines) out << l << "\n";
    out.close();
    // free seat in seat file
    auto buses = loadBuses();
    for (auto &b: buses) if (b.id == busid) {
        auto seats = loadSeatsFor(b);
        if (seat>=1 && seat<=b.seats) seats[seat] = 0;
        saveSeatsFor(b, seats);
        break;
    }
    cout << "{\"success\":true,\"pnr\":\"" << escapeJson(pnr) << "\"}";
}

void cmd_bookings() {
    ifstream in(BOOKINGS_FILE);
    cout << "{\"bookings\":[";
    if (!in.is_open()) { cout << "]}"; return; }
    string hdr; getline(in,hdr);
    string line; bool first=true;
    while (getline(in,line)) {
        if (line.empty()) continue;
        // split to fields roughly
        vector<string> cols;
        string cur; bool inq=false;
        for (size_t i=0;i<line.size();++i) {
            char c=line[i];
            if (c=='\"') inq=!inq;
            else if (c==',' && !inq) { cols.push_back(cur); cur.clear(); }
            else cur.push_back(c);
        }
        cols.push_back(cur);
        if (cols.size()<7) continue;
        if (!first) cout << ",";
        cout << "{\"pnr\":\"" << escapeJson(cols[0]) << "\",\"name\":\"" << escapeJson(cols[1]) << "\",\"phone\":\"" << escapeJson(cols[2]) << "\",\"busid\":\"" << escapeJson(cols[3]) << "\",\"seat\":" << stoi(cols[4]) << ",\"fare\":" << stod(cols[5]) << ",\"datetime\":\"" << escapeJson(cols[6]) << "\"}";
        first=false;
    }
    in.close();
    cout << "]}";
}

// -------------------- Admin commands (simple) --------------------
void cmd_admin_addbus(int argc, char* argv[]) {
    // admin_addbus <number> <from> <to> <date> <time> <type> <seats> <fare>
    if (argc < 10) { cout << "{\"error\":\"usage: admin_addbus <number> <from> <to> <date> <time> <type> <seats> <fare>\"}"; return; }
    Bus b;
    b.id = genId();
    b.number = argv[2];
    b.from = argv[3];
    b.to = argv[4];
    b.date = argv[5];
    b.depart_time = argv[6];
    b.type = argv[7];
    b.seats = stoi(argv[8]);
    b.fare = stod(argv[9]);
    auto buses = loadBuses();
    buses.push_back(b);
    if (!saveBuses(buses)) { cout << "{\"success\":false,\"error\":\"failed to save buses\"}"; return; }
    // create seats file
    vector<int> seats(b.seats+1,0);
    saveSeatsFor(b, seats);
    cout << "{\"success\":true,\"id\":\"" << escapeJson(b.id) << "\"}";
}

void cmd_admin_removebus(int argc, char* argv[]) {
    // admin_removebus <busid>
    if (argc < 3) { cout << "{\"error\":\"usage: admin_removebus <busid>\"}"; return; }
    string id = argv[2];
    auto buses = loadBuses();
    vector<Bus> kept;
    bool found=false;
    for (auto &b: buses) {
        if (b.id == id) { found=true; continue; }
        kept.push_back(b);
    }
    if (!found) { cout << "{\"success\":false,\"error\":\"bus not found\"}"; return; }
    saveBuses(kept);
    // optionally remove seats file
    remove(seatsFilename(id).c_str());
    cout << "{\"success\":true}";
}

void cmd_admin_updatebus(int argc, char* argv[]) {
    // admin_updatebus <busid> <field> <value>  (field: number/from/to/date/depart_time/type/seats/fare)
    if (argc < 5) { cout << "{\"error\":\"usage: admin_updatebus <busid> <field> <value>\"}"; return; }
    string id = argv[2], field = argv[3], value = argv[4];
    auto buses = loadBuses();
    bool found=false;
    for (auto &b: buses) if (b.id == id) {
        found=true;
        if (field=="number") b.number=value;
        else if (field=="from") b.from=value;
        else if (field=="to") b.to=value;
        else if (field=="date") b.date=value;
        else if (field=="depart_time") b.depart_time=value;
        else if (field=="type") b.type=value;
        else if (field=="seats") {
            int newSeats = stoi(value);
            if (newSeats < b.seats) {
                // shrink seats file by trimming trailing seats if free
                auto seats = loadSeatsFor(b);
                if ((int)seats.size()-1 >= newSeats) {
                    seats.resize(newSeats+1);
                }
            } else if (newSeats > b.seats) {
                auto seats = loadSeatsFor(b);
                seats.resize(newSeats+1, 0);
                saveSeatsFor(b, seats);
            }
            b.seats = newSeats;
        }
        else if (field=="fare") b.fare = stod(value);
    }
    if (!found) { cout << "{\"success\":false,\"error\":\"bus not found\"}"; return; }
    saveBuses(buses);
    cout << "{\"success\":true}";
}

// -------------------- Main --------------------
int main(int argc, char* argv[]) {
    srand(time(nullptr));
    if (argc < 2) {
        cout << "{\"error\":\"no command provided\"}";
        return 0;
    }
    string cmd = argv[1];
    if (cmd == string("list_buses")) cmd_list_buses();
    else if (cmd == string("search")) cmd_search(argc, argv);
    else if (cmd == string("seats")) cmd_seats(argc, argv);
    else if (cmd == string("book")) cmd_book(argc, argv);
    else if (cmd == string("cancel")) cmd_cancel(argc, argv);
    else if (cmd == string("bookings")) cmd_bookings();
    else if (cmd == string("admin_addbus")) cmd_admin_addbus(argc, argv);
    else if (cmd == string("admin_removebus")) cmd_admin_removebus(argc, argv);
    else if (cmd == string("admin_updatebus")) cmd_admin_updatebus(argc, argv);
    else cout << "{\"error\":\"unknown command\"}";
    return 0;
}
