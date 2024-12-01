#include <algorithm>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <cstdint>

struct Segment {
  long double left;
  long double right;
};

std::map<char, Segment>
DefineSegments(const std::vector<std::pair<char, long double>>& alphabet) {
  std::map<char, Segment> segments;
  long double left = 0;

  for (const auto & i : alphabet) {
    Segment segment{};
    segment.left = left;
    segment.right = left + i.second;
    segments.emplace(i.first, segment);
    left = segment.right;
  }
  return segments;
}

void ArithmeticCoding(const std::vector<std::pair<char, long double>>& alphabet, std::string message) {
  std::map<char, Segment> segments = DefineSegments(alphabet);
  long double left = 0;
  long double right = 1;
  int new_message_length = 0;
  std::cout << std::setprecision(20) << std::endl;
  for (int i = 0; i < message.length(); i++) {
    char ch = message[i];
    long double newRight = left + (right - left) * segments[ch].right;
    long double newLeft = left + (right - left) * segments[ch].left;
    if (newRight - newLeft < 0.0000000000000000000000001) {
      long double q = floorl(-log2l(right - left));
      long double p = ceill(left * powl(2, q));
      std::bitset<61> code{(unsigned long long) p};
      std::cout << std::endl << "q = round_down(-log_2(R - L)) = "
                << "round_down(-log_2(" << right << " - " << left << ") = " << std::endl
                << "= round_down(-log_2(" << (right - left) << ") = "
                << q << " ==>" << std::endl
                << "==> " << left * powl(2, q) << " <= p <= " << right * powl(2, q) << " ==>" << std::endl
                << "==> p = " << p << " ==>" << std::endl
                << "==> закодированное сообщение:" << std::endl
                << code << std::endl
                << "A(S) = |A(S)| / |S| = " << q / new_message_length;
      return;
    }
    left = newLeft;
    right = newRight;
    new_message_length++;

    std::cout << message[i] << ": (" << left << ", " << right << ')' << std::endl;
  }
}

int Find(std::vector<std::pair<char, long double>>& v, char ch) {
  for (int i = 0; i < v.size(); i++) {
    if (v[i].first == ch) {
      return i;
    }
  }
  return -1;
}

int main() {
  std::vector<std::pair<char, long double>> alphabet;
  std::string message = "чикадзеевгенийалександрович";
  long double message_length = (long double) message.length();
  for (int i = 0; i < message_length; i++) {
    int is_in_alphabet = Find(alphabet, message[i]);
    if (is_in_alphabet != -1) {
      alphabet[is_in_alphabet].second += 1.0 / message_length;
    } else {
      alphabet.emplace_back(message[i], 1.0 / message_length);
    }
  }
  size_t alphabet_length = alphabet.size();

  std::sort(
      alphabet.begin(), alphabet.end(),
      [] (std::pair<char, long double> pair1, std::pair<char, long double> pair2) ->bool {return pair1.second >= pair2.second;}
  );

  ArithmeticCoding(alphabet, message);
  std::cout << std::endl;
}