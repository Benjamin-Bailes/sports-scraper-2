#include "Race.h"

#include <gumbo.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Horse.h"
#include "parsehtml.h"
#include "retrievehtml.h"

Race::Race(std::string _url) {
  if (_url.empty()) return;
  this->url = _url;

  this->html = (char*)malloc(1);
  if (!this->html) return;
  this->html[0] = '\0';
  retrieve_html(&this->html, this->url.c_str());
  std::cout << 3 << std::endl;

  this->gumbo_output = gumbo_parse(this->html);
  if (!this->gumbo_output) return;
  std::cout << 4 << std::endl;

  get_initial_data();
  clean_data();
  construct_horses();
}
Race::~Race() {
  // delete[] this->url;
  free(this->html);
}

//
// getset
char* Race::get_html() { return this->html; }
std::vector<Horse> Race::get_horses() { return this->horses; }
std::vector<std::string> Race::get_names() { return this->names; }
std::vector<std::string> Race::get_win_odds() { return this->win_odds; }
std::vector<std::string> Race::get_place_names() { return this->place_odds; }

//
// protected
void Race::get_initial_data() {
  GumboNode* root_node = gumbo_output->root;
  if (root_node->type != GUMBO_NODE_ELEMENT) return;

  const char att[] = "data-automation-id";
  const char name_att_val[] = "racecard-outcome-name";
  const char win_odds_att_val[] = "racecard-outcome-0-L-price";
  const char place_odds_att_val[] = "racecard-outcome-1-L-price";

  // get half way down tree first. This ensures duplicate names (and stats) are not picked up from other nodes
  char racecard_att_val[] = "racecard-body";
  GumboNode* racecard_node = get_div(root_node, att, racecard_att_val);
  search_in_divs(racecard_node, this->names, att, name_att_val);
  search_in_divs(racecard_node, this->win_odds, att, win_odds_att_val);
  search_in_divs(racecard_node, this->place_odds, att, place_odds_att_val);

  // top 4 positions, displayed in order, contained in top div of racecard
  GumboNode* racecard_positions_node = get_div(root_node, "class", "container_fqa53j6");
  search_in_divs(racecard_positions_node, this->ordered_winning_names, att, name_att_val);

  return;
}

void Race::clean_data() {
  // clean names vector
  size_t nameStart, nameEnd;
  int i = 0;
  while (i < names.size()) {
    if (names[i].find(". ") != std::string::npos) {
      nameStart = names[i].find(". ") + 2;
      nameEnd = names[i].size();
      names[i] = names[i].substr(nameStart, nameEnd - nameStart);
      i++;
    } else {
      names.erase(names.begin() + i);
    }
  }  // not sure if this is safe. will it always terminate
  // clean postion names
  i = 0;
  while (i < ordered_winning_names.size()) {
    if (ordered_winning_names[i].find(". ") != std::string::npos) {
      nameStart = ordered_winning_names[i].find(". ") + 2;
      nameEnd = ordered_winning_names[i].size();
      ordered_winning_names[i] = ordered_winning_names[i].substr(nameStart, nameEnd - nameStart);
      i++;
    } else {
      ordered_winning_names.erase(ordered_winning_names.begin() + i);
    }
  }

  return;
}

void Race::construct_horses() {
  for (int i = 0; i < names.size(); i++) {
    std::string _name = (!this->names[i].empty()) ? this->names[i] : "";
    float _win_odds = (!this->win_odds[i].empty()) ? std::stof(this->win_odds[i]) : 0;
    float _place_odds = (!this->place_odds[i].empty()) ? std::stof(this->place_odds[i]) : 0;

    this->horses.emplace_back(_name, _win_odds, _place_odds);
  }

  // set positions
  int _position = 1;
  for (std::string _pos_name : this->ordered_winning_names) {
    for (Horse& _horse : this->horses) {
      if (_pos_name == _horse.name) {
        _horse.set_position(_position);
      }
    }
    _position++;
  }

  return;
}

/*
  // order of names: finishes, then whole fleet,
  // need to keep order of fleet without duplications
  // keep first finishers for finish data - use different vectors

  // sort into names, and names of horses that placed (top positions are in a
  // different div / doesnt contain all horses, just top 4, order matters I
  // think as then we can retrice less data (ie position int)

  // different order than I thought - more divs containg stats and names
  // for (int i = names.size(); i >= 0; i--) {
  //   bool exists = std::any_of(
  //       names.end() - i, names.end(),
  //       [&](const std::string& _name) { return _name == names[i]; });
  //   if (exists) {
  //     std::cout << names[i] << std::endl;
  //   }
  // }

  // for (std::vector<std::string>::iterator it = names.end(); it !=
  // names.begin();
  //      it--) {
  //   bool exists =
  //       std::any_of(it + 1, names.end(),
  //                   [&](const std::string& _name) { return _name == *it; });
  //   if (exists) {
  //     std::cout << *it << std::endl;
  //   }
  // }

  // ------------------------------------
  // for (int i = 0; i < names.size(); ++i) {
  //   if (names[i].find(". ") != std::string::npos) {
  //     nameStart = names[i].find(". ") + 2;
  //     nameEnd = names[i].size();

  //     names[i] = names[i].substr(nameStart, nameEnd - nameStart);

  //     // // check if allready a horse
  //     // bool exists = std::any_of(
  //     //     horses.begin(), horses.end(),
  //     //     [&](const Horse& horse) { return horse.name == names[i]; });
  //     // if (!exists) {
  //     //   this->horses.emplace_back(names[i]);
  //     // }

  //   } else {
  //     names[i].erase();
  //   }
  // }
  //
  // for (std::vector<std::string>::iterator name = names.end();
  //      name != names.begin(); name--) {
  //   bool exists =
  //       std::any_of(name - 1, names.end(),
  //                   [&](const std::string& _name) { return _name == *name;
  //                   });
  //   if (exists) {
  //     std::cout << *name << std::endl;
  //   }
  // }

  // use same order to input more horse data, Horse& horse : horses
  // for (std::string t : win_odds) {
  //   std::cout << t << std::endl;
  // }
  */