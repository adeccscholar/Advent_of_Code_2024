#pragma once

#include "qt_form.h"
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <ranges>

class TAOC2024Processes {
public:
   using cntrlData  = std::tuple<std::string, std::string, std::string, std::function<void(TMyForm&)>>;
   using cntrlDatas = std::map<size_t, cntrlData>;
   using testDatas = std::map<size_t, std::string>;

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