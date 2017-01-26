// $Id: LeribssGeArray.cc 24.01.2016 A Fijalkowska $
//
/// \file LeribssGeArray.cc
/// \brief Implementation of the LeribssGeArray class, mostly Sergey's Ilyushkin code
//
//

#include "LeribssGeArray.hh"
#include "G4SystemOfUnits.hh"


LeribssGeArray::LeribssGeArray()
{
	SetBasicSizes();	
	HPGeDet = new HPGe();	
} 

LeribssGeArray::~LeribssGeArray()
{
	
}

void LeribssGeArray::SetBasicSizes()
{
	G4double inch = 2.54*cm;
  	HPGeXOffset= -2.5*inch;
  	//HPGeXOffset = -6.*inch;   	
	
}
  
void LeribssGeArray::Place(G4RotationMatrix *pRot, 
                           G4ThreeVector &tlate, 
                           const G4String &pName, 
                           G4LogicalVolume *pMotherLogical,  
                           G4int pCopyNo)
{
	double phi = 0.;
    double theta = 0.;
    double psi = 0.;
        
	if(pRot!=0)
	{
	    phi = pRot->getPhi();
        theta = pRot->getTheta();
        psi = pRot->getPsi();
    }
	G4RotationMatrix* rotHPGe1 = new G4RotationMatrix();
	//rotHPGe1->setPhi(phi);
	//rotHPGe1->setTheta(theta);
	//rotHPGe1->setPsi(psi);
	

  	G4ThreeVector posHPGe1(HPGeXOffset, 0.0,0.0);	
  	//G4ThreeVector posHPGe1(0.0, 0.0,0.0);
	HPGeDet->Place(rotHPGe1, posHPGe1," ", pMotherLogical, 2*pCopyNo);
	

  	G4RotationMatrix* rotHPGe2 = new G4RotationMatrix();
	//rotHPGe2->setPhi(phi);
	//rotHPGe2->setTheta(theta + 180.0*degree);
	//rotHPGe2->setPsi(psi);
	rotHPGe2->rotateZ(180.0*degree);

  	G4ThreeVector posHPGe2(-(HPGeXOffset), 0.0,0.0);	
	HPGeDet->Place(rotHPGe2, posHPGe2," ", pMotherLogical, 2*pCopyNo+1);
}              

/* 
void LeribssGeArray::Place(G4RotationMatrix *pRot, 
                           G4ThreeVector &tlate, 
                           const G4String &pName, 
                           G4LogicalVolume *pMotherLogical,  
                           G4int pCopyNo)
{
	double 	getPhi () const
    double 	getTheta () const
    double 	getPsi () const 


	G4RotationMatrix* rotHPGe1 = new G4RotationMatrix(pRot);
	rotHPGe1->rotateZ(0.0*degree);
  	G4ThreeVector posHPGe1(HPGeXOffset, 0.0,0.0);	
	HPGeDet->Place(rotHPGe1, posHPGe1," ", pMotherLogical, 2*pCopyNo);
	

  	G4RotationMatrix* rotHPGe2 = new G4RotationMatrix(pRot);
	rotHPGe2->rotateZ(180.0*degree);
  	G4ThreeVector posHPGe2(-(HPGeXOffset), 0.0,0.0);	
	HPGeDet->Place(rotHPGe2, posHPGe2," ", pMotherLogical, 2*pCopyNo+1);
} */

void LeribssGeArray::ConstructSDandField()
{
	HPGeDet->ConstructSDandField();
}
