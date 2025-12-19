#include <curl/curl.h>
#include <gumbo.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Horse.h"
#include "Race.h"
#include "parsehtml.h"
#include "retrievehtml.h"

int main() {
  char url[] = "https://www.sportsbet.com.au/racing-schedule/horse/today";
  // char* todays_html = (char*)malloc(1);
  char* todays_html = static_cast<char*>(std::malloc(1));
  if (!todays_html) return 1;
  todays_html[0] = '\0';
  retrieve_html(&todays_html, url);
  std::cout << 1 << std::endl;

  GumboOutput* output = gumbo_parse(todays_html);
  std::vector<std::string> links;
  search_for_links(output->root, &links);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  for (int i = 0; i < links.size(); i++) {
    links[i] = "https://www.sportsbet.com.au" + links[i];
    // std::cout << links[i] << std::endl;
  }
  std::cout << 2 << std::endl;

  Race race1 = Race(links[0]);

  // std::cout << race1.get_html() << std::endl;

  // FILE* out = fopen("page.html", "wb");  // write binary output
  // if (!out) return 1;
  // fprintf(out, "%s", race1.get_html());
  // fclose(out);

  free(todays_html);

  return 0;
}