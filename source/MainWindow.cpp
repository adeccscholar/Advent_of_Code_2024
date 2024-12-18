﻿// Project: Solutions for Advent of Code 2024
// implementation for the main widget class (formular) for the Qt Project
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "MainWindow.h"

#include "processes_aoc2024.h"

#include "qt_web_tools.h"
#include "qt_iteratoren.h"
#include "qt_stream.h"
#include "qt_form.h"

#include "my_common_tools.h"
#include "my_ranges_tools.h"

#include <QString>
#include <QListWidget>
#include <QComboBox>

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <functional>
#include <regex>
#include <stdexcept>
#include <format>
#include <print>
#include <ranges>


using namespace std::string_literals;

stream_raii_wrapper old_cout(std::cout), old_clog(std::clog);


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);
    ui.statusBar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("sbMain");
    statusLabel->setText("sbMain");
    ui.statusBar->addPermanentWidget(statusLabel);

    old_cout.activate(ui.memOutput, true);
    old_clog.activate(statusLabel, true);

    connect(ui.btnInfo,          &QPushButton::clicked,           this, &MainWindow::Info);
    connect(ui.btnOldRiddles,    &QPushButton::clicked,           this, &MainWindow::OldRiddle);
    connect(ui.btnOldURL,        &QPushButton::clicked,           this, &MainWindow::Description);
    connect(ui.btnInputData,     &QPushButton::clicked,           this, &MainWindow::DataURL);
    connect(ui.btnPasteData,     &QPushButton::clicked,           this, &MainWindow::PasteData);

    connect(ui.btnCopyResult1,   &QPushButton::clicked,           this, &MainWindow::CopyResult_1_Clipboard);
    connect(ui.btnCopyResult2,   &QPushButton::clicked,           this, &MainWindow::CopyResult_2_Clipboard);
    connect(ui.cbxOldRiddles,    &QComboBox::currentIndexChanged, this, &MainWindow::ChangeRiddle);
    connect(ui.chkTestData,      &QCheckBox::stateChanged,        this, &MainWindow::CopyTestData);

    connect(ui.btnTest,          &QPushButton::clicked,           this, &MainWindow::TestCase);
    connect(ui.btnClose,         &QPushButton::clicked,           this, [this]() { this->close(); });

    //ui.btnTest->setVisible(false);

    Init();
   }

MainWindow::~MainWindow() {
   }

void MainWindow::Init() {
   TAOC2024Processes::Init(TMyForm { this });
   }

void MainWindow::Description() {
   TMyForm frm(this);
   call_URL_with_qt(std::get<2>(TAOC2024Processes::GetData(frm.CurrentText("cbxOldRiddles"))));
   //ui.memInput->setText(getQTextFromUrl(std::get<2>(TAOC2024Processes::GetData(frm.CurrentText("cbxOldRiddles")))));
   }

void MainWindow::DataURL() {
   TMyForm frm(this);
   call_URL_with_qt(std::get<2>(TAOC2024Processes::GetData(frm.CurrentText("cbxOldRiddles")))+"/input"s);
   }

void MainWindow::PasteData() {
   TMyForm frm(this);
   TMyWait wait("memInput"s);
   TAOC2024Processes::CleanResults(frm);
   frm.SetCheckBox("chkTestData"s, false);
   std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
   std::println(oss, "{}", getClipboardContent());
   return;
   }

void MainWindow::ChangeRiddle(int index) {
   TAOC2024Processes::ChangeRiddle(TMyForm { this });
   }

void MainWindow::OldRiddle() {
   TAOC2024Processes::Processes(TMyForm{ this });
   }


void MainWindow::Info() {
   TAOC2024Processes::Info(TMyForm{ this });
   }

void MainWindow::CopyTestData(int state) {
   if (state == Qt::Checked) TAOC2024Processes::CopyTestData(TMyForm{ this });
   }

void MainWindow::CopyResult_1_Clipboard() {
   QApplication::clipboard()->setText(ui.edtResult_1->text());
   }

void MainWindow::CopyResult_2_Clipboard() {
   QApplication::clipboard()->setText(ui.edtResult_2->text());
   }

void MainWindow::TestCase() {
   TAOC2024Processes::TestCase(TMyForm { this } );
   }