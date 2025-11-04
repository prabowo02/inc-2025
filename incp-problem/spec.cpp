#include<tcframe/spec.hpp>
#include<bits/stdc++.h>

using namespace std;
using namespace tcframe;

const int MAXN = 100;
const int MAXLENGTH = 100;

class ProblemSpec : public BaseProblemSpec {
protected:
  int N;
  vector<string> S;

  vector<string> ANS;

  void InputFormat() {
    LINE(N);
    LINES(S) % SIZE(N);
  }

  void OutputFormat() {
    LINES(ANS);
  }

  void Constraints() {
    CONS(between(N, 1, MAXN));
    CONS(allBetween(S, 1, MAXLENGTH));
    CONS(isLowerCaseAlphabet(S));
  }

  void Subtask1() {}

private:
  bool between(int x, int l, int r) {
    return l <= x && x <= r;
  }
  bool allBetween(const vector<string> &v, int l, int r) {
    for (const auto &s: v) if (!between(s.size(), l, r)) return false;
    return true;
  }

  bool isLowerCaseAlphabet(const vector<string> &v) {
    return all_of(v.begin(), v.end(), [](const string& s) {
      return all_of(s.begin(), s.end(), [](char c) {
        return c >= 'a' && c <= 'z';
      });
    });
  }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
  void SampleTestCase1() {
    Subtasks({1});
    Input({
      "10",
      "cyan",
      "circle",
      "ceramic",
      "coccyx",
      "delicacy",
      "chacha",
      "coach",
      "blackjack",
      "church",
      "hiccup",
    });
    Output({
      "syan",
      "sirkle",
      "seramik",
      "koksyx",
      "delikasy",
      "caca",
      "koac",
      "blakkjakk",
      "curc",
      "hikkup",
    });
  }
  void SampleTestCase2() {
    Subtasks({1});
    Input({
      "6",
      "caesar",
      "cillian",
      "celtic",
      "character",
      "chef",
      "echelon",
    });
    Output({
      "kaesar",
      "sillian",
      "seltik",
      "carakter",
      "cef",
      "ecelon",
    });
  } 
  void SampleTestCase3() {
    Subtasks({1});
    Input({
      "5",
      "cacbcccdcecfcgchcicjckclcm",
      "cncocpcqcrcsctcucvcwcxcyczc",
      "abdefghijklmnopqrstuvwxyz",
      "cchh",
      "cc",
    });
    Output({
      "kakbkkkdsekfkgcsikjkkklkm",
      "knkokpkqkrksktkukvkwkxsykzk",
      "abdefghijklmnopqrstuvwxyz",
      "kch",
      "kk",
    });
  }
  void BeforeTestCase() {
    S.clear();
  }

  void TestGroup1() {
    Subtasks({1});
    CASE(N = 1, S = {"z"});
    CASE(N = 20, S = {"c", "ca", "co", "ci", "ce",
                      "cu", "cy", "ch", "cc", "cp",
                      "cuc", "cyc", "chc", "ccc", "cpc",
                      "cuu", "cyy", "chh", "cii", "cpp"});

    for (int i = 0; i < 3; i++) {
      // All non-c
      CASE(N = MAXN, S = randomStrings(N, MAXLENGTH,
                                        /*prob_non_c:*/100, /*prob_c:*/0,
                                        /*char_pools:*/{}));

      // Most of the characters are 'c'
      CASE(N = MAXN, S = randomStrings(N, MAXLENGTH,
                                        /*prob_non_c:*/5, /*prob_c:*/95,
                                        /*char_pools:*/{}));

      // Most of the characters are 'c'
      CASE(N = MAXN, S = randomStrings(N, MAXLENGTH,
                                        /*prob_non_c:*/5, /*prob_c:*/95,
                                        /*char_pools:*/{'a', 'b', 'e', 'y', 'h'}));

      // Either 'c' or 'h'
      CASE(N = MAXN, S = randomStrings(N, MAXLENGTH,
                                        /*prob_non_c:*/50, /*prob_c:*/50,
                                        /*char_pools:*/{'h'}));

      // All of the syllables are in the form of "c."
      CASE(N = MAXN, S = syllableStartsWithC(N, MAXLENGTH, /*ends_with_c*/false));
      CASE(N = MAXN, S = syllableStartsWithC(N, MAXLENGTH, /*ends_with_c*/true));
    }

    // All of the characters are 'c'
    CASE(N = MAXN, S = randomStrings(N, MAXLENGTH,
                                      /*prob_non_c:*/0, /*prob_c:*/100,
                                      /*char_pools:*/{}));
  }

private:
  // Leave pool empty to use all of the characters.
  vector<string> randomStrings(int N, int string_length,
                                int prob_non_c, int prob_c,
                                vector<char> pool) {
    if (pool.size() == 0) {
      // append all of the characters
      for (char c = 'a'; c <= 'z'; c++) {
        if (c == 'c') continue;
        pool.push_back(c);
      }
    }
    vector<string> texts;
    for (int i = 0; i < N; i++) {
      string text = "";
      for (int j = 0; j < string_length; j++) {
        int rng = prob_non_c + prob_c;
        if (rnd.nextInt(rng) < prob_non_c) {
          // need to generate non-c char
          text += pool[rnd.nextInt(pool.size())];
        } else {
          // need to generate c char
          text += 'c';
        }
      }

      texts.emplace_back(text);
    }
    return texts;
  }

  vector<string> syllableStartsWithC(int N, int string_length,
                                      bool ends_with_c) {
    vector<string> pool;
    for (char c = 'a'; c <= 'z'; c++) {
      string tmp = "c";
      tmp += c;
      pool.push_back(tmp);
    }
    pool.push_back("c");
    if (ends_with_c) {
      string_length--;
    }
    vector<string> texts;
    for (int i = 0; i < N; i++) {
      random_shuffle(pool.begin(), pool.end());
      string text = "";
      int idx = 0;
      while (idx < pool.size()) {
        if (text.length() + pool[idx].length() > string_length) {
          break;
        }
        text += pool[idx++];
      }
      // Append the text with random syllables until it reaches max length
      while (true) {
        int idx = rnd.nextInt(pool.size());
        if (text.length() + pool[idx].length() > string_length) {
          break;
        }
        text += pool[idx];
      }
      if (ends_with_c) {
        text += "c";
      }

      texts.emplace_back(text);
    }
    return texts;
  }
};
