#include "printer.h"

void Printer::printGNGLL(GNGLLProtocol data)
{
  auto [hours, minutes, seconds] = GPSTools::parseUtcTime(data.utcTime);

  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  printInfo("Time", fmt::format("{}:{}:{}", hours, minutes, seconds));
  printNumber("Latitude", GPSTools::parseLatitude(data.latitude));
  printNumber("Longitude", GPSTools::parseLongitude(data.longitude, data.longitudeDirection));
  printInfo("Status", data.status);
}

void Printer::printGNGSA(GNGSAProtocol data)
{
  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  printInfo("Mode", data.mode);
  printInfo("Fix Type", data.fixType);
  printInfo("Satellite", GPSTools::stringifyVector(data.satellites));
  printInfo("PDOP", data.PDOP);
  printInfo("HDOP", data.HDOP);
  printInfo("VDOP", data.VDOP);
}

void Printer::printGNRMC(GNRMCProtocol data)
{
  auto [hours, minutes, seconds] = GPSTools::parseUtcTime(data.utcTime);
  auto [day, month, year] = GPSTools::parseUtcTime(data.utcTime);

  printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  printInfo("Time", fmt::format("{}:{}:{}", hours, minutes, seconds));
  printInfo("Date", fmt::format("{}:{}:{}", day, month, year));
  printNumber("Latitude", GPSTools::parseLatitude(data.latitude));
  printNumber("Longitude", GPSTools::parseLongitude(data.longitude, data.longitudeDirection));
  printNumber("Speed", GPSTools::parseSpeed(data.speed, GPSTools::ms), "m/s");
}

void Printer::printGPGSV(GPGSVProtocol data)
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

Printer::Printer()
{
}

Printer::~Printer()
{
}

void Printer::print(std::string_view sample)
{
  auto type = GPSTools::split(sample, ',').at(0);

  if (type == "$GNGLL")
  {
    printGNGLL(GNGLL::parse(sample));
  }
  else if (type == "$GNGSA")
  {
    printGNGSA(GNGSA::parse(sample));
  }
  else if (type == "$GNRMC")
  {
    printGNRMC(GNRMC::parse(sample));
  }
  else if (type == "$GPGSV")
  {
    printGPGSV(GPGSV::parse(sample));
  }
  else
  {
    fmt::println("This is not working.");
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

void Printer::printSatellite(satelliteProtocol satellite)
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