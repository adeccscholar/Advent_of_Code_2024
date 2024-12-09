#pragma once

#include "qt_stream.h"
#include "qt_iteratoren.h"

#include <QApplication>
#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QComboBox>
#include <QGroupBox>

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdexcept>

class TMyForm {
friend void swap(TMyForm& lhs, TMyForm& rhs) noexcept {
              lhs.swap(rhs);
              }
public:
   TMyForm() = delete;
   TMyForm(TMyForm const&) = delete;

   TMyForm(TMyForm&& ref) noexcept {
      std::swap(frm, ref.frm);
      }

   TMyForm(QWidget* p) : frm(p) { }

   void swap(TMyForm& ref) noexcept {
      std::swap(frm, ref.frm);
      }


   // operator QWidget* () { return frm; }

   static void ProcessEvents(void) {
      QApplication::processEvents();
      }

   void SetCaption(std::string const& text) {
      frm->setWindowTitle(QString::fromStdString(text));
      }

   void SetEdit(std::string const& name, std::string const& value) {
      Find<QLineEdit>(name)->setText(QString::fromStdString(value));
      }

   
   void SetEdit(std::vector<std::string> const& names, std::string const& value) {
      for(auto const& name : names)  Find<QLineEdit>(name)->setText(QString::fromStdString(value));
      }

   void SetEdit(std::vector<std::pair<std::string, std::string>> const& paras) {
      for (auto const& [name, value] : paras)  Find<QLineEdit>(name)->setText(QString::fromStdString(value));
      }

   void GetEdit(std::string const& name, std::string& value) {
      value = Find<QLineEdit>(name)->text().toStdString();
      }

   std::string GetEditAs(std::string const& strField) {
      std::string value;
      GetEdit(strField, value);
      return value;
      }

   void InitCheckBox(std::string const& name, std::string const& text, bool value = true) {
      auto fld = Find<QCheckBox>(name);
      fld->setText(QString::fromStdString(text));
      fld->setCheckState(value ? Qt::Checked : Qt::Unchecked);
      }

   void SetCheckBox(std::string const& name, std::string const& value) {
      Find<QCheckBox>(name)->setText(QString::fromStdString(value));
      }

   void SetCheckBox(std::string const& name, bool value) {
      Find<QCheckBox>(name)->setCheckState(value ? Qt::Checked : Qt::Unchecked);
      }

   void GetCheckBox(std::string const& strField, bool& value) {
      value = Find<QCheckBox>(strField)->checkState() == Qt::Checked;
      }

   bool GetCheckBox(std::string const& strField) {
      bool value;
      GetCheckBox(strField, value);
      return value;
      }

   void SetLabel(std::string const& name, std::string const& value) {
      Find<QLabel>(name)->setText(QString::fromStdString(value));
      }

   void SetButton(std::string const& name, std::string const& value) {
      Find<QPushButton>(name)->setText(QString::fromStdString(value));
      }

   void SetGroupBox(std::string const& name, std::string const& value) {
      Find<QGroupBox>(name)->setTitle(QString::fromStdString(value));
      }

   void Clear(std::string const& name) {
      Find<QTextEdit>(name)->clear();
      }

   std::streambuf* GetAsStreamBuf(std::string const& name, bool clean) {
      return new MemoStreamBuf(Find<QTextEdit>(name), clean);
      }

   void EnableUpdates(std::string const& name, bool boActive) {
      Find<QTextEdit>(name)->setUpdatesEnabled(boActive);
      }

   void LastItem(std::string const& name) {
      auto ptr = Find<QComboBox>(name);
      if (auto cnt = ptr->count(); cnt > 0) ptr->setCurrentIndex(cnt - 1);
      }

   std::string CurrentText(std::string const& name) {
      auto ptr = Find<QComboBox>(name);
      return ptr->currentText().toStdString();
      }

   std::string GetText(std::string const& name) {
      return Find<QTextEdit>(name)->toPlainText().toStdString();
      }

   my_qt_textedit_lines GetLines(std::string const& name) {
      return my_qt_textedit_lines(Find<QTextEdit>(name));
      }

   template <typename ty>
   ty* Find(std::string const& name) {
      auto* comp = frm->findChild<QObject* >(QString::fromStdString(name));
      if (!comp) throw std::runtime_error(std::format("A field with the name \"{}\" don\'t exist in the formular \"{}\"", name, FormName()));
      else [[likely]] {
         ty* field = dynamic_cast<ty*>(comp);
         if(!field) throw std::runtime_error(std::format("A field \"{}\" exist in the formular \"{}\", but can\'t cast to "
                                                         "requested type", name, FormName(), typeid(ty).name()));
         else [[likely]] {
            return field;
            }
         }
      }


private:
   QWidget* frm;

   std::string FormName() {
      return frm->objectName().toStdString();;
      }

   };



class TMyWait {
private:
   QWidget* currentWidget = nullptr;
   QTextEdit* textfield = nullptr;
public:
   TMyWait(std::string const& element = "") {
      currentWidget = QApplication::activeWindow();
      if (currentWidget) {
         currentWidget->setCursor(Qt::WaitCursor);
      
         if (element.size() > 0) {
            TMyForm frm(currentWidget);
            textfield = frm.Find<QTextEdit>(element);
            textfield->setUpdatesEnabled(false);
            }
         }
      }

   TMyWait(TMyWait const&) = delete;

   ~TMyWait(void) {
      if (currentWidget) {
         if(textfield) textfield->setUpdatesEnabled(true);
         currentWidget->unsetCursor();
         }
   }
};

