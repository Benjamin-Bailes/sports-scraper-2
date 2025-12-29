#include <curl/curl.h>
#include <gumbo.h>

#include <cstdlib>
// #include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Horse.h"
#include "Race.h"
#include "asyncdownloads.hpp"
#include "parsehtml.h"
#include "retrievehtml.hpp"

int main() {
  // today page
  const char url[] = "https://www.sportsbet.com.au/racing-schedule/horse/today";
  char* todays_html = static_cast<char*>(std::malloc(1));  // char* todays_html = (char*)malloc(1);
  if (!todays_html) return 1;
  todays_html[0] = '\0';
  retrieve_html(todays_html, url);

  // parse links to all races
  GumboOutput* output = gumbo_parse(todays_html);
  free(todays_html);
  if (!output) return 1;
  std::vector<std::string> links;
  search_for_links(output->root, &links);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
  for (int i = 0; i < links.size(); i++) {
    links[i] = "https://www.sportsbet.com.au" + links[i];
  }

  /*
  //
  //
  //
  // CURLM* multi = curl_multi_init();

  // std::unordered_map<CURL*, std::string> responses;  // handle + downloaded html
  // std::vector<CURL*> handles;

  // for (std::string& link : links) {
  //   CURL* easy = curl_easy_init();
  //   if (!easy) return 1;

  //   responses[easy] = "";

  //   curl_easy_setopt(easy, CURLOPT_URL, link.c_str());
  //   curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, CurlWriter<std::string>::write);
  //   curl_easy_setopt(easy, CURLOPT_WRITEDATA, &responses[easy]);

  //   curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1L);                                     // accept redirects - default is upto 50 i think
  //   curl_easy_setopt(easy, CURLOPT_USERAGENT, "Mozilla/5.0 (compatible; async-curl/1.0)");  // help pretend to be browser I think

  //   curl_multi_add_handle(multi, easy);
  //   handles.push_back(easy);
  // }

  // int running = 0;
  // curl_multi_perform(multi, &running);
  // int running_prev = running;
  // while (running) {
  //   curl_multi_poll(multi, nullptr, 0, 3000, nullptr);  // sleep until I/O is ready or timeout to reduce cpu usage
  //   curl_multi_perform(multi, &running);
  //   std::cout << "\rpages dowloaded: " << running;
  // }

  // for (CURL* easy : handles) {
  //   char* url = nullptr;
  //   curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &url);

  //   std::cout << "Downloaded from: " << url << "\n";
  //   std::cout << "Size: " << responses[easy].size() << " bytes\n\n";

  //   curl_multi_remove_handle(multi, easy);
  //   curl_easy_cleanup(easy);
  // }

  // curl_multi_cleanup(multi);
  //
  //
  //
  */

  // get html asynchronously from each race page
  std::vector<Request> requests = asyncdownloads(links);
  std::cout << "\n";
  // for (auto request : requests) {
  //   std::cout << "Downloaded from: " << request.url << "\n";
  //   std::cout << "Size: " << request.html.size() << " bytes\n\n";
  // }

  int num_races = requests.size();
  int i = 0;
  std::vector<Race> races;
  for (auto& request : requests) {
    races.emplace_back(request.url, request.html.c_str());
    std::cout << "\33[2K\rparsing: " << static_cast<float>(i) / num_races * 100 << "%" << std::flush;
    i++;
  }
  std::cout << "\n";

  //
  //
  // for (int i = 0; i < 10; i++) {
  //   std::string name;
  //   name = "out/page" + std::to_string(i) + ".html";
  //   std::string total = requests[i].url + requests[i].html;
  //   FILE* out = fopen(name.c_str(), "wb");  // write binary output
  //   if (!out) return 1;
  //   fprintf(out, "%s", total.c_str());
  //   fclose(out);
  // }
  //
  //

  std::ofstream output_file;
  output_file.open("out/data3.csv");

  i = 0;
  std::string race_url;
  std::string row = "name, win odds, place odds, position, race url\n";
  output_file << row;

  for (Race& race : races) {
    row.clear();
    race_url.clear();
    race_url = race.get_url();

    if (race.is_race_complete()) {
      for (Horse& horse : race.get_horses()) {
        row = horse.name + "," + horse.win_odds + "," + horse.place_odds + "," + std::to_string(horse.position) + "," + race_url + "\n";
        output_file << row;
      }
    } else {
      row = "XX,XX,XX,XX," + race_url + "," + std::string(race.get_html(), 0, 41) + "\n";
      output_file << row;
    }

    std::cout << "\33[2K\ruploading: " << static_cast<float>(i) / num_races * 100 << "%" << std::flush;
    i++;
  }

  output_file.close();
  std::cout << "\n";

  return 0;
}