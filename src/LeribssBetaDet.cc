// $Id: LeribssBetaDet.cc 01.02.2017 A Fijalkowska $
//
/// \file LeribssBetaDet.cc
/// \brief Implementation of the LeribssBetaDet class
//
//

#include "LeribssBetaDet.hh"

#include "G4Transform3D.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

LeribssBetaDet::LeribssBetaDet()
{
	SetBasicSizes();	
    materialsManager = MaterialsManager::GetInstance();
    betaDetVisAtt = new G4VisAttributes( G4Colour(140./255.,0,0));
	betaDetVisAtt->SetForceAuxEdgeVisible(true);
	betaDetVisAtt->SetForceSolid(true);
} 

LeribssBetaDet::~LeribssBetaDet()
{
	delete betaDetVisAtt;
}

void LeribssBetaDet::SetBasicSizes()
{
	inch = 2.54*cm;	
    betaInnRad = 1*inch;
    betaOutRad = 1.25*inch;
    betaLength = 7.5*inch;

    lightBoxX = 1.5*inch;	
  	lightBoxY = 2.3*inch;
  	lightBoxZ = 2.3*inch;
  	coneLargeRadius = lightBoxY/2. * sqrt(2.);
    coneSmallRadius = 0.5*inch;
    coneLength = 1.5*inch;
}



void LeribssBetaDet::BetaLine(G4RotationMatrix *pRot, 
                           G4ThreeVector &tlate, 
                           const G4String &pName, 
                           G4LogicalVolume *pMotherLogical,  
                           G4int pCopyNo)
{

	G4Material* BC408Material = materialsManager->GetBC408NoLight();
    G4Tubs* betaLine = new G4Tubs("betaLine", 
                                   betaInnRad, 
                                   betaOutRad, 
                                   betaLength/2., 
                                   0.0*M_PI, 
                                   2.0*M_PI);
 
    G4LogicalVolume* betaLineLogic = new G4LogicalVolume(betaLine, 
                                                         BC408Material, 
                                                         "betaLineLogic");
     

	betaLineLogic->SetVisAttributes(betaDetVisAtt);

    //const G4ThreeVector posBeamLine(0.0,0.0,-betaLength/4.);
    const G4ThreeVector posBeamLine(0.0,0.0,0.0);
	new G4PVPlacement(0, posBeamLine, betaLineLogic, pName, pMotherLogical, 0, 0 );
}


