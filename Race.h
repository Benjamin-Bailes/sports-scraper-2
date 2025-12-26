#ifndef Race_H
#define Race_H

#include <gumbo.h>

#include <iostream>
#include <string>
#include <vector>

#include "Horse.h"
#include "retrievehtml.h"

class Race {
 protected:
  // source
  std::string url;
  char* html;
  GumboOutput* gumbo_output;

  // raw data
  // std::vector<const char*> name_reffs;
  std::vector<std::string> names;
  std::vector<std::string> ordered_winning_names;  // vector in order of finishing position (does not contain all horses - need to run js to do that)
  std::vector<std::string> win_odds;
  std::vector<std::string> place_odds;

  // nice data
  std::string race_name;
  std::vector<Horse> horses;

  void get_initial_data();
  void clean_data();
  void construct_horses();

 public:
  Race(std::string _url);
  ~Race();

  // TO HELP WITH CONSTRUCTING IN A VECTOR - something to do with copying resulting in double free malloc error
  // Race(const Race&) {
  //   std::cerr << "COPY\n";
  // }
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

  // getset
  char* get_html();
  std::vector<Horse> get_horses();
  std::vector<std::string> get_names();
  std::vector<std::string> get_win_odds();
  std::vector<std::string> get_place_names();
};

#endif