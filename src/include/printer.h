#pragma once

#include <fmt/format.h>
#include <fmt/color.h>

#include "gps.h"

class Printer
{
private:
  static void printGGA(GGA);
  static void printGLL(GLL);
  static void printGSA(GSA);
  static void printGSV(GSV);
  static void printRMC(RMC);
  static void printVTG(VTG);
  static void printZDA(ZDA);

  static void printTitle(std::string_view);
  static void printSubtitle(std::string_view);
  static void printInfo(std::string_view, auto);
  static void printNumber(std::string_view, auto, std::string_view = "");
  static void printSatellite(Satellite satellite);

public:
  Printer();
  ~Printer();

  static void print(std::string_view);
  static void clearScreen();
};
