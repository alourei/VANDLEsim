// $Id: ArgonneVandleArray.cc 206.05.2017 A Fijalkowska $
//
/// \file ArgonneVandleArray.cc
/// \brief Implementation of the ArgonneVandleArray class
//
//

#include "ArgonneVandleArray.hh"
#include "G4SystemOfUnits.hh"


ArgonneVandleArray::ArgonneVandleArray()
{
	SetBasicSizes();	
	int smallSize = 0;
	smallBar = new VANDLEBar(smallSize);
	int mediumSize = 1;
	mediumBar = new VANDLEBar(mediumSize);
} 

ArgonneVandleArray::~ArgonneVandleArray()
{
    std::vector<G4RotationMatrix*>::iterator it;
	for (it = rotations.begin(); it != rotations.end(); ++it)
        delete (*it);
    rotations.clear();
}

void ArgonneVandleArray::SetBasicSizes()
{
    smallArchRadius = 50.8574*cm;
    mediumArchRadius = 100.362*cm;
  	nrOfSmallModules = 14;
  	nrOfMediumModules = 26;  	
	
}



void ArgonneVandleArray::PlaceSmallBars(G4RotationMatrix *pRot, 
                                       G4ThreeVector &tlate, 
                                       const G4String &pName, 
                                       G4LogicalVolume *pMotherLogical,  
                                       G4int pCopyNo)
{
	const G4ThreeVector zero(0.0,0.0,0.0);
	G4ThreeVector centerPosition(0.0,smallArchRadius,0.0);
	G4double angle = 5.0*deg;
	const G4double deltaAngle = 4.5*deg;
	
    for( int i = 0; i < nrOfSmallModules; ++i )
    {
		if(i == 7)
		    angle = 147.6*deg;
		    
        G4RotationMatrix rot;
        rot.rotateZ(-90*deg+angle);		
	
		centerPosition[0] = smallArchRadius * cos(angle);
		centerPosition[1] = smallArchRadius * sin(angle);
        G4Transform3D transform(rot, centerPosition);

		G4String name = pName+"_VandleBar";
				
        smallBar->Place(transform, 
                        name, 
                        pMotherLogical, 
                        (nrOfSmallModules + nrOfMediumModules) * pCopyNo + i);
		angle += deltaAngle;
	}
	
}


void ArgonneVandleArray::PlaceMediumBars(G4RotationMatrix *pRot, 
                                        G4ThreeVector &tlate, 
                                        const G4String &pName, 
                                        G4LogicalVolume *pMotherLogical,  
                                        G4int pCopyNo)
{
	
	const G4ThreeVector zero(0.0,0.0,0.0);
	G4ThreeVector centerPosition(0.0,mediumArchRadius,0.0);
	G4double angle = 39.9*deg;
	G4double deltaAngle;
	for( int i = 0; i < nrOfMediumModules; ++i )
	//for( int i = 0; i < 1; ++i )
	{

		if(i == 3 || i == 7 || i ==11 || i == 13 || i == 17 || i == 21)
		    deltaAngle = 4.95*deg;
		else
		    deltaAngle = 3.7*deg;
		G4RotationMatrix rot;	
		rot.rotateZ(-90*deg+angle);	

	
		centerPosition[0] = mediumArchRadius * cos(angle);
		centerPosition[1] = mediumArchRadius * sin(angle);
        G4Transform3D transform(rot, centerPosition);
        
		G4String name = pName+"_VandleBar";
		int copyNr = (nrOfSmallModules + nrOfMediumModules) * pCopyNo + nrOfSmallModules + i;
        mediumBar->Place(transform, 
		                 name, 
		                 pMotherLogical, 
		                 copyNr);   
		angle += deltaAngle;
	}
	
}


void ArgonneVandleArray::Place(G4RotationMatrix *pRot, 
                              G4ThreeVector &tlate, 
                              const G4String &pName, 
                              G4LogicalVolume *pMotherLogical,  
                              G4int pCopyNo)
{
	
	PlaceSmallBars(pRot, tlate, pName, pMotherLogical, pCopyNo);
	PlaceMediumBars(pRot, tlate, pName, pMotherLogical, pCopyNo);
}

                         
void ArgonneVandleArray::ConstructSDandField()
{
	smallBar->ConstructSDandField();
	mediumBar->ConstructSDandField();
}


