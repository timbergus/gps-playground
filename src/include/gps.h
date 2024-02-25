#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <tuple>

struct GGA
{
  std::string type;
  std::string utcTime;
  std::string latitude;
  std::string latitudeDirection;
  std::string longitude;
  std::string longitudeDirection;
  std::string quality;
  std::string satellitesUsed;
  std::string HDOP;
  std::string altitude;
  std::string geoidalSeparation;
  std::string DGPS;
};

struct GLL
{
  std::string type;
  std::string latitude;
  std::string latitudeDirection;
  std::string longitude;
  std::string longitudeDirection;
  std::string utcTime;
  std::string status;
};

struct GSA
{
  std::string type;
  std::string mode;
  std::string fixType;
  std::vector<std::string> satellites;
  std::string PDOP;
  std::string HDOP;
  std::string VDOP;
  std::string checksum;
};

struct Satellite
{
  std::string id;
  std::string elevation;
  std::string azimuth;
  std::string snr;
};

struct GSV
{
  std::string type;
  std::string numberOfMessages;
  std::string sequenceNumber;
  std::string satellitesInView;
  std::vector<Satellite> satellites;
};

struct RMC
{
  std::string type;
  std::string utcTime;
  std::string status;
  std::string latitude;
  std::string latitudeDirection;
  std::string longitude;
  std::string longitudeDirection;
  std::string speed;
  std::string course;
  std::string utcDate;
  std::string mode;
};

struct VTG
{
  std::string type;
  std::string course;
  std::string courseMagnetic;
  std::string speedKn;
  std::string speedKh;
  std::string mode;
};

struct ZDA
{
  std::string type;
  std::string utcTime;
  std::string utcDay;
  std::string utcMonth;
  std::string utcYear;
  std::string localZoneHours;
  std::string localZoneMinutes;
};

class GPS
{

private:
  static constexpr double KNTOMS{0.514444444};
  static constexpr double KNTOKMH{1.85};

public:
  enum Units
  {
    ms,
    kmh
  };

private:
  static std::vector<std::string> tokenize(std::string_view);

public:
  GPS();
  ~GPS();

  static GGA parseGGA(std::string_view);
  static GLL parseGLL(std::string_view);
  static GSA parseGSA(std::string_view);
  static GSV parseGSV(std::string_view);
  static RMC parseRMC(std::string_view);
  static VTG parseVTG(std::string_view);
  static ZDA parseZDA(std::string_view);

  // Tools

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
