//
// $Id: DetectorConstruction.cc 12.16.2016 A. Fijalkowska $
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
   //it has to be Logic Vol type, not Physical Vol!!!!! TODO!!!

  return experimentalHallPhys;
}


void DetectorConstruction::ConstructSDandField() 
{
  // PMT SD
  if (!pmtSD.Get()) 
  {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction /VANDLEDet/pmtSD" << G4endl;
    PMTSD* pmtSDSingle = new PMTSD("/VANDLEDet/pmtSD");
    pmtSDSingle->SetModuleDeph(1); //TODO - get deph from VANDLE bar
    pmtSDSingle->SetPMTDeph(2);
    pmtSD.Put(pmtSDSingle);
  }

  //sensitive detector is not actually on the photocathode.
  //processHits gets done manually by the stepping action.
  //It is used to detect when photons hit and get absorbed&detected at the
  //boundary to the photocathode (which doesnt get done by attaching it to a
  //logical volume.
  //It does however need to be attached to something or else it doesnt get
  //reset at the begining of events  
  SetSensitiveDetector(vandleBar->GetPMTLogical(), pmtSD.Get());
  
  // Scintillator SD
   if (!scintillatorSD.Get()) 
   {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction /VANDLEDet/scintillatorSD" << G4endl;
    ScintillatorSD* scintillatorSingle = new ScintillatorSD("/VANDLEDet/scintillatorSD");
    scintillatorSingle->SetModuleDeph(1);
    scintillatorSD.Put(scintillatorSingle);
  }
  SetSensitiveDetector(vandleBar->GetScintilatorLogical(), scintillatorSD.Get());
}
