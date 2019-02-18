    ///////////////////////////////
   //                           //
  // Argument class header     //
 //                           //
///////////////////////////////

#ifndef BETASCOPE_BETAANALYSIS_HPP_BETAANALYSIS_H
#define BETASCOPE_BETAANALYSIS_HPP_BETAANALYSIS_H

//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
//------ROOT----------------//
#include <TFile.h>
#include <TTree.h>
#include <TThread.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include "TimeWindow.hpp"

class betaAnalysis
{
  TFile *iFile = new TFile;
  //TTree *iTree = new TTree;

  TFile *oFile = new TFile;
  TTree *oTree = new TTree;

  const char* path;

  static const int numCh = 8; //Beta-Scope only have maximum of 4 recording channels.
  std::vector<unsigned int> channel = {};
  std::vector<int> invertPulse = {};

  unsigned int numEvent;

  std::string filePrefix = "stats_";
  std::string ofileName;
  std::string ifileName;

  std::string iTreeName = "wfm";
  int compressionLevel = 8;


  bool restricSignalMaxSearchRange = false; //restricting the pulse maximum searching region.
  double signalMaxSearchRange_Min = -1000.0; //unit pico-second
  double signalMaxSearchRange_Max = 1000.0;

  bool skipBadVector = false;

  std::string sampleLeaf = "w2";

  TTreeReader *treeReader = 0;

  TTreeReaderArray<double> *voltageReader[numCh] = {};
  TTreeReaderArray<double> *timeReader[numCh] = {};

  bool saveTimestamp = false;
  ULong64_t i_scope_time;
  TTreeReaderValue<ULong64_t> *f_scope_time;

  //===for SLAC raw x y data===
  ///*
  bool SLAC_data_switch = true;
  TTreeReaderArray<double> *xposReader;
  TTreeReaderArray<double> *yposReader;
  TTreeReaderArray<double> *scope_time_reader;
  TTreeReaderArray<double> *eudaq_time_reader;
  //*/
  //===========================

  int m_inoise;

  double voltageScalar = 1000.0;
  double timeScalar = 1.0e12;

  unsigned int trigCH = 3;

  int fineCFDStep = 500;

  int maxNumPeakDetection = 4;

  bool parallelProcessing = false;

  bool ifile_status = true;

  unsigned int npoint;

  std::string objectLocation = "default";

  std::vector<double> w[numCh];
  std::vector<double> t[numCh];
  std::vector<double> wRaw[numCh];
  std::vector<double> pmax[numCh];
  std::vector<double> negPmax[numCh];
  std::vector<double> multiplePmax[numCh];
  std::vector<double> multipleTmax[numCh];
  std::vector<double> tmax[numCh];
  std::vector<double> negTmax[numCh];
  std::vector<double> riseTime[numCh];
  std::vector<double> pulseArea[numCh];
  std::vector<double> noise[numCh];
  std::vector<double> noiseAfterSignal[numCh];
  std::vector<double> dvdt[numCh];
  std::vector<double> cfd[numCh];
  std::vector<double> cfdFall[numCh];
  std::vector<double> thTime[numCh];
  std::vector<double> fineCFDRise[numCh];
  std::vector<int> singlePeak[numCh];
  std::vector<double> timeAtThreshold[numCh];
  std::vector<double> frontBaselineInt[numCh];
  std::vector<double> backBaselineInt[numCh];
  std::vector<double> frontBaselineInt_indepBaseCorr[numCh];
  std::vector<double> backBaselineInt_indepBaseCorr[numCh];

  int numPeaks[numCh];

  //===for SLAC raw x y data===
  ///*
  std::vector<double> xpos;
  std::vector<double> ypos;
  //std::vector<double> scope_time;
  //std::vector<double> eudaq_time;
  //*/
  //===========================

  //===for SSRL analysis===
  bool ssrl_dynamic_noise = false;
  bool ssrl_bruteForceBaseline = false;
  bool ssrl_switch = false;
  bool ssrl_AC_position = false;
  std::vector<double> ssrl_pmax[numCh];
  std::vector<double> ssrl_tmax[numCh];
  std::vector<int>    ssrl_num_bunch[numCh];
  std::vector<int>    ssrl_pmax_index[numCh];
  std::vector<TimeWindow> ssrl_window_pmax[numCh];
  std::vector<TimeWindow> ssrl_window_tmax[numCh];
  std::vector<TimeWindow> ssrl_rms[numCh];
  std::vector<TimeWindow> ssrl_dvdt[numCh];
  std::vector<TimeWindow> ssrl_cfd[numCh];
  std::vector<TimeWindow> ssrl_riseTime[numCh];
  std::vector<TimeWindow> ssrl_pulseArea[numCh];
  std::vector<TimeWindow> ssrl_cfdFall[numCh];
  std::vector<TimeWindow> ssrl_toa[numCh];
  std::vector<double> ssrl_pos_x;
  std::vector<double> ssrl_pos_y;
  double SSRL_assist_threshold = 6.0;
  TTreeReaderArray<double> *SSRL_XposReader;
  TTreeReaderArray<double> *SSRL_YposReader;
  //=======================

