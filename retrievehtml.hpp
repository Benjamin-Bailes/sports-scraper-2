#ifndef RETRIEVEHTML_HPP_
#define RETRIEVEHTML_HPP_

#include <curl/curl.h>

#include <cstddef>
#include <string>

template <typename T>
struct CurlWriter;
template <>
struct CurlWriter<std::string> {
  static size_t write(char* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* s = static_cast<std::string*>(userdata);
    s->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
  }
};
template <>
struct CurlWriter<char*> {
  static size_t write(char* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t realsize = size * nmemb;
    char** html = (char**)(userdata);

    size_t len = 0;
    if (*html) {
      len = strlen(*html);
    }
    // copies old contents and deallocs old ptr automatically
    char* new_html = (char*)realloc(*html, len + realsize + 1);
    if (!new_html) return 0;

    memcpy(new_html + len, ptr, realsize);
    new_html[len + realsize] = '\0';
    *html = new_html;

    return size * realsize;
  }
};

//
//
template <typename T>
void retrieve_html(T& html, const char* url) {
  if (!url) return;
  CURL* curl = curl_easy_init();
  if (!curl) return;
  curl_easy_setopt(curl, CURLOPT_URL, url);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriter<T>::write);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return;
}

// size_t write_data_cstr(char* ptr, size_t size, size_t nmemb, void* userdata);
// template <typename T>
// void retrieve_html(T& html, const char* url);

// extern template void retrieve_html<std::string>(std::string&, const char*);
// extern template void retrieve_html<char*>(char*&, const char*);

#endif