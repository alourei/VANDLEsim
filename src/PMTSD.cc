//
// $Id: PMTSD.cc 12.08.2016 A. Fijalkowska$
//
/// \file PMTSD.cc
/// \brief Implementation of the pmt sensitive detector class
//
//
#include "PMTSD.hh"
#include "PMTHit.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"


PMTSD::PMTSD(G4String name)
  : G4VSensitiveDetector(name), moduleDeph(0), pmtDeph(0)
{
  collectionName.insert("pmtHitCollection");
}


PMTSD::~PMTSD() {}


void PMTSD::Initialize(G4HCofThisEvent* hitsCE)
{
  pmtHitCollection = new PMTHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  static G4int hitCID = -1;
  if(hitCID<0){
    hitCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitCID, pmtHitCollection);
}

G4bool PMTSD::ProcessHits(G4Step* aStep, G4TouchableHistory* )
{
	return false;
}

G4bool PMTSD::ProcessHitsConstStep(const G4Step* aStep, G4TouchableHistory* )
{  
  if(aStep->GetTrack()->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
 
  G4int pmtIndex = GetIndex(aStep, pmtDeph);
  G4int moduleIndex = GetIndex(aStep, moduleDeph);

  G4double hitTime = GetHitTime(aStep);
  G4double energyDep = GetEnergyDeposit(aStep);
  G4LogicalVolume* hitLogVol = GetHitLogVol(aStep);

  G4int hitsNr = pmtHitCollection->entries();
  G4bool pmtAlreadyHit = false;
  for(G4int i=0; i<hitsNr; i++)
  {
	  G4int hitsPMTIndex = (*pmtHitCollection)[i]->GetPMTIndex();
	  G4int hitsModuleIndex = (*pmtHitCollection)[i]->GetModuleIndex();
      if(pmtIndex == hitsPMTIndex && moduleIndex == hitsModuleIndex)
      {
         (*pmtHitCollection)[i]->AddHit(energyDep, hitTime); 
         pmtAlreadyHit = true;		  
         break;
      }
  }
  
  if(!pmtAlreadyHit)
  {
	  	PMTHit* aHit = new PMTHit(moduleIndex, pmtIndex, energyDep, hitTime);
	  	aHit->SetLogVol(hitLogVol);
		pmtHitCollection->insert( aHit );	  
  }

  return true;
}

G4int PMTSD::GetIndex(const G4Step* aStep, int deph)
{
	G4int voulmeNr =
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(deph);
	return voulmeNr;
}

G4double PMTSD::GetEnergyDeposit(const G4Step* aStep)
{
	return aStep->GetTotalEnergyDeposit();	
}

G4double PMTSD::GetHitTime(const G4Step* aStep)
{
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    return postStepPoint->GetGlobalTime();
}

G4LogicalVolume* PMTSD::GetHitLogVol(const G4Step* aStep)
{
	G4LogicalVolume* hitLogVol =
    aStep->GetPostStepPoint()->GetTouchable()->GetVolume()->GetLogicalVolume();
    return hitLogVol;
	
}
    
void PMTSD::EndOfEvent(G4HCofThisEvent* ) {}

void PMTSD::clear() {}