void LeribssBetaDet::LightCollection(G4RotationMatrix *pRot, 
                                     G4ThreeVector &tlate, 
                                     const G4String &pName, 
                                     G4LogicalVolume *pMotherLogical,  
                                     G4int pCopyNo)
{

	G4Material* BC408Material = materialsManager->GetBC408NoLight();

  	G4Box* lightBox1 = new G4Box("lightBox1",
  	                              lightBoxX/2., 
  	                              lightBoxY/2., 
  	                              lightBoxZ/2.);
  	
  	G4Tubs* lightBox2 = new G4Tubs("lightBox2", 
                                   0.0 ,
                                   betaInnRad, 
                                   lightBoxZ, 
                                   0.0*M_PI, 
                                   2.0*M_PI);
                                   
    const G4ThreeVector position12(-betaInnRad,0.0,0.0);                               
  	G4SubtractionSolid* lightBox12 = new G4SubtractionSolid("lightBox12", 
  	                                                         lightBox1,
  	                                                         lightBox2, 
  	                                                         0,
  	                                                         position12);

    G4Cons* lightBox3 = new G4Cons("lightbox3", 
                                    0.0, 
                                    coneLargeRadius, 
                                    0.0, 
                                    coneSmallRadius, 
                                    coneLength/2., 
                                    0.0*M_PI, 
                                    2.0*M_PI);
                                    
  	G4Box* lightBox4 = new G4Box("lightBox4", 
  	                             lightBoxY/2.,
  	                             lightBoxY/2., 
  	                             coneLength/2.);  	 
 
    const G4ThreeVector position34(0.0,0.0,0.0); 
  	G4IntersectionSolid* lightBox34 = new G4IntersectionSolid("lightBox34", 
  	                                                         lightBox3,
  	                                                         lightBox4, 
  	                                                         0,
  	                                                         position34);                                                        

    G4RotationMatrix rotLightColl;
    rotLightColl.rotateY(90.*deg);
    G4ThreeVector transLightColl(coneLength,0,0.0);
    G4Transform3D transfLightColl(rotLightColl,transLightColl);

    G4UnionSolid* lightCollBox = new G4UnionSolid("lightCollBox", 
                                                   lightBox12,
  	                                               lightBox34, 
  	                                               transfLightColl);
  	                                                          
    G4LogicalVolume* lightCollLogic = new G4LogicalVolume(lightCollBox, 
                                                          BC408Material, 
                                                         "lightCollLogic");                                                           	
  	lightCollLogic->SetVisAttributes(betaDetVisAtt);
  	G4ThreeVector posLightCollection( betaInnRad,
  	                                  0.0,
  	                                  -2.* betaLength/4. - lightBoxZ/2.);
	new G4PVPlacement(0, posLightCollection, lightCollLogic, pName, pMotherLogical, 0, 0 ); 


    G4RotationMatrix* rotLightColl2 = new G4RotationMatrix();
    rotLightColl2->rotateZ(180.*deg);
  	posLightCollection[0] = (-betaInnRad);
	new G4PVPlacement(rotLightColl2, 
	                  posLightCollection, 
	                  lightCollLogic, 
	                  pName, 
	                  pMotherLogical, 
	                  0, 
	                  1 );
  	 
}

void LeribssBetaDet::PMTtube(G4RotationMatrix *pRot, 
                                     G4ThreeVector &tlate, 
                                     const G4String &pName, 
                                     G4LogicalVolume *pMotherLogical,  
                                     G4int pCopyNo)
{

	G4double thickness = 1.0*mm;
	G4double outerRadius = 0.5*inch;
	G4double innerRadius = outerRadius - thickness;
	
	G4double startPhi = 0.0 * degree;
	G4double deltaPhi = 360.0 * degree;
	G4double halfLength = 2.5*inch;
	

	G4Tubs* PMTSolid = new G4Tubs("PMTSolid", 
	                              innerRadius, 
	                              outerRadius, 
	                              halfLength, 
	                              startPhi, 
	                              deltaPhi);
	G4Material* PMTMaterial = materialsManager->GetBorosilicate();
	G4LogicalVolume* PMTLogic = new G4LogicalVolume( PMTSolid, 
	                                                 PMTMaterial, 
	                                                 "PMTLogic");
	PMTLogic->SetVisAttributes(betaDetVisAtt);                                                 
	G4RotationMatrix* rotPmt = new G4RotationMatrix();
    rotPmt->rotateY(90.*deg);
    
    G4ThreeVector posPmt( betaInnRad + lightBoxX + halfLength,
  	                      0.0,
  	                      -2.* betaLength/4. - lightBoxZ/2.);
  	                      
  	new G4PVPlacement(rotPmt, 
	                  posPmt, 
	                  PMTLogic, 
	                  pName, 
	                  pMotherLogical, 
	                  0, 
	                  0 );
	 
	posPmt[0] = -betaInnRad - lightBoxX - halfLength ;                
	new G4PVPlacement(rotPmt, 
	                  posPmt, 
	                  PMTLogic, 
	                  pName, 
	                  pMotherLogical, 
	                  0, 
	                  0 );
}	
	
void LeribssBetaDet::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{


    BetaLine(pRot, tlate, pName+"betaLine",  pMotherLogical, pCopyNo);
    LightCollection(pRot, tlate, pName+"LightCollection",  pMotherLogical, pCopyNo);
    PMTtube(pRot, tlate, pName+"PMTtube",  pMotherLogical, pCopyNo);
}

