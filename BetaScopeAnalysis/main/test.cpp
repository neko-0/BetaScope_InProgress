#include "../WaveformAnalysis/include/Waveform_Analysis.hpp"
#include "../WaveformAnalysis/include/general.hpp"

#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
  std::cout<< "Testing\n";
  std::vector<double> w = {1,2,3,4,5,4,3,2,1};
  std::vector<double> t = {1,2,3,4,5,4,3,2,1};

  double range[2] = {0,0};

  std::pair<double, unsigned int>hi = Find_Singal_Maximum(w,t,false, range);
  std::cout<<hi.first<<std::endl;
  std::cout<<hi.second<<std::endl;
  return 0;
}
