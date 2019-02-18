#ifndef BETASCOPE_MEASUREMENT_INFO_HPP_MEASUREMENT_INFO_H
#define BETASCOPE_MEASUREMENT_INFO_HPP_MEASUREMENT_INFO_H

#include <string>
#include <vector>

void GenerateRunInfo();

struct MeasurementInfo
{
  //header section
  std::string sensor;
  std::string fluence;
  std::string temperature;
  int time_bin;
  double time_range_min;
  double time_range_max;
  bool self_correct;
  std::string average_pulse_ofile_name;
  int number_of_runs;
  int trigger_channel;
  int dut_channel;
  bool use_selected_events;

  //file section
  std::vector<std::string> bias[1024];
  std::vector<std::string> file_name[1024];
  std::vector<std::string> cut[4][1024];
  std::vector<double> CFD[1024];

  MeasurementInfo( std::string ifile );
  void PrintContent();
};

#endif //BETASCOPE_MEASUREMENT_INFO_HPP_MEASUREMENT_INFO_H
