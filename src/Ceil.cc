// $Id: Ceil.cc 16.05.2016 A Fijalkowska $
//
/// \file Ceil.cc
/// \brief Implementation of the Ceil class, mostly Sergey's Ilyushkin code
//  position: //MSU: 97.087 //LeRIBSS: 50.874 //MSU:148.27
//

#include "Ceil.hh"

#include "G4Transform3D.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

Ceil::Ceil()
{
	SetBasicSizes();	
    materialsManager = MaterialsManager::GetInstance();
} 

Ceil::~Ceil()
{
	
}

void Ceil::SetBasicSizes()
{
	inch = 2.54*cm;	
	
}


void Ceil::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{
	G4Material* concreteMaterial = materialsManager->GetConcrete();
	G4Material* hdpeMaterial = materialsManager->GetHDPE();
	
	const G4double offsetCenter = 0.0; 

	G4double boxWidth = 5.0*m;
	G4double boxThickness = 10.0*inch;
	G4double boxLength = boxWidth;
	
	G4Box* boxCeil = new G4Box("Ceil", boxWidth, boxThickness, boxLength); 

    G4LogicalVolume* ceilLogVol = new G4LogicalVolume(boxCeil, concreteMaterial, "ceilLogVol");
    
    G4VisAttributes* ceilVisAtt = new G4VisAttributes( G4Colour(0x00,0x00,0xFF));
	ceilVisAtt->SetForceAuxEdgeVisible(true);
	ceilVisAtt->SetForceSolid(true);
	ceilLogVol->SetVisAttributes(ceilVisAtt);

    G4RotationMatrix rotCeil;
	if(pRot != 0L)
	    rotCeil = G4RotationMatrix(*pRot);
	G4ThreeVector posCeil(0,boxThickness,0);
	posCeil = posCeil + tlate;
    G4Transform3D transfCeil(rotCeil,posCeil);     
	new G4PVPlacement(transfCeil, ceilLogVol, "ceil", pMotherLogical, 0, 0);
	    	
}

