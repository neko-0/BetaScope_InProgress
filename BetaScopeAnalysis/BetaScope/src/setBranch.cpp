#include "../include/betaAnalysis.h"

#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include <utility>
#include <vector>
#include <TSystem.h>

void betaAnalysis::SetBranch()
{
	for( std::size_t b = 0, max = this->channel.size(); b < max; b++ )
	{
		this->oTree->Branch( Form("w%d", this->channel.at(b)), &this->w[b] );

		this->oTree->Branch( Form("wRaw%d", this->channel.at(b)), &wRaw[b] );

		this->oTree->Branch( Form("t%d", this->channel.at(b)), &t[b] );

		this->oTree->Branch( Form("cfd%d", this->channel.at(b)), &cfd[b] );

		this->oTree->Branch( Form("cfd%d_fall", this->channel.at(b)), &cfdFall[b] );

		this->oTree->Branch( Form("rms%d", this->channel.at(b)), &noise[b] );

		this->oTree->Branch( Form("pmax%d", this->channel.at(b)), &pmax[b] );

		this->oTree->Branch( Form("neg_pmax%d", this->channel.at(b)), &negPmax[b] );

		this->oTree->Branch( Form("tmax%d", this->channel.at(b)), &tmax[b] );

		this->oTree->Branch( Form("neg_tmax%d", this->channel.at(b)), &negTmax[b] );

		this->oTree->Branch( Form("multiplePmax%d", this->channel.at(b)), "vector<double>", &multiplePmax[b] );

		this->oTree->Branch( Form("multipleTmax%d", this->channel.at(b)), "vector<double>", &multipleTmax[b] );

		this->oTree->Branch( Form("riseTime%d", this->channel.at(b)), &riseTime[b] );

		this->oTree->Branch( Form("dvdt%d", this->channel.at(b)), &dvdt[b] );

		this->oTree->Branch( Form("thTime%d", this->channel.at(b)), &this->thTime[b] );

		this->oTree->Branch( Form("fineCFDRise%d", this->channel.at(b)), &this->fineCFDRise[b] );

		this->oTree->Branch( Form("singlePeak%d", this->channel.at(b)), "vector<int>", &this->singlePeak[b] );

		this->oTree->Branch( Form("time_at_thresholdLevel%d", this->channel.at(b)), "vector<double>", &this->timeAtThreshold[b] );

		//this->oTree->Branch( Form("pulseArea_withInterEdge%d", this->channel.at(b)), "vector<double>", &this->pulseArea_withInterEdge[b] );

		this->oTree->Branch( Form("back_rms%d", this->channel.at(b)), "vector<double>", &this->noiseAfterSignal[b] );

		this->oTree->Branch( Form("pulseArea_1nsFront_3nsBack%d", this->channel.at(b)), "vector<double>", &this->pulseArea[b] );

		this->oTree->Branch( Form("frontBaselineInt%d", this->channel.at(b)), "vector<double>", &this->frontBaselineInt[b] );

		this->oTree->Branch( Form("backBaselineInt%d", this->channel.at(b)), "vector<double>", &this->backBaselineInt[b] );

		this->oTree->Branch( Form("frontBaselineInt_indepBaseCorr%d", this->channel.at(b)), "vector<double>", &this->frontBaselineInt_indepBaseCorr[b] );

		this->oTree->Branch( Form("backBaselineInt_indepBaseCorr%d", this->channel.at(b)), "vector<double>", &this->backBaselineInt_indepBaseCorr[b] );

		this->oTree->Branch( Form("numPeak%d", this->channel.at(b)), &this->numPeaks[b], Form("numPeak%d/i", this->channel.at(b)) );

		//===for SSRL ====
		if( this->ssrl_switch )
		{
			this->oTree->Branch( Form("ssrl_pmax%d", this->channel.at(b)), "vector<double>", &this->ssrl_pmax[b] );
			this->oTree->Branch( Form("ssrl_tmax%d", this->channel.at(b)), "vector<double>", &this->ssrl_tmax[b] );
			this->oTree->Branch( Form("ssrl_num_bunch%d", this->channel.at(b) ), "vector<int>", &this->ssrl_num_bunch[b] );
			this->oTree->Branch( Form("ssrl_riseTime%d", this->channel.at(b) ), "vector<TimeWindow>", &this->ssrl_riseTime[b] );
			this->oTree->Branch( Form("ssrl_pulseArea%d", this->channel.at(b) ), "vector<TimeWindow>", &this->ssrl_pulseArea[b] );
			this->oTree->Branch( Form("ssrl_pmax_index%d", this->channel.at(b)), "vector<int>", &this->ssrl_pmax_index[b] );
			this->oTree->Branch( Form("ssrl_window_pmax%d", this->channel.at(b)), "vector<TimeWindow>", &this->ssrl_window_pmax[b] );
			this->oTree->Branch( Form("ssrl_window_tmax%d", this->channel.at(b)), "vector<TimeWindow>", &this->ssrl_window_tmax[b] );
			this->oTree->Branch( Form("ssrl_rms%d", this->channel.at(b)), "vector<TimeWindow>", &this->ssrl_rms[b] );
			this->oTree->Branch( Form("ssrl_dvdt%d", this->channel.at(b)), "vector<TimeWindow>", &this->ssrl_dvdt[b] );
			this->oTree->Branch( Form("ssrl_cfd%d", this->channel.at(b)), "vector<TimeWindow>", &this->ssrl_cfd[b] );
			this->oTree->Branch( Form("ssrl_cfdFall%d", this->channel.at(b)), "vector<TimeWindow>", &this->ssrl_cfdFall[b] );
			this->oTree->Branch( Form("ssrl_toa%d", this->channel.at(b)), "vector<TimeWindow>", &this->ssrl_toa[b] );
		}
		//===========================

		auto check_volName = ((TTree *) this->iFile->Get(iTreeName.c_str()))->GetListOfBranches()->FindObject(Form("wC%d", this->channel.at(b)) );
		if(check_volName!=NULL){
			this->voltageReader[b] = new TTreeReaderArray<double>( *this->treeReader, Form("wC%d", this->channel.at(b)) );
			this->timeReader[b] = new TTreeReaderArray<double>( *this->treeReader, Form("tC%d", this->channel.at(b)) );
		}
		else{
			this->voltageReader[b] = new TTreeReaderArray<double>( *this->treeReader, Form("w%d", this->channel.at(b)) );
			this->timeReader[b] = new TTreeReaderArray<double>( *this->treeReader, Form("t%d", this->channel.at(b)) );
		}
	}

	//===for saving scope timestamp===
	///*
	if( this->saveTimestamp )
	{
		this->f_scope_time = new TTreeReaderValue<ULong64_t>( *this->treeReader, "i_scope_time" );
		this->oTree->Branch( "i_scope_time", &this->i_scope_time, "i_scope_time/l" );
	}
	//*/
	//================================

	//===for SLAC raw x y data===
	///*
	if( this->SLAC_data_switch )
	{
		this->oTree->Branch( "xpos", "vector<double>", &this->xpos );
		this->oTree->Branch( "ypos", "vector<double>", &this->ypos );
		//this->oTree->Branch( "scope_time", "vector<double>", &this->scope_time );
		//this->oTree->Branch( "eudaq_time", "vector<double>", &this->eudaq_time );
	}
	//*/
	//===========================

	//============================================================================
	//for SLAC raw x y data
	///*
	if( this->SLAC_data_switch )
	{
		this->xposReader = new TTreeReaderArray<double>( *this->treeReader, "dut_x" );
		this->yposReader = new TTreeReaderArray<double>( *this->treeReader, "dut_y" );
		//this->scope_time_reader = new TTreeReaderArray<double>( *this->treeReader, "scope_time" );
		//this->eudaq_time_reader = new TTreeReaderArray<double>( *this->treeReader, "eudaq_time" );
	}
	//*/
	//============================================================================

	if( this->ssrl_AC_position )
	{
		auto check_ypos = ((TTree *) this->iFile->Get(iTreeName.c_str()))->GetListOfBranches()->FindObject("y");
		auto check_zpos = ((TTree *) this->iFile->Get(iTreeName.c_str()))->GetListOfBranches()->FindObject("z");
		if( check_ypos != NULL && check_zpos != NULL )
		{
			this->SSRL_XposReader = new TTreeReaderArray<double>( *this->treeReader, "z" );
			this->SSRL_YposReader = new TTreeReaderArray<double>( *this->treeReader, "y" );
		}
		else
		{
			this->SSRL_XposReader = new TTreeReaderArray<double>( *this->treeReader, "xpos" );
			this->SSRL_YposReader = new TTreeReaderArray<double>( *this->treeReader, "ypos" );
		}
		this->oTree->Branch( "ssrl_pos_x", "vector<double>", &this->ssrl_pos_x );
		this->oTree->Branch( "ssrl_pos_y", "vector<double>", &this->ssrl_pos_y );
	}

	/*============================================================================
	Check to see if the TTree has timestamp and current branches. If yes, Create
	same branch for the output file.
	============================================================================*/
	auto check_current = ((TTree *) this->iFile->Get(iTreeName.c_str()))->GetListOfBranches()->FindObject("i_current");
	if( check_current != NULL )
	{
		printf("Current branch found.\n");
		this->enableCurrent = true;
		this->oTree->Branch( "current", &currentFiller, "current/D" );
		this->currentReader = new TTreeReaderValue<double>( *this->treeReader, "i_current" );
	}

	auto check_timestamp = ((TTree *) this->iFile->Get(iTreeName.c_str()))->GetListOfBranches()->FindObject("i_timestamp");
	if( check_timestamp != NULL )
	{
		printf("Timestamp branch found.\n");
		this->enableScopeTimestamp = true;
		this->oTree->Branch( "timestamp", &timestampFiller, "timestamp/D" );
		this->scopeTimestampReader = new TTreeReaderValue<double>( *this->treeReader, "i_timestamp" );
	}
	else
	{
		auto check_time_vector = ((TTree *) this->iFile->Get(iTreeName.c_str()))->GetListOfBranches()->FindObject("time");
		if( check_time_vector != NULL )
		{
			printf("Timestamp STL vector branch found.\n");
			this->enableScopeTimestamp = true;
			this->use_timestamp_v = true;
			this->oTree->Branch( "timestamp", &timestampFiller, "timestamp/D" );
			this->timestamp_v_reader = new TTreeReaderArray<double>( *this->treeReader, "time" );
		}
	}



	this->treeReader->Next();
	this->npoint = this->timeReader[0]->GetSize();
	this->m_inoise = 0.25 * this->npoint; //0.25
	this->treeReader->Restart();
	if ( this->npoint == 0 ){ std::cout << "there is no npoints, check your file" << std::endl; return void();}
}
