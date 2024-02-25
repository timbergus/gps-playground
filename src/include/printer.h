#pragma once

#include "gpsTools.h"

#include "gngll.h"
#include "gngsa.h"
#include "gnrmc.h"
#include "gpgsv.h"

class Printer
{
private:
  static void printGNGLL(GNGLLProtocol);
  static void printGNGSA(GNGSAProtocol);
  static void printGNRMC(GNRMCProtocol);
  static void printGPGSV(GPGSVProtocol);

  static void printTitle(std::string_view);
  static void printSubtitle(std::string_view);
  static void printInfo(std::string_view, auto);
  static void printNumber(std::string_view, auto, std::string_view = "");
  static void printSatellite(satelliteProtocol satellite);

public:
  Printer();
  ~Printer();

  static void print(std::string_view);
  static void clearScreen();
};
