#include <tcframe/spec.hpp>
#include <bits/stdc++.h>
using namespace tcframe;
using namespace std;

class ProblemSpec : public BaseProblemSpec {
protected:

    const int MAX_N = 1'000;
    const int MAX_M = 1'000;
    const int MAX_Q = 300'000;

    const string RECTANGLE = "RECTANGLES";
    const string NOT_RECTANGLE = "NO";

    int N, M, Q;
    vector<vector<char>> G;
    vector<string> ANS;
    vector<int> R, C;

    void InputFormat() {
        LINE(N, M);
        GRID(G) % SIZE(N, M);
        LINE(Q);
        LINES(R, C) % SIZE(Q);
    }

    void OutputFormat() {
        LINES(ANS) % SIZE(Q);
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(256);
    }

    void Constraints() {
        CONS(1 <= N && N <= MAX_N);
        CONS(1 <= M && M <= MAX_M);
        CONS(1 <= Q && Q <= MAX_Q);
        CONS(validCells(G));
        CONS(validQueries(R, C));
    }

    void Subtask1() {
        Points(100);
    }

private:
    bool validCells(vector<vector<char>> &grid) {
        if(grid.size() != N) return false;
        for(int i = 0; i < N; ++i) {
            if(grid[i].size() != M) return false;
            for(auto &c : grid[i]) {
                if(c != '.' && c != '#') return false;
            }
        }
        return true;
    }
    bool validQueries(vector<int> &rows, vector<int> &cols) {
        if(rows.size() != Q) return false;
        if(cols.size() != Q) return false;

        for(auto &i : rows) if(i < 1 || i > N) return false;
        for(auto &i : cols) if(i < 1 || i > M) return false;

        return true;
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Subtasks({1});
        Input({
            "5 5",
            "#...#",
            "#...#",
            "#####",
            "#...#",
            "#####",
            "3",
            "4 3",
            "1 3",
            "2 3"
        });
        Output({
            RECTANGLE,
            NOT_RECTANGLE,
            RECTANGLE
        });
    }

    void SampleTestCase2() {
        Subtasks({1});
        Input({
            "3 4",
            "####",
            "#.##",
            "####",
            "1",
            "2 2"
        });
        Output({
            RECTANGLE
        });
    }

    // Initially, the cells are all dirt (#)
    // Then, we start toggling the cells listed in vector Events
    // The initial grid is constructed using the first query_start elements of Events, the rest of them are actual queries

    // All indices are 0-based for ease of implementation. They will be reconverted to 1-based on AfterTestCases

    vector<pair<int,int>> events;

    void BeforeTestCase() {
        events.clear();
        G.clear();
        R.clear();
        C.clear();
    }

    void AfterTestCase() {
        Q = (int) R.size();
        for(auto &i : R) ++i;
        for(auto &i : C) ++i;
    }

