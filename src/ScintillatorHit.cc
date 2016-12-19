// $Id: ScintillatorHit.cc 12.09.2016 A. Fijalkowska $
//
/// \file ScintillatorHit.cc
/// \brief Implementation of the ScintillatorHit class
//
//
#include "ScintillatorHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "Exception.hh"

G4ThreadLocal G4Allocator<ScintillatorHit>* ScintillatorHitAllocator=0;


ScintillatorHit::ScintillatorHit(G4int moduleId, G4double enDep, 
                                 G4double timeVal, G4ThreeVector &position):
                                 moduleIndex(moduleId)
{
	energyDep.push_back(enDep);	
	time.push_back(timeVal);
	xPos.push_back(position.x());
	yPos.push_back(position.y());
	zPos.push_back(position.z());
	nrOfInteract = 1;
}                   


ScintillatorHit::~ScintillatorHit() {}


ScintillatorHit::ScintillatorHit(const ScintillatorHit &right) : G4VHit()
{ 
   nrOfInteract = right.nrOfInteract;
   moduleIndex = right.moduleIndex;
   energyDep = right.energyDep;
   time = right.time;
   xPos = right.xPos;
   yPos = right.yPos;
   zPos = right.zPos;
}

    
    
const ScintillatorHit& ScintillatorHit::operator=(const ScintillatorHit &right)
{
   nrOfInteract = right.nrOfInteract;
   moduleIndex = right.moduleIndex;
   energyDep = right.energyDep;
   time = right.time;
   xPos = right.xPos;
   yPos = right.yPos;
   zPos = right.zPos;
   return *this;
}


G4int ScintillatorHit::operator==(const ScintillatorHit &right) const{
  return 0;
}


void ScintillatorHit::Draw()
{
}


void ScintillatorHit::Print() {}

G4double ScintillatorHit::GetTime(G4int hitNr)
{
	if(hitNr >= nrOfInteract)
	{
		//TODO - message
		throw new InvalidArgumentException ( "ScintillatorHit::GetTime(G4int hitNr)" );
	}
	return time.at(hitNr);
}

G4double ScintillatorHit::GetEnergyDeposit(G4int hitNr)
{
	if(hitNr >= nrOfInteract)
	{
		//TODO - message
		throw new InvalidArgumentException ( "ScintillatorHit::GetEnergyDeposit(G4int hitNr)" );
	}
	return energyDep.at(hitNr);	
}

G4double ScintillatorHit::GetxPos(G4int hitNr)
{
	if(hitNr >= nrOfInteract)
	{
		throw new InvalidArgumentException ( "ScintillatorHit::GetxPos(G4int hitNr)" );
	}
	return xPos.at(hitNr);	
}
	
	
G4double ScintillatorHit::GetyPos(G4int hitNr)
{
	if(hitNr >= nrOfInteract)
	{
		throw new InvalidArgumentException ( "ScintillatorHit::GetyPos(G4int hitNr)" );
	}
	return yPos.at(hitNr);	
}

G4double ScintillatorHit::GetzPos(G4int hitNr)
{
	if(hitNr >= nrOfInteract)
	{
		throw new InvalidArgumentException ( "ScintillatorHit::GetzPos(G4int hitNr)" );
	}
	return zPos.at(hitNr);	
}
	
	
	
void ScintillatorHit::AddHit(G4double hitEnergyDep, G4double hitTime, 
                             G4ThreeVector &position)
{
	energyDep.push_back(hitEnergyDep);
	time.push_back(hitTime);
	xPos.push_back(position.x());
	yPos.push_back(position.y());
	zPos.push_back(position.z());
	nrOfInteract ++;	
}

