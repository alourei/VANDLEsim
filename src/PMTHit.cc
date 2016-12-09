// $Id: PMTHit.cc 12.09.2016 A. Fijalkowska $
//
/// \file PMTHit.cc
/// \brief Implementation of the PMTHit class
//
//
#include "PMTHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "Exception.hh"

G4ThreadLocal G4Allocator<PMTHit>* PMTHitAllocator=0;


PMTHit::PMTHit(G4int moduleIndexVal, G4int pmtIndexVal, 
               G4double energyDepVal, G4double hitTimeVal):
               moduleIndex(moduleIndexVal), pmtIndex(pmtIndexVal)
{
	energyDep.push_back(energyDepVal);	
	time.push_back(hitTimeVal);
	hitsNr = 1;
}                   


PMTHit::~PMTHit() {}


PMTHit::PMTHit(const PMTHit &right) : G4VHit()
{ 
   hitsNr = right.hitsNr;
   pmtIndex = right.pmtIndex;
   moduleIndex = right.moduleIndex;
   drawIt = right.drawIt;
   time = right.time;
   energyDep = right.energyDep;
}


const PMTHit& PMTHit::operator=(const PMTHit &right){
   hitsNr = right.hitsNr;
   pmtIndex = right.pmtIndex;
   moduleIndex = right.moduleIndex;
   drawIt = right.drawIt;
   time = right.time;
   energyDep = right.energyDep;
   return *this;
}


G4int PMTHit::operator==(const PMTHit &right) const{
  return 0;
}


void PMTHit::Draw()
{
	/*
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if( pVVisManager && drawIt)
  {
      G4VisAttributes attribs(G4Colour(1.,0.,0.));
      attribs.SetForceSolid(true);
      G4RotationMatrix rot;
      if(fPhysVol->GetRotation())
        rot=*(fPhysVol->GetRotation());
      G4Transform3D trans(rot,fPhysVol->GetTranslation());
      pVVisManager->Draw(*logVol,attribs,trans);
  }*/
}


void PMTHit::Print() {}

G4double PMTHit::GetTime(G4int hitNr)
{
	if(hitNr >= hitsNr)
	{
		//TODO - message
		throw new InvalidArgumentException ( "PMTHit::GetTime(G4int hitNr)" );
	}
	return time.at(hitNr);
}

G4double PMTHit::GetEnergyDeposit(G4int hitNr)
{
	if(hitNr >= hitsNr)
	{
		//TODO - message
		throw new InvalidArgumentException ( "PMTHit::GetEnergyDeposit(G4int hitNr)" );
	}
	return energyDep.at(hitNr);
	
}

	
void PMTHit::AddHit(G4double hitEnergyDep, G4double hitTime)
{
	energyDep.push_back(hitEnergyDep);
	time.push_back(hitTime);
	hitsNr ++;	
}
