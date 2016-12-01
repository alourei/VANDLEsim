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
// $Id: LXeDetectorConstruction.cc 82853 2014-07-14 09:07:11Z gcosmo $
//
/// \file DetectorConstruction.cc
/// \brief DetectorConstruction class
//
//
#include "DetectorConstruction.hh"
//#include "PMTSD.hh"
//#include "ScintSD.hh"

#include "G4SDManager.hh"


#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"



DetectorConstruction::DetectorConstruction()
{
  experimentalHallLogic = NULL;
  experimentalHallPhys = NULL;
}



DetectorConstruction::~DetectorConstruction() {}


G4VPhysicalVolume* DetectorConstruction::Construct(){

  if (experimentalHallPhys) {
     G4GeometryManager::GetInstance()->OpenGeometry();
     G4PhysicalVolumeStore::GetInstance()->Clean();
     G4LogicalVolumeStore::GetInstance()->Clean();
     G4SolidStore::GetInstance()->Clean();
     G4LogicalSkinSurface::CleanSurfaceTable();
     G4LogicalBorderSurface::CleanSurfaceTable();
  }

  materialsManager = MaterialsManager::GetInstance();
  return ConstructDetector();
}


G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{

  G4double expHall_x = 3.*m;
  G4double expHall_y = 3.*m;
  G4double expHall_z = 3.*m;
  G4Material* vaccum = materialsManager->GetVaccum();
  //Create experimental hall
  G4Box* experimentalHallSolid
    = new G4Box("experimentalHallSolid",expHall_x,expHall_y,expHall_z);
  experimentalHallLogic = new G4LogicalVolume(experimentalHallSolid,
                                             vaccum,
                                             "experimentalHallLogic",
                                             0,0,0);
  experimentalHallPhys = new G4PVPlacement(0,G4ThreeVector(),
                              experimentalHallLogic,"expHall",0,false,0);

  experimentalHallLogic->SetVisAttributes(G4VisAttributes::Invisible);


  //call VANDLEBar fabric method
    vandleBar
      = new VANDLEBar(0,G4ThreeVector(),experimentalHallLogic,false,0,1);


  return experimentalHallPhys;
}


//void DetectorConstruction::ConstructSDandField() 
//{

  /*if (!fMainVolume) return;

  // PMT SD

  if (!fPmt_SD.Get()) {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction /LXeDet/pmtSD" << G4endl;
    LXePMTSD* pmt_SD = new LXePMTSD("/LXeDet/pmtSD");
    fPmt_SD.Put(pmt_SD);

    pmt_SD->InitPMTs((fNx*fNy+fNx*fNz+fNy*fNz)*2); //let pmtSD know # of pmts
    pmt_SD->SetPmtPositions(fMainVolume->GetPmtPositions());
  }

  //sensitive detector is not actually on the photocathode.
  //processHits gets done manually by the stepping action.
  //It is used to detect when photons hit and get absorbed&detected at the
  //boundary to the photocathode (which doesnt get done by attaching it to a
  //logical volume.
  //It does however need to be attached to something or else it doesnt get
  //reset at the begining of events

  SetSensitiveDetector(fMainVolume->GetLogPhotoCath(), fPmt_SD.Get());

  // Scint SD

  if (!fScint_SD.Get()) {
    G4cout << "Construction /LXeDet/scintSD" << G4endl;
    LXeScintSD* scint_SD = new LXeScintSD("/LXeDet/scintSD");
    fScint_SD.Put(scint_SD);
  }
  SetSensitiveDetector(fMainVolume->GetLogScint(), fScint_SD.Get());*/
//}
