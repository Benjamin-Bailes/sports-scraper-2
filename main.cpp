#include <curl/curl.h>
#include <gumbo.h>

#include <cstdlib>
// #include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Horse.h"
#include "Race.h"
#include "asyncdownloads.hpp"
#include "parsehtml.h"
#include "retrievehtml.hpp"

std::string currentDate() {
  std::time_t now = std::time(nullptr);
  std::tm* local = std::localtime(&now);
  char buffer[11];  // YYYY-MM-DD + null
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", local);
  return std::string(buffer);
}

int main() {
  //
  // today page
  const char url[] = "https://www.sportsbet.com.au/racing-schedule/horse/today";
  char* todays_html = static_cast<char*>(std::malloc(1));  // char* todays_html = (char*)malloc(1);
  if (!todays_html) return 1;
  todays_html[0] = '\0';
  retrieve_html(todays_html, url);
  GumboOutput* output = gumbo_parse(todays_html);
  free(todays_html);
  if (!output) return 1;
  std::vector<std::string> links;
  search_for_links(output->root, &links);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  int num_links = links.size();
  for (int i = 0; i < num_links; i++) {
    links[i] = "https://www.sportsbet.com.au" + links[i];
  }

  //
  // get html asynchronously from each race page
  std::vector<Request> requests = asyncdownloads(links);
  std::cout << "\n";

  int num_races = requests.size();
  int i = 0;
  std::vector<Race> races;
  for (auto& request : requests) {
    races.emplace_back(request.url, request.html.c_str());
    std::cout << "\33[2K\rparsing: " << ceil(static_cast<float>(i) / num_races * 100) << "%" << std::flush;
    i++;
  }
  std::cout << "\n";

  //
  // out
  std::ofstream output_file;
  std::string file_name = "out/racing_" + currentDate() + ".csv";
  output_file.open(file_name);

  i = 0;
  std::string race_url;
  std::string row = "name, win odds, place odds, fluc open, fluc 1, fluc 2, jockey, weight, position, race url\n";
  output_file << row;

  for (Race& race : races) {
    row.clear();
    race_url.clear();
    race_url = race.get_url();

    if (race.is_race_complete()) {
      for (Horse& horse : race.get_horses()) {
        row = horse.get_name() + "," + horse.get_win_odds() + "," + horse.get_place_odds() + "," +
              horse.get_fluc_open() + "," + horse.get_fluc_1() + "," + horse.get_fluc_2() + "," +
              horse.get_jockey() + "," + horse.get_weight() + "," + horse.get_position() + "," +
              race_url + "\n";
        output_file << row;
      }
    } else {
      // row = "XX,XX,XX,XX," + race_url + "," + std::string(race.get_html(), 0, 200) + "\n";
      // output_file << row;
    }

    std::cout << "\33[2K\ruploading: " << ceil(static_cast<float>(i) / num_races * 100) << "%" << std::flush;
    i++;
  }

  output_file.close();
  std::cout << "\n";

  return 0;
}