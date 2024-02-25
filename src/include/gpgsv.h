#pragma once

#include <string>
#include <vector>

#include "gpsTools.h"

struct satelliteProtocol
{
  std::string id;
  std::string elevation;
  std::string azimuth;
  std::string snr;
};

struct GPGSVProtocol
{
  std::string type;
  std::string numberOfMessages;
  std::string sequenceNumber;
  std::string satellitesInView;
  std::vector<satelliteProtocol> satellites;
};

class GPGSV
{
public:
  GPGSV();
  ~GPGSV();

  static GPGSVProtocol parse(std::string_view);
};
