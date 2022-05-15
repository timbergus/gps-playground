// print.h

#ifndef PRINT_H
#define PRINT_H

#include <iostream>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include "color.h"

class Print
{
private:
  Color palette;

public:
  Print();
  ~Print();

  void print_title(std::string);
  void print_subtitle(std::string);
  void print_info(std::string, auto);
  void print_number(std::string, auto, std::string = "");
};

Print::Print()
{
}

Print::~Print()
{
}

void Print::print_title(std::string label)
{
  std::string formattedLabel = palette.set_color(label, "cyan", "underline");
  std::cout << formattedLabel << "\n\n";
}

void Print::print_subtitle(std::string label)
{
  std::string formattedLabel = palette.set_color(label, "yellow", "underline");
  std::cout << fmt::format("\n{}\n\n", formattedLabel);
}

void Print::print_info(std::string label, auto value)
{
  std::string formattedLabel = palette.set_color(fmt::format("{}:", label), "green");
  std::cout << fmt::format("{} {}\n", formattedLabel, value);
}

void Print::print_number(std::string label, auto value, std::string units)
{
  std::string formattedLabel = palette.set_color(fmt::format("{}:", label), "green");
  std::string formattedUnits = palette.set_color(units, "magenta");
  std::cout << fmt::format("{} {:.5f} {}\n", formattedLabel, value, formattedUnits);
}

#endif // PRINT_H
