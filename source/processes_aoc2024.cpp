// Project: Solutions for Advent of Code 2024
// file with the control data for process and common processes functions
// author: Volker Hillmann
// date:   05.12.20204, last change: 21.12.2024   
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

// formatter don't working, ranges adaption don't working, std::get for coords don't work


#include "processes_aoc2024.h"

#include "qt_stream.h"
#include "qt_iteratoren.h"
#include "my_common_tools.h"
#include "my_ranges_tools.h"
#include "my_grid2d.h"

// Header files with the implementations for a special day
#include "Day_01.h" //  1st day : Historian Hysteria     - both tasks solved
#include "Day_02.h" //  2nd day : Red-Nosed Reports      - both tasks solved
#include "Day_03.h" //  3rd day : Mull It Over           - both tasks solved
#include "Day_04.h" //  4th day : Ceres Search           - both tasks solved
#include "Day_05.h" //  5th day : Print Queue            - both tasks solved
#include "Day_06.h" //  6th day : Guard Gallivant        - first task solved
#include "Day_07.h" //  7th day : Bridge Repair          - both tasks solved 
#include "Day_08.h" //  8th day : Resonant Collinearity  - both tasks solved 
#include "Day_09.h" //  9th day : Disk Fragmenter        - first task solved 
#include "Day_10.h" // 10th day : Hoof It                - implementation started
#include "Day_11.h" // 11th day : Plutonian Pebbles      - first task solved
#include "Day_12.h" // 12th day : Garden Groups          - first task solved
#include "Day_13.h" // 13th day : Claw Contraption       - first task in progress
#include "Day_14.h" // 14th day : Restroom Redoubt       - implementation started
#include "Day_15.h" // 15th day : Warehouse Woes         - implementation started
#include "Day_16.h" // 16th day : Reindeer Maze          - first solved
#include "Day_17.h" // 17th day : Chronospatial Computer - implementation started
#include "Day_18.h" // 18th day : RAM Run                - both tasks solved 
#include "Day_19.h" // 19th day : Linen Layout           - implementation started
#include "Day_20.h" // 20th day : Race Condition         - implementation started
#include "Day_21.h" // 21th day : Keypad Conundrum       - implementation started
#include "Day_22.h" // 22nd day : Monkey Market          - both tasks solved
#include "Day_23.h" // 23rd day : LAN Party              - implementation started 
#include "Day_24.h" // 24th day : Crossed Wires          - implementation started 
#include "Day_25.h" // 25th day : Code Chronicle         - implementation started 

#include <iostream>
#include <string_view>
#include <vector>
#include <utility>
#include <functional>
#include <regex>
#include <stdexcept>
#include <execution>
#include <atomic>
#include <format>
#include <print>
#include <ranges>

using namespace std::string_literals;
using namespace std::placeholders;


template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDayxx(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }


