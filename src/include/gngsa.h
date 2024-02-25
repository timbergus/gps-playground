#pragma once

#include <string>
#include <vector>

#include "gpsTools.h"

struct GNGSAProtocol
{
  std::string type;
  std::string mode;
  std::string fixType;
  std::vector<std::string> satellites;
  std::string PDOP;
  std::string HDOP;
  std::string VDOP;
  std::string checksum;
};

class GNGSA
{
public:
  GNGSA();
  ~GNGSA();

  static GNGSAProtocol parse(std::string_view);
};
