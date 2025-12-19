#include "retrievehtml.h"

#include <curl/curl.h>

#include <cstdlib>
#include <cstring>

size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata) {
  char** html = static_cast<char**>(userdata);
  size_t len = strlen(*html);
  // copies old conntents and deallocs old ptr automatically
  char* new_html = (char*)realloc(*html, size * (len + nmemb + 1));
  if (!new_html) return 0;

  memcpy(new_html + len, ptr, nmemb);
  new_html[len + nmemb] = '\0';

  *html = new_html;

  return size * nmemb;
}

void retrieve_html(char** html, char* url) {
  if (url == nullptr) return;
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, html);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return;
}
