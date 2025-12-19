#ifndef Race_H
#define Race_H

#include <gumbo.h>

#include <string>
#include <vector>

#include "Horse.h"
#include "retrievehtml.h"

class Race {
 protected:
  // source
  char* url;
  char* html;
  GumboOutput* gumbo_output;

  // raw data
  // std::vector<const char*> name_reffs;
  std::vector<std::string> names;
  std::vector<std::string> win_odds;
  std::vector<std::string> place_odds;

  // nice data
  std::string race_name;
  std::vector<Horse> horses;

  void get_initial_data();
  void clean_data();

 public:
  Race(std::string _url);
  ~Race();

  // getset
  char* get_html();
};

#endif