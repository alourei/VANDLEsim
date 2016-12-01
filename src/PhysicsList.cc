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
// $Id: PhysicsList.cc 68752 2016-11-28 A. Fijalkowska $
//
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class based on optical/LXe/LXePhysicsList 
//
//
#include "PhysicsList.hh"

#include "GeneralPhysics.hh"
#include "EMPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "MuonPhysics.hh"
#include "NeutronHPphysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4SystemOfUnits.hh"



PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  defaultCutValue = 0.000001 * mm;

  // General Physics
  RegisterPhysics( new GeneralPhysics("general") );

  // EM Physics
  RegisterPhysics( new EMPhysics("standard EM"));

  //RegisterPhysics( new G4EmLivermorePhysics("Livermore EM"));

  // Muon Physics
  RegisterPhysics( new MuonPhysics("muon"));

  // Neutron Physics
  RegisterPhysics( new NeutronHPphysics("neutronHP"));
  
  // Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics( opticalPhysics );

  opticalPhysics->SetWLSTimeProfile("delta");

  opticalPhysics->SetScintillationYieldFactor(1.0);
  //opticalPhysics->SetScintillationExcitationRatio(0.0); //AF - what's that???

  opticalPhysics->SetMaxNumPhotonsPerStep(100);
  opticalPhysics->SetMaxBetaChangePerStep(10.0);

  opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
  opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);
  
  opticalPhysics->SetScintillationByParticleType(true);

}


PhysicsList::~PhysicsList() {}


void PhysicsList::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
}




