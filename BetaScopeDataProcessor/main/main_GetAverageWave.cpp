#include "../averageWave/include/averageWave.hpp"
#include "../DataProcessingConfig/include/measurement_info.h"

#include <vector>
#include <iostream>
#include <boost/thread.hpp>

#include <TThread.h>
#include <TROOT.h>

std::vector<Average_Wave> prepare_averageWave_workers( std::string measurement_info_txt)
{
  MeasurementInfo measurement_info( measurement_info_txt );
  std::vector<Average_Wave> output;
  for( int i =0; i < measurement_info.number_of_runs; i++)
  {
    std::cout <<  measurement_info.file_name[i].at(0) << " \n";
    Average_Wave outArg( measurement_info.file_name[i].at(0) );
    outArg.set_dut_ch( measurement_info.dut_channel);
    outArg.set_trig_ch( measurement_info.trigger_channel);
    output.push_back( outArg);
  }
  return output;
}

int main(int argc, char *argv[])
{
  ROOT::EnableThreadSafety();
  std::vector<Average_Wave> workers = prepare_averageWave_workers(argv[1]);
  std::vector<boost::thread *> workers_dorm;
  for( std::size_t i = 0, max = workers.size(); i<max; i++)
  {
    workers_dorm.push_back( new boost::thread( boost::bind(&Average_Wave::averaging, &workers.at(i) ) ) );
  }
  for( std::size_t i = 0, max = workers_dorm.size(); i<max; i++)
  {
    workers_dorm[i]->join();
    delete workers_dorm[i];
  }
  return 0;
}
