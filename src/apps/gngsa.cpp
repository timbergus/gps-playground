#include "gngsa.h"

GNGSA::GNGSA()
{
}

GNGSA::~GNGSA()
{
}

GNGSAProtocol GNGSA::parse(std::string_view sample)
{
  std::vector<std::string> rawData = GPSTools::split(sample, '*');

  auto tokens = GPSTools::split(rawData.at(0), ',');

  GNGSAProtocol data;

  data.type = tokens.at(0);
  data.mode = tokens.at(1);
  data.fixType = tokens.at(2);

  for (int i = 0; i < 12; i++)
  {
    data.satellites.push_back(tokens[i + 3]);
  }

  data.PDOP = tokens.at(15);
  data.HDOP = tokens.at(16);
  data.VDOP = tokens.at(17);

  return data;
}
