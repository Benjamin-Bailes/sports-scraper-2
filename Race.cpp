#include "Race.h"

#include <gumbo.h>

#include <iostream>
#include <string>
#include <vector>

#include "Horse.h"
#include "parsehtml.h"
#include "retrievehtml.h"

Race::Race(std::string _url) {
  // if (_url.empty()) return;
  this->url = new char[_url.size() + 1];
  std::strcpy(this->url, _url.c_str());

  this->html = (char*)malloc(1);
  retrieve_html(&this->html, this->url);
  std::cout << 3 << std::endl;

  this->gumbo_output = gumbo_parse(this->html);
  std::cout << 4 << std::endl;

  get_initial_data();
}
Race::~Race() {
  delete[] this->url;
  free(this->html);
}

char* Race::get_html() { return this->html; }

//
// protected
void Race::get_initial_data() {
  GumboNode* root_node = gumbo_output->root;
  if (root_node->type != GUMBO_NODE_ELEMENT) return;
  // Might try to do some intial traversal to cut down parsing

  char att[] = "data-automation-id";
  char name_att_val[] = "racecard-outcome-name";
  char win_odds_att_val[] = "racecard-outcome-0-L-price";
  char place_odds_att_val[] = "racecard-outcome-1-L-price";

  search_html_divs(root_node, this->names, att, name_att_val);
  search_html_divs(root_node, this->win_odds, att, name_att_val);
  search_html_divs(root_node, this->place_odds, att, name_att_val);

  for (auto t : names) {
    std::cout << t << '\n';
  }
  return;
}

void Race::clean_data() {
  // name
  for (auto t : names) {
    // this->horses.append();
    // size_t nameStart = t.find(". ") + 2;
    // size_t nameEnd = t.find('\n', nameStart);
    // this->name = t.substr(nameStart, nameEnd - nameStart);

    // // Extract number
    // size_t numStart = t.find('(', nameEnd) + 1;
    // size_t numEnd = t.find(')', numStart);
    // this->number = std::stoi(s.substr(numStart, numEnd - numStart));
  }

  return;
}
