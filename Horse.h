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

  std::string jockey;
  float weight;

  std::string fluc_open;
  std::string fluc_1;
  std::string fluc_2;
  float fluc_open_f;
  float fluc_1_f;
  float fluc_2_f;

  int position;

  //
  //
  Horse(std::string _name, float _win_odds, float _place_odds);
  Horse(std::string _name, std::string _win_odds, std::string _place_odds,
        std::string _fluc_open, std::string _fluc_1, std::string _fluc_2);

  void set_position(int _position);
};

#endif