#include "gpsTools.h"

GPSTools::GPSTools()
{
}

GPSTools::~GPSTools()
{
}

std::vector<std::string> GPSTools::split(std::string_view sample, char separator)
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

std::string GPSTools::stringifyVector(std::vector<std::string> tokens)
{
  std::string result = "[";

  for (auto token : tokens)
    result += "\"" + token + "\", ";

  result.pop_back();
  result.pop_back();

  result += "]";

  return result;
}

std::string GPSTools::stringifyMap(std::map<std::string, std::string> tokens)
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

std::tuple<std::string, std::string, std::string> GPSTools::parseUtcDate(std::string_view utcDate)
{
  std::string day{utcDate.substr(0, 2)};
  std::string month{utcDate.substr(2, 2)};
  std::string year{utcDate.substr(4, 2)};

  return std::make_tuple(day, month, year);
}

std::tuple<std::string, std::string, std::string> GPSTools::parseUtcTime(std::string_view utcTime)
{
  std::string hours{utcTime.substr(0, 2)};
  std::string minutes{utcTime.substr(2, 2)};
  std::string seconds{utcTime.substr(4, 2)};

  return std::make_tuple(hours, minutes, seconds);
}

double GPSTools::parseLatitude(std::string_view latitude)
{
  return std::stod(latitude.data()) / 100.0;
}

double GPSTools::parseLongitude(std::string_view longitude, std::string_view longitudeDirection)
{
  double sign = longitudeDirection == "W" ? -1.0 : 1.0;
  return sign * std::stod(longitude.data()) / 100.0;
}

double GPSTools::parseSpeed(std::string_view speed, Units units)
{
  double velocity = std::stod(speed.data());

  return units == ms ? velocity * KNTOMS : velocity * KNTOKMH;
}

bool GPSTools::isValidSample(std::string_view sample)
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
