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
  static std::vector<std::string> clean_split(std::string, std::string);
  static std::string stringify_vector(std::vector<std::string>);
  static std::string stringify_map(std::map<std::string, std::string>);

  static std::string get_type(std::string);
  static std::string get_time(std::string);
  static std::string get_date(std::string, std::string = "sp");
  static double get_latitude(std::string);
  static double get_longitude(std::string, std::string);
  static double get_speed(std::string, std::string = "ms");
};

Utils::Utils()
{
}

Utils::~Utils()
{
}

std::vector<std::string> Utils::clean_split(std::string sample, std::string delimeter)
{
  std::vector<std::string> result;

  char *token = strtok(&sample[0], delimeter.c_str());

  while (token)
  {
    result.push_back(token);
    token = strtok(NULL, delimeter.c_str());
  }

  return result;
}

std::vector<std::string> Utils::split(std::string sample, std::string delimeter)
{
  // This is a strings vector where we are going to store the splitted string.
  std::vector<std::string> result;

  // And this the initial possition of the string.
  int initial_position = 0;

  // This is the first search position.
  std::size_t found_at = sample.find(delimeter);

  /**
   * And here the substring is stored into the result vector. The substring is
   * the string from the beginning to the first delimeter. It gets data from
   * the initial position to the length we pass to the function. So the length
   * will be (initial_position - found_at).
   */

  result.push_back(sample.substr(initial_position, found_at));

  // We always need to move one fordware because we need to pass the previous
  // search position.
  initial_position = found_at + 1;

  while (found_at != std::string::npos)
  {
    found_at = sample.find(",", initial_position);
    result.push_back(sample.substr(initial_position, found_at - initial_position));
    initial_position = found_at + 1;
  }

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

#endif // UTILS_H
