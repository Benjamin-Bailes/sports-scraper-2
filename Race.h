#ifndef Race_H
#define Race_H

#include <gumbo.h>

#include <iostream>
#include <string>
#include <vector>

#include "Horse.h"
#include "retrievehtml.hpp"

class Race {
 protected:
  // source
  std::string url;
  const char* html;
  GumboOutput* gumbo_output;

  // raw data
  // std::vector<const char*> name_reffs;
  std::vector<std::string> names;
  std::vector<std::string> ordered_winning_names;  // vector in order of finishing position (does not contain all horses - need to run js to do that)
  std::vector<std::string> win_odds;
  std::vector<std::string> place_odds;
  std::vector<float> win_odds_f;
  std::vector<float> place_odds_f;

  // nice data
  std::string race_name;
  std::vector<Horse> horses;

  void get_initial_data();
  void clean_data();
  void construct_horses();

 public:
  // Race(std::string _url);
  Race(std::string _url, const char* _html);
  ~Race();

  /*
  // TO HELP WITH CONSTRUCTING IN A VECTOR - something to do with copying resulting in double free malloc error
  Race(const Race&) {
    std::cerr << "COPY\n";
  }
  Race(const Race&) = delete;
  Race& operator=(const Race&) = delete;
  Race(Race&& other) noexcept : html(other.html) {
    other.html = nullptr;
  }
  Race& operator=(Race&& other) noexcept {
    if (this != &other) {
      free(html);
      html = other.html;
      other.html = nullptr;
    }
    return *this;
  }
  //
  */

  // getset
  const char* get_html();
  std::vector<Horse> get_horses();
  std::vector<std::string> get_names();
  std::vector<std::string> get_win_odds();
  std::vector<std::string> get_place_names();
};

#endif