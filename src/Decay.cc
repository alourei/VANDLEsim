/* 
 * File:   Decay.cpp
 * Author: aleksandra
 * 
 * Created on 1 maj 2012, 20:16
 */
#include "Decay.hh"
#include "LoadDecayData.hh"
#include "Level.hh"
#include "FermiDistribution.hh"
#include "Gamma.hh"
#include "Exception.hh"
#include "MyTemplates.hh"
#include "PeriodicTable.hh"
#include "G4ParticleTable.hh"


#include "Randomize.hh"
#include <cmath>
#include<iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

Decay::Decay(int atomicMass, int atomicNumber, double qVal, std::vector<Level*> levels, Level* parentLevel) :
	atomicMass_(atomicMass), atomicNumber_(atomicNumber), qVal_(qVal), levels_(levels), parentLevel_(parentLevel)
{
	SetBetaFeedingDistribution();
	nrOfIsLevelsHits = 0;
}


Decay::Decay(const std::string &ensdfFilename)
{
	LoadDecayData* loadDecayData = new LoadDecayData(ensdfFilename);
	atomicMass_ = loadDecayData->GetAtomicMass();
	atomicNumber_ = loadDecayData->GetAtomicNumber();
	qVal_ = loadDecayData->GetQVal();
	levels_ = loadDecayData->GetLevels();
	parentLevel_ = loadDecayData->GetParentLevel();
	SetBetaFeedingDistribution();
	nrOfIsLevelsHits = 0;
}

void Decay::SetBetaFeedingDistribution()
{
	double maxBetaFeeding(0);
	double gammaEnergyPerDecay(0), totalBetaFeedingFunction(0);
	double level0Feeding (0);
	
	for (unsigned int i = 0; i != levels_.size(); ++i)
	{
		maxBetaFeeding += levels_.at(i)->GetBetaFeedingFunction();
		gammaEnergyPerDecay += levels_.at(i)->GetBetaFeedingFunction()*levels_.at(i)->GetEnergy();
		if(levels_.at(i)->GetEnergy() - 0. < 0.01)// searching for level 0
			level0Feeding = levels_.at(i)->GetBetaFeedingFunction();
		totalBetaFeedingFunction_.push_back(maxBetaFeeding);
		cout<<" level "<< levels_.at(i)->GetEnergy() << " feeding " << 
		levels_.at(i)->GetBetaFeedingFunction() << " max " << *(totalBetaFeedingFunction_.end()-1) << endl;
	}
	totalBetaFeedingFunction = *(totalBetaFeedingFunction_.end()-1);
	if(totalBetaFeedingFunction < 1e-5)
		throw Exception("Decay::SetBetaFeedingDistribution() No beta feedings in ensdf file");
	std::cout << "Mean gamma energy per dacay: " << gammaEnergyPerDecay/totalBetaFeedingFunction << std::endl;
	std::cout << "Level 0 feeding: " << level0Feeding/totalBetaFeedingFunction << std::endl;
}

Decay::~Decay()
{

	for(std::vector<Level*>::iterator it = levels_.begin(); it != levels_.end(); ++it)
		delete (*it);
	delete parentLevel_;
	std::cout << " nr of isomer levels hits: " << nrOfIsLevelsHits << endl;
}

Level* Decay::RoundPrimaryLevel()
{
	if (totalBetaFeedingFunction_.empty())
		throw Exception("No beta feedings in ensdf file");

	double randomNumber = G4UniformRand() * totalBetaFeedingFunction_.at(totalBetaFeedingFunction_.size() - 1);
	
	if (randomNumber < totalBetaFeedingFunction_.at(0))
		return levels_.at(0);

	for (unsigned int i = 1; i != totalBetaFeedingFunction_.size(); ++i)
	{
		if (randomNumber >= totalBetaFeedingFunction_.at(i - 1) && randomNumber < totalBetaFeedingFunction_.at(i))
			return levels_.at(i);
	}
}

std::vector<Event>  Decay::Execute()
{
	//if (totalBetaFeedingFunction_.size()>0 && totalBetaFeedingFunction_.at(totalBetaFeedingFunction_.size() - 1) > 1e-10)
		return this->BFeedingExecute();
	//else
	//	return this->noBFeedingExecute();
}

std::vector<Event>  Decay::BFeedingExecute()
{
	std::vector<Event> eventList;
	double currentTime(0), time(0);
	Level* currentLevel;
	Gamma* currentGamma;
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	if(isomerLevel_ == 0L)//there is not "lefted" isomer from previous decay
	{
		currentLevel = RoundPrimaryLevel();
		time = parentLevel_->RoundTimeOfDecay();
		//std::cout<<currentLevel->GetEnergy()<<std::endl;
		vector<Event> betaEvents = currentLevel->FindBetaEvents();//single b-, b+ or EC
			for(unsigned int i=0; i<betaEvents.size(); ++i)
			{
				eventList.push_back(Event(betaEvents.at(i).energy, time, betaEvents.at(i).type));
			}
			currentTime = time;
		if(currentLevel->IsIsomerLevel())//chcek if primary level is isomer
		{
			isomerLevel_ = currentLevel;
			return eventList;
		}
	}
	else //if the previous level ended on isomer level
	{
		currentLevel = isomerLevel_;
		isomerLevel_ = 0L;
	}
	
	while (currentLevel->GetEnergy() > 0.1)// until ground level
	{
		currentGamma = currentLevel->RoundGammaEmitedFromLevel();

		time = currentLevel->RoundTimeOfDecay();
		currentTime += time;	
		if (currentGamma == 0L)//no gamma transitions from level in database - one gamma to ground state
		{
			//total energy in one gamma transition
			eventList.push_back(Event(currentLevel->GetEnergy(), currentTime,  particleTable->FindParticle("gamma")));
			break;
		}
		
		vector<Event> gammaEvents = currentGamma->FindGammaEvents();
		for(unsigned int i=0; i<gammaEvents.size(); ++i)
			eventList.push_back(Event(gammaEvents.at(i).energy, currentTime, gammaEvents.at(i).type));

		currentLevel = currentGamma->GetFinalLevel();
		
		//if( fabs(currentLevel->GetEnergy() - 106.9) < 1)
		//{
		//	nrOfIsLevelsHits ++;
		//}
	
		if (currentLevel->GetEnergy() < 0.1)
			break;
		if(currentLevel->GetT12() > 100)//for 90Kr
		{
			nrOfIsLevelsHits ++;
			break;
		}
		if(currentLevel->IsIsomerLevel())
		{
			isomerLevel_ = currentLevel;
			break;
		}
	}
	return eventList;

}

/*std::vector<Event> Decay::NoBFeedingExecute()
{
//total energy for beta transition = tot en for gamma trans = Q/2
	std::vector<Event> eventList;

	double energy = fermiDistribution_->GetRandomBetaEnergy()* (qVal_/2. );
	double time = parentLevel_->roundTimeOfDecay();
	eventList.push_back(Event(qVal_/2., time, 0, atomicMass_, atomicNumber_));//X
	eventList.push_back(Event(energy, time, 1, atomicMass_, atomicNumber_)); //electron
	return new &eventList;
}*/

Level* Decay::isomerLevel_ = 0L;
