// $Id: ArgonneLN2Tank.cc 16.05.2017 A Fijalkowska $
//
/// \file ArgonneLN2Tank.cc
/// \brief Implementation of the ArgonneLN2Tank class, S.I. code
//
//

#include "ArgonneLN2Tank.hh"

#include "G4Transform3D.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

ArgonneLN2Tank::ArgonneLN2Tank()
{
	SetBasicSizes();	
    materialsManager = MaterialsManager::GetInstance();
    
} 

ArgonneLN2Tank::~ArgonneLN2Tank()
{
	
}

void ArgonneLN2Tank::SetBasicSizes()
{
 	
  	// crystal
  	zdet[0] = 0.;
  	zdet[1] = 141.3*cm;
  	zdet[2] = 150.*cm;
  	zdet[3] = 160.*cm;
  
  	innR[0] = 0.;
  	innR[1] = 0.;
  	innR[2] = 0.;
  	innR[3] = 0.;
  
  	outR[0] = 25.*cm;
  	outR[1] = 25.*cm;
  	outR[2] = 5.*cm;
  	outR[3] = 5.*cm;
}

void ArgonneLN2Tank::Place(G4RotationMatrix *pRot, 
                         G4ThreeVector &tlate, 
                         const G4String &pName, 
                         G4LogicalVolume *pMotherLogical,  
                         G4int pCopyNo)
{	
	G4LogicalVolume* tankLogVol = MakeTankShell();
	AddLN2(tankLogVol);
	

 	G4RotationMatrix rotTank;
	if(pRot != 0L)
	    rotTank = G4RotationMatrix(*pRot); 	
  	rotTank.rotateX(-90.*deg);
  	G4Transform3D transformTank(rotTank,tlate);
  	G4String tankName = pName+"_LN2tankPhys";
  	new G4PVPlacement(transformTank, tankLogVol, tankName, pMotherLogical, 0, pCopyNo );
  	 
}

G4LogicalVolume* ArgonneLN2Tank::MakeTankShell()
{
    G4Material *stainSteel = materialsManager->GetStainSteel();

  	G4Polycone *tank = new G4Polycone("Tank", 0.*deg, 360.*deg, 4, zdet, innR, outR );  
  		
  	G4LogicalVolume *tankLogVol = new G4LogicalVolume(tank, stainSteel, "tankLogVol"); 
  	G4VisAttributes *tankVisAtt = new G4VisAttributes( G4Colour(0x00,0x00,0xFF));
	tankVisAtt->SetForceAuxEdgeVisible(true);
	//TankVisAtt->SetForceSolid(true);
	tankLogVol->SetVisAttributes(tankVisAtt);
	
	return tankLogVol;
}

void ArgonneLN2Tank::AddLN2(G4LogicalVolume* tankShellLogVol)
{
	G4Material *LN2Material = materialsManager->GetLiqNitrogen();
	const G4double th = 1.5*cm;
     	
  	G4double zdetc[3];
  	G4double innRc[3]; 
  	G4double outRc[3];   	
  	// crystal
  	zdetc[0] = zdet[0]+th;
  	zdetc[1] = zdet[1];
  	zdetc[2] = zdet[2]-th;
  
  	innRc[0] = 0.;
  	innRc[1] = 0.;
  	innRc[2] = 0.;
  
  	outRc[0] = outR[0]-th;
  	outRc[1] = outR[1]-th;
  	outRc[2] = outR[2]-th;	
  	G4Polycone *tankCut = new G4Polycone("tankCut", 0.*deg, 360.*deg, 3, zdetc, innRc, outRc );  
  	G4LogicalVolume *tankCutLogVol = new G4LogicalVolume(tankCut, LN2Material, "tankCutLogVol");

	G4VisAttributes* litVisAtt = new G4VisAttributes( G4Colour(0./255.,204./255.,255./255.));
	litVisAtt->SetForceAuxEdgeVisible(true);
	litVisAtt->SetForceSolid(true);
    tankCutLogVol->SetVisAttributes(litVisAtt);
	
	G4RotationMatrix rotTankCut;
  	rotTankCut.rotateX(0.*deg);
  	G4ThreeVector  translationTankCut(0.0,0.0,0.0);
  	G4Transform3D transformTankCut(rotTankCut,translationTankCut);
  	new G4PVPlacement(transformTankCut, tankCutLogVol, "LN2tankCut", tankShellLogVol, 0, 0 );
}
