// rmc.h

#ifndef RMC_H
#define RMC_H

#include <iostream>
#include <string>
#include <vector>

#include "color.h"
#include "print.h"
#include "utils.h"

class RMC
{
private:
  Color palette;
  Print printer;

  struct rmc_t
  {
    std::string type;
    std::string utc_time;
    std::string status;
    std::string latitude;
    std::string latitude_direction;
    std::string longitude;
    std::string longitude_direction;
    std::string speed;
    std::string course;
    std::string utc_date;
    std::string mode;
    std::string checksum;
  } data;

public:
  RMC(std::vector<std::string>);
  ~RMC();

  void print_raw_data();
  void print_formatted_data();
};

RMC::RMC(std::vector<std::string> core_data)
{
  data.type = core_data[0].substr(1);
  data.utc_time = core_data[1];
  data.status = core_data[2];
  data.latitude = core_data[3];
  data.latitude_direction = core_data[4];
  data.longitude = core_data[5];
  data.longitude_direction = core_data[6];
  data.speed = core_data[7];
  data.course = core_data[8];
  data.utc_date = core_data[9];
  data.mode = core_data[12];
  data.checksum = core_data[13];
}

RMC::~RMC()
{
}

void RMC::print_raw_data()
{
  printer.print_title("RMC Sample Data");
  printer.print_info("Type", data.type);
  printer.print_info("UTC Time", data.utc_time);
  printer.print_info("Status", data.status);
  printer.print_info("Latitude", data.latitude);
  printer.print_info("Latitude Direction", data.latitude_direction);
  printer.print_info("Longitude", data.longitude);
  printer.print_info("Longitude Direction", data.longitude_direction);
  printer.print_info("Speed", data.speed);
  printer.print_info("Course", data.course);
  printer.print_info("UTC Date", data.utc_date);
  printer.print_info("Mode", data.mode);
}

void RMC::print_formatted_data()
{
  printer.print_title("RMC Sample Data");
  printer.print_info("Time", Utils::get_time(data.utc_time));
  printer.print_info("Date", Utils::get_date(data.utc_date));
  printer.print_number("Latitude", Utils::get_latitude(data.latitude));
  printer.print_number("Longitude", Utils::get_longitude(data.longitude, data.longitude_direction));
  printer.print_number("Speed", Utils::get_speed(data.speed, "ms"), "m/s");
}

#endif /* RMC_H */