  /*============================================================================
  Current and timestamp branches
  ============================================================================*/
  bool enableScopeTimestamp = false;
  bool enableCurrent = false;
  double timestampFiller; //unix time
  double currentFiller; //in unit of uA
  TTreeReaderValue<double> *scopeTimestampReader;
  TTreeReaderValue<double> *currentReader;
  TTreeReaderArray<double> *timestamp_v_reader; bool use_timestamp_v = false; //someone uses this in the SLAC testbeam.
  std::vector<double> omg_time_why;


  std::pair<double,double> noise2, noise3;
  std::pair<double,double> baseline2, baseline3;

  public:

    betaAnalysis(const char* ipath);
    betaAnalysis(const char* ipath, const char* custom);
    //betaAnalysis(const betaAnalysis &copy){ TThread::Lock(); std::cout << "copy constructor " << this << std::endl; TThread::UnLock(); this->objectLocation = "copy version"; }
    ~betaAnalysis()
    {
      TThread::Lock();
      std::cout << this << " call destructor at " << this->objectLocation <<std::endl;
      TThread::UnLock();

      //delete this->iTree;
      //delete this->iFile;
      //delete this->oTree;
      //delete this->oFile;
    }

    virtual void AllocateMemory();
    virtual void SetBranch();
    virtual void ClearBuffer();
    virtual void FillData();
    virtual void SaveFile();


    unsigned int get_numEvent(){ return this->numEvent; }
    std::string get_path(){ std::string str = this->path; return str; }

    unsigned int get_npoint(){ return this->npoint; }
    void set_npoint( unsigned int iValue ){ this->npoint = iValue;  }

    double get_voltageScalar(){ return this->voltageScalar; }
    void set_voltageScalar(double iValue){ this->voltageScalar = iValue; }

    double get_timeScalar(){ return this->timeScalar; }
    void set_timeScalar(double iValue){ this->timeScalar = iValue; }

    std::string get_iTreeName(){ return this->iTreeName; }
    void set_iTreeName( std::string iValue ){ this->iTreeName = iValue; }

    int get_compressionLevel(){ return this->compressionLevel; }
    void set_compressionLevel( int iValue ){ this->compressionLevel = iValue; }

    std::string get_sampleLeaf(){ return this->sampleLeaf; }
    void set_sampleLeaf( std::string iValue ){ this->sampleLeaf = iValue; }

    std::string get_filePrefix(){ return this->filePrefix; }
    void set_filePrefix( std::string iValue ){ this->filePrefix = iValue; }

    std::string get_ofileName(){ return this->ofileName; }
    std::string get_ifileName(){ return this->ifileName; }

    bool get_parallelStatus(){ return parallelProcessing; }
    void set_parallelStatus( bool iValue ){ this->parallelProcessing = iValue; }

    std::string get_objectLocation(){ return this->objectLocation; }
    void set_objectLocation(std::string iValue){ this->objectLocation = iValue; }

    bool get_restricSignalMaxSearchRange_status(){ return this->restricSignalMaxSearchRange; }
    void set_restricSignalMaxSearchRange_status(bool iValue){ this->restricSignalMaxSearchRange = iValue; }
    double get_signalMaxSearchRange_Min(){ return this->signalMaxSearchRange_Min; }
    void set_signalMaxSearchRange_Min(double iValue){ this->signalMaxSearchRange_Min = iValue; }
    double get_signalMaxSearchRange_Max(){ return this->signalMaxSearchRange_Max; }
    void set_signalMaxSearchRange_Max(double iValue){ this->signalMaxSearchRange_Max = iValue; }

    void enableChannel(int whichCh, int invert){
      this->channel.push_back( whichCh );
      this->invertPulse.push_back( invert );
      std::cout << "channel " << whichCh << " is enable. Invert pulses = " << invert << " (0=off, 1=on)" << std::endl;
    }

    void set_ifile_status(bool iValue){ this->ifile_status = iValue; }
    bool get_ifile_status(){ return this->ifile_status; }

    void set_SLAC_status(bool iValue){ this->SLAC_data_switch = iValue; }
    bool get_SLAC_status(){ return this->SLAC_data_switch; }

    void set_ssrl_status(bool iValue){ this->ssrl_switch = iValue; }
    bool get_ssrl_status(){ return this->ssrl_switch; }

    void set_ssrl_assist_threshold(double iValue){ this->SSRL_assist_threshold = iValue; }
    double get_ssrl_assist_threshold(){ return this->SSRL_assist_threshold; }

    void set_ssrl_AC_status(bool iValue){ this->ssrl_AC_position = iValue; }
    bool get_ssrl_AC_status(){ return this->ssrl_AC_position; }

    void set_timestamp_status( bool iValue){ this->saveTimestamp = iValue; }
    bool get_timestamp_status(){ return this->saveTimestamp; }

    void set_ssrl_dynamic_noise(bool iValue){ this->ssrl_dynamic_noise = iValue; }
    bool get_ssrl_dynamic_noise(){return this->ssrl_dynamic_noise; }

    void set_ssrl_bruteForceBaseline(bool iValue){ this->ssrl_bruteForceBaseline = iValue; }
    bool get_ssrl_bruteForceBaseline(){return this->ssrl_bruteForceBaseline; }

    void set_skipBadVector_status(bool iValue){ this->skipBadVector = iValue; }
    bool get_skipBadVector_status(){return this->skipBadVector;}

};


void *betaAnalyzer( void *iarg );
void parallelBetaAnalyzer( std::vector<betaAnalysis> argVector );




#endif // BETASCOPE_BETAANALYSIS_HPP_BETAANALYSIS_H
