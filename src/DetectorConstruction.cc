//
// $Id: DetectorConstruction.cc 12.16.2016 A. Fijalkowska $
//
/// \file DetectorConstruction.cc
/// \brief DetectorConstruction class
//
//
#include "DetectorConstruction.hh"
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

  G4double expHall_x = 5.*m;
  G4double expHall_y = 5.*m;
  G4double expHall_z = 5.*m;
  G4Material* vaccum = materialsManager->GetVaccum();
  //Create experimental hall
  G4Box* experimentalHallSolid
    = new G4Box("experimentalHallSolid",expHall_x,expHall_y,expHall_z);
  experimentalHallLogic = new G4LogicalVolume(experimentalHallSolid,
                                             vaccum,
                                             "experimentalHallLogic",
                                             0,0,0);
  experimentalHallPhys = new G4PVPlacement(0,
                                           G4ThreeVector(),
                                           experimentalHallLogic,
                                           "expHall",
                                           0,
                                           false,
                                           0);

  experimentalHallLogic->SetVisAttributes(G4VisAttributes::Invisible);

  int smallSize = 0;
  int mediumSize = 1;
  int largeSize = 2;
  //call VANDLEBar fabric method
  //vandleBar = new VANDLEBar(smallSize);
  //vandleBar->Place(0, 
  //                   G4ThreeVector(0.0,10.0*cm,0.0),
  //                   "bar1", 
  //                    experimentalHallLogic, 0);
  /*
  vandleBar->Place(0, 
                   G4ThreeVector(0.0,-10.0*cm,0.0),
                   "bar2", 
                    experimentalHallLogic, 1);  */

	
   /*HPGeDet = new HPGe();
   G4RotationMatrix* rot = new G4RotationMatrix();
   rot->rotateY(90.0*degree);
   G4ThreeVector pos1 = G4ThreeVector(0.0,0.0,0.0);
   HPGeDet->Place(rot, 
                  pos1,
                  "HPGe", 
                  experimentalHallLogic, 
                  0); */
 


  leribssGeArray = new LeribssGeArray();
  G4ThreeVector gePos = G4ThreeVector(0.0,0.0,0.0);
  leribssGeArray->Place(0, 
                        gePos,
                        "LeribssGeArray", 
                        experimentalHallLogic, 
                        0); 
    
   leribssVandleArray = new LeribssVandleArray();
   G4ThreeVector vandlePos = G4ThreeVector(0.0,0.0,0.0);
   leribssVandleArray->Place(0, 
                             vandlePos,
                             "leribssVandleArray", 
                             experimentalHallLogic, 
                             0);   
   
   leribssSupport = new LeribssSupport();  
   G4ThreeVector leribssSupportPos = G4ThreeVector(0.0,0.0,0.0);
   leribssSupport->Place(0, 
                         leribssSupportPos,
                         "leribssSupport", 
                         experimentalHallLogic, 
                         0);                                    

   
   leribssBeam = new LeribssBeam();
   G4ThreeVector leribssBeamPos = G4ThreeVector(0.0,0.0,0.0);
   leribssBeam->Place(0, 
                      leribssBeamPos,
                      "leribssBeam", 
                      experimentalHallLogic, 
                      0);   
   
   leribssFloor = new LeribssFloor();
   G4ThreeVector leribssFloorPos = G4ThreeVector(0.0,0.0,0.0);
   
   leribssFloor->Place(0, 
                      leribssFloorPos,
                      "leribssFloor", 
                      experimentalHallLogic, 
                      0); 
   
   leribssBetaDet = new LeribssBetaDet(); 
   G4ThreeVector leribssBetaDetPos = G4ThreeVector(0.0,0.0,0.0);
   leribssBetaDet->Place(0, 
                      leribssBetaDetPos,
                      "lleribssBetaDet", 
                      experimentalHallLogic, 
                      0);              
   return experimentalHallPhys;
}


void DetectorConstruction::ConstructSDandField() 
{
	leribssVandleArray->ConstructSDandField();
    //vandleBar->ConstructSDandField();
}
