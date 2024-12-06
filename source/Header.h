#pragma once

#include <QTextEdit>
#include <QTextBlock>
#include <QString>
#include <iterator>
#include <string>

class container_for_qt_textedit {
   friend void swap(container_for_qt_textedit& lhs, container_for_qt_textedit& rhs) noexcept { lhs.swap(rhs); }
private:
   QTextEdit* input = nullptr;
public:
   container_for_qt_textedit() = default;
   container_for_qt_textedit(container_for_qt_textedit const& ref) : input(ref.input) { }
   explicit container_for_qt_textedit(QTextEdit* textEdit) : input(textEdit) {}

   void swap(container_for_qt_textedit& other) noexcept {
      using std::swap;
      std::swap(input, other.input);
   }

   class view_for_qt_textedit {
   private:
      container_for_qt_textedit const& container;
   public:
      view_for_qt_textedit(container_for_qt_textedit const& param) : container(param) {}
      view_for_qt_textedit(view_for_qt_textedit const& other) = default;

      class Iterator {
      public:
         using iterator_category = std::input_iterator_tag;
         using value_type = std::string;
         using difference_type = std::ptrdiff_t;
         using pointer = const std::string*;
         using reference = const std::string&;

         Iterator() : txtInput(nullptr), iCurrentLine(-1) {}
         Iterator(QTextEdit* textEdit) : txtInput(textEdit), iCurrentLine(0) {
            if (txtInput && iCurrentLine < txtInput->document()->blockCount()) {
               updateCurrentText();
            }
         }

         reference operator*() { return strCurrent; }
         reference operator*() const { return strCurrent; }
         pointer   operator->() const { return &strCurrent; }

         Iterator& operator = (Iterator const& other) {
            if (this != &other) {
               txtInput = other.txtInput;
               iCurrentLine = other.iCurrentLine;
               strCurrent = other.strCurrent;
            }
            return *this;
         }


         Iterator& operator++() {
            ++iCurrentLine;
            if (txtInput && iCurrentLine < txtInput->document()->blockCount()) {
               updateCurrentText();
            }
            else {
               strCurrent.clear();
               iCurrentLine = -1;
            }
            return *this;
         }

         Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
         }

         bool operator==(const Iterator& other) const {
            return iCurrentLine == other.iCurrentLine;
         }

         bool operator!=(const Iterator& other) const {
            return !(*this == other);
         }

      private:
         QTextEdit* txtInput = nullptr;
         int iCurrentLine = 0;
         std::string strCurrent;

         void updateCurrentText() {
            if (txtInput) {
               QString qstr = txtInput->document()->findBlockByLineNumber(iCurrentLine).text();
               strCurrent = qstr.toStdString();
            }
         }
      };

      Iterator begin() { return Iterator(container.input); }
      Iterator end() { return Iterator(); }

      Iterator begin() const { return Iterator(container.input); }
      Iterator end() const { return Iterator(); }
   };

   view_for_qt_textedit view() { return view_for_qt_textedit(*this); }
   view_for_qt_textedit view() const { return view_for_qt_textedit(*this); }
};
