#include "../include/fineCFDScanArg.hpp"
#include "../../../BetaScopeAnalysis/CustomPrintf/include/customPrintf.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

std::tuple<int,double,double> fitAdjust(
  TTree *itree,
  TCut icut,
  int workerID,
  int dutChannel,
  int trigChannel,
  int CFDpercentage,
  int numBin,
  double minRange,
  double maxRange,
  int numLoop
)
{
  int numBinAfterAdjust=numBin;
  double meanAfterAdjust;
  double sigmaAfterAdjust;

  TH1D *histogram = new TH1D;
  histogram = make_CFDTdiff_hitogram(itree, icut, workerID, dutChannel, trigChannel, CFDpercentage, numBin, minRange, maxRange);
  //std::cout << "binSS  " << numBin << std::endl;
  meanAfterAdjust = histogram->GetXaxis()->GetBinCenter(histogram->GetMaximumBin());
  sigmaAfterAdjust = histogram->GetStdDev(1);
  std::tuple<double,double,double,bool,double,double> fitResult = getTimeDiffSigma(workerID, histogram, CFDpercentage, meanAfterAdjust, sigmaAfterAdjust);
  meanAfterAdjust = std::get<0>(fitResult);
  sigmaAfterAdjust = std::get<1>(fitResult);
  delete histogram;

  int counter = 0;
  double min_range=minRange;
  double max_range=maxRange;
  while(counter < numLoop)
  {
    double m = meanAfterAdjust;
    TH1D *adjusted_histogram = new TH1D;
    adjusted_histogram = make_CFDTdiff_hitogram(itree, icut, workerID, dutChannel, trigChannel, CFDpercentage, numBinAfterAdjust, min_range, max_range);
    std::tuple<double,double,double,bool,double,double> fitResult_local = getTimeDiffSigma(workerID, adjusted_histogram, CFDpercentage, meanAfterAdjust, sigmaAfterAdjust, min_range, max_range );
    if(std::get<3>(fitResult_local))
    {
      std::cout<<"counter " << counter<< std::endl;
      std::cout<< "workerID # " << workerID <<" bad fit in fitAdjust ... " << std::endl;
      min_range = -1000.0;
      max_range = 1000.0;
      meanAfterAdjust = adjusted_histogram->GetMean(1);
      sigmaAfterAdjust = adjusted_histogram->GetStdDev(1);
      numBinAfterAdjust = numBin;
    }
    else
    {
      meanAfterAdjust = std::get<0>(fitResult_local);
      sigmaAfterAdjust = std::get<1>(fitResult_local);
      min_range = meanAfterAdjust - abs((counter+3.5)*adjusted_histogram->GetStdDev(1));
      max_range = meanAfterAdjust + abs((counter+3.5)*adjusted_histogram->GetStdDev(1));
      numBinAfterAdjust = pow(adjusted_histogram->GetEntries()*1.0, 0.5);
      //std::cout << "min range " << min_range << std::endl;
      //std::cout << "mmax range " << max_range << std::endl;
      //std::cout << "bin  " << numBinAfterAdjust << std::endl;
    }
    delete adjusted_histogram;
    counter++;
  }
  //std::cout<<"i am done with loop " << std::endl;

  return std::make_tuple(numBinAfterAdjust, min_range, max_range);
}
