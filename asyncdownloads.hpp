#ifndef ASYNCDOWNLOADS_HPP_
#define ASYNCDOWNLOADS_HPP_

#include <curl/curl.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "retrievehtml.hpp"

struct Request {
  std::string url;
  std::string html;
  Request(std::string _url, std::string _html) : url(_url), html(_html) {}
};

std::vector<Request> asyncdownloads(std::vector<std::string>& links) {
  CURLM* multi = curl_multi_init();
  curl_multi_setopt(multi, CURLMOPT_MAX_HOST_CONNECTIONS, 2L);
  curl_multi_setopt(multi, CURLMOPT_MAX_TOTAL_CONNECTIONS, 8L);
  curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);

  std::unordered_map<CURL*, std::string> responses;  // handle + downloaded html
  std::vector<CURL*> handles;

  for (auto& link : links) {
    CURL* easy = curl_easy_init();
    if (!easy) return {};

    responses[easy] = "";

    curl_easy_setopt(easy, CURLOPT_URL, link.c_str());
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, CurlWriter<std::string>::write);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, &responses[easy]);

    // curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1L);  // accept redirects - default is up to 50 i think
    // curl_easy_setopt(easy, CURLOPT_USERAGENT,
    //                  "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
    //                  "(KHTML, like Gecko) Chrome/121.0.0.0 Safari/537.36");  // help pretend to be browser I think: Mozilla/5.0 (compatible; async-curl/1.0)
    // curl_easy_setopt(easy, CURLOPT_COOKIEFILE, "");
    // curl_easy_setopt(easy, CURLOPT_COOKIEJAR, "cookies.txt");

    curl_multi_add_handle(multi, easy);
    handles.push_back(easy);
  }

  int running = 0;
  curl_multi_perform(multi, &running);
  int running_prev = running;
  while (running) {
    curl_multi_poll(multi, nullptr, 0, 1000, nullptr);  // sleep until I/O is ready or timeout to reduce cpu usage
    curl_multi_perform(multi, &running);
    std::cout << "\33[2K\rdownloads left: " << running << std::flush;
  }

  std::vector<Request> requests;

  for (CURL* easy : handles) {
    char* url = nullptr;
    curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &url);

    requests.emplace_back(std::string(url), responses[easy]);
    // std::cout << "Downloaded from: " << url << "\n";
    // std::cout << "Size: " << responses[easy].size() << " bytes\n\n";

    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
  }

  curl_multi_cleanup(multi);

  return requests;
}

#endif