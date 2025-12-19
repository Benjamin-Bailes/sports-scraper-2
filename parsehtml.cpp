#include "parsehtml.h"

#include <gumbo.h>

#include <iostream>
#include <string>
#include <vector>

void search_for_links(GumboNode* node, std::vector<std::string>* links) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    const char* value = href->value;
    if (href && std::strstr(value, "/horse-racing/") != nullptr) {
      (*links).push_back(href->value);
      // std::cout << value << std::endl;
    }
  }

  GumboVector* children = &node->v.element.children;
  for (int i = 0; i < children->length; ++i) {
    search_for_links(static_cast<GumboNode*>(children->data[i]), links);
  }
}

// should use a template, std::vector<T>&, so I can switch between string (copy)
// and const char* gumbo owned refrence. if I use constexpr in a conditional
// statment in the function this will have zero overhead.
void get_text(GumboNode* node, std::vector<std::string>& out) {
  if (!node) return;
  if (node->type == GUMBO_NODE_TEXT) {
    // std::cout << node->v.text.text << std::endl;
    out.emplace_back(node->v.text.text);
    return;
  }

  if (node->type == GUMBO_NODE_ELEMENT) {
    GumboVector* children = &node->v.element.children;
    for (int i = 0; i < children->length; ++i) {
      get_text(static_cast<GumboNode*>(children->data[i]), out);
    }
  }
}

// _attribute = "data-automation-id"
// attribute_value = "racecard-outcome-name"
void search_html_divs(GumboNode* node, std::vector<std::string>& out,
                      char* _attribute, char* attribute_value) {
  if (!node) return;
  if (node->type != GUMBO_NODE_ELEMENT) return;

  GumboAttribute* attribute;
  if (node->v.element.tag == GUMBO_TAG_DIV &&
      (attribute =
           gumbo_get_attribute(&node->v.element.attributes, _attribute))) {
    // check for example that the attribute value
    if (strstr(attribute_value, attribute->value) != NULL) {
      get_text(node, out);
    }
  }

  // recursive
  GumboVector* children = &node->v.element.children;
  for (int i = 0; i < children->length; ++i) {
    search_html_divs(static_cast<GumboNode*>(children->data[i]), out,
                     _attribute, attribute_value);
  }
}