#include "../include/betaAnalysis.h"

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

betaAnalysis::betaAnalysis(const char* ipath)
{
	TThread::Lock();
	std::cout << std::string( 100, '*' ) << std::endl;
	std::cout << "calling the pre-set analysis" << std::endl;
	TThread::UnLock();

	this->path = ipath;

	this->iFile = new TFile( this->path );

	if( this->iFile->IsZombie() )
	{
		TThread::Lock();
		std::cout << this->path << " is bad" << std::endl;
		TThread::UnLock();
		this->ifile_status = false;
	}
	else
	{
		TThread::Lock();
		std::cout << this->path << " is good" << std::endl;
		TThread::UnLock();
	}

	std::string delimiter = "/";
	std::string opath = ipath;
	while( int(opath.find( delimiter )) != -1 )
	{
		opath.erase(0, opath.find( delimiter ) + delimiter.length() );
	}
	this->ofileName = this->filePrefix += opath;
	this->ifileName = ipath;

	//this->iTree = (TTree*) this->iFile->Get( this->iTreeName.c_str() );
	this->treeReader = new TTreeReader( this->iTreeName.c_str(), this->iFile );

	this->oFile = new TFile( this->ofileName.c_str(), "RECREATE", "", this->compressionLevel );
	this->oTree = new TTree( "wfm", "waveform analysis" );
	this->oTree->SetDirectory( this->oFile );

	this->numEvent = this->treeReader->GetEntries(true);

	this->channel.push_back(2);
	this->invertPulse.push_back(1);

	this->channel.push_back(3);
	this->invertPulse.push_back(0);

	for( std::size_t i = 0, max = this->channel.size(); i < max; i++)
	{
		std::cout << "enable channel: " << this->channel.at(i) << " invert: " << this->invertPulse.at(i) << std::endl;
		this->cfd[i].reserve( 101 );
		this->cfdFall[i].reserve( 101 );
		this->fineCFDRise[i].reserve( this->fineCFDStep );

		this->multiplePmax[i].reserve( this->maxNumPeakDetection );
		this->multipleTmax[i].reserve( this->maxNumPeakDetection );

		this->thTime[i].reserve( 50 );
		this->dvdt[i].reserve( 101 );
	}
}


betaAnalysis::betaAnalysis(const char* ipath, const char* custom)
{
	TThread::Lock();
	std::cout << std::string( 100, '*' ) << std::endl;
	std::cout << "calling the analysis with " << custom <<std::endl;
	TThread::UnLock();

	this->path = ipath;

	this->iFile = new TFile( this->path );

	if( this->iFile->IsZombie() )
	{
		TThread::Lock();
		std::cout << this->path << " is bad" << std::endl;
		TThread::UnLock();
		this->ifile_status = false;
	}
	else
	{
		TThread::Lock();
		std::cout << this->path << " is good" << std::endl;
		TThread::UnLock();

		std::string delimiter = "/";
		std::string opath = ipath;
		while( int(opath.find( delimiter )) != -1 )
		{
			opath.erase(0, opath.find( delimiter ) + delimiter.length() );
		}
		this->ofileName = this->filePrefix += opath;
		this->ifileName = ipath;

		//this->iTree = (TTree*) this->iFile->Get( this->iTreeName.c_str() );
		this->treeReader = new TTreeReader( this->iTreeName.c_str(), this->iFile );

		this->oFile = new TFile( this->ofileName.c_str(), "RECREATE", "", this->compressionLevel );
		this->oTree = new TTree( "wfm", "waveform analysis" );
		this->oTree->SetDirectory( this->oFile );

		this->numEvent = this->treeReader->GetEntries(true);
	}
}

void betaAnalysis::AllocateMemory()
{
	for( std::size_t i = 0, max = this->channel.size(); i < max; i++)
	{
		std::cout << "enable channel: " << this->channel.at(i) << " invert: " << this->invertPulse.at(i) << std::endl;
		this->cfd[i].reserve( 101 );
		this->cfdFall[i].reserve( 101 );
		this->fineCFDRise[i].reserve( this->fineCFDStep );

		this->multiplePmax[i].reserve( this->maxNumPeakDetection );
		this->multipleTmax[i].reserve( this->maxNumPeakDetection );

		this->thTime[i].reserve( 2000 );
		this->dvdt[i].reserve( 101 );

		this->w[i].reserve( 221560 );
		this->t[i].reserve( 221560 );
		this->wRaw[i].reserve( 221560 );

		this->timeAtThreshold[i].reserve( 15 );

		this->pulseArea[i].reserve(10);
		//this->pulseArea_withInterEdge_Extended[i].reserve(10);
		this->noiseAfterSignal[i].reserve(10);

		//===for SSRL analysis=======
		if( this->ssrl_switch )
		{
			this->ssrl_pmax[i].reserve( 20 );
			this->ssrl_tmax[i].reserve( 20 );
			this->ssrl_num_bunch[i].reserve( 2 );
			this->ssrl_pmax_index[i].reserve(20);
			this->ssrl_riseTime[i].reserve(1024);
			this->ssrl_pulseArea[i].reserve(1024);
			this->ssrl_window_pmax[i].reserve( 1024 );
			this->ssrl_window_tmax[i].reserve( 1024 );
			this->ssrl_rms[i].reserve( 1024 );
			this->ssrl_dvdt[i].reserve( 1024 );
			this->ssrl_cfd[i].reserve( 1024 );
			this->ssrl_cfdFall[i].reserve( 1024 );
			this->ssrl_toa[i].reserve( 1024 );
		}
		//===========================
	}

	//===for SLAC raw x y data===
	///*
	if( this->SLAC_data_switch )
	{
		this->xpos.reserve( 20480 );
		this->ypos.reserve( 20480 );
		//this->scope_time.reserve( 20480 );
		//this->eudaq_time.reserve( 20480 );
	}
	//*/
	//===========================

	if( this->ssrl_AC_position )
	{
		this->ssrl_pos_x.reserve( 2 );
		this->ssrl_pos_y.reserve( 2 );
	}
}
