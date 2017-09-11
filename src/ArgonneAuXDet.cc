// $Id: ArgonneAuxDet.cc 03.05.2017 A Fijalkowska $
//
/// \file ArgonneAuxDet.cc
/// \brief Implementation of the ArgonneAuxDet class, S.I. code
//
//

#include "ArgonneAuxDet.hh"

#include "G4Transform3D.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

ArgonneAuxDet::ArgonneAuxDet()
{
	SetBasicSizes();	
    materialsManager = MaterialsManager::GetInstance();
    
} 

ArgonneAuxDet::~ArgonneAuxDet()
{
	
}

void ArgonneAuxDet::SetBasicSizes()
{

}

 void ArgonneAuxDet::Place(G4RotationMatrix *pRot, 
                         G4ThreeVector &tlate, 
                         const G4String &pName, 
                         G4LogicalVolume *pMotherLogical,  
                         G4int pCopyNo)
{

	G4Material *stainSteelMaterial = materialsManager->GetStainSteel();
	G4Material* gasMaterial = materialsManager->GetAir();
	
  	G4double zdet[4];
  	G4double innR[4]; 
  	G4double outR[4];   	
  	// crystal
  	zdet[0] = 0.;
  	zdet[1] = 8.*cm;
  	zdet[2] = 13.*cm;
  	zdet[3] = 30.*cm;
  
  	innR[0] = 0.;
  	innR[1] = 0.;
  	innR[2] = 0.;
  	innR[3] = 0.;
  
  	outR[0] = 9.*cm;
  	outR[1] = 9.*cm;
  	outR[2] = 5.*cm;
  	outR[3] = 5.*cm;

  	G4Polycone *tank = new G4Polycone("auxDet", 0.*deg, 360.*deg, 4, zdet, innR, outR );  	
  	G4LogicalVolume *tankLogVol = new G4LogicalVolume(tank, 
  	                                                  stainSteelMaterial, 
  	                                                  "auxDetLogVol"); 

    const G4double th = 0.5*cm;
     	
  	G4double zdetc[3];
  	G4double innRc[3]; 
  	G4double outRc[3];   	
  	zdetc[0] = zdet[0];
  	zdetc[1] = zdet[1]-th;
  	zdetc[2] = zdet[2]-th;
  	zdetc[3] = zdet[3]-2.*th;
  
  	innRc[0] = 0.;
  	innRc[1] = 0.;
  	innRc[2] = 0.;
  	innRc[3] = 0.;
  
  	outRc[0] = outR[0]-th;
  	outRc[1] = outR[1]-th;
  	outRc[2] = outR[2]-th;	
  	outRc[3] = outR[3]-th;
  	
  	G4Polycone *tankCut = new G4Polycone("auxDetCut", 0.*deg, 360.*deg, 4, zdetc, innRc, outRc );  
  	G4LogicalVolume *tankCutLogVol = new G4LogicalVolume(tankCut, gasMaterial, "auxDetCutLogVol");

	G4VisAttributes* mLNVisAtt = new G4VisAttributes( G4Colour(0.,0.8,1.));
	mLNVisAtt->SetForceAuxEdgeVisible(true);
	mLNVisAtt->SetForceSolid(true);
    tankCutLogVol->SetVisAttributes(mLNVisAtt);
  	
    G4VisAttributes* TankVisAtt = new G4VisAttributes( G4Colour(0.,0.,1.));
	TankVisAtt->SetForceAuxEdgeVisible(true);
	tankLogVol->SetVisAttributes(TankVisAtt);
	
 	
  	
    G4RotationMatrix rotTankCut;
  	rotTankCut.rotateX(0.*deg);
  	G4ThreeVector  translationTankCut(0.0,0.0,th);
  	G4Transform3D transformTankCut(rotTankCut,translationTankCut);
  	new G4PVPlacement(transformTankCut, tankCutLogVol, "crystalAuxDet", tankLogVol, 0, 0 );
  	


  	
  	G4RotationMatrix rotTank;
  	rotTank.rotateY(90.*deg);
  	rotTank.rotateZ(-45.0*deg);
  	G4Transform3D transformTank(rotTank,tlate);  	
  	G4String auxDetName = pName+"_auxPhys";
  	new G4PVPlacement(transformTank, tankLogVol, auxDetName, pMotherLogical,0,pCopyNo);
}

