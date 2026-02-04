#ifndef PARSEHTML_H
#define PARSEHTML_H

#include <gumbo.h>

#include <cstddef>
#include <string>
#include <vector>

void search_for_links(GumboNode* node, std::vector<std::string>* links);

// get text in a node
void get_text(GumboNode* node, std::vector<std::string>& out);
// get first text in a node
std::string get_first_text(GumboNode* node);
// return text in desired divs
void search_in_divs(GumboNode* node, std::vector<std::string>& out, const char* _attribute, const char* attribute_value);
// return first text in desired divs
std::string get_text_from_first_div(GumboNode* node, const char* _attribute, const char* attribute_value);

void get_all_divs(GumboNode* curr_node, std::vector<GumboNode*>& matching_nodes, const char* _attribute, const char* attribute_value);
// first div
GumboNode* get_div(GumboNode* node, const char* _attribute, const char* attribute_value);

#endif