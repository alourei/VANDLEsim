/* 
 * File:   Gamma.cpp
 * Author: aleksandra
 * 
 * Created on 5 maj 2012, 16:43
 */

#include "Gamma.hh"
#include "Decay.hh"
#include "Exception.hh"
#include "globals.hh"
#include "G4AtomicShells.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

Gamma::Gamma(int atomicNumber, double energy, double gammaIntensity,
double cumulatedElectronConversionCoefficient, Level* finalLevel):
atomicNumber_(atomicNumber), energy_(energy), gammaIntensity_(gammaIntensity),
	cumulatedElectronConversionCoefficient_(cumulatedElectronConversionCoefficient), finalLevel_(finalLevel)
{
	atomicTransitionManager_ = G4AtomicTransitionManager::Instance();
	InitializeShellNumbers();
}


Gamma::~Gamma()
{
}

/*
	numbers are taken from geant documentation
	K - shell nr = 0
	L - shell nr = 1
	M - shell nr = 4
	N - shell nr = 9
*/
void Gamma::InitializeShellNumbers() 
{
	shellNumbers_[0] = 0;
	shellNumbers_[1] = 1;
	shellNumbers_[2] = 4;
	shellNumbers_[3] = 9;
}

std::vector<Event> Gamma::FindGammaEvents()
{
	std::vector<Event> gammaDecay;
	if(IsGammaDecay())
	{
		AddGammaEvent(gammaDecay, energy_);
		return gammaDecay;		
	}
	else //IC
	{
		FindICEvent(gammaDecay);
	}
	return gammaDecay;
}

void Gamma::FindICEvent(std::vector<Event> &gammaDecay)
{
	int primaryShellIndex = FindPrimaryShellIndex();
	int primaryVacancies = shellNumbers_[primaryShellIndex];
	if(!IsRadiativeTransitionReachableShell(primaryVacancies))
		if(!IsAugerReachableShell(primaryVacancies))
			return FindICEvent(gammaDecay);

	if(IsRadiativeTransitionReachableShell(primaryVacancies))
	{
		if(IsAugerReachableShell(primaryVacancies))
		{
			double totalRadTransitProb = atomicTransitionManager_->
				TotalRadiativeTransitionProbability(atomicNumber_, primaryVacancies);
			if(G4UniformRand()<totalRadTransitProb)
				AddXRaysEvent(gammaDecay, primaryVacancies);
			else
				AddAugerEvent(gammaDecay, primaryVacancies);	
		}
		else
		{
			AddXRaysEvent(gammaDecay, primaryVacancies);
		}
	}
	else //if(IsAugerReachableShell(primaryVacancies))
	{
		AddAugerEvent(gammaDecay, primaryVacancies);
		cout << "!(IsRadiativeTransitionReachableShell(primaryVacancies))"  << endl;
	}

	double primaryBindingEnergy = 
		G4AtomicShells::GetBindingEnergy(atomicNumber_, primaryVacancies) / keV;
	AddElectronEvent(gammaDecay, energy_-primaryBindingEnergy);
}

int Gamma::FindPrimaryShellIndex()
{	
	double randomNumber = G4UniformRand()* shellElectonConvCoeff_[3];
	if(randomNumber<=shellElectonConvCoeff_[0])//K - shell nr = 0
			return 0;
	for(int i=1; i<nuberOfShellIndexes_; i++)
	{
		if(randomNumber > shellElectonConvCoeff_[i-1] && randomNumber <= shellElectonConvCoeff_[i])
			return i;
	}		
}

bool Gamma::IsRadiativeTransitionReachableShell(int shellIndex)
{
	int numverOfRadiativeTransReachableShells = 
		atomicTransitionManager_->NumberOfReachableShells(atomicNumber_);
	if(shellIndex > numverOfRadiativeTransReachableShells)
		return false;
	else
		return true;
}

bool Gamma::IsAugerReachableShell(int shellIndex)
{
	int numverOfAugerReachableShells = 
		atomicTransitionManager_->NumberOfReachableAugerShells(atomicNumber_);
	if(shellIndex > numverOfAugerReachableShells)
		return false;
	else
		return true;
}

void Gamma::AddGammaEvent(std::vector<Event> &decay, double energy)
{
	decay.push_back(Event(energy, 0., G4ParticleTable::GetParticleTable()->FindParticle("gamma")));
}

void Gamma::AddElectronEvent(std::vector<Event> &decay, double energy)
{
	decay.push_back(Event(energy, 0., G4ParticleTable::GetParticleTable()->FindParticle("e-")));
}

void Gamma::AddXRaysEvent(std::vector<Event> &decay, int primaryVacancies)
{
	const G4FluoTransition* fluoTrans = atomicTransitionManager_->
		ReachableShell(atomicNumber_, primaryVacancies);
	const G4DataVector transEnergies = fluoTrans -> TransitionEnergies();
	const G4DataVector transProb = fluoTrans -> TransitionProbabilities();
	int xRayIndex = FindRandomIndex(transProb);
	AddGammaEvent(decay, transEnergies.at(xRayIndex)/keV);
}

void Gamma::AddAugerEvent(std::vector<Event> &decay, int primaryVacancies)
{
	const G4AugerTransition * augerTrans = atomicTransitionManager_->
		ReachableAugerShell(atomicNumber_, primaryVacancies);

	//shell from where electron can fill the vacancy
	const vector<G4int> augerPossibilities = *(augerTrans->TransitionOriginatingShellIds());
	G4DataVector totAugerTransFromShell;
	for(unsigned int i=0; i<augerPossibilities.size(); i++)
	{
		const G4DataVector augerTransProb =
			 *(augerTrans -> AugerTransitionProbabilities(augerPossibilities.at(i)));
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
	AddElectronEvent(decay, augerTransEnergy.at(augerIndex)/keV);
}

void Gamma::SetShellElectronConvCoef(std::string type, double value)
{
    CatSpaces(type);
    if(type == "KC" || type == "KC+" )
			for(int i=0; i<nuberOfShellIndexes_; i++)
	    	shellElectonConvCoeff_[i] +=value;
    else if (type == "LC" || type == "LC+" )
			for(int i=1; i<nuberOfShellIndexes_; i++)
	    	shellElectonConvCoeff_[i] +=value;
    else if (type == "MC" || type == "MC+" )
			for(int i=2; i<nuberOfShellIndexes_; i++)
	    	shellElectonConvCoeff_[i] +=value;
    else
			shellElectonConvCoeff_[3] +=value;
}

void Gamma::CatSpaces(std::string &word)
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


bool Gamma::IsGammaDecay()
{
    double randomNumber = (double)rand()/(double)RAND_MAX * gammaIntensity_
    *(1+cumulatedElectronConversionCoefficient_);
    if(randomNumber > gammaIntensity_*cumulatedElectronConversionCoefficient_ )
		return true;
    return false;
	
}

int Gamma::FindRandomIndex( const G4DataVector transProb)
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



