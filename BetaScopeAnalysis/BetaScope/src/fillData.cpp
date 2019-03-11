#include "../include/betaAnalysis.h"
#include "../include/TimeWindow.hpp"
#include "../../WaveformAnalysis/include/Waveform_Analysis.hpp"
//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <numeric>
//------ROOT----------------//
#include <TThread.h>
#include <TMath.h>

int slot_num( double tmax )
{
  int window_size = 2000;
  int num_increment = 23;
  int starting = 0; //-9000
  int window_slot = 0;

  for( int i = 1; i < num_increment; i++ )
  {
    if( tmax >= starting && tmax<= (starting+window_size) )
    {
      return window_slot;
    }
    else
    {
      starting = starting + window_size;
      window_slot++;
    }
  }

  return window_slot;
}


void betaAnalysis::FillData()
{
  bool limiting_search_region_OnOff = betaAnalysis::get_restricSignalMaxSearchRange_status();

  //double tempHolder;
  int fill = 0;

  double ssrl_bruteForcedBaselineCorr[this->channel.size()];
  if( this->ssrl_bruteForceBaseline )
  {
    std::vector<double> voltage[this->channel.size()];
    while(this->treeReader->Next())
    {
      for( std::size_t b = 0, max_b = this->channel.size(); b < max_b; b++ )
      {
        for(std::size_t i = 0, maxPnt = voltageReader[b]->GetSize() ; i < this->npoint; i++)
        {
          voltage[b].push_back( this->voltageReader[b]->At(i) * this->voltageScalar );
        }
        ssrl_bruteForcedBaselineCorr[b] = std::accumulate(voltage[b].begin(), voltage[b].end(), 0.0)/voltage[b].size();
        voltage[b].clear();
      }
    }
    this->treeReader->Restart();
  }
  std::cout<<ssrl_bruteForcedBaselineCorr[0]<<std::endl;

  while( this->treeReader->Next() )
  {
    //==========================================================================
    // Checking the status of the waveform and time vector size
    bool checkNextEvent = true;
    for( std::size_t b = 0, max_b = this->channel.size(); b < max_b; b++ )
    {
      if( this->skipBadVector )
      {
        while( (this->voltageReader[b]->GetSize() != 2001) || (this->timeReader[b]->GetSize() != 2001) )
        {
          std::cout << "Skip ill vector in On, found ill vector at " << fill << "with size " << this->voltageReader[b]->GetSize() <<", skipping whole event..." << std::endl;
          this->treeReader->Next();
          fill++;
        }
      }

      if( (this->timeReader[b]->GetSize() == 0) || (this->voltageReader[b]->GetSize() == 0) || (this->voltageReader[b]->GetSize() != this->timeReader[b]->GetSize()) )
      {
        while( this->voltageReader[b]->GetSize() == 0 && checkNextEvent )
        {
          std::cout << "Voltage Reader is empty at event " << fill << ", skipping whole event..." << std::endl;
          checkNextEvent = this->treeReader->Next();
          fill++;
        }
        while( this->timeReader[b]->GetSize() == 0 && checkNextEvent )
        {
          std::cout << "Time Reader is empty at event " << fill << ", skipping whole event..." << std::endl;
          checkNextEvent = this->treeReader->Next();
          fill++;
        }
        while( ( (this->voltageReader[b]->GetSize() != this->timeReader[b]->GetSize()) ) && checkNextEvent )
        {
          std::cout << "Voltage Reader and Time Reader have different array size at event " << fill << ", skipping whole event..." << std::endl;
          checkNextEvent = this->treeReader->Next();
          fill++;
        }
      }
    }
    //std::cout << "fill " << fill << std::endl;

    //==========================================================================
    // Start to fill waveform and time branches.

    for( std::size_t b = 0, max_b = this->channel.size(); b < max_b; b++ )
    {
      if( this->voltageReader[b]->GetSize() != this->npoint )
      {
        //std::cout << "Event at " << fill << " is different from the npoint " << this->npoint << ", updating the value to " << this->voltageReader[b]->GetSize() << std::endl;
        betaAnalysis::set_npoint( this->voltageReader[b]->GetSize() );
      }
      else{}

      if( this->ssrl_switch )
      {
        //const double dT =  this->timeReader[b]->At(1) - this->timeReader[b]->At(0);
        const double dT =  50.0;//pico-second increment

        for(unsigned int i = 0; i < this->npoint; i++)
        {
          this->w[b].push_back( this->voltageReader[b]->At(i) * this->voltageScalar );
          this->t[b].push_back( (0.0 + i*dT) * this->timeScalar );
          this->wRaw[b].push_back( this->voltageReader[b]->At(i) * this->voltageScalar );
        }
      }
      else
      {
        for(unsigned int i = 0; i < this->npoint; i++)
        {
          this->w[b].push_back( this->voltageReader[b]->At(i) * this->voltageScalar );
          this->t[b].push_back( this->timeReader[b]->At(i) * this->timeScalar );
          this->wRaw[b].push_back( this->voltageReader[b]->At(i) * this->voltageScalar );
        }
      }
    }

    //==========================================================================
    // Finding all the parameters, Pmax, Rise TIme, etc.

    double pmaxSearchRange[2] = {this->signalMaxSearchRange_Min, this->signalMaxSearchRange_Max};

    for( std::size_t b = 0, max_channel = this->channel.size(); b < max_channel; b++ )
    {

      if( this->invertPulse.at(b) )
      {
        for(std::size_t i = 0, max = this->w[b].size(); i < max; i++ )
        {
          this->w[b][i] = -1.0 * this->w[b][i];
        }
      }

      double RMS =0.0;
      if( this->ssrl_switch && !this->ssrl_dynamic_noise && !this->ssrl_bruteForceBaseline )
      {
        if( channel.at(b) != 3 ) SSRL_Baseline( this->w[b], this->ofileName );
      }
      else if( this->ssrl_dynamic_noise )
      {
        if(channel.at(b) != this->trigCH) SSRL_Baseline( this->w[b], RMS);
        else Correct_Baseline2( this->w[b], 0.25); //the SSRL Test beam trigger is kind of like a step function.
      }
      else if( this->ssrl_bruteForceBaseline )
      {
        for(std::size_t i = 0, max = this->w[b].size(); i < max; i++ )
        {
          this->w[b][i] = this->w[b][i]+1.0*ssrl_bruteForcedBaselineCorr[b];
        }
      }
      else
      { //special treatment of baseline for small SNR signal.
        std::vector<double> front_temp_voltage = this->w[b];
        std::vector<double> front_temp_time = this->t[b];

        std::vector<double> back_temp_voltage = this->w[b];
        std::vector<double> back_temp_time = this->t[b];

        std::pair<double,unsigned int> pmax_before_baseline = Find_Singal_Maximum( this->w[b], this->t[b], limiting_search_region_OnOff, pmaxSearchRange);
        double tmax_for_baseline = this->t[b].at(pmax_before_baseline.second);
        double temp_riseTime = Find_Rise_Time( this->w[b], this->t[b], pmax_before_baseline, 0.1, 0.9 );

        double temp_front_searchRange[2] = {tmax_for_baseline-temp_riseTime-15000.0, tmax_for_baseline-temp_riseTime-10000.0};
        std::pair<double,unsigned int> front_baseline_pmax = Find_Singal_Maximum( this->w[b], this->t[b], true, temp_front_searchRange );
        double front_baseline_tmax = this->t[b].at(front_baseline_pmax.second);
        double front_baseline_riseTime = Find_Rise_Time( this->w[b], this->t[b], front_baseline_pmax, 0.1, 0.9 );
        double temp_baselineRange[2] = {front_baseline_tmax-front_baseline_riseTime-5000.0, front_baseline_tmax+5000.0};
        Correct_Baseline3(front_temp_voltage, front_temp_time, temp_baselineRange );
        std::pair<double,unsigned int> front_baseline_pmax_corr = Find_Singal_Maximum( front_temp_voltage, front_temp_time, true, temp_front_searchRange );
        this->frontBaselineInt_indepBaseCorr[b].push_back( Pulse_Integration_with_Fixed_Window_Size( front_temp_voltage, front_temp_time, front_baseline_pmax_corr, "Simpson", 1000.0, 3000.0) );


        double temp_back_searchRange[2] = {tmax_for_baseline+10000.0, tmax_for_baseline+15000.0 };
        std::pair<double,unsigned int> back_baseline_pmax = Find_Singal_Maximum ( this->w[b], this->t[b], true, temp_back_searchRange);
        double back_baseline_tmax = this->t[b].at(back_baseline_pmax.second);
        double back_baseline_riseTime = Find_Rise_Time( this->w[b], this->t[b], back_baseline_pmax, 0.1, 0.9 );
        double temp_back_baselineRange[2] = {back_baseline_tmax-back_baseline_riseTime-5000.0, back_baseline_tmax+5000.0 };
        Correct_Baseline3(back_temp_voltage, back_temp_time, temp_back_baselineRange );
        std::pair<double,unsigned int> back_baseline_pmax_corr = Find_Singal_Maximum( back_temp_voltage, back_temp_time, true, temp_back_searchRange );
        this->backBaselineInt_indepBaseCorr[b].push_back( Pulse_Integration_with_Fixed_Window_Size( back_temp_voltage, back_temp_time, back_baseline_pmax_corr, "Simpson", 1000.0, 3000.0) );

        double baselineRange[2] = {tmax_for_baseline-temp_riseTime-3000.0, tmax_for_baseline-temp_riseTime-1000.0 };
        Correct_Baseline3(this->w[b], this->t[b], baselineRange);

        //for( int i = 0; i < this->npoint; i++ )
        //{
          //if(TMath::IsNaN(this->w[b][i])) std::cout << "NaN !! \n";
        //}
      }
      //else betaAnalysis::base_line(this->npoint, this->w[b], this->m_inoise);

      if( this->channel.at(b) == this->trigCH ){ betaAnalysis::set_restricSignalMaxSearchRange_status(false); }
      else{ betaAnalysis::set_restricSignalMaxSearchRange_status(limiting_search_region_OnOff); }

      std::pair<double,unsigned int> pmaxHolder = Find_Singal_Maximum( this->w[b], this->t[b], this->restricSignalMaxSearchRange, pmaxSearchRange );
      std::pair<double,unsigned int> neg_pmaxHolder = Find_Negative_Signal_Maximum( this->w[b], this->t[b], this->restricSignalMaxSearchRange, pmaxSearchRange);

      this->pmax[b].push_back( pmaxHolder.first );
      this->negPmax[b].push_back( neg_pmaxHolder.first );

      for(int k =0; k<101; k++){ double percentage = 1.0*k; this->cfd[b].push_back( Rising_Edge_CFD_Time( percentage, this->w[b], this->t[b], pmaxHolder ) ); }

      for(int k =0; k<101; k++){ this->cfdFall[b].push_back( Falling_Edge_CFD_Time( k, this->w[b], this->t[b], pmaxHolder ) ); }

      for( int step = 0; step < (this->fineCFDStep); step++ ){ double percentage = 0.2 * step; this->fineCFDRise[b].push_back( Rising_Edge_CFD_Time( percentage, this->w[b], this->t[b], pmaxHolder ) ); }

      //========================================================================
      //Filling Tmax
      this->tmax[b].push_back( Get_Tmax( this->t[b], pmaxHolder) );
      this->negTmax[b].push_back( Get_Tmax( this->t[b], neg_pmaxHolder) );

      //--------------------------------------------------------------------------------------------

      if( this->ssrl_dynamic_noise && channel.at(b) != this->trigCH )
      {
        this->noise[b].push_back( RMS );
        double SSRL_PmaxRange[2] = {this->signalMaxSearchRange_Min-2000.0, this->signalMaxSearchRange_Max-2000.0};
        std::pair<double,unsigned int> previous_signal = Find_Singal_Maximum( this->w[b], this->t[b], limiting_search_region_OnOff, SSRL_PmaxRange );
        this->pmax[b].push_back( previous_signal.first );
        this->tmax[b].push_back( Get_Tmax( this->t[b], previous_signal) );
      }
      else
      {
        this->noise[b].push_back( Find_Noise( this->w[b], this->m_inoise ) );
      }

      this->pulseArea[b].push_back( Pulse_Integration_with_Fixed_Window_Size( this->w[b], this->t[b], pmaxHolder, "Simpson", 1000.0, 3000.0) );

      this->frontBaselineInt[b].push_back( Pulse_Integration_with_Fixed_Window_Size( this->w[b], this->t[b], pmaxHolder, "Simpson", 12000.0, -8000.0) );

      this->backBaselineInt[b].push_back( Pulse_Integration_with_Fixed_Window_Size( this->w[b], this->t[b], pmaxHolder, "Simpson", -8000.0, 12000.0) );

      //------------------Rise Time 10 to 90---------------------------------------------------------

      this->riseTime[b].push_back( Find_Rise_Time( this->w[b], this->t[b], pmaxHolder, 0.1, 0.9 ) );

      //--------------------------------------------------------------------------------------------

      for(int d =0; d<101; d++){ this->dvdt[b].push_back( Find_Dvdt( d, 0, this->w[b], this->t[b], pmaxHolder ) ); }

      if( this->ssrl_dynamic_noise && channel.at(b) == this->trigCH )
      {
        for(int k = 0; k < 2000; k++){ this->thTime[b].push_back( Find_Time_At_Threshold( double(k), this->w[b], this->t[b]) ); }
      }
      else{ for(int k = 0; k < 2000; k++){ this->thTime[b].push_back( Find_Time_At_Threshold( double(k), this->w[b], this->t[b], pmaxHolder ) ); } }


      //**********************************************************************************************
      //====Multiple Peaks====

      int StartPoint = 0;

      bool multiplePeakDetection = true;

      //int previousIndex = 0;

      for (int peak = 0; peak < this->maxNumPeakDetection; peak++)
      {

        std::pair<double,unsigned int> multiplePeaksHolder = Find_Identical_Peak( this->w[b], this->t[b], StartPoint, false, this->signalMaxSearchRange_Min, this->signalMaxSearchRange_Max );

        this->multiplePmax[b].push_back( multiplePeaksHolder.first );

        unsigned int tsize = this->w[b].size();

        if ( multiplePeaksHolder.second  == tsize ) multiplePeaksHolder.second  = multiplePeaksHolder.second - 1;

        this->multipleTmax[b].push_back( this->t[b].at(multiplePeaksHolder.second) );

        StartPoint = multiplePeaksHolder.second;
      }

      if( multiplePeakDetection )
      {
        if( std::abs(this->multipleTmax[b].at(0) - this->multipleTmax[b].at(3)) > 150.0 )
        {
          this->singlePeak[b].push_back( 0 );
        }
        else
        {
          this->singlePeak[b].push_back( 1 );
        }

        multiplePeakDetection = false;
      }

      //double_pmax_holder.clear();


      //==============================================================================================

      int peak_counter = Signal_Peak_Counter( this->w[b], this->t[b], 12.0);
      this->numPeaks[b] = peak_counter;

      //==============================================================================================

      Get_TimeAcrossThreshold( 120.0, this->w[b], this->t[b], this->timeAtThreshold[b], 12 );

      //==============================================================================================

      if( this->ssrl_switch )
      {
        Get_PmaxTmax_Of_Multiple_Singal( this->SSRL_assist_threshold, this->w[b], this->t[b], this->ssrl_pmax[b], this->ssrl_tmax[b], this->ssrl_pmax_index[b] );
        this->ssrl_num_bunch[b].push_back( this->ssrl_pmax[b].size() );

        for( std::size_t i = 0, max = this->ssrl_pmax[b].size(); i < max; i++ )
        {
          std::pair<double,unsigned int> ssrl_pmax_holder = std::make_pair( this->ssrl_pmax[b].at(i), this->ssrl_pmax_index[b].at(i) );

          std::vector<double> ssrl_window_pmax_filler;
          ssrl_window_pmax_filler.push_back( this->ssrl_pmax[b].at(i) );

          std::vector<double> ssrl_window_tmax_filler;
          ssrl_window_tmax_filler.push_back( this->ssrl_tmax[b].at(i) );

          std::vector<double> ssrl_riseTime_filler;
          ssrl_riseTime_filler.push_back( Find_Rise_Time( this->w[b], this->t[b], ssrl_pmax_holder, 0.1, 0.9 ) );

          std::vector<double> ssrl_pulseArea_filler;
          ssrl_pulseArea_filler.push_back( Find_Pulse_Area( this->w[b], this->t[b], ssrl_pmax_holder) );

          std::vector<double> ssrl_rms_filler;
          ssrl_rms_filler.push_back( Find_Noise( this->w[b], this->m_inoise ) );

          std::vector<double> ssrl_cfd_filler;
          for(int k =0; k<101; k++){ double percentage = 1.0*k; ssrl_cfd_filler.push_back( Rising_Edge_CFD_Time( percentage, this->w[b], this->t[b], ssrl_pmax_holder ) ); }

          std::vector<double> ssrl_cfdFall_filler;
          for(int k =0; k<101; k++){ double percentage = 1.0*k; ssrl_cfdFall_filler.push_back( Falling_Edge_CFD_Time( percentage, this->w[b], this->t[b], ssrl_pmax_holder ) ); }

          std::vector<double> ssrl_dvdt_filler;
          for(int d =0; d<101; d++){ ssrl_dvdt_filler.push_back( Find_Dvdt( d, 0, this->w[b], this->t[b], ssrl_pmax_holder ) ); }

          std::vector<double> ssrl_toa_filler;
          for(int k = 0; k < 100; k++){ ssrl_toa_filler.push_back( Find_Time_At_Threshold( double(k), this->w[b], this->t[b], ssrl_pmax_holder ) ); }


          TimeWindow tw_pulseArea( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_pulseArea_filler );
          this->ssrl_pulseArea[b].push_back( tw_pulseArea );

          TimeWindow tw_riseTime( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_riseTime_filler );
          this->ssrl_riseTime[b].push_back( tw_riseTime );

          TimeWindow tw_rms( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_rms_filler );
          this->ssrl_rms[b].push_back( tw_rms  );

          TimeWindow tw_dvdt( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_dvdt_filler );
          this->ssrl_dvdt[b].push_back( tw_dvdt );

          TimeWindow tw_cfd( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_cfd_filler );
          this->ssrl_cfd[b].push_back( tw_cfd );

          TimeWindow tw_cfdFall( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_cfdFall_filler );
          this->ssrl_cfdFall[b].push_back( tw_cfdFall );

          TimeWindow tw_toa( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_toa_filler );
          this->ssrl_toa[b].push_back( tw_toa );

          TimeWindow tw_pmax( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_window_pmax_filler );
          this->ssrl_window_pmax[b].push_back( tw_pmax );

          TimeWindow tw_tmax( slot_num(this->ssrl_tmax[b].at(i) ), ssrl_window_tmax_filler );
          this->ssrl_window_tmax[b].push_back( tw_tmax );
        }
      }

      //==============================================================================================

      for( unsigned int i = 0; i < this->w[b].size(); i++)
      {
        this->w[b][i] = -1.0 * this->w[b][i];
      }
    }

    //======================================================================================
    //std::cout << "here slac  x" << this->xposReader->GetSize() << std::endl;
    //std::cout << "here slac  y" << this->yposReader->GetSize() << std::endl;
    //===for SLAC raw x y data===
    ///*
    if( this->SLAC_data_switch )
    {
      for( std::size_t i_pos = 0, max = this->xposReader->GetSize(); i_pos < max; i_pos++ )
      {
        //std::cout << "here slacss " << i_pos  <<std::endl;
        this->xpos.push_back( this->xposReader->At( i_pos ) );
        this->ypos.push_back( this->yposReader->At( i_pos ) );
      }
      /*
      uint64_t scope_fill;
      uint64_t eudaq_fill;
      try{ scope_fill = this->scope_time_reader->At(0); }
      catch(const std::out_of_range &e){std::cout << "filling scope time fail\n";scope_fill = 101;}
      try{ eudaq_fill = this->eudaq_time_reader->At(0); }
      catch(const std::out_of_range &e){std::cout << "filling eudaq time fail\n";eudaq_fill = 101;}
      this->scope_time.push_back( scope_fill );
      this->eudaq_time.push_back( eudaq_fill );
      */
    }
    //*/
    //==========================

    //===SSRL AC position filling==========
    if( this->ssrl_AC_position )
    {
      this->ssrl_pos_x.push_back( this->SSRL_XposReader->At(0) );
      this->ssrl_pos_y.push_back( this->SSRL_YposReader->At(0) );
    }
    //======================================

    //=====saving timestamp=================
    if( this->saveTimestamp )
    {
      this->i_scope_time = **this->f_scope_time;
    }
    //======================================

    /*==========================================================================
    Saving the timestamp and current from the Beta-Scope DAQ.
    ==========================================================================*/
    if( this->enableCurrent ) this->currentFiller = **this->currentReader;
    if( this->enableScopeTimestamp )
    {
      if(!use_timestamp_v) this->timestampFiller = **this->scopeTimestampReader;
      else
      {
        //std::cout<<"hellow there!\n";
        //std::cout << this->timestamp_v_reader->At(0) << std::endl;
        /*if(fill == 0)
        {
          for(std::size_t why = 0, max = this->timestamp_v_reader->GetSize(); why < max; why++)
          {
            this->omg_time_why.push_back(this->timestamp_v_reader->At(why));
          }
        }
        if(fill < numEvent ) this->timestamp_filler = this->omg_time_why.at(fill); //omg why store everything into single entry.
        */
        this->timestampFiller = this->timestamp_v_reader->At(0);
      }
    }

    //======================================================================================

    //TThread::Lock();
    this->oTree->Fill();
    betaAnalysis::ClearBuffer();
    //TThread::UnLock();

    if ( fill % 300 == 0)
    {
      TThread::Lock();
      /*
      time_t now = time(0);
      tm *ltm = localtime(&now);
      auto year = ltm->tm_year;
      auto mon = ltm->tm_mon;
      auto day = ltm->tm_mday;
      auto hour = ltm->tm_hour;
      auto min = ltm->tm_min;
      auto sec = ltm->tm_sec;
      std::cout<< "["<< 1960+year << "/" << 1+mon << "/" << day << "/ " << 1+hour<<":"<<1+min<<":"<<1+sec<<" ] ";
      */
      auto now = std::chrono::system_clock::now();
      std::time_t current_time = std::chrono::system_clock::to_time_t(now);
      std::string ct(std::ctime(&current_time));
      std::string date = ct.substr(0, ct.length()-1);

      if( this->parallelProcessing )
      {
        std::cout << "["<< date << "]" << this->ifileName <<" Finished = " << fill << std::endl;
      }
      else
      {
        std::cout << "["<< date << "]" << " Finished = " << fill << std::endl;
      }
      TThread::UnLock();
    }

    fill ++;
  }
}
