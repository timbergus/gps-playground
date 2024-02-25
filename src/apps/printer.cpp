#include "printer.h"

void Printer::printGGA(GGA data)
{
  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
}

void Printer::printGLL(GLL data)
{
  auto [hours, minutes, seconds] = GPS::parseUtcTime(data.utcTime);

  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  printInfo("Time", fmt::format("{}:{}:{}", hours, minutes, seconds));
  printNumber("Latitude", GPS::parseLatitude(data.latitude));
  printNumber("Longitude", GPS::parseLongitude(data.longitude, data.longitudeDirection));
  printInfo("Status", data.status);
}

void Printer::printGSA(GSA data)
{
  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  printInfo("Mode", data.mode);
  printInfo("Fix Type", data.fixType);
  printInfo("Satellite", GPS::stringifyVector(data.satellites));
  printInfo("PDOP", data.PDOP);
  printInfo("HDOP", data.HDOP);
  printInfo("VDOP", data.VDOP);
}

void Printer::printGSV(GSV data)
{
  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  printInfo("Number of Messages", data.numberOfMessages);
  printInfo("Sequence Number", data.sequenceNumber);
  printInfo("Satellites in View", data.satellitesInView);

  printSubtitle("Satellites");

  for (int i = 0; i < std::stoi(data.numberOfMessages); i++)
  {
    printSatellite(data.satellites[i]);
  }
}

void Printer::printRMC(RMC data)
{
  auto [hours, minutes, seconds] = GPS::parseUtcTime(data.utcTime);
  auto [day, month, year] = GPS::parseUtcTime(data.utcTime);

  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  printInfo("Time", fmt::format("{}:{}:{}", hours, minutes, seconds));
  printInfo("Date", fmt::format("{}:{}:{}", day, month, year));
  printNumber("Latitude", GPS::parseLatitude(data.latitude));
  printNumber("Longitude", GPS::parseLongitude(data.longitude, data.longitudeDirection));
  printNumber("Speed", GPS::parseSpeed(data.speed, GPS::ms), "m/s");
}

void Printer::printVTG(VTG data)
{
  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
}

void Printer::printZDA(ZDA data)
{
  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
}

Printer::Printer()
{
}

Printer::~Printer()
{
}

void Printer::print(std::string_view sample)
{
  auto type = GPS::split(sample, ',').at(0);

  if (type.find("GGA"))
  {
    printGLL(GPS::parseGLL(sample));
  }
  else if (type.find("GLL"))
  {
    printGLL(GPS::parseGLL(sample));
  }
  else if (type.find("GSA"))
  {
    printGSA(GPS::parseGSA(sample));
  }
  else if (type.find("GSV"))
  {
    printGSV(GPS::parseGSV(sample));
  }
  else if (type.find("RMC"))
  {
    printRMC(GPS::parseRMC(sample));
  }
  else if (type.find("VTG"))
  {
    printVTG(GPS::parseVTG(sample));
  }
  else if (type.find("ZDA"))
  {
    printZDA(GPS::parseZDA(sample));
  }
  else
  {
    fmt::println("This is not working :(");
  }
}

void Printer::printTitle(std::string_view label)
{
  fmt::print(fmt::emphasis::underline | fg(fmt::color::cyan), "{}\n\n", label);
}

void Printer::printSubtitle(std::string_view label)
{
  fmt::print(fmt::emphasis::underline | fg(fmt::color::yellow), "\n{}\n\n", label);
}

void Printer::printInfo(std::string_view label, auto value)
{
  fmt::print("{} {}\n", fmt::format(fg(fmt::color::yellow_green), "{}:", label), value);
}

void Printer::printNumber(std::string_view label, auto value, std::string_view units)
{
  fmt::print(
      "{} {:.7f} {}\n",
      fmt::format(fg(fmt::color::yellow_green), "{}:", label),
      value,
      fmt::format(fg(fmt::color::blue_violet), "{}", units));
}

void Printer::printSatellite(Satellite satellite)
{
  fmt::print(
      "{1} {2} {0} {3} {4} {0} {5} {6} {0} {7} {8}\n",
      fmt::format(fg(fmt::color::red), "|"),
      fmt::format(fg(fmt::color::yellow_green), "Id: "),
      satellite.id,
      fmt::format(fg(fmt::color::yellow_green), "Elevation: "),
      satellite.elevation,
      fmt::format(fg(fmt::color::yellow_green), "Azimuth: "),
      satellite.azimuth,
      fmt::format(fg(fmt::color::yellow_green), "SNR: "),
      satellite.snr);
}

void Printer::clearScreen()
{
#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif
}
