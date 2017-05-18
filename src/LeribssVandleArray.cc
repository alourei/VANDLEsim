// $Id: LeribssVandleArray.cc 26.01.2016 A Fijalkowska $
//
/// \file LeribssVandleArray.cc
/// \brief Implementation of the LeribssVandleArray class, mostly Sergey's Ilyushkin code
//
//

#include "LeribssVandleArray.hh"
#include "G4SystemOfUnits.hh"


LeribssVandleArray::LeribssVandleArray()
{
	SetBasicSizes();	
	vandleBar = new VANDLEBar(moduleSize);	
} 

LeribssVandleArray::~LeribssVandleArray()
{
	
}

void LeribssVandleArray::SetBasicSizes()
{
	moduleSize = 0;
	inch = 2.54*cm;
	//sqrt(20.438*20.438-(1.188976*1.188976/4.0))-0.5944882
    radius = 19.83486391938587150847*inch;
  	nrOfModules = 48;  	
	
}


void LeribssVandleArray::Place(G4RotationMatrix *pRot, 
                               G4ThreeVector &tlate, 
                               const G4String &pName, 
                               G4LogicalVolume *pMotherLogical,  
                               G4int pCopyNo)
{
	
	G4cout << "The Center Radius of the cylinder is :" 
	       << radius / cm 
	       << " cm" 
	       << G4endl;

	const G4ThreeVector zero(0.0,0.0,0.0);
	G4ThreeVector centerPosition(0.0,radius,0.0);

	G4RotationMatrix* rot = NULL;
	static G4double angle = M_PI/6.5;
	const G4double deltaAngle = 0.0945;


	for( int i = 0; i < nrOfModules; ++i ){
		rot = new G4RotationMatrix();
		//rot->rotateX(90.0*deg);	
		if(i == 24){angle = M_PI+M_PI/6.5;}
		rot->rotateZ(-angle);		
	
		centerPosition[0] = radius * cos(angle);
		centerPosition[1] = radius * sin(angle);
		
        
		G4String cylinderString( "Bar");
		std::stringstream cylinderModuleID;
		cylinderModuleID << i;
		cylinderString += cylinderModuleID.str();
          
		vandleBar->Place(rot, 
		                 centerPosition, 
		                 cylinderString, 
		                 pMotherLogical, 
		                 nrOfModules * pCopyNo + i);              
		angle += deltaAngle;
	}
}

void LeribssVandleArray::ConstructSDandField()
{
	vandleBar->ConstructSDandField();
}
