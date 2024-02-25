#include "gpgsv.h"

GPGSV::GPGSV()
{
}

GPGSV::~GPGSV()
{
}

GPGSVProtocol GPGSV::parse(std::string_view sample)
{
  std::vector<std::string> rawData = GPSTools::split(sample, '*');

  auto tokens = GPSTools::split(rawData.at(0), ',');

  GPGSVProtocol data;

  data.type = tokens.at(0);
  data.numberOfMessages = tokens.at(1);
  data.sequenceNumber = tokens.at(2);
  data.satellitesInView = tokens.at(3);

  for (int i = 1; i <= std::stoi(data.numberOfMessages); i++)
  {
    satelliteProtocol satellite;

    satellite.id = tokens[i * 4 + 0];
    satellite.elevation = tokens[i * 4 + 1];
    satellite.azimuth = tokens[i * 4 + 2];
    satellite.snr = tokens[i * 4 + 3];

    data.satellites.push_back(satellite);
  }

  return data;
}
