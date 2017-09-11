// $Id: ArgonneGeArray.cc 24.01.2016 A Fijalkowska $
//
/// \file ArgonneGeArray.cc
/// \brief Implementation of the ArgonneGeArray class, mostly Sergey's Ilyushkin code
//
//

#include "ArgonneGeArray.hh"
#include "G4SystemOfUnits.hh"


ArgonneGeArray::ArgonneGeArray()
{
	SetBasicSizes();	
	HPGeDet = new HPGe();	
} 

ArgonneGeArray::~ArgonneGeArray()
{
	
}

void ArgonneGeArray::SetBasicSizes()
{  	
	
}
  
void ArgonneGeArray::Place(G4RotationMatrix *pRot, 
                          G4ThreeVector &tlate, 
                          const G4String &pName, 
                          G4LogicalVolume *pMotherLogical,  
                          G4int pCopyNo)
{
	
	//G4double xOff = (1.5 + 50.5)*mm;
	//G4double zOff = 125.0*mm; 
	G4double xOff = 0*mm;
	G4double zOff = 35.92*mm; 
 
	G4double yOff = zOff;

  	G4RotationMatrix rotHPGe1;
	rotHPGe1.rotateY(90.0*degree);
	rotHPGe1.rotateX(45.0*degree);
  	const G4ThreeVector posHPGe1(-xOff,-yOff,zOff);	
  	G4Transform3D transformHPGe1(rotHPGe1,posHPGe1);
  	HPGeDet->Place(transformHPGe1," ", pMotherLogical, 2*pCopyNo);


  	G4RotationMatrix rotHPGe2;
	rotHPGe2.rotateY(-90.0*degree);
	rotHPGe2.rotateX(-45.0*degree);
  	const G4ThreeVector posHPGe2(xOff,-yOff,-zOff);
  	//const G4ThreeVector posHPGe2(0.0,0.0,0.0);	
  	G4Transform3D transformHPGe2(rotHPGe2,posHPGe2);
	HPGeDet->Place(transformHPGe2," ", pMotherLogical, 2*pCopyNo+1);

}

void ArgonneGeArray::ConstructSDandField()
{
	HPGeDet->ConstructSDandField();
}
