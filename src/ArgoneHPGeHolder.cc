
// $Id: ArgoneHPGeHolder.cc 10.05.2017 A Fijalkowska $
//
/// \file ArgoneHPGeHolder.cc
/// \brief Implementation of the ArgoneHPGeHolder class, SI code
//
//

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "ArgoneHPGeHolder.hh"

ArgoneHPGeHolder::ArgoneHPGeHolder()
{
    materialsManager = MaterialsManager::GetInstance();
    SetBasicSizes();

} 

ArgoneHPGeHolder::~ArgoneHPGeHolder()
{

}

void ArgoneHPGeHolder::SetBasicSizes()
{
	inch = 2.54*cm;
    HPGeholderBox_x = 2.975*inch;
	HPGeholderBox_y = HPGeholderBox_x;
	HPGeholderBox_z = 3.90625*inch;
	
}


void ArgoneHPGeHolder::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{

	G4Material* aluminiumMaterial = materialsManager->GetAluminum(); 
    G4VSolid* mainHPGeHolderSolid = MakeMainHPGeHolderSolid();  
    G4VSolid* suppHPGeHolderSolid = AddSupplementHPGeHolderSolid(mainHPGeHolderSolid);
    G4VSolid* HPGeHolderRingSolid = AddHPGeHolderRing(suppHPGeHolderSolid);
	G4VSolid* HPGeHolderPlateSolid = AddHPGeHolderPlate(HPGeHolderRingSolid);
  

    G4LogicalVolume* HPGeHolderLogVol = new G4LogicalVolume(HPGeHolderPlateSolid, 
                                                            aluminiumMaterial, 
                                                            "HPGeHolderLogVol");

	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(1,0.5,0.));
	visAtt->SetForceAuxEdgeVisible(true);
	visAtt->SetForceSolid(true);
	HPGeHolderLogVol->SetVisAttributes(visAtt);

    G4double xOff = 161.0*mm;
	G4double yOff = 261.0*mm;
	G4double zOff = yOff;

	G4RotationMatrix rotHolderR;
  	rotHolderR.rotateX(-45.*deg);
  	G4ThreeVector translHolderR(0.0,-yOff,-zOff);
  	G4Transform3D transfHolderR(rotHolderR,translHolderR);
  	G4String nameR = pName+"_HPGeHolderR";
  	new G4PVPlacement(transfHolderR, HPGeHolderLogVol, nameR, pMotherLogical, 0, 0 );

	G4RotationMatrix rotHolderL;
  	rotHolderL.rotateX(-45.*deg);
  	rotHolderL.rotateY(180.*deg);
  	G4ThreeVector  translationHolderL(0.0,-yOff,zOff);
  	G4Transform3D transformHolderL(rotHolderL,translationHolderL);
  	G4String nameL = pName+"_HPGeHolderL";
  	new G4PVPlacement(transformHolderL, HPGeHolderLogVol, nameL, pMotherLogical, 0, 0 );  	

  	
  	
}


G4VSolid* ArgoneHPGeHolder::MakeMainHPGeHolderSolid()
{
	const G4double HPGeholderBoxCut_x = 2.6*inch;
	const G4double HPGeholderBoxCut_y = HPGeholderBoxCut_x;
	const G4double HPGeholderBoxCut_z = 2.0*HPGeholderBox_z;
	
	G4Box* HPGeholderBoxBase = new G4Box("HPGeholderBoxBase", 
	                                      HPGeholderBox_x, 
	                                      HPGeholderBox_y, 
	                                      HPGeholderBox_z);
	G4Box* HPGeholderBoxCut = new G4Box("HPGeholderBoxCut", 
	                                     HPGeholderBoxCut_x, 
	                                     HPGeholderBoxCut_y, 
	                                     HPGeholderBoxCut_z);

	G4RotationMatrix rotHPGeholderBoxCut;
    rotHPGeholderBoxCut.rotateZ(0.0*deg);
    const G4ThreeVector  translationHPGeholderBoxCut(0.0,0.0,0.0);
    G4Transform3D transformHPGeholderBoxCut(rotHPGeholderBoxCut,
                                            translationHPGeholderBoxCut);
    G4SubtractionSolid* HPGeHolderBox = new G4SubtractionSolid("HPGeHolderBox", 
                                                                HPGeholderBoxBase, 
                                                                HPGeholderBoxCut, 
                                                                transformHPGeholderBoxCut);
    return HPGeHolderBox;
}

