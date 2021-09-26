// gsa.h

#ifndef GSA_H
#define GSA_H

#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

class GSA
{
private:
  struct gsa_t
  {
    std::string type;
    std::string mode;
    std::string fix_type;
    std::vector<std::string> satellites;
    std::string PDOP;
    std::string HDOP;
    std::string VDOP;
    std::string checksum;
  } data;

public:
  GSA(std::vector<std::string>);
  ~GSA();

  bool is_valid(std::string, std::string);

  void print_data();
};

GSA::GSA(std::vector<std::string> core_data)
{
  data.type = core_data[0].substr(1);
  data.mode = core_data[1];
  data.fix_type = core_data[2];
  for (int i = 0; i < 12; i++)
  {
    data.satellites.push_back(core_data[i + 3]);
  }
  data.PDOP = core_data[4];
  data.HDOP = core_data[5];
  data.VDOP = core_data[6];
  data.checksum = core_data[7];
}

GSA::~GSA()
{
}

bool GSA::is_valid(std::string core_data, std::string checksum)
{
  std::string data = core_data.substr(1);

  int check = 0;

  for (int i = 0; i < (int)data.size(); i++)
  {
    check = char(check ^ data.at(i));
  }

  char hex_check[20];

  sprintf(hex_check, "%X", check);

  return hex_check == checksum;
}

void GSA::print_data()
{
  std::cout << "Mode: " << data.mode << std::endl;
  std::cout << "Fix Type: " << data.fix_type << std::endl;
  std::cout << "Satellite: " << Utils::stringify_vector(data.satellites) << std::endl;
  std::cout << "PDOP: " << data.PDOP << std::endl;
  std::cout << "HDOP: " << data.HDOP << std::endl;
  std::cout << "VDOP: " << data.VDOP << std::endl;
}

#endif /* GSA_H */
