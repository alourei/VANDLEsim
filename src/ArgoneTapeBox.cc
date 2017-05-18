
// $Id: ArgoneTapeBox.cc 10.05.2017 A Fijalkowska $
//
/// \file ArgoneTapeBox.cc
/// \brief Implementation of the ArgoneTapeBox class, S Ilushkin code
//
//

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

#include "ArgoneTapeBox.hh"

ArgoneTapeBox::ArgoneTapeBox()
{
    materialsManager = MaterialsManager::GetInstance();
    SetBasicSizes();

} 

ArgoneTapeBox::~ArgoneTapeBox()
{

}

void ArgoneTapeBox::SetBasicSizes()
{
	
}



void ArgoneTapeBox::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{
	G4Material *stainSteelMaterial = materialsManager->GetStainSteel();
	
	

	G4double boxWidth = 10.*cm;
	G4double boxThickness = 50.*cm;
	G4double boxLength = 60.*cm;
	
	G4double boxcWidth = boxWidth-10.*mm;
	G4double boxcThickness = boxThickness-10.*mm;
	G4double boxcLength = boxLength-10.*mm;
	
	G4Box* TapeBoxs = new G4Box("TapeBoxs", boxWidth, boxThickness, boxLength);
	G4Box* TapeBoxc = new G4Box("TapeBoxc", boxcWidth, boxcThickness, boxcLength);
	
	G4RotationMatrix rotCut;
  	rotCut.rotateX(0.*deg);
  	G4ThreeVector translCut(0.0,0.0,0.0);
  	G4Transform3D transfCut(rotCut,translCut);
  	G4SubtractionSolid* TapeBox = new G4SubtractionSolid("TapeBox", 
  	                                                      TapeBoxs, 
  	                                                      TapeBoxc, 
  	                                                      transfCut);
	
	G4LogicalVolume *tapeBoxLogVol = new G4LogicalVolume(TapeBox, 
	                                                     stainSteelMaterial, 
	                                                     "tapeBoxLogVol"); 
	
	G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.5,1.,0.));
	//TankVisAtt->SetForceAuxEdgeVisible(true);
	visAtt->SetForceSolid(true);
	tapeBoxLogVol->SetVisAttributes(visAtt);
	
	G4RotationMatrix rotTapeBox;
	if(pRot != 0L)
	    rotTapeBox = G4RotationMatrix(*pRot);
  	rotTapeBox.rotateY(90.*deg); 	
  	G4ThreeVector translTapeBox =  tlate;
  	G4Transform3D transfTapeBox(rotTapeBox,translTapeBox);
  	G4String name = pName+"_TapeBox";
  	new G4PVPlacement(transfTapeBox, tapeBoxLogVol, name, pMotherLogical, 0, pCopyNo);
 	
}
