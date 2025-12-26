#ifndef Horse_H
#define Horse_H

#include <string>

class Horse {
 public:
  std::string name;
  std::string race;
  float win_odds;
  float place_odds;
  int position;

  Horse(std::string _name, float _win_odds, float _place_odds);

  void set_position(int _position);
};

#endif