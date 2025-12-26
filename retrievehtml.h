#ifndef RETRIEVEHTML_H
#define RETRIEVEHTML_H

#include <cstddef>

size_t write_data_cstr(char* ptr, size_t size, size_t nmemb, void* userdata);
void retrieve_html(char** html, const char* url);

#endif