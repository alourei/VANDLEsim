
// $Id: EventAction.cc 11.30.2016 A. Fijalkowska $
//
/// \file EventAction.cc
/// \brief Implementation of the EventAction class
//
//
#include "EventAction.hh"
#include "PMTHit.hh"
#include "ScintillatorHit.hh"
#include "AnalysisManager.hh"
#include "Trajectory.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction()
{
  forceDrawPhotons = true;
  forceNoPhotons = false;
  pmtCollID = -1;
  scintillatorCollID = -1;
}
 
EventAction::~EventAction(){}


void EventAction::BeginOfEventAction(const G4Event* anEvent)
{
 
  //New event, add the user information object
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(pmtCollID < 0)
    pmtCollID=SDman->GetCollectionID("pmtHitCollection");
   if(scintillatorCollID < 0) 
    scintillatorCollID=SDman->GetCollectionID("scintillatorHitCollection");

}
 

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
	
  G4int eventID = anEvent->GetEventID();
// periodic printing every 100 events
  if( eventID % 100 == 0 )
  {
    G4cout << "Finished Running Event # " << eventID << G4endl;
  }
  
  
  //sensitive detectores
  PMTHitsCollection* pmtHC = 0;
  ScintillatorHitsCollection* scintHC = 0;
  G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
  //Get the hit collections
  if(hitsCE)
  {
    if(pmtCollID >= 0)
       pmtHC = (PMTHitsCollection*)( hitsCE->GetHC(pmtCollID) );
    if(scintillatorCollID >= 0)
       scintHC = (ScintillatorHitsCollection*)( hitsCE->GetHC(scintillatorCollID) );
  }

  //G4cout << "!!!! scintillatorCollID " << scintillatorCollID << G4endl;
   AnalysisManager* analysisManager = AnalysisManager::GetInstance();
  if(pmtHC)
  {
      analysisManager->AddHit(pmtHC, eventID);
  }
  
  if(scintHC)
  {
      analysisManager->AddHit(scintHC, eventID);
  }

/*  
  //print trajectories - TDOD method
  G4TrajectoryContainer* trajectoryContainer=anEvent->GetTrajectoryContainer();
 
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // extract the trajectories and draw them
  if (G4VVisManager::GetConcreteInstance()){
    for (G4int i=0; i<n_trajectories; i++){
      Trajectory* trj = (Trajectory*)
        ((*(anEvent->GetTrajectoryContainer()))[i]);
      if(trj->GetParticleName()=="opticalphoton"){
        trj->SetForceDrawTrajectory(forceDrawPhotons);
        trj->SetForceNoDrawTrajectory(forceNoPhotons);
      }
      trj->DrawTrajectory();
    }
  }*/


	
}
