#include "../include/betaAnalysis.h"

void betaAnalysis::ClearBuffer()
{
	for( std::size_t clean = 0, max = this->channel.size(); clean < max; clean++)
	{
		if( this->channel.at(clean) )
		{
			this->w[clean].clear();

			this->t[clean].clear();

			this->wRaw[clean].clear();

			this->pmax[clean].clear();

			this->negPmax[clean].clear();

			this->tmax[clean].clear();

			this->negTmax[clean].clear();

			this->multiplePmax[clean].clear();

			this->multipleTmax[clean].clear();

			this->cfd[clean].clear();

			this->cfdFall[clean].clear();

			this->noise[clean].clear();

			this->pulseArea[clean].clear();

			this->riseTime[clean].clear();

			this->dvdt[clean].clear();

			this->thTime[clean].clear();

			this->fineCFDRise[clean].clear();

			this->singlePeak[clean].clear();

			this->timeAtThreshold[clean].clear();

			this->noiseAfterSignal[clean].clear();

			this->frontBaselineInt[clean].clear();

			this->backBaselineInt[clean].clear();

			this->frontBaselineInt_indepBaseCorr[clean].clear();
			this->backBaselineInt_indepBaseCorr[clean].clear();

			if( this->ssrl_switch )
			{
				this->ssrl_pmax[clean].clear();
				this->ssrl_tmax[clean].clear();
				this->ssrl_pmax_index[clean].clear();
				this->ssrl_num_bunch[clean].clear();
				this->ssrl_riseTime[clean].clear();
				this->ssrl_pulseArea[clean].clear();
				this->ssrl_window_pmax[clean].clear();
				this->ssrl_window_tmax[clean].clear();
				this->ssrl_rms[clean].clear();
				this->ssrl_dvdt[clean].clear();
				this->ssrl_cfd[clean].clear();
				this->ssrl_cfdFall[clean].clear();
				this->ssrl_toa[clean].clear();
			}
		}
	}

	//===for SLAC raw x y data===
	///*
	if( this->SLAC_data_switch )
	{
		this->xpos.clear();
		this->ypos.clear();
		//this->scope_time.clear();
		//this->eudaq_time.clear();
	}
	//*/
	//===========================

	if( this->ssrl_AC_position )
	{
		this->ssrl_pos_x.clear();
		this->ssrl_pos_y.clear();
	}

}
