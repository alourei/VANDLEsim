// $Id: ArgonneLN2Tank.hh 10.05.2017 A Fijalkowska $
//
/// \file ArgonneLN2Tank.hh
/// \brief Definition of the ArgonneLN2Tank class
//
#ifndef ArgonneLN2Tank_H
#define ArgonneLN2Tank_H1
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "MaterialsManager.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"

class ArgonneLN2Tank
{
  public:

    ArgonneLN2Tank(); 
    virtual ~ArgonneLN2Tank(); 
    void Place(G4RotationMatrix *pRot, 
               G4ThreeVector &tlate, 
               const G4String &pName, 
               G4LogicalVolume *pMotherLogical,  
               G4int pCopyNo = 0);

               
  private:
     void SetBasicSizes();
                        
    G4LogicalVolume* MakeTankShell();
    void AddLN2(G4LogicalVolume* tankShellLogVol);                                                                                       
    MaterialsManager* materialsManager;
    G4double zdet[4];
  	G4double innR[4]; 
  	G4double outR[4];

};



#endif
