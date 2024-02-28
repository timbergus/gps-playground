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

std::any GPS::parse(std::string_view sample)
{
  auto tokens = tokenize(sample);

  auto type = tokens.at(0);

  if (type.find("GGA") != std::string::npos)
  {
    GGA data;

    data.type = tokens.at(0);
    data.utcTime = tokens.at(1);
    data.latitude = tokens.at(2);
    data.latitudeDirection = tokens.at(3);
    data.longitude = tokens.at(4);
    data.longitudeDirection = tokens.at(5);
    data.quality = tokens.at(6);
    data.satellitesUsed = tokens.at(7);
    data.HDOP = tokens.at(8);
    data.altitude = tokens.at(9);
    data.geoidalSeparation = tokens.at(11);
    data.DGPS = tokens.at(14);

    return data;
  }
  else if (type.find("GLL") != std::string::npos)
  {
    GLL data;

    data.type = tokens.at(0);
    data.latitude = tokens.at(1);
    data.latitudeDirection = tokens.at(2);
    data.longitude = tokens.at(3);
    data.longitudeDirection = tokens.at(4);
    data.utcTime = tokens.at(5);
    data.status = tokens.at(6);

    return data;
  }
  else if (type.find("GSA") != std::string::npos)
  {
    GSA data;

    data.type = tokens.at(0);
    data.mode = tokens.at(1);
    data.fixType = tokens.at(2);
    data.PDOP = tokens.at(15);
    data.HDOP = tokens.at(16);
    data.VDOP = tokens.at(17);

    for (int i = 0; i < 12; i++)
    {
      data.satellites.push_back(tokens[i + 3]);
    }

    return data;
  }
  else if (type.find("GSV") != std::string::npos)
  {
    GSV data;

    data.type = tokens.at(0);
    data.numberOfMessages = tokens.at(1);
    data.sequenceNumber = tokens.at(2);
    data.satellitesInView = tokens.at(3);

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
  else if (type.find("RMC") != std::string::npos)
  {
    RMC data;

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
  else if (type.find("VTG") != std::string::npos)
  {
    VTG data;

    data.type = tokens.at(0);
    data.course = tokens.at(1);
    data.courseMagnetic = tokens.at(3);
    data.speedKn = tokens.at(5);
    data.speedKh = tokens.at(7);
    data.mode = tokens.at(9);

    return data;
  }
  else if (type.find("ZDA") != std::string::npos)
  {
    ZDA data;

    data.type = tokens.at(0);
    data.utcTime = tokens.at(1);
    data.utcDay = tokens.at(2);
    data.utcMonth = tokens.at(3);
    data.utcYear = tokens.at(4);
    data.localZoneHours = tokens.at(5);
    data.localZoneMinutes = tokens.at(6);

    return data;
  }
  else
  {
    throw std::runtime_error("Sample type not supported");
  }
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
