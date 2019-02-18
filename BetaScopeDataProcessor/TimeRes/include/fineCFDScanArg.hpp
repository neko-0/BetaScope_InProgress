#ifndef FineCFDScanArg_h
#define FineCFDScanArg_h

#include <vector>
#include <string>
#include <tuple>

#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <TH1.h>

struct FineResCFDScanPmodeArgs
{
  int workerID;
  std::string in_file;
  double Dut_tmin;
  double Dut_tmax;
  double Dut_pmin;
  double Dut_pmax;
  double Trig_tmin;
  double Trig_tmax;
  double Trig_pmin;
  double Trig_pmax;
  std::string SensorName = " ";
  std::string Bias = " ";
  std::string Fluence = " ";
  std::string Temperature = " ";
  int totalStep = 500;
  double toRealCFD = 0.2;
  int dut_ch;
  int trig_ch;
};

TH1D *make_CFDTdiff_hitogram(
  TTree* itree,
  TCut icut,
  int workerID,
  int dutChannel,
  int trigChannel,
  int CFDpercentage,
  int numBin=100,
  double minRange=1,
  double maxRange=1
);

std::tuple<double,double,double,bool,double,double> getTimeDiffSigma(
  int workerID,
  TH1D *histogram,
  int CFDpercentage,
  double mean,
  double sigma,
  double minRange=-5000.0,
  double maxRange=5000.0,
  bool savePlot=false,
  std::string plotName ="example"
);

void *scanRunner(void *args);

std::tuple<int,double,double> fitAdjust(
  TTree *itree,
  TCut icut,
  int workerID,
  int dutChannel,
  int trigChannel,
  int CFDpercentage,
  int numBin=100,
  double minRange=1,
  double maxRange=1,
  int numLoop=3
);

std::vector<std::pair<double,double>> cfdScanner(
  TTree *itree,
  TCut icut,
  int workerID,
  int dutChannel,
  int trigChannel,
  int totalStep,
  std::string plotName
);

#endif
