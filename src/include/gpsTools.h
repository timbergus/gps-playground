#pragma once

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>

#include <fmt/format.h>
#include <fmt/color.h>

class GPSTools
{
public:
  enum Units
  {
    ms,
    kmh
  };

private:
  static constexpr double KNTOMS{0.514444444};
  static constexpr double KNTOKMH{1.85};

public:
  GPSTools();
  ~GPSTools();

  static std::vector<std::string> split(std::string_view, char);
  static std::string stringifyVector(std::vector<std::string>);
  static std::string stringifyMap(std::map<std::string, std::string>);
  static std::tuple<std::string, std::string, std::string> parseUtcDate(std::string_view);
  static std::tuple<std::string, std::string, std::string> parseUtcTime(std::string_view);
  static double parseLatitude(std::string_view);
  static double parseLongitude(std::string_view, std::string_view);
  static double parseSpeed(std::string_view, Units);
  static bool isValidSample(std::string_view);
};
