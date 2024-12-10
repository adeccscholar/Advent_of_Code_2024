#pragma once

// Project: Solutions for Advent of Code 2024
// implementation to use std::ostream with Qt Widgets (Qt 6)
// example to find and isolate basic functionality for different problems
// 
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include <QLabel>
#include <QTextEdit>

#include <iostream>
#include <string>
#include <sstream>

class LabelStreamBuf : public std::streambuf {
private:
   QLabel* value;
   std::stringstream os;
public:
   explicit LabelStreamBuf(QLabel* para, bool boClean = true) : std::streambuf() {
      value = para;
      if (boClean) value->clear();
      }

   virtual ~LabelStreamBuf(void) { value = nullptr; }

   virtual int_type overflow(int_type c) override {
      switch (c) {
         case '\n':
            value->setText(QString::fromStdString(os.str()));
            os.str("");
            break;
         [[likely]] default:
            os.put(c);
         }
      return c;
      }
   };


class MemoStreamBuf : public std::streambuf {
private:
   QTextEdit* value;
   std::stringstream os;
public:
   MemoStreamBuf(QTextEdit* para, bool boClean = true) : std::streambuf() {
      value = para;
      if (boClean) value->clear();
      }

   virtual ~MemoStreamBuf(void) { value = nullptr; }

   virtual int_type overflow(int_type c) override {
      switch (c) {
         case '\n':
            if (os.str().length() > 0) [[likely]] {
               value->moveCursor(QTextCursor::End);
               value->insertPlainText(QString::fromStdString(os.str()));
               value->insertPlainText("\n");
               }
            else {
               value->append("");
               }         
            os.str("");
            break;
         [[likely]] default:
            os.put(c);
         }
      return c;
      }

   };

class stream_raii_wrapper {
private:
   std::ostream& str;
   std::streambuf* old = nullptr;
public:
   stream_raii_wrapper(void) = delete;
   stream_raii_wrapper(stream_raii_wrapper const&) = delete;

   stream_raii_wrapper(std::ostream& ref) : str(ref) { old = nullptr; }
   ~stream_raii_wrapper(void) {
      if (old != nullptr) {
         delete str.rdbuf(old);
         }
      }

   void reset(void) {
      if (old != nullptr) {
         delete str.rdbuf(old);
         old = nullptr;
         }
      }

   void activate(QTextEdit* elem, bool clean = false) {
      reset();
      old = str.rdbuf(new MemoStreamBuf(elem, clean));
      }

   void activate(QLabel* elem, bool clean = false) {
      reset();
      old = str.rdbuf(new LabelStreamBuf(elem, clean));
      }
   };

