#pragma once

#include <string>
#include <vector>

#include "gpsTools.h"

struct GNRMCProtocol
{
  std::string type;
  std::string utcTime;
  std::string status;
  std::string latitude;
  std::string latitudeDirection;
  std::string longitude;
  std::string longitudeDirection;
  std::string speed;
  std::string course;
  std::string utcDate;
  std::string mode;
};

class GNRMC
{
public:
  GNRMC();
  ~GNRMC();

  static GNRMCProtocol parse(std::string_view);
};
