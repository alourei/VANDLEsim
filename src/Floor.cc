// $Id: Floor.cc 26.01.2016 A Fijalkowska $
//
/// \file Floor.cc
/// \brief Implementation of the Floor class, mostly Sergey's Ilyushkin code
//  Leribss floor position (y axis )= -2*m
//  Argone position = 154.305*cm 
//  MSU = 148.27 (?) or 97.087

#include "Floor.hh"
#include "G4Transform3D.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

Floor::Floor()
{
	SetBasicSizes();	
    materialsManager = MaterialsManager::GetInstance();
} 

Floor::~Floor()
{
	
}

void Floor::SetBasicSizes()
{
	inch = 2.54*cm;	

	
}


void Floor::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{


	G4Material* concreteMaterial = materialsManager->GetConcrete();
	G4double boxWidth = 4.0*m;
	G4double boxThickness = 10.0*inch;
	G4double boxLength = boxWidth;
	
	G4Box* boxFloor = new G4Box("Floor", boxWidth, boxThickness, boxLength); 
    G4LogicalVolume* floorLogVol = new G4LogicalVolume(boxFloor, concreteMaterial, "floorLogVol");

     
    G4VisAttributes* floorVisAtt = new G4VisAttributes( G4Colour(0x00,0x00,0xFF));
	floorVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
	//floorVisAtt->SetForceSolid(true);
	floorLogVol->SetVisAttributes(floorVisAtt);

	G4RotationMatrix rotFloor;
	if(pRot != 0L)
	    rotFloor = G4RotationMatrix(*pRot);
	G4ThreeVector posFloor(0,-boxThickness,0);
	posFloor = posFloor + tlate;
    G4Transform3D transfFloor(rotFloor,posFloor);     
	new G4PVPlacement(transfFloor, floorLogVol, pName+"Floor", pMotherLogical, 0, pCopyNo);   	
}

