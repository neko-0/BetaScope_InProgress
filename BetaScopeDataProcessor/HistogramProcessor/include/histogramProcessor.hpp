#ifndef getResults_h
#define getResults_h

//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
//------ROOT----------------//
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <TTree.h>
#include <iomanip>
#include <TFile.h>
#include "TF1.h"
#include <TLeaf.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TSystem.h>
#include <TImage.h>
#include <TPaveText.h>
//======RooFit=================//
#include <RooFit.h>
#include <RooDataHist.h>
#include <RooHistPdf.h>
#include <RooRealVar.h>
#include <RooPlot.h>
#include <RooGaussian.h>
#include <RooLandau.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooFFTConvPdf.h>
#include <RooAbsPdf.h>
#include <RooAddPdf.h>
#include <RooExtendPdf.h>

namespace RooFit {}
using namespace RooFit;

class HistogramProcessor
{
  int entries;
  int corr_ch;
  int coin_events;

  int DUT     = 1;
  int Trigger = 2;

  double tmax_min, tmax_max, pmax_min, pmax_max;

  int    ini_bin = 60;
  double ini_rangeMin = 1;
  double ini_rangeMax = 1;

  bool auto_adjust = true;

  int recursionNum = 5;

  std::vector<std::vector<std::string>> plotArg[2] = {};

  std::string current_file;

  int landau_left  = 5;
  int landau_right = 20;

  public:
  std::string public_cuts = "no cuts yet";

  std::map<std::string,std::pair<double,double>> oData[2] = {};

  void SetupParameters();

/*==============================================================================
// Tmax vs Pmax scatter plot
//==============================================================================
  void scatterPlot(
  const char* path,
  int         dut,
  int         trig,
  TCut        cuts     = "",
  bool        saveFile = false,
  double      Ymin     = -1.0,
  double      Ymax     = -1.0,
  double      Xmax     = -1.0
  );

//==============================================================================
// A generic scatter plot
//==============================================================================
  void scatterPlot(
  const char* path,
  std::string Xpar,
  std::string Ypar,
  int         channel,
  TCut        cuts     = "",
  bool        saveFile = false,
  double      Ymin     = -1.0,
  double      Ymax     = -1.0,
  double      Xmax     = -1.0
);*/

  std::tuple<double,double,int,double,double> Make_Histogram(
  const char* path,
  TTree *loadTree,
  std::string HistName,
  std::string expression,
  std::string FitFunc,
  std::string xTitle,
  TCut        cuts,
  int         Channel,
  int         binNum,
  double      range_min,
  double      range_max,
  bool        makePlot = true,
  bool        savePlot = false,
  bool        confine  = false
  );

  std::tuple<double,double,int,double,double> Make_Histogram(
    const char* path,
    TTree *loadTree,
    std::string HistName,
    std::string expression,
    std::string FitFunc,
    std::string xTitle,
    TCut        cuts,
    int         Channel,
    int         binNum,
    double      range_min,
    double      range_max,
    bool        makePlot,
    bool        savePlot,
    bool        confine,
    std::string useRoo,
  TCut        sideband1,
    TCut        sideband2
  );

  void ProcessHistogram(
  const char* path,
  std::string biasVoltage,
  TCut        selection,
  bool        saveResult = false,
  TCut        sideband1 = "",
    TCut        sideband2 = ""
  );

  void landau_hist_factor( int left, int right ){
    landau_left = left;
    landau_right = right;
  };

  void adjust_hist( bool on_off){
    auto_adjust = on_off;
  };

  void set_ch(int dut = 2, int trigger = 3){
    DUT = dut - 1;
    Trigger = trigger - 1;
  };

  int get_DUT(){ return this->DUT; }
  int get_Trigger(){ return this->Trigger; }
};

#endif
