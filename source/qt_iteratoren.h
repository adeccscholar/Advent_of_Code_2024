#pragma once

#include "my_common_tools.h"

#include <QString>
#include <QListWidget>
#include <QComboBox>

#include <iterator>
#include <string>
#include <string_view>
#include <optional>
#include <stdexcept>


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
