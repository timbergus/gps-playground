#pragma once

#include <fmt/format.h>
#include <fmt/color.h>
#include <any>

#include "gps.h"

class Printer
{
private:
  static void printTitle(std::string_view);
  static void printSubtitle(std::string_view);
  static void printInfo(std::string_view, auto);
  static void printNumber(std::string_view, auto, std::string_view = "");
  static void printSatellite(Satellite satellite);

public:
  Printer();
  ~Printer();

  static std::string formatUtcTime(std::tuple<std::string, std::string, std::string>);
  static std::string formatUtcDate(std::tuple<std::string, std::string, std::string>);

  static void print(std::string_view);
  static void clearScreen();
};
