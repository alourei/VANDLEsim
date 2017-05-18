
// $Id: ArgoneBetaDet.cc 10.05.2017 A Fijalkowska $
//
/// \file ArgoneBetaDet.cc
/// \brief Implementation of the ArgoneBetaDet class, S Ilushkin code
//
//

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

#include "ArgoneBetaDet.hh"

ArgoneBetaDet::ArgoneBetaDet()
{
    materialsManager = MaterialsManager::GetInstance();
    SetBasicSizes();
} 

ArgoneBetaDet::~ArgoneBetaDet()
{

}

void ArgoneBetaDet::SetBasicSizes()
{
	offset = 29.*mm;
}



void ArgoneBetaDet::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{
    G4LogicalVolume* betaDetLogVol = MakeBetaDet();
	G4LogicalVolume *betaFrameLogVol = MakeBetaFrame();
	
	G4RotationMatrix rotBetaDet;
  	if(pRot != 0L)
	    rotBetaDet = G4RotationMatrix(*pRot);
  	G4ThreeVector translBetaDet(0.0,-offset,0.0);
  	translBetaDet =  translBetaDet + tlate;
  	G4Transform3D transfBetaDet(rotBetaDet,translBetaDet);
  	G4String nameBeta = pName+"_BetaDet";
  	new G4PVPlacement(transfBetaDet, betaDetLogVol, nameBeta, pMotherLogical, 0, pCopyNo);  
  	
  	
  	
	G4RotationMatrix rotF1;
	if(pRot != 0L)
	    rotF1 = G4RotationMatrix(*pRot);
  	rotF1.rotateY(90.*deg); 
  	G4ThreeVector translF1(1.3*offset,0.0,0.0);	
  	translF1 =  translF1 + tlate;
  	G4Transform3D transfF1(rotF1,translF1);
  	G4String name = pName+"_BetaDetF1";
  	new G4PVPlacement(transfF1, betaFrameLogVol, name,  pMotherLogical, 0, 2*pCopyNo ); 


    G4RotationMatrix rotF2 = rotF1;
    G4ThreeVector translF2(-1.3*offset,0.0,0.0);	
  	G4Transform3D transfF2(rotF2,translF2);
  	new G4PVPlacement(transfF2, betaFrameLogVol, name,  pMotherLogical, 0, 2*pCopyNo+1 );  	
}


G4LogicalVolume* ArgoneBetaDet::MakeBetaDet()
{
	G4Material* BC408Material = materialsManager->GetBC408NoLight();
    G4double boxWidth = 60.*mm; //x //120.*mm
	G4double boxThickness = 1.*mm; //y 40.*mm
	G4double boxLength = 20.*mm; //z //2.*mm
	
	G4Box* betaDetBox = new G4Box("BetaDetBox", boxWidth, boxThickness, boxLength);
	
    G4RotationMatrix rotTR;
    rotTR.rotateX(90.*deg);
    G4ThreeVector translTR(0.0,offset,offset);
    G4Transform3D transfTR(rotTR,translTR);
    G4UnionSolid* boxTR = new G4UnionSolid("boxTR", 
                                            betaDetBox, 
                                            betaDetBox, 
                                            transfTR);
     
    G4RotationMatrix rotTRB;
    rotTRB.rotateX(0.*deg);
    G4ThreeVector translTRB(0.0,2.*offset,0.0);
    G4Transform3D transfTRB(rotTRB,translTRB);
    G4UnionSolid* boxTRB = new G4UnionSolid("boxTRB", 
                                             boxTR, 
                                             betaDetBox, 
                                             transfTRB);
     
     G4RotationMatrix rotTRBL;
     rotTRBL.rotateX(90.*deg);
     G4ThreeVector translTRBL(0.0,offset,-offset);
     G4Transform3D transfTRBL(rotTRBL,translTRBL);
     G4UnionSolid* boxTRBL = new G4UnionSolid("boxTRBL", 
                                               boxTRB, 
                                               betaDetBox, 
                                               transfTRBL);
	G4LogicalVolume* betaDetLogVol = new G4LogicalVolume(boxTRBL, BC408Material, "betaDetLogVol");
	 
	G4VisAttributes* betaDetVis = new G4VisAttributes( G4Colour(230./255.,133./255.,35./255.));
	//betaDetVis->SetForceAuxEdgeVisible(true);
	betaDetVis->SetForceSolid(true);
	betaDetLogVol->SetVisAttributes(betaDetVis);
	return betaDetLogVol;
}

G4LogicalVolume* ArgoneBetaDet::MakeBetaFrame()
{
	G4Material *stainSteel = materialsManager->GetStainSteel();
	
	G4double frameOutRad = 48.0*mm;
	G4double frameInnRad = 0.*mm; 	
  	G4double frameLength = 1.5*mm;

  	G4double betaDetFrameCutW = 30.5*mm; //30,5
	G4double betaDetFrameCutTh = betaDetFrameCutW;
  	G4double betaDetFrameCutL = 2.*frameLength;

    G4Tubs* betaDetFrameH = new G4Tubs("betaDetFrameH", 
                                        frameInnRad, 
                                        frameOutRad, 
                                        frameLength, 
                                        0.0*M_PI, 
                                        2.0*M_PI);
    G4Box* betaDetFrameCut = new G4Box("betaDetFrameCut", 
                                        betaDetFrameCutW,
                                        betaDetFrameCutTh, 
                                        betaDetFrameCutL);

    G4RotationMatrix rotBetaDetFrameCut;
  	rotBetaDetFrameCut.rotateX(0.*deg);
  	G4ThreeVector translBetaDetFrameCut(0.0,0.0,0.0);
  	G4Transform3D transfBetaDetFrameCut(rotBetaDetFrameCut,translBetaDetFrameCut);
  	G4SubtractionSolid* betaDetFrameC = new G4SubtractionSolid("betaDetFrameC", 
  	                                                            betaDetFrameH, 
  	                                                            betaDetFrameCut, 
  	                                                            transfBetaDetFrameCut);


    G4LogicalVolume *betaFrameLogVol = new G4LogicalVolume(betaDetFrameC, 
                                                           stainSteel, 
                                                           "betaFrameLogVol");
     
	G4VisAttributes* betaFrameVisAtt = new G4VisAttributes( G4Colour(0.9,0.9,0.9));
	//betaFrameVisAtt->SetForceAuxEdgeVisible(true);
	betaFrameVisAtt->SetForceSolid(true);
	betaFrameLogVol->SetVisAttributes(betaFrameVisAtt);
	return betaFrameLogVol;
}
