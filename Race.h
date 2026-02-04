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

  bool race_complete;

  // nice data
  std::string race_name;
  std::vector<Horse> horses;

  void get_data();

 public:
  // Race(std::string _url);
  Race(std::string _url, const char* _html);

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
  std::string get_url();
  bool is_race_complete();
  std::vector<Horse> get_horses();
  // std::vector<std::string> get_names();
  // std::vector<std::string> get_win_odds();
  // std::vector<std::string> get_place_names();
};

#endif