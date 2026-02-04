#include "Race.h"

#include <gumbo.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Horse.h"
#include "parsehtml.h"
#include "retrievehtml.hpp"

/*
Race::Race(std::string _url) {
  if (_url.empty()) return;
  this->url = _url;

  this->html = (char*)malloc(1);
  if (!this->html) return;
  this->html[0] = '\0';
  retrieve_html(this->html, this->url.c_str());
  std::cout << 3 << std::endl;

  this->gumbo_output = gumbo_parse(this->html);
  if (!this->gumbo_output) return;
  std::cout << 4 << std::endl;

  get_initial_data();
  clean_data();
  construct_horses();
}
*/
Race::Race(std::string _url, const char* _html) {
  this->url = _url;
  if (!_html) return;
  this->html = _html;

  this->gumbo_output = gumbo_parse(this->html);
  if (!this->gumbo_output) return;

  get_data();

  // clean_data();
  // construct_horses();
}

//
// getset
const char* Race::get_html() { return this->html; }
std::string Race::get_url() { return this->url; }
bool Race::is_race_complete() { return this->race_complete; }
std::vector<Horse> Race::get_horses() { return this->horses; }

//
// protected
void Race::get_data() {
  if (!this->gumbo_output) return;
  GumboNode* root_node = gumbo_output->root;
  if (root_node->type != GUMBO_NODE_ELEMENT) return;

  const char att[] = "data-automation-id";
  const char class_att[] = "class";

  const char racecard_att_val[] = "racecard-body";
  const char racecard_outcome_att_val[] = "outcomeCard_f7jc198";

  const char name_att_val[] = "racecard-outcome-name";
  const char win_odds_att_val[] = "racecard-outcome-0-L-price";
  const char place_odds_att_val[] = "racecard-outcome-1-L-price";
  const char flucs_att_val[] = "priceFlucsContainer_f1qh6j2w";
  const char jockey_att_val[] = "runnerJockeyInfoDesktop_fmumgvr";
  const char weight_att_val[] = "runnerInfoCont_f1op6hsi";

  // get half way down tree first. This ensures duplicate names (and stats) are not picked up from other nodes
  GumboNode* racecard_node = get_div(root_node, att, racecard_att_val);

  std::vector<GumboNode*> racecard_outcomes;
  get_all_divs(racecard_node, racecard_outcomes, class_att, racecard_outcome_att_val);

  //
  // constuct horses
  for (auto& racecard_outcome : racecard_outcomes) {
    std::string _name = get_text_from_first_div(racecard_outcome, att, name_att_val);
    std::string _win_odds = get_text_from_first_div(racecard_outcome, att, win_odds_att_val);
    std::string _place_odds = get_text_from_first_div(racecard_outcome, att, place_odds_att_val);
    std::string _jockey = get_text_from_first_div(racecard_outcome, class_att, jockey_att_val);
    std::string _weight = get_text_from_first_div(racecard_outcome, class_att, weight_att_val);

    std::vector<std::string> curr_flucs;
    search_in_divs(racecard_outcome, curr_flucs, class_att, flucs_att_val);

    if (!_name.empty()) {
      this->horses.emplace_back(_name, _win_odds, _place_odds);
      if (curr_flucs.size() == 3) {
        horses.back().set_fluc_open(curr_flucs[0]);
        horses.back().set_fluc_1(curr_flucs[1]);
        horses.back().set_fluc_2(curr_flucs[2]);
      }
      horses.back().set_jockey(_jockey);
      horses.back().set_weight(_weight);
    }
  }

  //
  // top 4 positions, displayed in order, contained in top div of racecard
  GumboNode* racecard_positions_node = get_div(root_node, "class", "container_fqa53j6");

  std::vector<std::string> ordered_winning_names;
  search_in_divs(racecard_positions_node, ordered_winning_names, att, name_att_val);
  int i = 0;
  while (i < ordered_winning_names.size()) {
    if (ordered_winning_names[i].find(". ") != std::string::npos) {
      size_t nameStart = ordered_winning_names[i].find(". ") + 2;
      size_t nameEnd = ordered_winning_names[i].size();
      ordered_winning_names[i] = ordered_winning_names[i].substr(nameStart, nameEnd - nameStart);
      i++;
    } else {
      ordered_winning_names.erase(ordered_winning_names.begin() + i);
    }
  }

  int _position = 1;
  if (ordered_winning_names.size() == 0) {
    this->race_complete = false;
  } else {
    this->race_complete = true;

    for (std::string _pos_name : ordered_winning_names) {
      for (Horse& _horse : this->horses) {
        if (_pos_name == _horse.get_name()) {
          _horse.set_position(std::to_string(_position));
        }
      }
      _position++;
    }
  }

  return;
}

/* 

// void Race::clean_data() {
//   // clean names vector
//   size_t nameStart, nameEnd;
//   int i = 0;
//   while (i < names.size()) {
//     if (names[i].find(". ") != std::string::npos) {
//       nameStart = names[i].find(". ") + 2;
//       nameEnd = names[i].size();
//       names[i] = names[i].substr(nameStart, nameEnd - nameStart);
//       i++;
//     } else {
//       names.erase(names.begin() + i);
//     }
//   }  // not sure if this is safe. will it always terminate?

//   // clean postion names
//   i = 0;
//   while (i < ordered_winning_names.size()) {
//     if (ordered_winning_names[i].find(". ") != std::string::npos) {
//       nameStart = ordered_winning_names[i].find(". ") + 2;
//       nameEnd = ordered_winning_names[i].size();
//       ordered_winning_names[i] = ordered_winning_names[i].substr(nameStart, nameEnd - nameStart);
//       i++;
//     } else {
//       ordered_winning_names.erase(ordered_winning_names.begin() + i);
//     }
//   }

//   // clean odds
//   i = 0;
//   while (i < win_odds.size()) {
//     try {
//       this->win_odds_f.push_back(std::stof(this->win_odds[i]));
//       i++;
//     } catch (const std::exception& e) {
//       win_odds.erase(win_odds.begin() + i);  // update string version too
//     }
//   }
//   i = 0;
//   while (i < place_odds.size()) {
//     try {
//       this->place_odds_f.push_back(std::stof(this->place_odds[i]));
//       i++;
//     } catch (const std::exception& e) {
//       place_odds.erase(place_odds.begin() + i);
//     }
//   }

//   return;
// }

// void Race::construct_horses() {
//   std::string _name;
//   std::string _win_odds, _place_odds;
//   // float _win_odds_f, _place_odds_f;

//   for (int i = 0; i < names.size(); i++) {
//     _name = (!this->names[i].empty()) ? this->names[i] : "";
//     // _win_odds_f = win_odds_f[i] ? win_odds_f[i] : 0;
//     // _place_odds_f = place_odds_f[i] ? place_odds_f[i] : 0;
//     _win_odds = (i < win_odds.size()) ? win_odds[i] : "";
//     _place_odds = (i < place_odds.size()) ? place_odds[i] : "";

//     this->horses.emplace_back(_name, _win_odds, _place_odds);
//   }

//   // set positions
//   int _position = 1;
//   if (ordered_winning_names.size() == 0) {
//     this->race_complete = false;
//   } else {
//     this->race_complete = true;

//     for (std::string _pos_name : this->ordered_winning_names) {
//       for (Horse& _horse : this->horses) {
//         if (_pos_name == _horse.name) {
//           _horse.set_position(_position);
//         }
//       }
//       _position++;
//     }
//   }

//   return;
// }

*/