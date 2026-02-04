#ifndef PARSEHTML_H
#define PARSEHTML_H

#include <gumbo.h>

#include <cstddef>
#include <string>
#include <vector>

void search_for_links(GumboNode* node, std::vector<std::string>* links);

// get text in a node
void get_text(GumboNode* node, std::vector<std::string>& out);
// return all text in desired divs
void search_in_divs(GumboNode* node, std::vector<std::string>& out, const char* _attribute, const char* attribute_value);
// return first div
GumboNode* get_div(GumboNode* node, const char* _attribute, const char* attribute_value);
// values obotained from pass by refference
void get_all_divs(GumboNode* curr_node, std::vector<GumboNode*>& matching_nodes, const char* _attribute, const char* attribute_value);

#endif