// $Id: LeribssBeam.cc 26.01.2016 A Fijalkowska $
//
/// \file LeribssBeam.cc
/// \brief Implementation of the LeribssBeam class, mostly Sergey's Ilyushkin code
//
//

#include "LeribssBeam.hh"

#include "G4Transform3D.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

LeribssBeam::LeribssBeam()
{
	SetBasicSizes();	
    materialsManager = MaterialsManager::GetInstance();
} 

LeribssBeam::~LeribssBeam()
{
	
}

void LeribssBeam::SetBasicSizes()
{
	inch = 2.54*cm;	

	
}


void LeribssBeam::MakeIBeam(G4RotationMatrix *pRot, 
                            G4ThreeVector &tlate, 
                            const G4String &pName, 
                            G4LogicalVolume *pMotherLogical,  
                            G4int pCopyNo)
{

	G4Material* ironMaterial = materialsManager->GetIron();

	const G4double railX = 12.431*inch;
    const G4double railY = 30.249*inch;  
     
    const G4double railYOffset = 0.125*inch;  
    const G4double railZOffset = 0.0;//15.75*inch; 
     	
	G4double IBeamDis = 2.0*railX;

	G4double boxhWidth = 2.25*inch;
	G4double boxhThickness = 0.25*inch;
	G4double boxhLength = 50.0*inch;
	
	G4double boxvWidth = boxhThickness;
	G4double boxvThickness = 2.0*inch;
	G4double boxvLength = boxhLength;
	
	G4Box* boxV = new G4Box("boxV", boxvWidth, boxvThickness, boxvLength);
	G4Box* boxU = new G4Box("boxU", boxhWidth, boxhThickness, boxhLength);
	G4Box* boxB = new G4Box("boxB", boxhWidth, boxhThickness, boxhLength);
     
    G4RotationMatrix rotBoxV;
    rotBoxV.rotateX(0.*deg);
    G4ThreeVector  translationBoxV(0.0,(boxvThickness+boxhThickness),0.0);
    G4Transform3D transformBoxV(rotBoxV,translationBoxV);
    G4UnionSolid* boxVU = new G4UnionSolid("boxUBV", boxV, boxU, transformBoxV);
     
    G4RotationMatrix rotBoxB;
    rotBoxB.rotateX(0.*deg);
    G4ThreeVector  translationBoxB(0.0,-(boxvThickness+boxhThickness),0.0);
    G4Transform3D transformBoxB(rotBoxB,translationBoxB);
    G4UnionSolid* boxVUB = new G4UnionSolid("boxVUB", boxVU, boxB, transformBoxB);         
     
    G4RotationMatrix rotIBeam;
    rotIBeam.rotateX(0.*deg);
    G4ThreeVector  translationIBeam(IBeamDis,0.0,0.0);
    G4Transform3D transformIBeam(rotIBeam,translationIBeam);
    G4UnionSolid* IBeam = new G4UnionSolid("IBeam", boxVUB, boxVUB, transformIBeam);
     
    G4LogicalVolume* IBeamLogic = new G4LogicalVolume(IBeam, ironMaterial, "IBeamLogic");
     

    G4VisAttributes* IBeamVisAtt = new G4VisAttributes( G4Colour(139./255.,0./255.,0./255.));
	IBeamVisAtt->SetForceAuxEdgeVisible(true);
	IBeamVisAtt->SetForceSolid(true);
	IBeamLogic->SetVisAttributes(IBeamVisAtt);
     
    const G4ThreeVector posIBeam(-IBeamDis/2.0,railY+railYOffset,railZOffset/2.0);
	new G4PVPlacement( 0, posIBeam, IBeamLogic, pName, pMotherLogical, 0, 0 );
}