    void TestGroup1() {
        Subtasks({1});
        // Manually hand-crafted cases
        CASE(
            N = 1; M = 1; 
            initG();
            events = {{0, 0}};
            splitEvents(0);
        );

        CASE(
            N = 2; M = 3; 
            initG();
            events = {
                {0, 0},
                {1, 1},
                {0, 2},
                {0, 1},
                {1, 0},
                {1, 2}
            };
            splitEvents(3);
        );

        CASE(
            // simulate merging and unmerging of rectangle

            N = 3; M = 3;
            initG();

            // first rectangle
            events.emplace_back(0, 0);
            events.emplace_back(0, 1);
            events.emplace_back(1, 0);
            events.emplace_back(1, 1);

            // second rectangle
            events.emplace_back(1, 2);
            events.emplace_back(2, 1);
            events.emplace_back(2, 2);

            // erase first rectangle
            events.emplace_back(0, 0);
            events.emplace_back(0, 1);
            events.emplace_back(1, 0);

            // erase second rectangle
            events.emplace_back(1, 2);
            events.emplace_back(2, 1);
            events.emplace_back(2, 2);

            // re-add first rectangle
            events.emplace_back(0, 0);
            events.emplace_back(0, 1);
            events.emplace_back(1, 0);

            splitEvents(0);
        );
        
        CASE(
            // fill slowly from the center to the border
            N = rnd.nextInt(1, 10) * 2 + 1;
            M = N;
            initG();

            vector<tuple<int, int, int>> cells;
            
            // sort the cells by chebyshev distance
            int center = N / 2;
            for(int i = 0; i < N; ++i) {
                for(int j = 0; j < M; ++j) {
                    int dist = max(abs(center - i), abs(center - j));
                    cells.emplace_back(dist, i, j);
                }
            }
            sort(cells.begin(), cells.end());

            for(auto &[_, r, c] : cells)
                events.emplace_back(r, c);

            splitEvents(0);
        );

        // Totally random; expected very few or none RECTANGLES
        for(int _ = 0; _ < 1; ++_) {
            CASE(
                N = MAX_N; M = MAX_M;
                initG();

                // initial toggled states
                int init = rnd.nextInt(1, N * M / 2);
                randomEvent(init);

                Q = MAX_Q;
                randomEvent(Q);

                splitEvents(init);
            );
        }

        // Slightly less random; activate either a column or a row all at once
        for(int _ = 0; _ < 1; ++_) {
            CASE(
                N = MAX_N; M = MAX_M;
                initG();

                while(events.size() < MAX_Q) {
                    int coin_flip = rnd.nextInt(0, 1);
                    if(coin_flip) {
                        int row = rnd.nextInt(0, N - 1);
                        toggleRow(row, true);
                    }
                    else {
                        int col = rnd.nextInt(0, M - 1);
                        toggleColumn(col, true);
                    }
                }

                int init = (int) events.size() - MAX_Q;
                splitEvents(init);
            );
        }

        // Same case, but start with all puddles
        for(int _ = 0; _ < 1; ++_) {
            CASE(
                N = MAX_N; M = MAX_M;
                initG();

                for(int i = 0; i < N; ++i)
                    for(int j = 0; j < M; ++j)
                        events.emplace_back(i, j);

                while(events.size() < MAX_Q + N * M) {
                    int coin_flip = rnd.nextInt(0, 1);
                    if(coin_flip) {
                        int row = rnd.nextInt(0, N - 1);
                        toggleRow(row, true);
                    }
                    else {
                        int col = rnd.nextInt(0, M - 1);
                        toggleColumn(col, true);
                    }
                }

                int init = (int) events.size() - MAX_Q;
                splitEvents(init);
            );
        } 

        // Slightly less random; activate either a column or a row all at once, but add some noise
        for(int _ = 0; _ < 1; ++_) {
            CASE(
                N = MAX_N; M = MAX_M;
                initG();

                while(events.size() < MAX_Q) {
                    int coin_flip = rnd.nextInt(0, 1);
                    if(coin_flip) {
                        int row = rnd.nextInt(0, N - 1);
                        toggleRow(row, true);
                    }
                    else {
                        int col = rnd.nextInt(0, M - 1);
                        toggleColumn(col, true);
                    }

                    // in between the process, add some noises
                    // the noises will be toggled exactly twice to ensure the grid is back to rectangular shape

                    int noise_count = rnd.nextInt(1, 50);
                    addNoises(noise_count);
                }

                int init = (int) events.size() - MAX_Q;
                splitEvents(init);
            );
        }

        // create disjoint sets of small rectangles
        for(int _ = 0; _ < 1; ++_) {
            CASE(
                N = MAX_N; M = MAX_M;
                initG();
                vector<tuple<int,int,int,int>> rect;
                generateRectangles(0, 0, N - 1, M - 1, rect);

                for(auto &[r1, c1, r2, c2] : rect) {
                    toggleSubgrid(r1, c1, r2, c2);
                    // add noise events in between

                    int noise_count = rnd.nextInt(1, 50);
                    addNoises(noise_count);
                }

                int init = max(0, (int) events.size() - MAX_Q);
                splitEvents(init);
            );
        }

        // simulate rectangle merging and splitting
        for(int _ = 0; _ < 1; ++_) {
            CASE(
                N = MAX_N; M = MAX_M;
                initG();

                vector cnt(N, vector(M, 0));

                // generate some rectangles, possibly overlapping

                // try not to create the rectangles too many or too large, to ensure that number of query is enough
                int rectangle_count = 50;

                vector<tuple<int,int,int,int>> rect;
                int R1, C1, R2, C2;
                int subgrid_height = 200, subgrid_width = 200;
                for(int i = 0; i < rectangle_count; ++i) {
                    if(i % 10 == 0) {
                        // to raise the chance of rectangle collision, make sure that every 10 rectangles appear in a particular subgrid

                        R1 = rnd.nextInt(0, N - subgrid_height);
                        R2 = R1 + subgrid_height - 1;

                        C1 = rnd.nextInt(0, M - subgrid_width);
                        C2 = C1 + subgrid_width - 1;
                    }

                    int height = rnd.nextInt(1, subgrid_height);
                    int width = rnd.nextInt(1, subgrid_width);

                    int r1 = rnd.nextInt(R1, R2 - height + 1);
                    int r2 = r1 + height - 1;

                    int c1 = rnd.nextInt(C1, C2 - width + 1);
                    int c2 = c1 + width - 1;

                    rect.emplace_back(r1, c1, r2, c2);
                }

                rnd.shuffle(rect.begin(), rect.end());

                // add rectangles
                for(auto &[r1, c1, r2, c2] : rect) {
                    vector<pair<int,int>> cells;
                    for(int i = r1; i <= r2; ++i) {
                        for(int j = c1; j <= c2; ++j) {
                            if(!cnt[i][j]) cells.emplace_back(i, j);
                            ++cnt[i][j];
                        }
                    }

                    rnd.shuffle(cells.begin(), cells.end());
                    for(auto &cell : cells)
                        events.emplace_back(cell);

                    int noise_count = rnd.nextInt(1, 50);
                    addNoises(noise_count);
                }

                // remove rectangles
                for(auto &[r1, c1, r2, c2] : rect) {
                    vector<pair<int,int>> cells;
                    for(int i = r1; i <= r2; ++i) {
                        for(int j = c1; j <= c2; ++j) {
                            if(cnt[i][j] == 1) cells.emplace_back(i, j);
                            --cnt[i][j];
                        }
                    }

                    rnd.shuffle(cells.begin(), cells.end());
                    for(auto &cell : cells)
                        events.emplace_back(cell);

                    int noise_count = rnd.nextInt(1, 50);
                    addNoises(noise_count);
                }

                int init = max(0, (int) events.size() - MAX_Q);
                splitEvents(init);
            );
        }


        // just very big rectangle with area = MAX_Q, constructed such that there are a lot of RECTANGLES query during the process
        for(int _ = 0; _ < 1; ++_) {
            CASE(
                N = MAX_N; M = MAX_M;
                initG();

                vector<pair<int, int>> dimensions;
                for(int i = 1; i <= MAX_Q; ++i) {
                    if(MAX_Q % i == 0) {
                        int r = i, c = MAX_Q / i;
                        if(r <= N && c <= M) 
                            dimensions.emplace_back(r, c);
                    }
                }

                int select = rnd.nextInt(0, (int) dimensions.size() - 1);
                int height = dimensions[select].first;
                int width = dimensions[select].second;

                int r1 = rnd.nextInt(0, N - height);
                int c1 = rnd.nextInt(0, M - width);

                constructRectangle(r1, c1, r1 + height - 1, c1 + width - 1);
                splitEvents(0);
            );
        }

        // One large pool, and toggle four corners randomly
        CASE(
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            G = vector<vector<char>>(N, vector<char>(M, '.'));
            for (int i = 0; i < Q / 2; ++i) {
                int r = rnd.nextInt(2) ? 0 : N - 1;
                int c = rnd.nextInt(2) ? 0 : M - 1;

                R.push_back(r); C.push_back(c);
                R.push_back(r); C.push_back(c);
            }
        );
        // No pool, and toggle back and forth random cells
        CASE(
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            G = vector<vector<char>>(N, vector<char>(M, '#'));
            for (int i = 0; i < Q / 2; ++i) {
                int r = rnd.nextInt(N);
                int c = rnd.nextInt(M);

                R.push_back(r); C.push_back(c);
                R.push_back(r); C.push_back(c);
            }
        );
    }

private:
    void initG() {
        G.resize(N, vector(M, '#'));
    }

