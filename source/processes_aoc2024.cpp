// Project: Solutions for Advent of Code 2024
// file with the control data for process and common processes functions
// author: Volker Hillmann
// date:   05.12.20204, last change: 15.12.2024   
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

// todo for the rebuild
// get_coords missing
// formatter don't working, grid print missing
// 8th Day, using of the new grid templates
// 6th Day, problem with combination, and get_coords missing


#include "processes_aoc2024.h"

#include "qt_stream.h"
#include "qt_iteratoren.h"
#include "my_common_tools.h"
#include "my_ranges_tools.h"
//#include "my_grid_view.h"
#include "my_grid2d.h"

// Header files with the implementations for a special day
#include "Day_01.h" //  1st day : Historian Hysteria    - both missions solved
#include "Day_02.h" //  2nd day : Red-Nosed Reports     - both missions solved
#include "Day_03.h" //  3rd day : Mull It Over          - both missions solved
#include "Day_04.h" //  4th day : Ceres Search          - both missions solved
#include "Day_05.h" //  5th day : Print Queue           - both missions solved
#include "Day_06.h" //  6th day : Guard Gallivant       - first mission solved
#include "Day_07.h" //  7th day : Bridge Repair         - both missions solved 
#include "Day_08.h" //  8th day : Resonant Collinearity - first part in progress 
#include "Day_09.h" //  9th day : Disk Fragmenter       - first part in progress 
#include "Day_10.h" // 10th day : Hoof It               - implementation started
#include "Day_11.h" // 11th day : Plutonian Pebbles     - first mission solved
#include "Day_12.h" // 12th day : Garden Groups         - implementation started
#include "Day_13.h" // 13th day : Claw Contraption      - first part in progress
#include "Day_14.h" // 14th day : Restroom Redoubt      - implementation started
#include "Day_15.h" // 15th day : Warehouse Woes        - implementation started

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


template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDayxx(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
}


TAOC2024Processes::cntrlDatas TAOC2024Processes::control = {
   {  1, {" 1st day"s, "Historian Hysteria"s,    "https://adventofcode.com/2024/day/1"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay1 (frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  2, {" 2nd day"s, "Red-Nosed Reports"s,     "https://adventofcode.com/2024/day/2"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay2 (frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  3, {" 3rd day"s, "Mull It Over"s,          "https://adventofcode.com/2024/day/3"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay3 (frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  4, {" 4th day"s, "Ceres Search"s,          "https://adventofcode.com/2024/day/4"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay4 (frm.GetText("memInput"s))); }, statusRiddle::both_solved }},
   {  5, {" 5th day"s, "Print Queue"s,           "https://adventofcode.com/2024/day/5"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay5 (frm)); },                      statusRiddle::both_solved }},
   {  6, {" 6th day"s, "Guard Gallivant"s,       "https://adventofcode.com/2024/day/6"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay6 (frm.GetText("memInput"))); },  statusRiddle::first_solved }},
   {  7, {" 7th day"s, "Bridge Repair"s,         "https://adventofcode.com/2024/day/7"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay7 (frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  8, {" 8th day"s, "Resonant Collinearity"s, "https://adventofcode.com/2024/day/8"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay8 (frm.GetText("memInput"))); },  statusRiddle::first_in_progress }},
   {  9, {" 9th day"s, "Disk Fragmenter"s,       "https://adventofcode.com/2024/day/9"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay9 (frm.GetText("memInput"))); },  statusRiddle::first_solved }},
   { 10, {"10th day"s, "Hoof It"s,               "https://adventofcode.com/2024/day/10"s, [](TMyForm& frm) { SetResult(frm, RiddleDay10(frm.GetText("memInput"))); },  statusRiddle::prepared }},
   { 11, {"11th day"s, "Plutonian Pebbles"s,     "https://adventofcode.com/2024/day/11"s, [](TMyForm& frm) { SetResult(frm, RiddleDay11(frm.GetText("memInput"))); },  statusRiddle::first_solved }},
   { 12, {"12th day"s, "Garden Groups"s,         "https://adventofcode.com/2024/day/12"s, [](TMyForm& frm) { SetResult(frm, RiddleDay12(frm.GetText("memInput"))); },  statusRiddle::prepared }},
   { 13, {"13th day"s, "Claw Contraption"s,      "https://adventofcode.com/2024/day/13"s, [](TMyForm& frm) { SetResult(frm, RiddleDay13(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 14, {"14th day"s, "Restroom Redoubt"s,      "https://adventofcode.com/2024/day/14"s, [](TMyForm& frm) { SetResult(frm, RiddleDay14(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 15, {"15th day"s, "Warehouse Woes"s,        "https://adventofcode.com/2024/day/15"s, [](TMyForm& frm) { SetResult(frm, RiddleDay15(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   //{ 16, {"16th day"s, ""s } }
   //{ 17, {"17th day"s, ""s } }
   //{ 18, {"18th day"s, ""s } }
   //{ 19, {"19th day"s, ""s } }
   //{ 20, {"20nd day"s, ""s } }
   //{ 21, {"21st day"s, ""s } }
   //{ 22, {"22nd day"s, ""s } }
   //{ 23, {"23rd day"s, ""s } }
   //{ 24, {"24th day"s, ""s } }
   //{ 25, {"25th day"s, ""s } }
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

   frm.SetButton("btnTest"s,          "test case"s);
   frm.SetButton("btnClose"s,         "close application");


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

void TAOC2024Processes::CleanResults(TMyForm& frm) {
   frm.SetEdit({ "edtResult_1"s, "edtResult_2"s }, ""s);
   frm.SetLabel("lblEvaluation"s, ""s);
   }

void TAOC2024Processes::CheckResults(TMyForm& frm, size_t day) {
   if (auto it = test.find(day); it != test.end() && std::get<1>(it->second) && frm.GetCheckBox("chkTestData"s)) {
      auto& [check_1, check_2] = *std::get<1>(it->second);
      size_t result_1 = toInt<size_t>(frm.GetEditAs("edtResult_1"s));
      size_t result_2 = toInt<size_t>(frm.GetEditAs("edtResult_2"s));
      std::println(std::cout, "");
      std::println(std::cout, "Checking the solution with the test values");
      std::println(std::cout, " 1st part: {} {} {}", result_1, (result_1 == check_1 ? "==" : "!="), check_1);
      std::println(std::cout, " 2nd part: {} {} {}", result_2, (result_2 == check_2 ? "==" : "!="), check_2);
      if (result_1 == check_1) {
         if (result_2 == check_2) {
            frm.SetLabel("lblEvaluation"s, "Both values are valid."s);
            std::println(std::cout, "Congratulations! Both results correspond to the expected values.");
            }
         else {
            frm.SetLabel("lblEvaluation"s, "Only 1st value is valid."s);
            std::println(std::cout, "Only the value for the first part correspond to the expected value.");
            }
         }
      if (result_1 != check_1) {
         if (result_2 == check_2) {
            frm.SetLabel("lblEvaluation"s, "Only 2nd value is valid."s);
            std::println(std::cout, "There is some wrong, only the value for the second part correspond to the expected value.");
            }
         else {
            frm.SetLabel("lblEvaluation"s, "None of the values are valid."s);
            std::println(std::cout, "There is some wrong, none of the values is actually correspond to the expected values.");
            }
         }
      }
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
