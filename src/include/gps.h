#pragma once

#include "gngll.h"
#include "gngsa.h"
#include "gnrmc.h"
#include "gpgsv.h"

enum GPSType
{
  T_GNGLL,
  T_GNGSA,
  T_GNRMC,
  T_GPGSV
};

class GPS
{
public:
  GNGLL gngll;
  GNGSA gngsa;
  GNRMC gnrmc;
  GPGSV gpgsv;

private:
  GPSType gpsType;

public:
  GPS(GPSType);
  ~GPS();

  void parse(std::string_view);
};
