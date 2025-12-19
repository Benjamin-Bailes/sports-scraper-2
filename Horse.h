#ifndef Horse_H
#define Horse_H

#include <string>

class Horse {
 public:
  int id;
  std::string name;
  std::string race;
  float win_odds;
  float place_odds;
  int position;

  Horse(const std::string& name);
};

#endif