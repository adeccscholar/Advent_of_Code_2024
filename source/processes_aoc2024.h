#pragma once

#include "qt_form.h"
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <ranges>

enum class statusRiddle : uint16_t { none = 0, added = 1, prepared = 2, 
                                     first_in_progress = 3, first_solved = 4, 
                                     second_in_progress = 5, both_solved = 6 };


class TAOC2024Processes {
public:
   using cntrlData  = std::tuple<std::string, std::string, std::string, std::function<void(TMyForm&)>, statusRiddle>;
   using cntrlDatas = std::map<size_t, cntrlData>;
   using testDatas = std::map<size_t, std::tuple<std::string, std::optional<std::tuple<size_t, size_t>>>>;

   static std::string inline strApplication = "Advent of Code 2024"s;
   static cntrlDatas control;
   static testDatas  test;

   static void Init(TMyForm&& frm);
   static void Info(TMyForm&& frm);
   static void Processes(size_t day, TMyForm&& frm);
   static void SetResult(TMyForm& frm, std::pair<std::string, std::string> const&);
   static void CopyTestData(TMyForm&& frm);
   static void ChangeRiddle(TMyForm&& frm);

   static void TestCase(TMyForm&& frm);

   static cntrlData const& GetData(std::string const&);
   static size_t GetDay(std::string const&);
   };

template <>
struct std::formatter<statusRiddle> : std::formatter<std::string_view> {
   template <typename FormatContext>
   auto format(statusRiddle t, FormatContext& ctx) const {
      string_view out = "undefined";
      switch (t) {
         case statusRiddle::none:               out = "not started";           break;
         case statusRiddle::added:              out = "added to project";      break;
         case statusRiddle::prepared:           out = "implemention started";  break;
         case statusRiddle::first_in_progress:  out = "first part in progress"; break;
         case statusRiddle::first_solved:       out = "first mission solved";  break;
         case statusRiddle::second_in_progress: out = "second part in progress"; break;
         case statusRiddle::both_solved:        out = "both missions solved";  break;
         default: out = "undefined status, critical";
         }
      return std::formatter<std::string_view>::format(out, ctx);
      }
   };
