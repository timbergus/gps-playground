// utils.h

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <map>

#define KNTOMS 0.514444444
#define KNTOKMH 1.85

class Utils
{
private:
public:
  Utils();
  ~Utils();

  static std::vector<std::string> split(std::string, std::string);
  static std::string stringify_vector(std::vector<std::string>);
  static std::string stringify_map(std::map<std::string, std::string>);

  static bool is_valid_sample(std::string);
  static std::string get_type(std::string);
  static std::string get_time(std::string);
  static std::string get_date(std::string, std::string = "sp");
  static double get_latitude(std::string);
  static double get_longitude(std::string, std::string);
  static double get_speed(std::string, std::string = "ms");

  static void clear_screen();
};

Utils::Utils()
{
}

Utils::~Utils()
{
}

std::vector<std::string> Utils::split(std::string sample, std::string delimeter)
{
  // This is a strings vector where we are going to store the splitted string.
  std::vector<std::string> result;

  // And this the initial position of the string.
  std::string::size_type beg = 0;

  for (auto end = 0; size_t(end = sample.find(delimeter, end)) != std::string::npos; ++end)
  {
    result.push_back(sample.substr(beg, end - beg));
    beg = end + 1;
  }

  result.push_back(sample.substr(beg));

  return result;
}

std::string Utils::stringify_vector(std::vector<std::string> tokens)
{
  std::string result = "[";

  for (auto token : tokens)
    result += "\"" + token + "\", ";

  result.pop_back();
  result.pop_back();

  result += "]";

  return result;
}

std::string Utils::stringify_map(std::map<std::string, std::string> tokens)
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

// https://www.tigoe.com/pcomp/code/Processing/127

bool Utils::is_valid_sample(std::string sample)
{
  std::vector<std::string> fragments{Utils::split(sample, "*")};

  std::string data = fragments[0].substr(1);

  int check = 0;

  for (size_t i = 0; i < data.size(); i++)
  {
    check = char(check ^ data.at(i));
  }

  std::string hex_check;

  hex_check = fmt::format("{0:x}", check);

  return hex_check == fragments[1];
}

std::string Utils::get_type(std::string sample)
{
  return sample.substr(1, 5);
}

std::string Utils::get_time(std::string utc_time)
{
  std::string hours = utc_time.substr(0, 2);
  std::string minutes = utc_time.substr(2, 2);
  std::string seconds = utc_time.substr(4, 2);

  return hours + ":" + minutes + ":" + seconds;
}

std::string Utils::get_date(std::string utc_date, std::string language)
{
  std::string day = utc_date.substr(0, 2);
  std::string month = utc_date.substr(2, 2);
  std::string year = utc_date.substr(4, 2);

  if (language == "en")
  {
    return month + "/" + day + "/" + year;
  }
  else
  {
    return day + "/" + month + "/" + year;
  }
}

double Utils::get_latitude(std::string latitude)
{
  return std::stod(latitude) / 100.0;
}

double Utils::get_longitude(std::string longitude, std::string longitude_direction)
{
  double sign = longitude_direction == "W" ? -1.0 : 1.0;
  return sign * std::stod(longitude) / 100.0;
}

double Utils::get_speed(std::string speed, std::string units)
{
  double parsedSpeed = 0.0;

  if (units == "ms")
  {
    parsedSpeed = std::stod(speed) * KNTOMS;
  }
  else if (units == "kmh")
  {
    parsedSpeed = std::stod(speed) * KNTOKMH;
  }

  return parsedSpeed;
}

void Utils::clear_screen()
{
#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif
}

#endif // UTILS_H
