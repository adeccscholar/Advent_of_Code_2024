#pragma once

// Project: Solutions for Advent of Code 2024
// implementation of a basically iterator implementation for split a string_view in a range
// example to find and isolate basic functionality for different problems
// this classes are part of the adecc Scholar repository and separated for this project
// 
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "my_common_tools.h"

#include <iterator>
#include <string_view>
#include <optional>

struct my_line_iterator {
   using iterator_category = std::input_iterator_tag;
   using value_type = std::string_view;
   using difference_type = std::ptrdiff_t;
   using reference_type = value_type const&;
   using pointer_type = const value_type*;

   my_line_iterator() { };
   my_line_iterator(std::string_view const& input) : theText(input) { ++*this; };

   my_line_iterator& operator = (std::string_view const& vw) {
      theText = vw;
      start_pos = 0u;
      end_pos = 0u;
      return *this;
      }

   my_line_iterator& operator = (my_line_iterator const& ref) {
      theText = ref.theText;
      theLine = ref.theLine;
      start_pos = ref.start_pos;
      end_pos = ref.end_pos;
      return *this;
      }

   reference_type operator*() const { return theLine; }
   pointer_type operator->() const { return &theLine; }

   my_line_iterator& operator++() {
      if (theText) {
         end_pos = theText->find('\n', start_pos);
         if (end_pos != std::string_view::npos) {
            theLine = theText->substr(start_pos, end_pos - start_pos);
            start_pos = end_pos + 1;
            }
         else {
            //theText = { };
            theText = std::nullopt;
            }
         }
      return *this;
      }

   my_line_iterator operator++(int) {
      auto elem(*this);
      ++*this;
      return elem;
      }

   friend  bool operator == (my_line_iterator const& lhs, my_line_iterator const& rhs) {
      return lhs.theText == rhs.theText;
      }

   friend  bool operator != (my_line_iterator const& lhs, my_line_iterator const& rhs) {
      return !(lhs == rhs);
      }

   std::string AsString() {
      if (!theText) throw std::runtime_error("text isn't initialize.");
      else return to_String(trim(*theText));
      }
private:
   std::optional<std::string_view> theText = { };
   std::string_view                theLine;
   size_t                          start_pos = 0u, end_pos = 0u;
};

struct my_lines {
   my_lines(std::string_view const& input) { theText = input; }
   my_lines(my_lines const& ref) { theText = ref.theText; }

   my_line_iterator begin() const { return my_line_iterator(theText); }
   my_line_iterator end() const { return my_line_iterator(); }


   std::string_view theText;

};
