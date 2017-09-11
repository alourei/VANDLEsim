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
#include "ArgonneFrame.hh"
#include "ArgonneAuxDet.hh"
#include "ArgonneGeArray.hh"
#include "ArgonneHPGeHolder.hh"
#include "ArgonneBeamLine.hh"
#include "ArgonneTapeBox.hh"
#include "ArgonneBetaDet.hh"
#include "ArgonneLN2Tank.hh"
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
  argonneVandleArray = new ArgonneVandleArray();
  G4ThreeVector vandlePosition = G4ThreeVector(0.0,0.0,0.0);
  argonneVandleArray->Place(0, 
                         vandlePosition,
                         "ArgonneVandleArray", 
                          experimentalHallLogic, 
                          0);
                

   ArgonneFrame* argonneFrame = new ArgonneFrame();
   argonneFrame->Place(0, 
                      vandlePosition,
                      "ArgonneFrame", 
                      experimentalHallLogic, 
                      0);
   
   	
   G4ThreeVector auxDetPos(80.*mm,-105.0*mm,0.);                   
   ArgonneAuxDet* argonneAuxDet = new ArgonneAuxDet();
   argonneAuxDet->Place(0, 
                      auxDetPos,
                      "ArgonneAuxDet", 
                      experimentalHallLogic, 
                      0);
                      
                      
   ArgonneGeArray* argonneGeArray = new ArgonneGeArray();
   argonneGeArray->Place(0, 
                      vandlePosition,
                      "ArgonneGeArray", 
                      experimentalHallLogic, 
                      0);
                      
   ArgonneHPGeHolder* argonneHPGeHolder = new ArgonneHPGeHolder();
   argonneHPGeHolder->Place(0, 
                      vandlePosition,
                      "ArgonneHPGeHolder", 
                      experimentalHallLogic, 
                      0);
    
   ArgonneBeamLine* argonneBeamLine = new ArgonneBeamLine();
   argonneBeamLine->Place(0, 
                         vandlePosition,
                         "ArgonneBeamLine", 
                         experimentalHallLogic, 
                         0);
                      
   ArgonneTapeBox* argonneTapeBox = new ArgonneTapeBox();
   G4ThreeVector argonneTapeBoxPos(170.0*cm,0.0,0.0);
   argonneTapeBox->Place(0, 
                        argonneTapeBoxPos,
                        "ArgonneTapeBox", 
                        experimentalHallLogic, 
                        0);   
 
   ArgonneBetaDet* argonneBetaDet = new ArgonneBetaDet();
   G4ThreeVector ArgonneBetaDetPos(0.0,0.0,0.0);
   argonneBetaDet->Place(0, 
                        ArgonneBetaDetPos,
                        "ArgonneBetaDet", 
                        experimentalHallLogic, 
                        0); 
                          
   ArgonneLN2Tank* argonneLN2Tank = new ArgonneLN2Tank();
   G4ThreeVector ArgonneLN2TankPos(-1.45*m,-159.3215*cm,-0.4*m);
   argonneLN2Tank->Place(0, 
                        ArgonneLN2TankPos,
                        "ArgonneLN2Tank", 
                        experimentalHallLogic, 
                        0); 
                          
   Floor* argonneFloor = new Floor();
   G4ThreeVector ArgonneFloorPos(0,-159.3215*cm,0);
   argonneFloor->Place(0, 
                      ArgonneFloorPos,
                      "ArgonneFloor", 
                      experimentalHallLogic, 
                      0);  
                                     
   return experimentalHallPhys;
}



void DetectorConstruction::ConstructSDandField() 
{
	argonneVandleArray->ConstructSDandField();
    //vandleBar->ConstructSDandField();
}






