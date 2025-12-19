#ifndef PARSEHTML_H
#define PARSEHTML_H

#include <gumbo.h>

#include <cstddef>
#include <string>
#include <vector>

void search_for_links(GumboNode* node, std::vector<std::string>* links);

// should use a template, std::vector<T>&, so I can switch between string (copy)
// and const char* gumbo owned refrence. if I use constexpr in a conditional
// statment in the function this will have zero overhead.
void get_text(GumboNode* node, std::vector<std::string>& out);

void search_html_divs(GumboNode* node, std::vector<std::string>& out,
                      char* _attribute, char* attribute_value);

#endif