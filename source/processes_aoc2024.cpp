// Project: Solutions for Advent of Code 2024
// file with the control data for process and common processes functions
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

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
#include "Day_09.h" //  9th day : Disk Fragmenter       - implemention started

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
std::pair<std::string, std::string> RiddleDay10(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDayxx(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
}


TAOC2024Processes::cntrlDatas TAOC2024Processes::control = {
   {  1, {" 1st day"s,  "Historian Hysteria"s,    "https://adventofcode.com/2024/day/1"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay1(frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  2, {" 2nd day"s,  "Red-Nosed Reports"s,     "https://adventofcode.com/2024/day/2"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay2(frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  3, {" 3rd day"s,  "Mull It Over"s,          "https://adventofcode.com/2024/day/3"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay3(frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  4, {" 4th day"s,  "Ceres Search"s,          "https://adventofcode.com/2024/day/4"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay4(frm.GetText("memInput"s))); }, statusRiddle::both_solved }},
   {  5, {" 5th day"s,  "Print Queue"s,           "https://adventofcode.com/2024/day/5"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay5(frm)); }, statusRiddle::both_solved }},
   {  6, {" 6th day"s,  "Guard Gallivant"s,       "https://adventofcode.com/2024/day/6"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay6(frm.GetText("memInput"))); }, statusRiddle::first_solved }},
   {  7, {" 7th day"s,  "Bridge Repair"s,         "https://adventofcode.com/2024/day/7"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay7(frm.GetLines("memInput"))); }, statusRiddle::second_in_progress }},
   {  8, {" 8th day"s,  "Resonant Collinearity"s, "https://adventofcode.com/2024/day/8"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay8(frm.GetText("memInput"))); }, statusRiddle::first_in_progress }},
   {  9, {" 9th day"s,  "Disk Fragmenter"s,       "https://adventofcode.com/2024/day/9"s,  [](TMyForm& frm) { SetResult(frm, RiddleDay9(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   { 10, {"10th day"s, "Hoof It"s,                "https://adventofcode.com/2024/day/10"s, [](TMyForm& frm) { SetResult(frm, RiddleDay10(frm.GetText("memInput"))); }, statusRiddle::prepared }},
   //{ 11, {"11th day"s, ""s } }
   //{ 12, {"12th day"s, ""s } }
   //{ 13, {"13th day"s, ""s } }
   //{ 14, {"14th day"s, ""s } }
   //{ 15, {"15th day"s, ""s } }
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

   frm.SetButton("btnOldURL"s,        "description for choosen day"s);
   frm.SetButton("btnInputData"s,     "input for the choosen day"s);
   frm.SetButton("btnPasteData"s,     "paste data from clipboard"s);
   frm.SetButton("btnOldRiddles"s,    "solve task for choosen day"s);

   frm.SetGroupBox("gbxResults"s,     "results"s);
   frm.SetEdit("edtResult_1"s,        ""s);
   frm.SetButton("btnCopyResult1"s,   ">"s);
   frm.SetEdit("edtResult_2"s,        ""s);
   frm.SetButton("btnCopyResult2"s,   ">"s);

   frm.SetButton("btnTest"s,          "test case"s);


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
   frm.SetEdit({ "edtResult_1"s, "edtResult_2"s }, ""s);
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

size_t TAOC2024Processes::GetDay(std::string const& strDay) {
   if (auto it = std::ranges::find_if(control, [&strDay](auto const& elem) {
                                                     return std::get<0>(elem.second) == strDay;
                                                     }); it != control.end()) return it->first;
   else throw std::runtime_error(std::format("element for {} not found in GetDay().", strDay));
   }

TAOC2024Processes::cntrlData const& TAOC2024Processes::GetData(std::string const& strDay) {
   if (auto it = std::ranges::find_if(control, [&strDay](auto const& elem) {
                                                     return std::get<0>(elem.second) == strDay;
                                                     } ); it != control.end()) return it->second;
   else throw std::runtime_error(std::format("element for {} not found in GetData().", strDay));
   }

void TAOC2024Processes::Processes(size_t day, TMyForm&& frm) {
   frm.Clear("memOutput"s);
   try {
      if(auto it = control.find(day); it != control.end()) {
         auto const& [day, cntrl] = *it;
         std::println(std::cout, "{}, Day {}: {}", strApplication, day, std::get<1>(cntrl));
         std::println(std::clog, "starting {} \"{}\"...", std::get<0>(cntrl), std::get<1>(cntrl));
         TMyWait wait("memOutput"s);
         auto func_start = std::chrono::high_resolution_clock::now();
         std::get<3>(cntrl)(frm);
         auto func_ende = std::chrono::high_resolution_clock::now();
         auto func_time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);

         if (auto it = test.find(day); it != test.end() && std::get<1>(it->second) && frm.GetCheckBox("chkTestData"s)) {
            auto & [check_1, check_2] = *std::get<1>(it->second);
            size_t result_1 = toInt<size_t>(frm.GetEditAs("edtResult_1"s));
            size_t result_2 = toInt<size_t>(frm.GetEditAs("edtResult_2"s));
            std::println(std::cout, "");
            std::println(std::cout, "Checking the solution with the test values");
            std::println(std::cout, " 1st part: {} {} {}", result_1, (result_1 == check_1 ? "==" : "!="), check_1);
            std::println(std::cout, " 2nd part: {} {} {}", result_2, (result_2 == check_2 ? "==" : "!="), check_2);
            if(result_1 == check_1 && result_2 == check_2) {
               std::println(std::cout, "Both results correspond to the expected values.");
               }
            }

         std::println(std::cout, "\ndaily task finished successful in {:.3f} sec", func_time.count() / 1000.);
         std::println(std::clog, "{} : {} finished", std::get<0>(cntrl), std::get<1>(cntrl));
         }
      else {
         std::println(std::cout, "{}, Day {}: no informations in the control structure", strApplication, day);
         std::println(std::clog, "missing informations for day {}", day);
         }
      }
   catch(std::exception const& ex) {
      std::println(std::cout, "error in program {}\n{}", strApplication, ex.what());
      }
   }
