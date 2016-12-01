/* 
 * File:   Level.cpp
 * Author: aleksandra
 * 
 * Created on 5 maj 2012, 16:44
 */

#include "Level.hh"
#include "Beta.hh"
#include "Gamma.hh"
#include "Exception.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Level::Level(double energy, double betaFeedingFunc, std::vector<Gamma*> &gammaEmitedFromLevel, int atomicNumber):
energy_(energy), betaFeedingFunction_(betaFeedingFunc), gammaEmitedFromLevel_(gammaEmitedFromLevel), atomicNumber_(atomicNumber)
{
	beta_ = 0L;
}

Level::Level(double energy, int atomicNumber): energy_(energy), atomicNumber_(atomicNumber)
{
	beta_ = 0L;
	betaFeedingFunction_ = 0;
}


Level::~Level()
{
	for(std::vector<Gamma*>::iterator it =  gammaEmitedFromLevel_.begin(); it != gammaEmitedFromLevel_.end(); ++it)
		delete (*it);
		
	if (beta_ != 0L)
		delete beta_;
		
}

void Level::SetHalfLifeTime(std::string T12)
{
    std::stringstream iss(T12);
    double halfLifeTimeValue = 0.0;
    std::string halfLifeTimeUnit;
    iss >> halfLifeTimeValue;
    if (halfLifeTimeValue == 0.0)
    {
			halfLifeTimeInSeconds_ = 0.;
    }
    else
    {
			iss >> halfLifeTimeUnit;

			if(halfLifeTimeUnit == "F")
			{
	   		 halfLifeTimeInSeconds_ = halfLifeTimeValue*pow(10., -9.);
			}
			else if(halfLifeTimeUnit == "MS")
			{
					halfLifeTimeInSeconds_ = halfLifeTimeValue * 0.001;
			}
			else if(halfLifeTimeUnit == "US")
			{
					halfLifeTimeInSeconds_ = halfLifeTimeValue*pow(10., -6.);
			}
			else if(halfLifeTimeUnit == "S")
			{
			    halfLifeTimeInSeconds_ = halfLifeTimeValue;
			}
			else if(halfLifeTimeUnit == "M")
			{
				halfLifeTimeInSeconds_ = halfLifeTimeValue * 60.;
			}
			else if (halfLifeTimeUnit == "H")
			{
	   	 halfLifeTimeInSeconds_ = halfLifeTimeValue * 3600.;
			}
			else if (halfLifeTimeUnit == "Y")
			{
	   		 halfLifeTimeInSeconds_ = halfLifeTimeValue * 31556926.;
			}
			else
			{
				halfLifeTimeInSeconds_ = 0;
		//throw Exception("Parse faild: unknown time unit", Exception::ERROR_UNKNOWN_TIME_UNIT);
			}
    }
}


void Level::AddGamma(Gamma* gamma)
{
    gammaEmitedFromLevel_.push_back(gamma);
    if (sumOfGammaIntensity_.empty())
			sumOfGammaIntensity_.push_back(gamma->GetTotalIntensity());
    else
			sumOfGammaIntensity_.push_back(gamma->GetTotalIntensity()+ (*(sumOfGammaIntensity_.end() - 1)));

}
/**Check if beta emission fro isomer state, not implemented yet**/
bool Level::IsBetaDecayFromLevel()
{
	//double randomIntensity = (double)rand()/(double)RAND_MAX *100;
	//if(ran)
	return false;
}

Gamma* Level::RoundGammaEmitedFromLevel()
{
	if(sumOfGammaIntensity_.empty())
		return 0L;
  double randomIntensity = (double)rand()/(double)RAND_MAX * (*(sumOfGammaIntensity_.end()-1));
  if(randomIntensity < (*sumOfGammaIntensity_.begin()))
		return (*gammaEmitedFromLevel_.begin());
		
  for( unsigned int i=1; i<sumOfGammaIntensity_.size(); ++i )
		if(randomIntensity >= sumOfGammaIntensity_.at(i-1) && randomIntensity < sumOfGammaIntensity_.at(i))
	    return gammaEmitedFromLevel_.at(i);
	
}
double Level::RoundTimeOfDecay()//in sec
{
    double randomNumber = (double)rand()/(double)RAND_MAX;
    return -log(1-randomNumber)*halfLifeTimeInSeconds_/log(2);
}


void Level::SetBetaMinus()
{

	if(maxBetaEnergy_ -0. <1e-10)
		throw Exception("Level::SetBetaMinus() Set qVal before");
	if(beta_ != 0L)
	{
		throw Exception("Level::SetBetaMinus() Decay type was already setted");
	}
		
	beta_ = new Beta(maxBetaEnergy_, atomicNumber_, -1);
}

void Level::SetBetaPlus(double bPlInt, double ecInt)
{
	if(maxBetaEnergy_ -0. <1e-10)
		throw Exception("Level::SetBetaPlus() Set qVal before");
	if(beta_ != 0L)
		throw Exception("Level::SetBetaPlus() Decay type was already setted");
		
	beta_ = new Beta(maxBetaEnergy_-1022., atomicNumber_, 1);		
	beta_ ->SetBetaPlusECInt(bPlInt, ecInt);
}


bool Level::IsIsomerLevel()
{
	/**Time limit for isomer state, if time of life is higher than the limit
	level is treated as an isomer state**/
	const double timeLimitInSec = 2.;//What is good value?
	if (halfLifeTimeInSeconds_ > timeLimitInSec)
		return true;
	else
		return false;
}