G4VSolid* ArgoneHPGeHolder::AddSupplementHPGeHolderSolid(G4VSolid* mainHolder)
{
	const G4double HPGeholderBoxExtra_x = 2.0*inch;
	const G4double HPGeholderBoxExtra_y = 0.25*inch;
	const G4double HPGeholderBoxExtra_z = HPGeholderBoxExtra_y;
	
	G4Box* HPGeholderExtra = new G4Box("HPGeholderExtra", 
	                                    HPGeholderBoxExtra_x, 
	                                    HPGeholderBoxExtra_y, 
	                                    HPGeholderBoxExtra_z);
	G4RotationMatrix rotHPGeholderExtra1;
    rotHPGeholderExtra1.rotateZ(0.0*deg);
    G4ThreeVector transHPGeholderExtra1(0.0,
                                        HPGeholderBox_y+HPGeholderBoxExtra_y,
                                        -(HPGeholderBox_z-HPGeholderBoxExtra_z));
    G4Transform3D transfHPGeholderExtra1(rotHPGeholderExtra1,
                                         transHPGeholderExtra1);
    G4UnionSolid* HPGeholderBoxExtra1 = new G4UnionSolid("HPGeholderBoxExtra1", 
                                                          mainHolder, 
                                                          HPGeholderExtra, 
                                                          transfHPGeholderExtra1);
    
     G4RotationMatrix rotHPGeholderExtra2;
     rotHPGeholderExtra2.rotateZ(0.0*deg);
     G4ThreeVector translHPGeholderExtra2(0.0,
                                         -(HPGeholderBox_y+HPGeholderBoxExtra_y),
                                         -(HPGeholderBox_z-HPGeholderBoxExtra_z));
     G4Transform3D transfHPGeholderExtra2(rotHPGeholderExtra2,translHPGeholderExtra2);
     G4UnionSolid* HPGeholderBoxExtra2 = new G4UnionSolid("HPGeholderBoxExtra1", 
                                                           HPGeholderBoxExtra1, 
                                                           HPGeholderExtra, 
                                                           transfHPGeholderExtra2);
     return HPGeholderBoxExtra2;

}


G4VSolid* ArgoneHPGeHolder::AddHPGeHolderRing(G4VSolid* suppHPGeHolderSolid)
{

    G4double holderRingRad = 6.0*inch;
    G4double holderRingLength = 0.375*inch;
    	
    G4double holderRingCut_x = 2.595*inch;
    G4double holderRingCut_y = holderRingCut_x;
    G4double holderRingCut_z = 2.0*holderRingLength;
    	
    G4Tubs* holderRingTube = new G4Tubs("holderRingTube", 
                                         0.0, 
                                         holderRingRad, 
                                         holderRingLength, 
                                         0.0*M_PI, 
                                         2.0*M_PI);
    G4Box* holderRingCut = new G4Box("holderRingCut", 
                                      holderRingCut_x, 
                                      holderRingCut_y, 
                                      holderRingCut_z);
    	
    G4RotationMatrix rotholderRingCut;
    rotholderRingCut.rotateZ(0.0*deg);
    G4ThreeVector translHolderRingCut(0.0,0.0,0.0);
    G4Transform3D transfHolderRingCut(rotholderRingCut,translHolderRingCut);
    G4SubtractionSolid* holderRing = new G4SubtractionSolid("holderRing", 
                                                             holderRingTube, 
                                                             holderRingCut, 
                                                             transfHolderRingCut);
    	
    G4RotationMatrix rotholderRing;
    rotholderRing.rotateZ(0.0*deg);
    G4ThreeVector translHolderRing(0.0,0.0,-(HPGeholderBox_z+holderRingLength));
    G4Transform3D transfHolderRing(rotholderRing,translHolderRing);
    G4UnionSolid* HPGeHolderRing = new G4UnionSolid("HPGeHolderRing", 
                                                    suppHPGeHolderSolid, 
                                                    holderRing, 
                                                    transfHolderRing);
                                                    
    return HPGeHolderRing;
}




G4VSolid* ArgoneHPGeHolder::AddHPGeHolderPlate(G4VSolid* HPGeHolderRingSolid)
{  
     
    G4double holderPlate_x = 7.31*inch;
    G4double holderPlate_y = 7.0*inch;
    G4double holderPlate_z = 0.44*inch;
    	
    G4double holderPlateCut_x = 1.35*HPGeholderBox_x;
    G4double holderPlateCut_y = holderPlateCut_x;
    G4double holderPlateCut_z = 2.0*holderPlate_z;

    G4Box* holderPlateBox = new G4Box("holderPlateBox", 
                                       holderPlate_x, 
                                       holderPlate_y, 
                                       holderPlate_z);
    G4Box* holderPlateCut = new G4Box("holderPlateCut", 
                                       holderPlateCut_x, 
                                       holderPlateCut_y, 
                                       holderPlateCut_z);
    	
    G4RotationMatrix rotholderPlateCut;
    rotholderPlateCut.rotateZ(0.0*deg);
    G4ThreeVector translHolderPlateCut(0.0,0.0,0.0);
    G4Transform3D transfHolderPlateCut(rotholderPlateCut,translHolderPlateCut);
    G4SubtractionSolid* holderPlate = new G4SubtractionSolid("holderPlate", 
                                                              holderPlateBox, 
                                                              holderPlateCut, 
                                                              transfHolderPlateCut);

    G4RotationMatrix rotholderPlate;
    rotholderPlate.rotateZ(0.0*deg);
    G4ThreeVector translHolderPlate(0.0,0.0,-(HPGeholderBox_z-holderPlate_z));
    G4Transform3D transfHolderPlate(rotholderPlate,translHolderPlate);
    G4UnionSolid* HPGeHolder = new G4UnionSolid("HPGeholder", 
                                                 HPGeHolderRingSolid, 
                                                 holderPlate, 
                                                 transfHolderPlate);
     
    return HPGeHolder;
}
