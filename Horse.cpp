#include "Horse.h"

#include <string>

Horse::Horse(std::string _name) { set_name(_name); }
Horse::Horse(std::string _name, std::string _win_odds, std::string _place_odds) {
  set_name(_name);
  set_position("10");
  set_win_odds(_win_odds);
  set_place_odds(_place_odds);
}

//
// set
void Horse::set_name(std::string _name) {
  size_t nameStart = _name.find(". ") + 2;
  size_t nameEnd = _name.size();
  this->name = _name.substr(nameStart, nameEnd - nameStart);
}
void Horse::set_win_odds(std::string _win_odds) { this->win_odds = _win_odds; }
void Horse::set_place_odds(std::string _place_odds) { this->place_odds = _place_odds; }
void Horse::set_fluc_open(std::string _fluc_open) { this->fluc_open = _fluc_open; }
void Horse::set_fluc_1(std::string _fluc_1) { this->fluc_1 = _fluc_1; }
void Horse::set_fluc_2(std::string _fluc_2) { this->fluc_2 = _fluc_2; }
void Horse::set_position(std::string _position) {
  if (!_position.empty()) {
    this->position = _position;
  } else {
    this->position = "10";  // default non place position. i.e > 4
  }
}

//
// get
std::string Horse::get_name() { return this->name; }
std::string Horse::get_win_odds() { return this->win_odds; }
std::string Horse::get_place_odds() { return this->place_odds; }
std::string Horse::get_fluc_open() { return this->fluc_open; }
std::string Horse::get_fluc_1() { return this->fluc_1; }
std::string Horse::get_fluc_2() { return this->fluc_2; }
std::string Horse::get_position() { return this->position; }
