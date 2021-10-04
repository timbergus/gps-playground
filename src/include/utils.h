// utils.h

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <map>

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

  char *next_token;
  char *token = strtok_s(&sample[0], delimeter.c_str(), &next_token);

  while (token)
  {
    result.push_back(token);
    token = strtok_s(NULL, delimeter.c_str(), &next_token);
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

#endif // UTILS_H
