#pragma once

// Project: Solutions for Advent of Code 2024
// iterators for qt widgets (Qt 6)
// example to find and isolate basic functionality for different problems
// 
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"
#include "my_iterators.h"

#include <QString>
#include <QListWidget>
#include <QComboBox>

#include <iterator>
#include <string>
#include <string_view>
#include <optional>
#include <stdexcept>


struct my_qt_textedit_lines {
   my_qt_textedit_lines(QTextEdit* other) { 
      input = other;
      strBuffer = getText(input);
      theText = { strBuffer.data(), strBuffer.size() };
      }

   my_qt_textedit_lines(my_qt_textedit_lines const& ref) { 
      input = ref.input;
      strBuffer = ref.strBuffer;
      theText = ref.theText; 
      }

   my_line_iterator begin() const { return my_line_iterator(theText); }
   my_line_iterator end() const { return my_line_iterator(); }


   std::string getText(QTextEdit* textEdit) {
      return textEdit->toPlainText().toStdString();
      }

   QTextEdit*  input;
   std::string strBuffer;
   std::string_view theText;
};


template <typename ty>
concept my_qt_out_ty = std::is_same_v<ty, QListWidget> || std::is_same_v<ty, QComboBox>;


template <my_qt_out_ty widget_ty>
class qt_widget_output_iterator {
public:
   using iterator_category = std::output_iterator_tag;
   using value_type = std::string;
   using difference_type = std::ptrdiff_t;
   using pointer = void;
   using reference = void;

   explicit qt_widget_output_iterator(widget_ty* widget) : element(widget) {}

   qt_widget_output_iterator& operator=(const std::string& value) {
      QString qValue = QString::fromStdString(value); // Umwandlung von std::string zu QString
      if constexpr (std::is_same_v<widget_ty, QListWidget>) {
         element->addItem(qValue);
         }
      else if constexpr (std::is_same_v<widget_ty, QComboBox>) {
         element->addItem(qValue);
         }
      return *this;
      }

   qt_widget_output_iterator& operator*() { return *this; }
   qt_widget_output_iterator& operator++() { return *this; }
   qt_widget_output_iterator operator++(int) { return *this; }

private:
   widget_ty* element;
};

template <my_qt_out_ty widget_ty>
qt_widget_output_iterator<widget_ty> make_qt_widget_output_iterator(widget_ty* widget) {
   return qt_widget_output_iterator<widget_ty>(widget);
   }
