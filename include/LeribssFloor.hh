// $Id: LeribssFloor.hh 29.01.2016 A Fijalkowska $
//
/// \file LeribssFloor.hh
/// \brief Definition of the LeribssFloor class
//
#ifndef LeribssFloor_H
#define LeribssFloor_H1
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "MaterialsManager.hh"
#include "G4Material.hh"
class LeribssFloor
{
  public:

    LeribssFloor(); 
    virtual ~LeribssFloor(); 
    void Place(G4RotationMatrix *pRot, 
               G4ThreeVector &tlate, 
               const G4String &pName, 
               G4LogicalVolume *pMotherLogical,  
               G4int pCopyNo = 0);
               
  private:
     void SetBasicSizes();
     
     void MakeCeilFloor(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo);                                                        
                                                                                     
     MaterialsManager* materialsManager;
     //G4VisAttributes* supportVisAtt;
     
     G4double inch; 
};



#endif
