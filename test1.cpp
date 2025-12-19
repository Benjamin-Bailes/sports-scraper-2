// #include <stdio.h>
#include <curl/curl.h>
#include <gumbo.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata) {
//   std::string* html = static_cast<std::string*>(userdata);
//   html->append(ptr, size * nmemb);
//   return size * nmemb;
// }
// size_t write_file(char* ptr, size_t size, size_t nmemb, void* userdata) {
//   FILE* fp = static_cast<FILE*>(userdata);
//   return fwrite(ptr, size, nmemb, fp);
// }
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

static void search_for_links(GumboNode* node, std::vector<std::string>* links) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    // std::cout << href->value << std::endl;
    (*links).push_back(href->value);
  }

  GumboVector* children = &node->v.element.children;
  for (int i = 0; i < children->length; ++i) {
    search_for_links(static_cast<GumboNode*>(children->data[i]), links);
  }
}

int main() {
  char url[] = "https://www.sportsbet.com.au/racing-schedule/horse/today";
  // std::string html;
  char* html = (char*)malloc(1);

  // FILE* out = fopen("page.html", "wb");  // write binary output
  // if (!out) return 1;
  // fclose(out);
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  GumboOutput* output = gumbo_parse(html);
  std::vector<std::string>* links;
  search_for_links(output->root, links);
  gumbo_destroy_output(&kGumboDefaultOptions, output);

  std::cout << (*links)[1];

  free(html);
  return 0;
}