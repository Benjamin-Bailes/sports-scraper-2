#ifndef RETRIEVEHTML_H
#define RETRIEVEHTML_H

#include <cstddef>

size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata);
void retrieve_html(char** html, char* url);

#endif