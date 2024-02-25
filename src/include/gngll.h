#pragma once

#include <string>
#include <vector>

#include "gpsTools.h"

struct GNGLLProtocol
{
  std::string type;
  std::string latitude;
  std::string latitudeDirection;
  std::string longitude;
  std::string longitudeDirection;
  std::string utcTime;
  std::string status;
};

class GNGLL
{
public:
  GNGLL();
  ~GNGLL();

  static GNGLLProtocol parse(std::string_view);
};
