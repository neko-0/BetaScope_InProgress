#include "../DataSelection/include/dataSelection.h"
#include "../DataProcessingConfig/include/measurement_info.h"
#include "../../BetaScopeAnalysis/CustomPrintf/include/customPrintf.h"

#include <iostream>
#include <vector>
#include <boost/thread.hpp>

#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <TThread.h>
#include <TROOT.h>

struct ExtractorInfo{
    std::string ifileName;
    TCut cut;
    std::string ofileName;
};

void runExtractor(ExtractorInfo inputArg){
  TFile *ifile = TFile::Open( inputArg.ifileName.c_str() );
  TFile *ofile = new TFile( inputArg.ofileName.c_str(), "RECREATE" );
  TTree *otree = ((TTree*) ifile->Get("wfm"))->CopyTree( inputArg.cut );
  ofile->cd();
  otree->Write();
  delete ofile;
  delete ifile;
}


std::vector<ExtractorInfo> Extract_Events( std::string measurement_info_txt )
{
  MeasurementInfo measurement_info( measurement_info_txt );

  std::vector<ExtractorInfo> outArgsVec;
  for( int i = 0; i < measurement_info.number_of_runs; i++ )
  {
    std::cout <<  measurement_info.file_name[i].at(0) << " ";

    ExtractorInfo outArg;
    outArg.ifileName = measurement_info.file_name[i].at(0);
    /*==========================================================================
    parsing the cut
    ==========================================================================*/
    std::string delimiter = " ";
    std::string temp_str  = measurement_info.cut[measurement_info.dut_channel-1][i].at(0);
    std::vector<std::string> cut_v;
    std::cout<< temp_str << std::endl;
    while( int(temp_str.find( delimiter )) != -1 )
    {
      cut_v.push_back( temp_str.substr(0, temp_str.find( delimiter ) ) );
      temp_str.erase(0, temp_str.find( delimiter ) + delimiter.length() );
    }
    cut_v.push_back( temp_str );

    DataSelection *selection = new DataSelection( cut_v, measurement_info.dut_channel, measurement_info.trigger_channel );

    outArg.ofileName = "Selected_"+measurement_info.file_name[i].at(0);
    outArg.cut = selection->cuts;

    cut_v.clear();
    delete selection;
    outArgsVec.push_back(outArg);
  }
  return outArgsVec;
}

int main(int argc, const char *argv[])
{
  if(argc < 2 or argc > 3) printf("Error! Check input parameters\n");
  else
  {
    ROOT::EnableThreadSafety();
    std::vector<ExtractorInfo> iArgs = Extract_Events(argv[1]);
    std::vector<boost::thread *> workers;
    for(int i = 0; i<iArgs.size(); i++)
    {
      workers.push_back(new boost::thread( runExtractor, iArgs[i]));
    }
    for(int i = 0; i<iArgs.size(); i++)
    {
      workers[i]->join();
      delete workers[i];
    }
    return 0;
  }
}