TAOC2024Processes::cntrlDatas TAOC2024Processes::control = {
   {  1, {" 1st day"s, "Historian Hysteria"s,     "https://adventofcode.com/2024/day/1"s,  [](TMyForm& frm) { SetResult(frm, nsDay01::RiddleDay1 (frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  2, {" 2nd day"s, "Red-Nosed Reports"s,      "https://adventofcode.com/2024/day/2"s,  [](TMyForm& frm) { SetResult(frm, nsDay02::RiddleDay2 (frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  3, {" 3rd day"s, "Mull It Over"s,           "https://adventofcode.com/2024/day/3"s,  [](TMyForm& frm) { SetResult(frm, nsDay03::RiddleDay3 (frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  4, {" 4th day"s, "Ceres Search"s,           "https://adventofcode.com/2024/day/4"s,  [](TMyForm& frm) { SetResult(frm, nsDay04::RiddleDay4 (frm.GetText("memInput"s))); }, statusRiddle::both_solved }},
   {  5, {" 5th day"s, "Print Queue"s,            "https://adventofcode.com/2024/day/5"s,  [](TMyForm& frm) { SetResult(frm, nsDay05::RiddleDay5 (frm)); },                      statusRiddle::both_solved }},
   {  6, {" 6th day"s, "Guard Gallivant"s,        "https://adventofcode.com/2024/day/6"s,  [](TMyForm& frm) { SetResult(frm, nsDay06::RiddleDay6 (frm.GetText("memInput"))); },  statusRiddle::first_solved }},
   {  7, {" 7th day"s, "Bridge Repair"s,          "https://adventofcode.com/2024/day/7"s,  [](TMyForm& frm) { SetResult(frm, nsDay07::RiddleDay7 (frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  8, {" 8th day"s, "Resonant Collinearity"s,  "https://adventofcode.com/2024/day/8"s,  [](TMyForm& frm) { SetResult(frm, nsDay08::RiddleDay8 (frm.GetText("memInput"))); },  statusRiddle::both_solved }},
   {  9, {" 9th day"s, "Disk Fragmenter"s,        "https://adventofcode.com/2024/day/9"s,  [](TMyForm& frm) { SetResult(frm, nsDay09::RiddleDay9 (frm.GetText("memInput"))); },  statusRiddle::first_solved }},
   { 10, {"10th day"s, "Hoof It"s,                "https://adventofcode.com/2024/day/10"s, [](TMyForm& frm) { SetResult(frm, RiddleDay10(frm.GetText("memInput"))); },           statusRiddle::prepared }},
   { 11, {"11th day"s, "Plutonian Pebbles"s,      "https://adventofcode.com/2024/day/11"s, [](TMyForm& frm) { SetResult(frm, nsDay11::RiddleDay11(frm.GetText("memInput"))); },  statusRiddle::first_solved }},
   { 12, {"12th day"s, "Garden Groups"s,          "https://adventofcode.com/2024/day/12"s, [](TMyForm& frm) { SetResult(frm, nsDay12::RiddleDay12(frm.GetText("memInput"))); },  statusRiddle::first_solved }},
   { 13, {"13th day"s, "Claw Contraption"s,       "https://adventofcode.com/2024/day/13"s, [](TMyForm& frm) { SetResult(frm, nsDay13::RiddleDay13(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 14, {"14th day"s, "Restroom Redoubt"s,       "https://adventofcode.com/2024/day/14"s, [](TMyForm& frm) { SetResult(frm, nsDay14::RiddleDay14(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 15, {"15th day"s, "Warehouse Woes"s,         "https://adventofcode.com/2024/day/15"s, [](TMyForm& frm) { SetResult(frm, RiddleDay15(frm.GetLines("memInput"))); },          statusRiddle::prepared }},
   { 16, {"16th day"s, "Reindeer Maze"s,          "https://adventofcode.com/2024/day/16"s, [](TMyForm& frm) { SetResult(frm, nsDay16::RiddleDay16(frm.GetText("memInput"))); },  statusRiddle::first_solved }},
   { 17, {"17th day"s, "Chronospatial Computer"s, "https://adventofcode.com/2024/day/17"s, [](TMyForm& frm) { SetResult(frm, nsDay17::RiddleDay17(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 18, {"18th day"s, "RAM Run"s,                "https://adventofcode.com/2024/day/18"s, [](TMyForm& frm) { SetResult(frm, nsDay18::RiddleDay18(frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   { 19, {"19th day"s, "Linen Layout"s,           "https://adventofcode.com/2024/day/19"s, [](TMyForm& frm) { SetResult(frm, nsDay19::RiddleDay19(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 20, {"20th day"s, "Race Condition"s,         "https://adventofcode.com/2024/day/20"s, [](TMyForm& frm) { SetResult(frm, nsDay20::RiddleDay20(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 21, {"21th day"s, "Keypad Conundrum"s,       "https://adventofcode.com/2024/day/21"s, [](TMyForm& frm) { SetResult(frm, nsDay21::RiddleDay21(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 22, {"22nd day"s, "Monkey Market"s,          "https://adventofcode.com/2024/day/22"s, [](TMyForm& frm) { SetResult(frm, nsDay22::RiddleDay22(frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   { 23, {"23rd day"s, "LAN Party"s,              "https://adventofcode.com/2024/day/23"s, [](TMyForm& frm) { SetResult(frm, nsDay23::RiddleDay23(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 24, {"24th day"s, "Crossed Wires"s,          "https://adventofcode.com/2024/day/24"s, [](TMyForm& frm) { SetResult(frm, nsDay24::RiddleDay24(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 25, {"25th day"s, "Code Chronicle"s,         "https://adventofcode.com/2024/day/25"s, [](TMyForm& frm) { SetResult(frm, nsDay25::RiddleDay25(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
};

void TAOC2024Processes::Init(TMyForm&& frm) {
   frm.SetCaption(strApplication);
   frm.SetLabel("lblLicense1"s,       "adecc Scholar's Advent of Code 2024 App"s);
   frm.SetLabel("lblLicense2"s,       "copyright ©2024, all rights reserved"s);
   frm.SetLabel("lblLicense3"s,       "adecc Systemhaus GmbH"s);
   frm.SetButton("btnInfo"s,          "information"s);
   frm.SetGroupBox("gbxOldRiddle"s,   "daily tasks"s);
   frm.SetLabel("lblRiddleBox"s,      "choose the daily task");
   frm.InitCheckBox("chkTestData",    "copy test data", false);

   frm.SetButton("btnOldRiddles"s,    "solve task for choosen day"s);
   frm.SetButton("btnPasteData"s,     "paste data from clipboard"s);
   frm.SetLabel("lblWebURLs"s,        "resources from the web"s);
   frm.SetButton("btnOldURL"s,        "description for choosen day"s);
   frm.SetButton("btnInputData"s,     "input for the choosen day"s);

   frm.SetGroupBox("gbxResults"s,     "results"s);
   frm.SetEdit("edtResult_1"s,        ""s);
   frm.SetButton("btnCopyResult1"s,   ">"s);
   frm.SetEdit("edtResult_2"s,        ""s);
   frm.SetButton("btnCopyResult2"s,   ">"s);
   frm.SetLabel("lblEvaluation"s,     ""s);

   frm.SetButton("btnClose"s, "close application");

   frm.SetButton("btnTest"s,  "test case"s);
   frm.SetButtonVisible("btnTest"s, boHasTestButton);

   auto values = control | std::views::elements<1> 
                         | std::views::filter([](auto& d) {
                                 return std::get<4>(d) != statusRiddle::none && std::get<4>(d) != statusRiddle::added ; 
                                 })
                         | std::views::elements<0>;

   std::ranges::copy(values, make_qt_widget_output_iterator(frm.Find<QComboBox>("cbxOldRiddles")));
   frm.LastItem("cbxOldRiddles"s);
   
   Info(std::forward<TMyForm>(frm));
   }


void TAOC2024Processes::Info(TMyForm&& frm) {
   std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
   std::println(oss, "Advent of Code 2024");
   std::println(oss, "-------------------");
   for(auto const& [day, data] : control) {
      std::println(oss, "{:>8} : {:<30} ({:>2}) - {}", std::get<0>(data), std::get<1>(data), day, std::get<4>(data));
      }
   }

void TAOC2024Processes::SetResult(TMyForm& frm, std::pair<std::string, std::string> const& value) {
   frm.SetEdit({ { "edtResult_1"s, value.first }, { "edtResult_2"s, value.second } });
   }



void TAOC2024Processes::CopyTestData(TMyForm&& frm) {
   if (auto it = test.find(GetDay(frm.CurrentText("cbxOldRiddles"))); it != test.end()) {
      std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
      std::println(oss, "{}", std::get<0>(it->second));
      }
   }

void TAOC2024Processes::ChangeRiddle(TMyForm&& frm) {
   frm.Clear("memOutput"s);
   CleanResults(frm);
   std::string choosen = frm.CurrentText("cbxOldRiddles");
   auto day = GetDay(choosen);
   if (auto it = test.find(day); it != test.end()) {
      if(frm.GetCheckBox("chkTestData"s)) {
         std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
         std::println(oss, "{}", std::get<0>(it->second));
         }
      auto const& data = control[day];
      frm.SetCaption(std::format("{}: {} [{}]", strApplication, std::get<1>(data), std::get<4>(data)));
      std::println(std::clog, "selected daily task for {}, {}", std::get<0>(data), std::get<1>(data));
      }
   else {
      frm.SetCaption(std::format("{}", strApplication));
      std::println(std::clog, "information for the selected {} not found.", choosen);
      }
   }

TAOC2024Processes::cntrlDatas::const_iterator TAOC2024Processes::GetControlInfo(std::string const& strDay) {
   if (auto it = std::ranges::find_if(control, [&strDay](auto const& elem) {
                                                     return std::get<0>(elem.second) == strDay;
                                                     }); it != control.end()) return it;
   else {
      throw std::runtime_error(std::format("element for {} not found in the control data in function Get().", strDay));
      }
   }

size_t TAOC2024Processes::GetDay(std::string const& strDay) {
   return GetControlInfo(strDay)->first;
   }

TAOC2024Processes::cntrlData const& TAOC2024Processes::GetData(std::string const& strDay) {
   return GetControlInfo(strDay)->second;
   }

void TAOC2024Processes::StartDailyTask(TMyForm& frm, TAOC2024Processes::cntrlData const& cntrl) {
   frm.Clear("memOutput"s);
   std::println(std::cout, "{}, {}: {}", strApplication, std::get<0>(cntrl), std::get<1>(cntrl));
   std::println(std::clog, "starting processing for {} \"{}\"...", std::get<0>(cntrl), std::get<1>(cntrl));
   frm.ProcessEvents();
   }


void TAOC2024Processes::Processes(TMyForm&& frm) {
   try {
      auto selected_day = frm.CurrentText("cbxOldRiddles");
      if (auto it = std::ranges::find_if(control, [&selected_day](auto const& elem) {
                                  return std::get<0>(elem.second) == selected_day;
                                  }); it != control.end()) {
         auto const& [day, cntrl] = *it;
         StartDailyTask(frm, cntrl);
         TMyWait wait("memOutput"s);
 
         auto func_start = std::chrono::high_resolution_clock::now();
         std::get<3>(cntrl)(frm);
         auto func_ende = std::chrono::high_resolution_clock::now();
         auto func_time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);
         CheckResults(frm, day);
         std::println(std::cout, "\ndaily task finished successful in {:.3f} sec", func_time.count() / 1000.);
         std::println(std::clog, "{} : {} finished", std::get<0>(cntrl), std::get<1>(cntrl));
         }
      else {
         // error
         }
      }
   catch(std::exception const& ex) {
      frm.Clear("memOutput"s);
      CleanResults(frm);
      std::println(std::clog, "error in program {}", strApplication);
      std::println(std::cout, "error in program {}\n{}", strApplication, ex.what());
      }
   }
