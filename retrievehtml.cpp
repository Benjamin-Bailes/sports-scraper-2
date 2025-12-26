#include "retrievehtml.h"

#include <curl/curl.h>

#include <cstdlib>
#include <cstring>
#include <string>

size_t write_data_cstr(char* ptr, size_t size, size_t nmemb, void* userdata) {
  size_t realsize = size * nmemb;
  char** html = (char**)(userdata);

  size_t len = 0;
  if (*html) {
    len = strlen(*html);
  }
  // copies old conntents and deallocs old ptr automatically
  char* new_html = (char*)realloc(*html, len + realsize + 1);
  if (!new_html) return 0;

  memcpy(new_html + len, ptr, realsize);
  new_html[len + realsize] = '\0';
  *html = new_html;

  return size * realsize;
}

void retrieve_html(char** html, const char* url) {
  if (!url) return;
  CURL* curl = curl_easy_init();
  if (!curl) return;
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_cstr);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, html);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return;
}
