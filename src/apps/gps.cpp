#include "gps.h"

std::vector<std::string> GPS::tokenize(std::string_view sample)
{
  std::vector<std::string> rawData = split(sample, '*');

  return split(rawData.at(0), ',');
}

GPS::GPS()
{
}

GPS::~GPS()
{
}

GGA GPS::parseGGA(std::string_view sample)
{
  auto tokens = tokenize(sample);

  GGA data{
      .type = tokens.at(0),
      .utcTime = tokens.at(1),
      .latitude = tokens.at(2),
      .latitudeDirection = tokens.at(3),
      .longitude = tokens.at(4),
      .longitudeDirection = tokens.at(5),
      .quality = tokens.at(6),
      .satellitesUsed = tokens.at(7),
      .HDOP = tokens.at(8),
      .altitude = tokens.at(9),
      .geoidalSeparation = tokens.at(11),
      .DGPS = tokens.at(14),
  };

  return data;
}

GLL GPS::parseGLL(std::string_view sample)
{
  auto tokens = tokenize(sample);

  GLL data{
      .type = tokens.at(0),
      .latitude = tokens.at(1),
      .latitudeDirection = tokens.at(2),
      .longitude = tokens.at(3),
      .longitudeDirection = tokens.at(4),
      .utcTime = tokens.at(5),
      .status = tokens.at(6),
  };

  return data;
}

GSA GPS::parseGSA(std::string_view sample)
{
  auto tokens = tokenize(sample);

  GSA data{
      .type = tokens.at(0),
      .mode = tokens.at(1),
      .fixType = tokens.at(2),
      .PDOP = tokens.at(15),
      .HDOP = tokens.at(16),
      .VDOP = tokens.at(17),
  };

  for (int i = 0; i < 12; i++)
  {
    data.satellites.push_back(tokens[i + 3]);
  }

  return data;
}

GSV GPS::parseGSV(std::string_view sample)
{
  auto tokens = tokenize(sample);

  GSV data{
      .type = tokens.at(0),
      .numberOfMessages = tokens.at(1),
      .sequenceNumber = tokens.at(2),
      .satellitesInView = tokens.at(3),
  };

  for (int i = 1; i <= std::stoi(data.numberOfMessages); i++)
  {
    Satellite satellite;

    satellite.id = tokens[i * 4 + 0];
    satellite.elevation = tokens[i * 4 + 1];
    satellite.azimuth = tokens[i * 4 + 2];
    satellite.snr = tokens[i * 4 + 3];

    data.satellites.push_back(satellite);
  }

  return data;
}

RMC GPS::parseRMC(std::string_view sample)
{
  auto tokens = tokenize(sample);

  RMC data{
      .type = tokens.at(0),
      .utcTime = tokens.at(1),
      .status = tokens.at(2),
      .latitude = tokens.at(3),
      .latitudeDirection = tokens.at(4),
      .longitude = tokens.at(5),
      .longitudeDirection = tokens.at(6),
      .speed = tokens.at(7),
      .course = tokens.at(8),
      .utcDate = tokens.at(9),
      .mode = tokens.at(11),
  };

  return data;
}

VTG GPS::parseVTG(std::string_view sample)
{
  auto tokens = tokenize(sample);

  VTG data{
      .type = tokens.at(0),
      .course = tokens.at(1),
      .courseMagnetic = tokens.at(3),
      .speedKn = tokens.at(5),
      .speedKh = tokens.at(7),
      .mode = tokens.at(9),
  };

  return data;
}

ZDA GPS::parseZDA(std::string_view sample)
{
  auto tokens = tokenize(sample);

  ZDA data{
      .type = tokens.at(0),
      .utcTime = tokens.at(1),
      .utcDay = tokens.at(2),
      .utcMonth = tokens.at(3),
      .utcYear = tokens.at(4),
      .localZoneHours = tokens.at(5),
      .localZoneMinutes = tokens.at(6),
  };

  return data;
}

// Tools

std::vector<std::string> GPS::split(std::string_view sample, char separator)
{
  std::string segment;
  std::vector<std::string> tokens;
  std::stringstream sampleStream{sample.data()};

  while (std::getline(sampleStream, segment, separator))
  {
    tokens.push_back(segment);
  }

  return tokens;
}

std::string GPS::stringifyVector(std::vector<std::string> tokens)
{
  std::string result = "[";

  for (auto token : tokens)
    result += "\"" + token + "\", ";

  result.pop_back();
  result.pop_back();

  result += "]";

  return result;
}

std::string GPS::stringifyMap(std::map<std::string, std::string> tokens)
{
  std::string result = "{ ";

  for (auto token : tokens)
  {
    result += "\"" + token.first + "\": \"" + token.second + "\", ";
  }

  result.pop_back();
  result.pop_back();

  result += " }";

  return result;
}

std::tuple<std::string, std::string, std::string> GPS::parseUtcDate(std::string_view utcDate)
{
  std::string day{utcDate.substr(0, 2)};
  std::string month{utcDate.substr(2, 2)};
  std::string year{utcDate.substr(4, 2)};

  return std::make_tuple(day, month, year);
}

std::tuple<std::string, std::string, std::string> GPS::parseUtcTime(std::string_view utcTime)
{
  std::string hours{utcTime.substr(0, 2)};
  std::string minutes{utcTime.substr(2, 2)};
  std::string seconds{utcTime.substr(4, 2)};

  return std::make_tuple(hours, minutes, seconds);
}

double GPS::parseLatitude(std::string_view latitude)
{
  return std::stod(latitude.data()) / 100.0;
}

double GPS::parseLongitude(std::string_view longitude, std::string_view longitudeDirection)
{
  double sign = longitudeDirection == "W" ? -1.0 : 1.0;
  return sign * std::stod(longitude.data()) / 100.0;
}

double GPS::parseSpeed(std::string_view speed, Units units)
{
  double velocity = std::stod(speed.data());

  return units == ms ? velocity * KNTOMS : velocity * KNTOKMH;
}

bool GPS::isValidSample(std::string_view sample)
{
  std::string segment;
  std::vector<std::string> tokens;
  std::stringstream sampleStream{sample.data()};

  while (std::getline(sampleStream, segment, '*'))
  {
    tokens.push_back(segment);
  }

  std::string data = tokens.at(0).substr(1);

  int check = 0;

  for (size_t i = 0; i < data.size(); i++)
  {
    check = char(check ^ data.at(i));
  }

  std::stringstream hexCheck;

  hexCheck << std::uppercase << std::hex << check;

  return hexCheck.str() == tokens.at(1);
}
