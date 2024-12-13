// Project: Solutions for Advent of Code 2024
// main function for the Qt Project
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QSettings>
#include <QFontDatabase>

// attention, die is Windows

static bool isDarkModeEnabled() {
#ifdef _WIN32
   QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);
   QVariant value = settings.value("AppsUseLightTheme");
   return (value.isValid() && value.toInt() == 0); // 0 = Dark Mode, 1 = Light Mode
#else
   QPalette palette = QApplication::palette();
   return palette.color(QPalette::Window).lightness() < 128; // Dunkelmodus, wenn die Helligkeit der Fensterfarbe unter 128 liegt
#endif
   }

static void applyDarkMode(QApplication& app) {
   QPalette darkPalette;
   darkPalette.setColor(QPalette::Window, QColor(18, 18, 18));
   darkPalette.setColor(QPalette::WindowText, Qt::white);
   darkPalette.setColor(QPalette::Base, QColor(28, 28, 28));
   darkPalette.setColor(QPalette::AlternateBase, QColor(38, 38, 38));
   darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
   darkPalette.setColor(QPalette::ToolTipText, Qt::white);
   darkPalette.setColor(QPalette::Text, Qt::white);
   darkPalette.setColor(QPalette::Button, QColor(28, 28, 28));
   darkPalette.setColor(QPalette::ButtonText, Qt::white);
   darkPalette.setColor(QPalette::BrightText, Qt::red);

   app.setPalette(darkPalette);
}


void loadFonts() {
   QStringList fontFiles = {
       ":/Fonts/Fonts/DejaVuSans.ttf",
       ":/Fonts/Fonts/DejaVuSansCondensed.ttf",
       ":/Fonts/Fonts/DejaVuSansCondensed-Bold.ttf",
       ":/Fonts/Fonts/DejaVuSansCondensed-BoldOblique.ttf",
       ":/Fonts/Fonts/DejaVuSansCondensed-Oblique.ttf",
       ":/Fonts/Fonts/DejaVuSansMono.ttf",
       ":/Fonts/Fonts/DejaVuSansMono-Bold.ttf",
       ":/Fonts/Fonts/DejaVuSansMono-Oblique.ttf",
       ":/Fonts/Fonts/DejaVuSansMono-BoldOblique.ttf"
      };

   for (const QString& fontFile : fontFiles) {
      int fontId = QFontDatabase::addApplicationFont(fontFile);
      if (fontId == -1) {
         qDebug() << "Failed to load font:" << fontFile;
         }
      else {
         QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
         qDebug() << "Loaded font families for" << fontFile << ":" << fontFamilies;
         }
      }
   }


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool hasDarkMode = isDarkModeEnabled();
    loadFonts();

    QFile file(":/Data/adecc_scholar.css"); 
    if (file.open(QFile::ReadOnly | QFile::Text)) {
       QTextStream stream(&file);
       QString style = stream.readAll();
       a.setStyleSheet(style); 
       }

    a.setWindowIcon(QIcon(":/adecc_Scholar.ico"));

    if (hasDarkMode) {
       applyDarkMode(a);
       }
    MainWindow w;


    w.show();
    return a.exec();
}
