#include "../include/fineCFDScanArg.hpp"

#include <TGraph.h>

#include <tuple>
#include <iostream>
#include <string>

std::vector<std::pair<double,double>> cfdScanner(
  TTree *itree,
  TCut icut,
  int workerID,
  int dutChannel,
  int trigChannel,
  int totalStep,
  std::string plotName
)
{
  std::vector<std::pair<double,double>> out_Res_v;

  for( int cfd_per = 0; cfd_per < totalStep; cfd_per++ )
  {
    std::tuple<int, double, double> fitResults = fitAdjust(itree, icut, workerID, dutChannel, trigChannel, cfd_per);
    //std::cout<<"I am here after fitAdjust" << std::endl;
    TH1D *histo = new TH1D;
    histo = make_CFDTdiff_hitogram( itree, icut, workerID, dutChannel, trigChannel, cfd_per, std::get<0>(fitResults), std::get<1>(fitResults), std::get<2>(fitResults) );
    //std::cout<<"I am here nowwowowowowo" << std::endl;
    //double mean = std::get<1>(fitResults);
    //double sigma = std::get<2>(fitResults);
    double minRange = std::get<1>(fitResults);
    double maxRange = std::get<2>(fitResults);
    std::string outputPlotName = plotName.substr(4,plotName.size())+"_workerID"+std::to_string(workerID);
    std::tuple<double,double,double,bool,double,double> tDiffResults = getTimeDiffSigma( workerID, histo, cfd_per, histo->GetMean(1), histo->GetStdDev(1), minRange, maxRange, true, outputPlotName);
    std::cout << plotName << " on CFD " << cfd_per << " Res Result: " << std::get<1>(tDiffResults) << " , " << std::get<2>(tDiffResults)  << std::endl;
    out_Res_v.push_back( std::make_pair( std::get<1>(tDiffResults) , std::get<2>(tDiffResults) ) );
    delete histo;
  }

  return out_Res_v;
}
