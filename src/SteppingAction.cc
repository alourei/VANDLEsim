//
// $Id: SteppingAction.cc 12.07.2016 A. Fijalkowska $
//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
#include "SteppingAction.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4String.hh"

SteppingAction::SteppingAction()
{

}

SteppingAction::~SteppingAction() {}



void SteppingAction::UserSteppingAction(const G4Step * theStep)
{

  G4Track* theTrack = theStep->GetTrack();

  G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  G4ParticleDefinition* particleType = theTrack->GetDefinition();

  G4String processName =theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  G4String particleName = theTrack->GetDefinition()->GetParticleName();
  
  G4VPhysicalVolume* volume = theTrack -> GetVolume();    
  G4String volumeName = volume -> GetName();
  
  if(strcmp(volumeName, "PMTPhotocathPhysical") == 0 )
  {
	std::cout << " name: " << particleName 
			  << " particle: " << particleName
		      << " process: " << processName
		      << " voulme name: " << volumeName << std::endl;
   }
		

}