    void toggleSubgrid(int r1, int c1, int r2, int c2) {
        vector<pair<int,int>> sequences;
        for(int i = r1; i <= r2; ++i) {
            for(int j = c1; j <= c2; ++j) {
                sequences.emplace_back(i, j);
            }
        }
        rnd.shuffle(sequences.begin(), sequences.end());

        for(auto &[r, c] : sequences)
            events.emplace_back(r, c);
    }

    void randomEvent(int freq) {
        for(int i = 0; i < freq; ++i) {
            int r = rnd.nextInt(0, N - 1);
            int c = rnd.nextInt(0, M - 1);
            events.emplace_back(r, c);
        }
    }

    void splitEvents(int query_start) {
        for(int i = 0; i < events.size(); ++i) {
            auto &[r, c] = events[i];

            if(i < query_start) 
                G[r][c] = (G[r][c] == '#' ? '.' : '#');
            else 
                R.emplace_back(r), C.emplace_back(c);
        }
    }

    // if randomSequence, the order of toggling is shuffled
    void toggleRow(int row, bool randomSequence) {
        vector coords(M, pair<int,int>());
        for(int j = 0; j < M; ++j)
            coords[j] = make_pair(row, j);

        if(randomSequence)
            rnd.shuffle(coords.begin(), coords.end());

        for(auto &[r, c] : coords)
            events.emplace_back(r, c);
    }

