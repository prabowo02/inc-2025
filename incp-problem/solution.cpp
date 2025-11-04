#include <iostream>
#include <string>
using namespace std;

const char kEndOfChar = '_';

bool isConsonant(char c) {
  if (c == 'a' || c == 'i' || c == 'e' || c == 'o' || c == 'u') {
    return false;
  }
  return true;
}

bool isCase1(char c) {
  // Followed by a, o, or u
  if (c == 'a' || c == 'o' || c == 'u') {
    return true;
  }
  // Followed by a consonant other than h and y
  if (isConsonant(c) && c != 'h' && c != 'y') {
    return true;
  }
  // End of a word (represented by '_')
  if (c == kEndOfChar) {
    return true;
  }
  return false;
}

bool isCase2(char c) {
  // Followed by e, i, or y.
  if (c == 'e' || c == 'i' || c == 'y') {
    return true;
  }
  return false;
}

bool isCase3(char c) {
  // The next character is h.
  if (c == 'h') {
    return true;
  }
  return false;
}

string solve(string s) {
  string result = "";
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == 'c') {
      char next_char = kEndOfChar;
      if (i + 1 != s.size()) {
        next_char = s[i + 1];
      }
      if (isCase1(next_char)) {
        result += 'k';
      } else if (isCase2((next_char))) {
        result += 's';
      } else if (isCase3(next_char)) {
        result += 'c';
        // to represents ch as c, we can increment the iterator to skip adding h
        i++;
      }
    } else {
      result += s[i];
    }
  }
  return result;
}

int main() {
  int n;
  cin >> n;
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;
    cout << solve(s) << endl;
  }
  return 0;
}
