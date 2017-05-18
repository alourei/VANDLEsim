//
// $Id: DetectorConstruction.cc 12.16.2016 A. Fijalkowska $
//
/// \file DetectorConstruction.cc
/// \brief DetectorConstruction class
//
//
#include "DetectorConstruction.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "ArgoneFrame.hh"
#include "ArgoneAuxDet.hh"
#include "ArgoneGeArray.hh"
#include "ArgoneHPGeHolder.hh"
#include "ArgoneBeamLine.hh"
#include "ArgoneTapeBox.hh"
#include "ArgoneBetaDet.hh"
#include "ArgoneLN2Tank.hh"
#include "Floor.hh"

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
  //vandleBar = new VANDLEBar(mediumSize);
  argoneVandleArray = new ArgoneVandleArray();
  G4ThreeVector vandlePosition = G4ThreeVector(0.0,0.0,0.0);
  argoneVandleArray->Place(0, 
                         vandlePosition,
                         "argoneVandleArray", 
                          experimentalHallLogic, 
                          0);
                

   ArgoneFrame* argoneFrame = new ArgoneFrame();
   argoneFrame->Place(0, 
                      vandlePosition,
                      "argoneFrame", 
                      experimentalHallLogic, 
                      0);
   
   	
   G4ThreeVector auxDetPos(80.*mm,-105.0*mm,0.);                   
   ArgoneAuxDet* argoneAuxDet = new ArgoneAuxDet();
   argoneAuxDet->Place(0, 
                      auxDetPos,
                      "argoneAuxDet", 
                      experimentalHallLogic, 
                      0);
                      
                      
   ArgoneGeArray* argoneGeArray = new ArgoneGeArray();
   argoneGeArray->Place(0, 
                      vandlePosition,
                      "argoneGeArray", 
                      experimentalHallLogic, 
                      0);
                      
   ArgoneHPGeHolder* argoneHPGeHolder = new ArgoneHPGeHolder();
   argoneHPGeHolder->Place(0, 
                      vandlePosition,
                      "argoneHPGeHolder", 
                      experimentalHallLogic, 
                      0);
    
   ArgoneBeamLine* argoneBeamLine = new ArgoneBeamLine();
   argoneBeamLine->Place(0, 
                         vandlePosition,
                         "argoneBeamLine", 
                         experimentalHallLogic, 
                         0);
                      
   ArgoneTapeBox* argoneTapeBox = new ArgoneTapeBox();
   G4ThreeVector argoneTapeBoxPos(170.0*cm,0.0,0.0);
   argoneTapeBox->Place(0, 
                        argoneTapeBoxPos,
                        "argoneTapeBox", 
                        experimentalHallLogic, 
                        0);   
 
   ArgoneBetaDet* argoneBetaDet = new ArgoneBetaDet();
   G4ThreeVector argoneBetaDetPos(0.0,0.0,0.0);
   argoneBetaDet->Place(0, 
                        argoneBetaDetPos,
                        "argoneBetaDet", 
                        experimentalHallLogic, 
                        0); 
                          
   ArgoneLN2Tank* argoneLN2Tank = new ArgoneLN2Tank();
   G4ThreeVector argoneLN2TankPos(-1.45*m,-159.3215*cm,-0.4*m);
   argoneLN2Tank->Place(0, 
                        argoneLN2TankPos,
                        "argoneLN2Tank", 
                        experimentalHallLogic, 
                        0); 
                          
   Floor* argoneFloor = new Floor();
   G4ThreeVector argoneFloorPos(0,-159.3215*cm,0);
   argoneFloor->Place(0, 
                      argoneFloorPos,
                      "argoneFloor", 
                      experimentalHallLogic, 
                      0);  
                                     
   return experimentalHallPhys;
}



void DetectorConstruction::ConstructSDandField() 
{
	argoneVandleArray->ConstructSDandField();
    //vandleBar->ConstructSDandField();
}






