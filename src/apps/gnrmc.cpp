#include "gnrmc.h"

GNRMC::GNRMC()
{
}

GNRMC::~GNRMC()
{
}

GNRMCProtocol GNRMC::parse(std::string_view sample)
{
  std::vector<std::string> rawData = GPSTools::split(sample, '*');

  auto tokens = GPSTools::split(rawData.at(0), ',');

  GNRMCProtocol data;

  data.type = tokens.at(0);
  data.utcTime = tokens.at(1);
  data.status = tokens.at(2);
  data.latitude = tokens.at(3);
  data.latitudeDirection = tokens.at(4);
  data.longitude = tokens.at(5);
  data.longitudeDirection = tokens.at(6);
  data.speed = tokens.at(7);
  data.course = tokens.at(8);
  data.utcDate = tokens.at(9);
  data.mode = tokens.at(11);

  return data;
}