    void toggleColumn(int col, bool randomSequence) {
        vector coords(N, pair<int,int>());
        for(int i = 0; i < N; ++i)
            coords[i] = make_pair(i, col);

        if(randomSequence)
            rnd.shuffle(coords.begin(), coords.end());

        for(auto &[r, c] : coords)
            events.emplace_back(r, c);
    }

    void generateRectangles(int r1, int c1, int r2, int c2, vector<tuple<int,int,int,int>> &rect) {
        // for a subgrid spanning from cell (r1, c1) to (r2, c2), decide one of the following 3 actions:

        int MAX_AREA = 100;
        // - make the subgrid a rectangle (guaranteed if area <= MAX_AREA)
        // - split into two horizontally
        // - split into two vertically

        // if the length or width is too small, it's not possible to create boundary gap
        if(r1 + 1 >= r2 || c1 + 1 >= c2) return;

        if((r2 - r1 + 1) * (c2 - c1 + 1) <= MAX_AREA) {
            rect.emplace_back(r1 + 1, c1 + 1, r2 - 1, c2 - 1);
            return;
        }

        int coin_flip = rnd.nextInt(0, 1);
        if(coin_flip) {
            // split horizontally

            int split_col = rnd.nextInt(c1, c2 - 1);
            generateRectangles(r1, c1, r2, split_col, rect);
            generateRectangles(r1, split_col + 1, r2, c2, rect);
        }
        else {
            // split vertically

            int split_row = rnd.nextInt(r1, r2 - 1);
            generateRectangles(r1, c1, split_row, c2, rect);
            generateRectangles(split_row + 1, c1, r2, c2, rect);
        }

        return;
    }

    // construct single rectangle such that there are a lot of rectangles in the process
    void constructRectangle(int r1, int c1, int r2, int c2) {
        if(r1 > r2 || c1 > c2) return;

        int area = (r2 - r1 + 1) * (c2 - c1 + 1);
        int coin_flip = rnd.nextInt(0, (area <= 10'000 ? 2 : 1));

        if(coin_flip == 0) {
            // split vertically; the splitting line will be constructed last
            int split_row = rnd.nextInt(r1, r2);
            constructRectangle(r1, c1, split_row - 1, c2);
            constructRectangle(split_row + 1, c1, r2, c2);

            vector<pair<int,int>> sequences;
            for(int i = c1; i <= c2; ++i) sequences.emplace_back(split_row, i);
            rnd.shuffle(sequences.begin(), sequences.end());
            for(auto &cell : sequences)
                events.emplace_back(cell);
        }
        else if(coin_flip == 1) {
            // split horizontally; the splitting line will be constructed last
            int split_col = rnd.nextInt(c1, c2);
            constructRectangle(r1, c1, r2, split_col - 1);
            constructRectangle(r1, split_col + 1, r2, c2);

            vector<pair<int,int>> sequences;
            for(int i = r1; i <= r2; ++i) sequences.emplace_back(i, split_col);
            rnd.shuffle(sequences.begin(), sequences.end());
            for(auto &cell : sequences)
                events.emplace_back(cell);
        }
        else {
            // immediately construct the rectangle, make sure that this does not happen when the area is too large
            vector<pair<int,int>> sequences;
            for(int i = r1; i <= r2; ++i) {
                for(int j = c1; j <= c2; ++j) {
                    sequences.emplace_back(i, j);
                }
            }
            rnd.shuffle(sequences.begin(), sequences.end());

            for(auto &cell : sequences)
                events.emplace_back(cell);
        }
    }

    void addNoises(int noise_count) {
        vector<pair<int,int>> noises;

        for(int i = 0; i < noise_count; ++i) {
            int r = rnd.nextInt(0, N - 1);
            int c = rnd.nextInt(0, M - 1);
            noises.emplace_back(r, c);
            noises.emplace_back(r, c);
        }
        rnd.shuffle(noises.begin(), noises.end());

        for(auto &noise : noises)
            events.emplace_back(noise);
    }

};
