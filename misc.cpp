#include <cstdlib>
#include <cstring>
#include <iostream>

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

int main() {
  const char* init = "Hello world";
  size_t length = strlen(init);
  char* my_string = (char*)malloc(length + 1);
  strcpy(my_string, init);

  std::cout << my_string << "\n" << strlen(my_string) << "\n" << length << "\n";

  char buffer[6] = " nose";

  write_data(buffer, sizeof(char), 5, &my_string);

  std::cout << "\n" << my_string << "\n" << strlen(my_string) << "\n";

  // if (my_string[length + 5] == '\0') {
  //   std::cout << "yippie";
  // }
  free(my_string);
  return 0;
}