#include "../TimeRes/include/fineCFDScanArg.hpp"
#include "../DataSelection/include/dataSelection.h"
#include "../DataProcessingConfig/include/measurement_info.h"
#include "../../BetaScopeAnalysis/CustomPrintf/include/customPrintf.h"

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
#include <TGraphErrors.h>
#include <TVectorD.h>
#include <TThread.h>
#include <TStopwatch.h>
#include <TROOT.h>


void CreateDir(const char* DirName)
{
	int dir_check;
	dir_check = mkdir(DirName, ACCESSPERMS);
	if(dir_check == 0){printf("Directory is created. \n");}
	else{printf("Directory already exists! Previous data will be replaced...\n");}
}

void cdTo(std::string Destination)
{
	char GoTo[1024];
	snprintf (GoTo, sizeof(GoTo), "cd %s", Destination.c_str());
	std::cout << "going to : " << GoTo << std::endl;
	chdir ("cd ave_pulse/");
	system ("pwd");
}

void ProcessingCFDScan_Pmode( std::string measurement_info_txt )
{
	//reading the CFD Runlist..
	printf("Start processing...\n");

  gROOT->SetBatch(true);

  TStopwatch StopWatch;

	StopWatch.Start();

	MeasurementInfo measurement_info( measurement_info_txt );

  ROOT::EnableThreadSafety();

  TThread::Initialize();

  static const int num_Worker = measurement_info.number_of_runs;

  TThread *worker_pool[num_Worker];

  FineResCFDScanPmodeArgs assignment[num_Worker];

	for( int i = 0; i < measurement_info.number_of_runs; i++ )
	{
		printf( "Working on %s \n", measurement_info.bias[i].at(0).c_str() );

		char ifile_location[1024];
		sprintf( ifile_location, " ../%s", measurement_info.file_name[i].at(0).c_str() );

		//parsing the cut
		std::string delimiter = " ";
	  std::string temp_str  = measurement_info.cut[measurement_info.dut_channel-1][i].at(0);
		std::vector<std::string> cut_v;
		while( int(temp_str.find( delimiter )) != -1 )
		{
	    cut_v.push_back( temp_str.substr(0, temp_str.find( delimiter ) ) );
			temp_str.erase(0, temp_str.find( delimiter ) + delimiter.length() );
		}
		cut_v.push_back( temp_str );

		if( cut_v.size() == 8 )
		{

      FineResCFDScanPmodeArgs make_assigment;
      make_assigment.in_file = ifile_location;
      make_assigment.workerID = i;
      make_assigment.Dut_tmin = std::stod( cut_v.at(0) );//dut_TminC,
      make_assigment.Dut_tmax = std::stod( cut_v.at(1) ); //dut_TmaxC,
      make_assigment.Dut_pmin  = std::stod( cut_v.at(2) );  //dut_PminC,
      make_assigment.Dut_pmax = std::stod( cut_v.at(3) ); //dut_PmaxC,
      make_assigment.Trig_tmin = std::stod( cut_v.at(4) ); //trig_TminC,
      make_assigment.Trig_tmax = std::stod( cut_v.at(5) ); //trig_TmaxC,
      make_assigment.Trig_pmin = std::stod( cut_v.at(6) ); //trig_PminC,
      make_assigment.Trig_pmax = std::stod( cut_v.at(7) ); //trig_PmaxC,
      make_assigment.SensorName = measurement_info.sensor; //SensorName,
      make_assigment.Bias = measurement_info.bias[i].at(0); //Bias,
      make_assigment.Fluence = measurement_info.fluence; //Fluence,
      make_assigment.Temperature = measurement_info.temperature; //Temperature,
      make_assigment.dut_ch = measurement_info.dut_channel; //dut Channel
      make_assigment.trig_ch = measurement_info.trigger_channel; //trig Channel

      assignment[i] = make_assigment;
		}
		else std::cout << "invalid cut or not implemented yet" << std::endl;
	}

  for( int i = 0; i < num_Worker; i++ )
  {
    worker_pool[i] = new TThread( "", scanRunner, (void*)&assignment[i] );
    worker_pool[i]->Initialize();
  }

  for( int i = 0; i < num_Worker; i++ )
  {
    worker_pool[i]->Run();
  }

  TThread::Ps();

  for( int i = 0; i < num_Worker; i++ )
  {
    worker_pool[i]->Join();
  }

  for( int i = 0; i < num_Worker; i++ )
  {
    delete worker_pool[i];
  }

  TThread::Ps();

  StopWatch.Stop();

	std::cout << " Time : " << StopWatch.RealTime() << "sec " << std::endl;

  gROOT->SetBatch(false);
}


int main (int argc, char **argv)
{
	if( argc == 3 && ( strcmp( "--dir",argv[1] ) == 0 ) )
	{
		CreateDir(argv[2]);
		//Create_CFDScan_Runlist(argv[1]);
	}
	else if (argc == 2)
	{
		ProcessingCFDScan_Pmode( argv[1] );
	}
	else
	{
		printf("Error! Check parameters \n"); return -1;
	}

	return 0;
}
