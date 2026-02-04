#include "Horse.h"

#include <string>

Horse::Horse(std::string _name, float _win_odds, float _place_odds)
    : name(_name), win_odds_f(_win_odds), place_odds_f(_place_odds) {
  this->position = 10;
}
Horse::Horse(std::string _name, std::string _win_odds, std::string _place_odds,
             std::string _fluc_open, std::string _fluc_1, std::string _fluc_2)
    : name(_name), win_odds(_win_odds), place_odds(_place_odds), fluc_open(_fluc_open), fluc_1(_fluc_1), fluc_2(_fluc_2) {
  this->position = 10;
}

void Horse::set_position(int _position) {
  if (_position) {
    this->position = _position;
  } else {
    this->position = 10;  // default non place position. i.e > 4
  }
}