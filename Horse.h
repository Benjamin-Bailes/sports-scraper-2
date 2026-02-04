#ifndef Horse_H
#define Horse_H

#include <string>

class Horse {
 protected:
  std::string race;
  std::string name;

  std::string win_odds;
  std::string place_odds;

  std::string fluc_open;
  std::string fluc_1;
  std::string fluc_2;

  std::string jockey;
  std::string weight;

  std::string position;

 public:
  Horse(std::string _name);
  Horse(std::string _name, std::string _win_odds, std::string _place_odds);

  // set
  void set_name(std::string _name);
  void set_win_odds(std::string _win_odds);
  void set_place_odds(std::string _place_odds);
  void set_fluc_open(std::string _fluc_open);
  void set_fluc_1(std::string _fluc_1);
  void set_fluc_2(std::string _fluc_2);
  void set_position(std::string _position);
  void set_jockey(std::string _jockey);
  void set_weight(std::string _weight);

  // get
  std::string get_name();
  std::string get_win_odds();
  std::string get_place_odds();
  std::string get_fluc_open();
  std::string get_fluc_1();
  std::string get_fluc_2();
  std::string get_position();
  std::string get_jockey();
  std::string get_weight();
};

#endif