void LeribssBeam::MakeCeilIBeam(G4RotationMatrix *pRot, 
                                G4ThreeVector &tlate, 
                                const G4String &pName, 
                                G4LogicalVolume *pMotherLogical,  
                                G4int pCopyNo)
{

	G4Material* ironMaterial = materialsManager->GetIron();

    const G4double railY = 43.124*inch;  
     
    const G4double railYOffset = 0.125*inch;  
    const G4double railZOffsetCenter = 0.0;//15.75*inch; 
    const G4double railZOffset = 17.5*inch;

	G4double boxhWidth = 3.5*inch;
	G4double boxhThickness = 0.25*inch;
	G4double boxhLength = 30.0*inch;
	
	G4double boxvWidth = boxhThickness;
	G4double boxvThickness = 7.25*inch;
	G4double boxvLength = boxhLength;
	
	G4Box* boxV = new G4Box("boxV", boxvWidth, boxvThickness, boxvLength);
	G4Box* boxU = new G4Box("boxU", boxhWidth, boxhThickness, boxhLength);
	G4Box* boxB = new G4Box("boxB", boxhWidth, boxhThickness, boxhLength);
     
    G4RotationMatrix rotBoxV;
    rotBoxV.rotateX(0.*deg);
    G4ThreeVector  translationBoxV(0.0,(boxvThickness+boxhThickness),0.0);
    G4Transform3D transformBoxV(rotBoxV,translationBoxV);
    G4UnionSolid* boxVU = new G4UnionSolid("boxUBV", boxV, boxU, transformBoxV);
     
    G4RotationMatrix rotBoxB;
    rotBoxB.rotateX(0.*deg);
    G4ThreeVector  translationBoxB(0.0,-(boxvThickness+boxhThickness),0.0);
    G4Transform3D transformBoxB(rotBoxB,translationBoxB);
    G4UnionSolid* IBeam = new G4UnionSolid("IBeam", boxVU, boxB, transformBoxB);         
     
    G4LogicalVolume* ceilIBeamLogic = new G4LogicalVolume(IBeam, ironMaterial, "ceilIBeamLogic");
     
    G4VisAttributes* ceilIBeamVisAtt = new G4VisAttributes(G4Colour(139./255.,0./255.,0./255.));
	ceilIBeamVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
	ceilIBeamVisAtt->SetForceSolid(true);
	ceilIBeamLogic->SetVisAttributes(ceilIBeamVisAtt);

	
	G4RotationMatrix rotIBeam;
    rotIBeam.rotateY(-60.0*deg);
    const G4ThreeVector posIBeam(0.0,railY,railZOffsetCenter/2.0+railZOffset);
    G4Transform3D transformIBeam(rotIBeam,posIBeam);     
	new G4PVPlacement(transformIBeam, ceilIBeamLogic, pName, pMotherLogical, 0, 0);
}


void LeribssBeam::BeamLine(G4RotationMatrix *pRot, 
                                G4ThreeVector &tlate, 
                                const G4String &pName, 
                                G4LogicalVolume *pMotherLogical,  
                                G4int pCopyNo)
{

	G4Material* stainSteelMaterial = materialsManager->GetStainSteel();

	G4double BeamLine_inn_rad = 1.54*inch;
  	G4double BeamLine_out_rad = 1.7*inch;
  	G4double BeamLine_length = 30.0*inch;
  	
    G4Tubs* BeamLine = new G4Tubs("BeamLine", BeamLine_inn_rad, BeamLine_out_rad, BeamLine_length, 0.0*M_PI, 2.0*M_PI);
 
    G4LogicalVolume* beamLineLogic = new G4LogicalVolume(BeamLine, stainSteelMaterial, "beamLineLogic");
     
    G4VisAttributes* BeamLineVisAtt = new G4VisAttributes( G4Colour(0,140./255.,0));
	BeamLineVisAtt->SetForceAuxEdgeVisible(true);
	BeamLineVisAtt->SetForceSolid(true);
	beamLineLogic->SetVisAttributes(BeamLineVisAtt);

    const G4ThreeVector posBeamLine(0.0,0.0,0.0);
	new G4PVPlacement(0, posBeamLine, beamLineLogic, pName, pMotherLogical, 0, 0 );
}

void LeribssBeam::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{


    MakeIBeam(pRot, tlate, pName+"IBeam",  pMotherLogical, pCopyNo);
    MakeCeilIBeam(pRot, tlate, pName+"CeilIBeam",  pMotherLogical, pCopyNo);
    BeamLine(pRot, tlate, pName+"BeamLine",  pMotherLogical, pCopyNo);
}

