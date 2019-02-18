#include "../include/histogramProcessor.hpp"
#include "../../DataOutputFormat/include/dataOutputFormat.hpp"

//==============================================================================
// processing the histogram and output results
//==============================================================================

void HistogramProcessor::ProcessHistogram(
  const char* path,
  std::string biasVoltage,
  TCut selection,
  bool saveResult,
  TCut sideband1,
  TCut sideband2
 )
{
  gROOT->Reset();

  TCut temp_cuts = selection;

  public_cuts = temp_cuts;

  current_file = path;
  TFile *loadFile = TFile::Open(path);
  TTree* loadTree = (TTree*) loadFile->Get("wfm");

  //std::map<std::string,std::pair<double,double>> oData[2] = {}; //for storing the output data.

  int fineAdjustmentTime = 0;

  for( int ch = 0; ch < 2; ch++ ) //dut first and then trigger
  {
    for( std::size_t i = 0, max = this->plotArg[ch].size(); i < max; i++ )
    {
      double par;
      double par_err;
      int whichCh;

      if( ch == 0 ) whichCh = this->DUT;
      else whichCh = this->Trigger;

      std::string plotName = this->plotArg[ch].at(i).at(0);

      std::tuple<int,double,double> fitAdjust { this->ini_bin, this->ini_rangeMin, this->ini_rangeMin }; //histogram binning and ranges

      bool confineRise;

      if( (ch == 0) && (plotName.compare("Rise Time") == 0) ){ confineRise = true; } //restrict the rise time fit around the distribution peak
      else{ confineRise = false; }

      while( fineAdjustmentTime < (this->recursionNum) ) //number of times to readjust the ranges and binning of the histogram
      {
        if( this->plotArg[ch].at(i).size() == 5 && this->plotArg[ch].at(i).at(4).compare("Roo") == 0 )
        {
          std::tuple<double,double,int,double,double> temp =  //this function return the parameter, parameter error, and the binning and ranges of fitted histogram.
          HistogramProcessor::Make_Histogram(
              path,
              loadTree,
              this->plotArg[ch].at(i).at(0),
              this->plotArg[ch].at(i).at(1),
              this->plotArg[ch].at(i).at(2),
              this->plotArg[ch].at(i).at(3),
              temp_cuts,
              whichCh,
              std::get<0>(fitAdjust),
              std::get<1>(fitAdjust),
              std::get<2>(fitAdjust),
              false,
              false,
              confineRise,
              "Roo",
              sideband1,
              sideband2 );

          std::get<0>(fitAdjust) = std::get<2>(temp); //These lines update the binning and ranges.
          std::get<1>(fitAdjust) = std::get<3>(temp);
          std::get<2>(fitAdjust) = std::get<4>(temp);

          fineAdjustmentTime ++;
        }
        else
        {
          std::tuple<double,double,int,double,double> temp =  //this function return the parameter, parameter error, and the binning and ranges of fitted histogram.
          HistogramProcessor::Make_Histogram(
              path,
              loadTree,
              this->plotArg[ch].at(i).at(0),
              this->plotArg[ch].at(i).at(1),
              this->plotArg[ch].at(i).at(2),
              this->plotArg[ch].at(i).at(3),
              temp_cuts,
              whichCh,
              std::get<0>(fitAdjust),
              std::get<1>(fitAdjust),
              std::get<2>(fitAdjust),
              false,
              false,
              confineRise );

          std::get<0>(fitAdjust) = std::get<2>(temp); //These lines update the binning and ranges.
          std::get<1>(fitAdjust) = std::get<3>(temp);
          std::get<2>(fitAdjust) = std::get<4>(temp);

          fineAdjustmentTime ++;
        }
      }

      if( this->plotArg[ch].at(i).size() == 5 && this->plotArg[ch].at(i).at(4).compare("Roo") == 0 )
      {
        std::tuple<double,double,int,double,double> out_tuple =
        HistogramProcessor::Make_Histogram(
        path,
        loadTree,
        this->plotArg[ch].at(i).at(0),
        this->plotArg[ch].at(i).at(1),
        this->plotArg[ch].at(i).at(2),
        this->plotArg[ch].at(i).at(3),
        temp_cuts,
        whichCh,
        std::get<0>(fitAdjust),
        std::get<1>(fitAdjust),
        std::get<2>(fitAdjust),
        true,
        true,
        confineRise,
        "Roo",
        sideband1,
        sideband2 );
        par     = std::get<0>(out_tuple);
        par_err = std::get<1>(out_tuple);
      }
      else
      {
        std::tuple<double,double,int,double,double> out_tuple =
        HistogramProcessor::Make_Histogram(
        path,
        loadTree,
        this->plotArg[ch].at(i).at(0),
        this->plotArg[ch].at(i).at(1),
        this->plotArg[ch].at(i).at(2),
        this->plotArg[ch].at(i).at(3),
        temp_cuts,
        whichCh,
        std::get<0>(fitAdjust),
        std::get<1>(fitAdjust),
        std::get<2>(fitAdjust),
        true,
        true,
        confineRise );
        par     = std::get<0>(out_tuple);
        par_err = std::get<1>(out_tuple);
      }

      //par     = std::get<0>(out_tuple);
      //par_err = std::get<1>(out_tuple);

      fineAdjustmentTime = 0; //reset for the next histogram.

      std::pair<double,double> output = std::make_pair( par, par_err );

      this->oData[ch].insert( std::pair<std::string,std::pair<double,double>>( plotName, output ) ); //appending the results into the std map.
    }
  }

  loadFile-> Close();
  delete loadFile;

  //if( saveResult ){
  //  DataOutputFormat outfile;
  //  outfile.CreateBetaScopeOutputFile( biasVoltage, oData );
  //}
  //else{}
}
