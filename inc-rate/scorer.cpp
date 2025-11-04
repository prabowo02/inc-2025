#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ifstream inp;
ifstream out;
ifstream con;

void ac(std::string reason="") {
    std::cout << "AC\n";
    if (reason != "") {
        std::cout << reason << '\n';
    }
    exit(0);
}

void wa(std::string reason="") {
    std::cout << "WA\n";
    if (reason != "") {
        std::cout << reason << '\n';
    }
    exit(0);
}

void ok(double points, std::string reason="") {
    std::cout << "OK\n";
    std::cout << points;
    if (reason != "") {
        std::cout << " " << reason << '\n';
    }
    exit(0);
}

void registerScorer(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Must be run with arguments [input-file] [output-file] [contestant-output]\n";
        exit(0);
    }

    inp = std::ifstream(argv[1]);
    out = std::ifstream(argv[2]);
    con = std::ifstream(argv[3]);
}

template<class T> inline void readStream(std::ifstream &ifs, T &t) { if (!(ifs >> t)) wa(); }

std::string readString(std::ifstream &ifs) {
    std::string s;
    readStream(ifs, s);
    return s;
}

int readInt(std::ifstream &ifs) {
    int x;
    readStream(ifs, x);
    return x;
}

void eof() {
    std::string dummy;
    if (con >> dummy) wa();
}

string X;
int L, R;

vector<int> JURY, CONT;
bool imposJURY, imposCONT;

void getInput() {
    readStream(inp, X);
    readStream(inp, L);
    readStream(inp, R);

    int jurySz, conSz;

    {
        // read JURY output
        readStream(out, jurySz);

        if(jurySz == -1) imposJURY = true;
        else {
            JURY.resize(jurySz);
            for(auto &i : JURY)
                readStream(out, i);
        }
    }

    {
        // read CONTESTANT output
        int sz;
        readStream(con, conSz);

        if (conSz != jurySz) wa();

        if(conSz == -1) imposCONT = true;
        else {
            CONT.resize(conSz);
            for(auto &i : CONT) {
                readStream(con, i);
                if(i < L || i > R) wa();
            }
        }
    }

    eof();
}

bool valid() {
    if((int) JURY.size() != (int) CONT.size()) return false;

    int numJURY = 0, denJURY = 1;
    for(auto &i : JURY) numJURY += i;
    denJURY = (int) JURY.size();

    int g = gcd(numJURY, denJURY);
    numJURY /= g; denJURY /= g;

    int numCONT = 0, denCONT = 1;
    for(auto &i : CONT) numCONT += i;
    denCONT = (int) CONT.size();

    g = gcd(numCONT, denCONT);
    numCONT /= g; denCONT /= g;

    return numCONT == numJURY && denCONT == denJURY;
}


void check() {
    if(imposCONT != imposJURY) wa();
    if(imposCONT) ac();
    if(!valid()) wa();
    ac();
}

int main(int argc, char* argv[]) {
    registerScorer(argc, argv);
    getInput();
    check();
}