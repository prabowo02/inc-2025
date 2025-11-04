#include <tcframe/spec.hpp>
#include <bits/stdc++.h>
using namespace tcframe;
using namespace std;

class ProblemSpec : public BaseProblemSpec {
protected:
    
    const int MAX_LR = 1'000;
    const int MAX_X = 1'000;
    const int MAX_DIGIT = 6;

    string X;
    int L, R;

    int IMPOSSIBLE, K;
    vector<int> ANS;

    void InputFormat() {
        LINE(X);
        LINE(L, R);
    }

    void OutputFormat1() {
        LINE(K);
        LINE(ANS % SIZE(K));
    }

    void OutputFormat2() {
        LINE(IMPOSSIBLE);
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(256);
    }

    void Constraints() {
        CONS(1 <= L && L <= R && R <= MAX_LR);
        CONS(validNumber(X));
        CONS(isBetween(0, X, MAX_X));
    }

    void Subtask1() {
        Points(100);
    }

    void StyleConfig() {
        CustomScorer();
    }

private:
    bool validNumber(string S) {
        int dot = 0, cnt = 0;
        for(int i = 0; i < S.size(); ++i) {
            if(S[i] == '.') {
                if(i == 0 || i == (int) S.size() - 1) return false;
                ++dot;
            }
            else if(S[i] < '0' || S[i] > '9') return false;
            else ++cnt;
        }
        return dot < 2 && cnt <= MAX_DIGIT;
    }

    // check if LOW <= S <= HIGH
    bool isBetween(int LOW, string S, int HIGH) {
        int num = 0, den = 1;
        for(int i = 0; i < S.size(); ++i) {
            if('0' <= S[i] && S[i] <= '9') num = num * 10 + (S[i] - '0');
            else if(S[i] == '.') {
                int p = (int) S.size() - 1 - i;
                for(int j = 0; j < p; ++j) den *= 10;
            }
        }

        return LOW * den <= num && num <= den * HIGH;
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Subtasks({1});
        Input({
            "8.6",
            "1 10"
        });
        Output({
            "5",
            "10 9 10 7 7"
        });
    }

    void SampleTestCase2() {
        Subtasks({1});
        Input({
            "9",
            "1 10"
        });
        Output({
            "1",
            "9"
        });
    }

    void SampleTestCase3() {
        Subtasks({1});
        Input({
            "2.79",
            "3 5"
        });
        Output({"-1"});
    }


    void BeforeTestCase() {
        X = "";
    }

    void AfterTestCase() {

    }

    void TestGroup1() {
        Subtasks({1});

        CASE(X = "100.04"; L = 50; R = 150);
        CASE(X = "1.99999"; L = 2; R = 3);
        CASE(X = "3.00001"; L = 2; R = 3);
        CASE(X = "0.00001"; L = 1; R = 1'000);
        CASE(X = "1000"; L = 1000; R = 1000);
        CASE(X = "1000.00"; L = 1000; R = 1000);

        // generate random
        CASE(generateRandomX(3); generateLR());
        CASE(generateRandomX(5); generateLR());
        CASE(generateRandomX(6); generateLR());

        // generate based on irreducible denominator
        for(int len = 1; len <= 5; ++len) {
            // we would like X to have exactly len digits after the decimal separator
            // the denominator should be a factor of 10^len
            // more precisely, the denominator should be in the form 2^a * 5^b where max(a, b) = len
            // then, find numerator such that it is coprime to the denominator. monte carlo is used in this case

            // type 0: case where denominator is exactly 10^len, ensuring maximum answer length
            // type 1: the denominator is q = 2^a * 5^b where max(a, b) = len and min(a, b) < len

            for(int type = 0; type < 2; ++type) {
                CASE(
                    int den = 1;
                    for(int k = 0; k < len; ++k) den *= 10;

                    if(type == 1) {
                        int coin_flip = rnd.nextInt(0, 1);
                        int z = rnd.nextInt(1, len);
    
                        int divider = (coin_flip ? 5 : 2);
                        for(int k = 0; k < z; ++k) den /= divider;
                    }

                    // the integral part may have at most MAX_DIGIT - len digits
                    int min_numerator = den + 1;
                    int max_numerator = 1;
                    for(int k = 0; k < min(MAX_DIGIT - len, 3); ++k) max_numerator *= 10;
                    max_numerator = max_numerator * den - 1;

                    int num = 0;
                    while(num == 0 || gcd(num, den) > 1) {
                        num = rnd.nextInt(min_numerator, max_numerator);
                    }

                    constructX(num, den);
                    generateLR();

                );
            }
        }
    }

private:
    // len > 1
    void generateRandomX(int len) {
        // ensure that integral part is not longer than 3
        int pos = rnd.nextInt(0, min(len - 2, 2));

        for(int i = 0; i < len; ++i) {
            int d = rnd.nextInt(i ? 0 : 1, 9);
            X += ('0' + d);
            if(i == pos) X += '.';
        }

        return;
    }

    void generateLR() {
        int num = 0;
        for(int i = 0; i < X.size(); ++i) {
            if(X[i] == '.') break;
            num = num * 10 + (X[i] - '0');
        }

        L = rnd.nextInt(1, num);
        R = rnd.nextInt(num + 1, MAX_LR);
    }

    void constructX(int num, int den) {
        int integral = num / den;
        if(integral == 0) X += "0";
        else {
            while(integral) {
                X += ('0' + integral % 10);
                integral /= 10;
            }
            reverse(X.begin(), X.end());
        }

        if(num % den == 0) return;
        num %= den;
        X += ".";

        while(num > 0) {
            num = num * 10;
            int d = num / den;

            assert(0 <= d && d < 10);
            X += ('0' + d);

            num %= den;
        }

        return;
    }
};
