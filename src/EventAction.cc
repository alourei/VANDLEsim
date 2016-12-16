//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: EventAction.cc 11.30.2016 A. Fijalkowska $
//
/// \file EventAction.cc
/// \brief Implementation of the EventAction class
//
//
#include "EventAction.hh"
#include "PMTHit.hh"
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
}
 
EventAction::~EventAction(){}


void EventAction::BeginOfEventAction(const G4Event* anEvent)
{
 
  //New event, add the user information object
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(pmtCollID < 0)
    pmtCollID=SDman->GetCollectionID("pmtHitCollection");

}
 

void EventAction::EndOfEventAction(const G4Event* anEvent)
{

  
  G4int eventID = anEvent->GetEventID();
// periodic printing every 100 events
  if( eventID % 100 == 0 )
  {
    G4cout << "Finished Running Event # " << eventID << G4endl;
  }
	
	
	
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
  }


//sensitive detectores
  PMTHitsCollection* pmtHC = 0;
  G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
  AnalysisManager* analysisManager = AnalysisManager::GetInstance();
  //Get the hit collections
  if(hitsCE)
  {
    if(pmtCollID>=0)
        pmtHC = (PMTHitsCollection*)( hitsCE->GetHC(pmtCollID) );
  }

 
  if(pmtHC)
  {
      analysisManager->AddHit(pmtHC, eventID);
  }
	
}
