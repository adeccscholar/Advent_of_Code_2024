#pragma once

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <tuple>
#include <string>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    QLabel* statusLabel;

    void Init();
    void Info();
    void PasteData();
    void Description();
    void DataURL();
    void OldRiddle();
    void CopyTestData(int);
    void CopyResult_1_Clipboard();
    void CopyResult_2_Clipboard();
    void TestCase();

    void ChangeRiddle(int index);
};
