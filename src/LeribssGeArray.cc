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
  	//HPGeXOffset= -2.5*inch;
  	HPGeXOffset= -1.7*inch; //AF testy
  	//HPGeXOffset = -6.*inch;   	
	
}
  
void LeribssGeArray::Place(G4RotationMatrix *pRot, 
                           G4ThreeVector &tlate, 
                           const G4String &pName, 
                           G4LogicalVolume *pMotherLogical,  
                           G4int pCopyNo)
{
	G4RotationMatrix* rotHPGe1 = new G4RotationMatrix();
	

  	G4ThreeVector posHPGe1(HPGeXOffset, 0.0,0.0);	
	HPGeDet->Place(rotHPGe1, posHPGe1," ", pMotherLogical, 2*pCopyNo);
	

  	G4RotationMatrix* rotHPGe2 = new G4RotationMatrix();
	rotHPGe2->rotateZ(180.0*degree);

  	G4ThreeVector posHPGe2(-(HPGeXOffset), 0.0,0.0);	
	HPGeDet->Place(rotHPGe2, posHPGe2," ", pMotherLogical, 2*pCopyNo+1);
}              


void LeribssGeArray::ConstructSDandField()
{
	HPGeDet->ConstructSDandField();
}
