#include "gngll.h"

GNGLL::GNGLL()
{
}

GNGLL::~GNGLL()
{
}

GNGLLProtocol GNGLL::parse(std::string_view sample)
{
  std::vector<std::string> rawData = GPSTools::split(sample, '*');

  auto tokens = GPSTools::split(rawData.at(0), ',');

  GNGLLProtocol data;

  data.type = tokens.at(0);
  data.latitude = tokens.at(1);
  data.latitudeDirection = tokens.at(2);
  data.longitude = tokens.at(3);
  data.longitudeDirection = tokens.at(4);
  data.utcTime = tokens.at(5);
  data.status = tokens.at(6);

  return data;
}
