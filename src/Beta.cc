/* 
 * File:   Beta.cpp
 * Author: aleksandra
 * 
 * Created on 5 maj 2012, 16:44
 */

#include "Beta.hh"
#include "Decay.hh"
#include "Exception.hh"
#include "Randomize.hh"
#include "G4AtomicShells.hh"
#include "G4AtomicTransitionManager.hh"
#include "G4AugerTransition.hh"
#include "G4FluoTransition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Beta::Beta(double maxEnergy, int atomicNumber, int eCharge):
maxBetaEnergy_(maxEnergy), atomicNumber_(atomicNumber), eCharge_(eCharge)
{
	betaEnergyDistribution_ = 0L;
	if(maxEnergy > 0)
	{
		if(eCharge_ == -1 || eCharge_ == 1)
    	betaEnergyDistribution_ = new FermiDistribution(atomicNumber_, maxBetaEnergy_, eCharge_);
  	else
  		throw Exception("Beta::Beta(double maxEnergy, int atomicNumber, int eCharge) Wrong eCharge value");
  }
  atomicTransitionManager_ = G4AtomicTransitionManager::Instance();
	ifECCoef_ = false;
}

Beta::~Beta()
{

	if(betaEnergyDistribution_ != 0L)
		delete betaEnergyDistribution_;
	//betyEn_.close();

}

std::vector<Event> Beta::FindBetaEvents()
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	std::vector<Event> betaDecay;
	if(eCharge_ < 0)
	{
		double randomBetaEnergy = betaEnergyDistribution_->GetRandomBetaEnergy();//beta minus
		betaDecay.push_back(Event(randomBetaEnergy , 0., particleTable->FindParticle("e-")));
	}
	else 
	{
			double randomNumber = G4UniformRand() *(betaPlusInt_ + ECInt_);
			if(randomNumber<betaPlusInt_)//beta plus
			{
				betaDecay.push_back(Event(betaEnergyDistribution_->GetRandomBetaEnergy(), 0., particleTable->FindParticle("e+")));
			}
			else //EC
			{
				int primaryVacancies;
				if(!ifECCoef_)
				{
					primaryVacancies =0;
				}
				else 	
					primaryVacancies = FindPrimaryVacancies();
				//Auger or X-rays
				G4AtomicTransitionManager* atomicTransitionManager = G4AtomicTransitionManager::Instance();
				double totalRadTransitProb = atomicTransitionManager->TotalRadiativeTransitionProbability(atomicNumber_, primaryVacancies);
				randomNumber=G4UniformRand();
				if(randomNumber<totalRadTransitProb)//x rays	
				{
					const G4FluoTransition* fluoTrans = atomicTransitionManager->ReachableShell(atomicNumber_, primaryVacancies);
					const G4DataVector transEnergies = fluoTrans -> TransitionEnergies();
					const G4DataVector transProb = fluoTrans -> TransitionProbabilities();
					int xRayIndex = FindRandomIndex(transProb);
					betaDecay.push_back(Event(transEnergies.at(xRayIndex), 0., particleTable->FindParticle("gamma")));
				}
				else//non rad - Auger
				{
					const G4AugerTransition * augerTrans = atomicTransitionManager->ReachableAugerShell(atomicNumber_, primaryVacancies);
					//shell from where electron can fill the vacancy
					const vector<G4int> augerPossibilities = *(augerTrans->TransitionOriginatingShellIds());
					G4DataVector totAugerTransFromShell;
					for(unsigned int i=0; i<augerPossibilities.size(); i++)
					{
						const G4DataVector augerTransProb = *(augerTrans -> AugerTransitionProbabilities(augerPossibilities.at(i)));
						double sumProbAugerTrans(0.);
						for(unsigned int j=0; j<augerTransProb.size(); j++)
							sumProbAugerTrans += augerTransProb.at(j);
						totAugerTransFromShell.push_back(sumProbAugerTrans);
					}
					int index = FindRandomIndex(totAugerTransFromShell);
					//electron from augerPossibilities.at(index) will fill the vacancy
					int electronShellId = augerPossibilities.at(index);
					const G4DataVector augerTransProb = *(augerTrans -> AugerTransitionProbabilities(electronShellId));
					const G4DataVector augerTransEnergy =  *(augerTrans -> AugerTransitionEnergies(electronShellId));
					int augerIndex = FindRandomIndex(augerTransProb);
					betaDecay.push_back(Event(augerTransEnergy.at(augerIndex), 0., particleTable->FindParticle("e-")));	
				}
			}
	}
	return betaDecay;
}

void Beta::SetECCoef(std::string type, double value)
{
   CatSpaces(type);
   if(type == "CK" || type == "CK+" )
		for(int i=0; i<4; i++)
	    ECCoeff_[i] +=value;
   else if (type == "CL" || type == "CL+" )
		for(int i=1; i<4; i++)
	    ECCoeff_[i] +=value;
   else if (type == "CM" || type == "CM+" )
		for(int i=2; i<4; i++)
	    ECCoeff_[i] +=value;
   else
		ECCoeff_[3] +=value;
		
		ifECCoef_ = true;
}


void Beta::CatSpaces(std::string &word)
{
   size_t spaceIndex;
   while (true)
   {
   spaceIndex = word.find(" ");
   if (spaceIndex != std::string::npos)
   {
       word.erase(int(spaceIndex), 1);
   }
   else
       break;
   }
}

int Beta::FindPrimaryVacancies()//k - index of gamma
{
	const unsigned int nrOfShells = atomicTransitionManager_-> NumberOfShells(atomicNumber_);
	double randomNumber = G4UniformRand()*ECCoeff_[3];
	if(randomNumber<=ECCoeff_[0])//K - shell nr = 0
		return 0;
		
	else if(randomNumber>ECCoeff_[0] && randomNumber<=ECCoeff_[1])// L - shell 1
		if(1>nrOfShells)
			return nrOfShells;
		else
			return 1;
		
	else if(randomNumber>ECCoeff_[1] && randomNumber<=ECCoeff_[2])// M - shell 4
		if(4>nrOfShells)
			return nrOfShells;
		else
			return 4;	
		
	else if(randomNumber>ECCoeff_[2] && randomNumber<=ECCoeff_[3])// N - shell 9
		if(9>nrOfShells)
			return nrOfShells;
		else
			return 9;
			
	else
	{
		cout<<"error - wrong finding X Rays energy!!!"<<endl;
		return 0;
	}
}

		

int Beta::FindRandomIndex( const G4DataVector transProb)
{
	double sumProb (0.);
	G4DataVector transProbSum;
	for (unsigned int i=0; i<transProb.size(); i++)
	{
		sumProb += transProb.at(i);
		transProbSum.push_back(sumProb);
	}
		
	double randomNumber = G4UniformRand()*sumProb;
	int index(-1);
	for (unsigned int i=0; i<transProb.size(); i++)
	{
		if(randomNumber < transProbSum.at(i))
		{
			index = i;
			break;
		}
	}
	return index;
}
