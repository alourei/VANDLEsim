//
// $Id: ScintillatorSD.cc 12.08.2016 A. Fijalkowska$
//
/// \file ScintillatorSD.cc
/// \brief Implementation of the pmt sensitive detector class
//
//
#include "ScintillatorSD.hh"
#include "ScintillatorHit.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"


ScintillatorSD::ScintillatorSD(G4String name)
  : G4VSensitiveDetector(name), moduleDeph(0)
{
  collectionName.insert("scintillatorHitCollection");
}


ScintillatorSD::~ScintillatorSD() {}


void ScintillatorSD::Initialize(G4HCofThisEvent* hitsCE)
{
  scintillatorHitCollection = new ScintillatorHitsCollection
                      (SensitiveDetectorName, collectionName[0]);
  static G4int hitCID = -1;
  if(hitCID<0){
    hitCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitCID, scintillatorHitCollection);
}


G4bool ScintillatorSD::ProcessHits(G4Step* aStep, G4TouchableHistory* )
{
  
  	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* neutron = particleTable->FindParticle("neutron");
	if(aStep->GetTrack()->GetDefinition() != neutron)
	  return false;

   //TODO make particle delta EK, not E dep, for neutrons Edep = 0!!!
   G4double energyDep = GetEnergyDeposit(aStep);
   if(energyDep <= 0)
      return false;

  G4int moduleIndex = GetIndex(aStep, moduleDeph);
  G4double hitTime = GetHitTime(aStep);
  G4ThreeVector position = GetPosition(aStep);

  G4int hitsNr = scintillatorHitCollection->entries();
  G4bool moduleAlreadyHit = false;
  for(G4int i=0; i<hitsNr; i++)
  {
	  G4int hitsModuleIndex = (*scintillatorHitCollection)[i]->GetModuleIndex();
      if(moduleIndex == hitsModuleIndex)
      {
         (*scintillatorHitCollection)[i]->AddHit(energyDep, hitTime, position); 
         moduleAlreadyHit = true;		  
         break;
      }
  }
  
  if(!moduleAlreadyHit)
  {
	  	ScintillatorHit* aHit = new ScintillatorHit(moduleIndex, energyDep, hitTime, position);
		scintillatorHitCollection->insert( aHit );	  
  }

  return true;
}

G4int ScintillatorSD::GetIndex(const G4Step* aStep, int deph)
{
	G4int voulmeNr =
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(deph);
	return voulmeNr;
}

G4double ScintillatorSD::GetEnergyDeposit(const G4Step* aStep)
{	
	return -1.* aStep->GetDeltaEnergy();	
}

G4double ScintillatorSD::GetHitTime(const G4Step* aStep)
{
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    return postStepPoint->GetGlobalTime();
}

G4ThreeVector ScintillatorSD::GetPosition(const G4Step* aStep)
{
   G4StepPoint* thePrePoint = aStep->GetPreStepPoint(); 
   G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

   //Get the average position of the hit
   G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
   pos/=2.;
   return pos;	
}
    
void ScintillatorSD::EndOfEvent(G4HCofThisEvent* ) {}

void ScintillatorSD::clear() {}

