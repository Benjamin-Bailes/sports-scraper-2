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

  // getset
  const char* get_html();
  std::string get_url();
  bool is_race_complete();
  std::vector<Horse> get_horses();
};

#endif