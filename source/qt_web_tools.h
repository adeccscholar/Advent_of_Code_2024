#pragma once

#include <QCoreApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QEventLoop>
#include <QVariant>
#include <QUrl>
#include <QDebug>

#include <string>
#include <stdexcept>

using namespace std::string_literals;

inline void setClipboardContent(std::string const& value) {
   QApplication::clipboard()->setText(QString::fromStdString(value));
   }

inline std::string getClipboardContent() {
   return QApplication::clipboard()->text().toStdString();
   }

inline void call_URL_with_qt(std::string const& strUrl) {
   QString qUrlString = QString::fromStdString(strUrl);
   QUrl url(qUrlString);
   if (!url.isValid()) {
      throw std::invalid_argument("invalid URL:"s + strUrl);
      }

   if (!QDesktopServices::openUrl(url)) {
      throw std::runtime_error("couldn't open URL:"s + strUrl);
      }
   }

inline QString getQTextFromUrl(std::string const& strUrl) {
   QWebEngineView webView;
   QEventLoop loop;
   QUrl url(QString::fromStdString(strUrl));

   // Verbindet das 'loadFinished' Signal mit dem Eventloop, um zu warten
   QObject::connect(&webView, &QWebEngineView::loadFinished, &loop, &QEventLoop::quit);

   webView.load(url);
   loop.exec();

   QString pageText;
   webView.page()->runJavaScript("document.body.innerText", [&pageText](const QVariant& result) {
      pageText = result.toString();
      });

   //loop.exec();

   return pageText;
   }

inline std::string getTextFromUrl(std::string const& strUrl) {
   return getQTextFromUrl(strUrl).toStdString();
   }