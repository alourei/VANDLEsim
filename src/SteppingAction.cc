//
// $Id: SteppingAction.cc 12.07.2016 A. Fijalkowska $
//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
#include "SteppingAction.hh"
#include "PMTSD.hh"
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
#include <string>
#include <iostream>

SteppingAction::SteppingAction()
{
  expectedNextStatus = Undefined;
}

SteppingAction::~SteppingAction() {}



void SteppingAction::UserSteppingAction(const G4Step* theStep)
{
	
	//UserTrackInformation* trackInformation
    //=(UserTrackInformation*)theTrack->GetUserInformation(); 
  //PrintStep(theStep);  

  G4Track* theTrack = theStep->GetTrack();
  if ( theTrack->GetCurrentStepNumber() == 1 ) expectedNextStatus = Undefined;

  
  //Quit track out of world - TODO method??
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  if(!thePostPV)
  {
    expectedNextStatus=Undefined;
    return;
  }




  G4OpBoundaryProcessStatus boundaryStatus=Undefined;	
  static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;

  //find the boundary process only once
  if(!boundary)
  {
    G4ProcessManager* pm
      = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }
  
  

  //DrawOpPhotonsTrajectoryInVandleBar(theStep, trackInf);
  //draw op photons trajectory(step, trackInf);
  //kill op photons out of vandle module
  //setObsorptionStatus
  // check ..

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition())
  {
/*
    if(thePrePV->GetName()=="VANDLEbar")
      //force drawing of photons in WLS slab
      trackInformation->SetForceDrawTrajectory(true);
    else if(thePostPV->GetName()=="expHall")
      //Kill photons entering expHall from something other than Slab
      theTrack->SetTrackStatus(fStopAndKill);

    //Was the photon absorbed by the absorption process
    if(thePostPoint->GetProcessDefinedStep()->GetProcessName()
       =="OpAbsorption"){
      eventInformation->IncAbsorption();
      trackInformation->AddTrackStatusFlag(absorbed);
    }*/

    boundaryStatus=boundary->GetStatus();


    if(thePostPoint->GetStepStatus()==fGeomBoundary)
    {
		
      if(expectedNextStatus==StepTooSmall)
      {
        if(boundaryStatus!=StepTooSmall)
        {
          G4ExceptionDescription ed;
          ed << "LXeSteppingAction::UserSteppingAction(): "
                << "No reallocation step after reflection!"
                << G4endl;
          G4Exception("SteppingAction::UserSteppingAction()", "VANDLEProj",
          FatalException,ed,
          "Something is wrong with the surface normal or geometry");
        }
      }
      expectedNextStatus=Undefined;
      if (boundaryStatus == Detection) 
      {
		 G4SDManager* SDman = G4SDManager::GetSDMpointer();
         G4String sdName="/VANDLEDet/pmtSD";
         PMTSD* pmtSD = (PMTSD*)SDman->FindSensitiveDetector(sdName);
         if(pmtSD)pmtSD->ProcessHitsConstStep(theStep,NULL);
		  
	  }
    }
  }
  //if(fRecorder)fRecorder->RecordStep(theStep);
  
  

}


/* This method doesn't work
G4ThreadLocal G4OpBoundaryProcess* SteppingAction::FindBoundaryProcess(const G4Step* theStep)
{
   static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;

  //find the boundary process only once
  if(!boundary)
  {
    G4ProcessManager* pm
      = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }
  return boundary;	
}*/

void SteppingAction::PrintStep(const G4Step* theStep)
{
   G4Track* theTrack = theStep->GetTrack();
   G4String processName =theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
   G4String particleName = theTrack->GetDefinition()->GetParticleName();  
   G4VPhysicalVolume* volume = theTrack->GetVolume();    
   G4String volumeName = volume->GetName();
   G4double deltaEn = theStep->GetDeltaEnergy();
  
   //if(strcmp(volumeName, "VANDLEbar") == 0 )
   if(strcmp(particleName, "neutron") == 0 )
   {
	 std::cout << " particle: " << particleName
		       << " process: " << processName
		       << " delta en: " << deltaEn
		       << " voulme name: " << volumeName << std::endl;
    }
}
