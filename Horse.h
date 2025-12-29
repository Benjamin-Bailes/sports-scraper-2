#ifndef Horse_H
#define Horse_H

#include <string>

class Horse {
 public:
  std::string race;
  std::string name;

  std::string win_odds;
  std::string place_odds;
  float win_odds_f;
  float place_odds_f;

  int position;

  //
  //
  Horse(std::string _name, float _win_odds, float _place_odds);
  Horse(std::string _name, std::string _win_odds, std::string _place_odds);

  void set_position(int _position);
};

#endif