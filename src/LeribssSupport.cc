// $Id: LeribssSupport.cc 26.01.2016 A Fijalkowska $
//
/// \file LeribssSupport.cc
/// \brief Implementation of the LeribssSupport class, mostly Sergey's Ilyushkin code
//
//

#include "LeribssSupport.hh"

#include "G4Transform3D.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

LeribssSupport::LeribssSupport()
{
	SetBasicSizes();	
    materialsManager = MaterialsManager::GetInstance();
} 

LeribssSupport::~LeribssSupport()
{
	
}

void LeribssSupport::SetBasicSizes()
{
	inch = 2.54*cm;	
	     //boxM        	
    half_length_xm = 26.0*inch;    
    half_length_ym = 24.75*inch;    
    half_thickness_zm = 0.5*inch;      
    upCut = 8.5*inch;   
    bCut = 7.5*inch;  

     //boxUpCut     
    boxUpCutX = sqrt(2.0*upCut/2.0*upCut/2.0);    
    boxUpCutY = boxUpCutX;    
    boxUpCutZ = 2.0*half_thickness_zm;
    boxBCutAngle = atan(bCut/upCut)*(180.0/M_PI);

     //boxBCut
    boxBCutX = upCut*cos(atan(bCut/upCut));      
    boxBCutY = sqrt(upCut*upCut - boxBCutX*boxBCutX); 
    boxBCutZ = 2.0*half_thickness_zm;


    
    tubeLength = 14.75/2.0*inch;
    boxDist = 2.0*tubeLength + 2.0*half_thickness_zm; 
    //G4double boxPos = boxDist/2.0;
    boxOffset = 0.5*inch;
	
}


G4SubtractionSolid* LeribssSupport::MakeBracket()
{
	bracketBase_x = 5.0*inch;
	bracketBase_y = 2.5*inch;
	bracketBase_z = 0.5*inch;
	
	const G4double bracketCut_x = sqrt(bracketBase_x*bracketBase_x + bracketBase_x*bracketBase_x)/4.0;
	const G4double bracketCut_y = bracketCut_x;
	const G4double bracketCut_z = 2.0*bracketBase_z;
	
	G4Box* bracketBase = new G4Box("bracketBase", bracketBase_x, bracketBase_y, bracketBase_z);
	G4Box* bracketCut = new G4Box("bracketCut", bracketCut_x, bracketCut_y, bracketCut_z);
	
	G4RotationMatrix rotBracketCut;
    rotBracketCut.rotateZ(45.0*deg);
    const G4ThreeVector  translationBracketCut(-bracketBase_x,-bracketBase_y,0.0);
    G4Transform3D transformBracketCut(rotBracketCut,translationBracketCut);
    G4SubtractionSolid* bracket = new G4SubtractionSolid("bracket", 
                                                          bracketBase, 
                                                          bracketCut, 
                                                          transformBracketCut);
    return bracket;
     
     
	
}


void LeribssSupport::MakeDetectorPlate(G4RotationMatrix *pRot, 
                                                G4ThreeVector &tlate, 
                                                const G4String &pName, 
                                                G4LogicalVolume *pMotherLogical,  
                                                G4int pCopyNo)
{
	detPlate_x = 16.0/2.0*inch;
	detPlate_y = 8.860/2.0*inch;
	detPlate_z = 0.5/2.0*inch;
	
	const G4double detPlateAdd_x = detPlate_x;
	const G4double detPlateAdd_z = 1.0/2.0*inch;
	
	const G4double detPlateAddOffset = 2.93*inch;

	
	
	G4Box* detPlateBase = new G4Box("detPlateBase", 
	                                detPlate_x, 
	                                detPlate_y, 
	                                detPlate_z);
	
	const G4double height = 3.0*inch;
	const G4double width = 16.0*inch;
	const G4double thickness = 1.0*inch;
	const G4double radius = 0.75*inch;
	G4UnionSolid* detPlateAdd = roundedHalfPlate(height,width,thickness,radius);	

    G4RotationMatrix rotPlateAdd;
    rotPlateAdd.rotateZ(0.*deg);
    G4ThreeVector  translationPlateAdd(0.0,
                                       -detPlateAddOffset,
                                       detPlate_z+detPlateAdd_z);

    G4Transform3D transformPlateAdd(rotPlateAdd,translationPlateAdd);
    G4UnionSolid* detPlate = new G4UnionSolid("detPlate", 
                                               detPlateBase, 
                                               detPlateAdd, 
                                               transformPlateAdd);
     const G4double Plate1OffsetX = 18.790*inch;
     const G4double Plate1OffsetZ = half_thickness_zm + detPlate_z;

     
     G4LogicalVolume* detPlateLog = new G4LogicalVolume(detPlate, 
                                                        aluminiumMaterial, 
                                                        "supprotRingLog");
     detPlateLog->SetVisAttributes(supportVisAtt);
     G4RotationMatrix* rotPlate1 = new G4RotationMatrix();
     rotPlate1->rotateZ(-90.0*deg);
     const G4ThreeVector posDetPlate1(Plate1OffsetX,
                                      0.0,
                                      -boxDist/2.0-Plate1OffsetZ);
	 new G4PVPlacement( rotPlate1, 
	                    posDetPlate1, 
	                    detPlateLog, 
	                    pName, 
	                    pMotherLogical, 
	                    0, 
	                    0 ); 
	 
	 

     G4RotationMatrix* rotPlate2 = new G4RotationMatrix();
     rotPlate2->rotateZ(90.0*deg);
     const G4ThreeVector posDetPlate2(-Plate1OffsetX,0.0,-boxDist/2.0-Plate1OffsetZ);
	 new G4PVPlacement(rotPlate2, posDetPlate2, detPlateLog, pName, pMotherLogical, 0, 0 ); 
}

G4SubtractionSolid* LeribssSupport::MakeCeilingSupportBox()
{
	 const G4double ceilSuppThick = 0.5*inch;
     
     boxOut_x = 3.0*inch;
	 boxOut_y = 3.125*inch;
	 boxOut_z = 8.375*inch;
	
	 const G4double boxIn_x = boxOut_x - ceilSuppThick;
	 const G4double boxIn_y = boxOut_y - ceilSuppThick;
	 const G4double boxIn_z = 10.0*inch;
	
	 G4Box* boxOut = new G4Box("boxOut", boxOut_x, boxOut_y, boxOut_z);
     G4Box* boxIn = new G4Box("boxIn", boxIn_x, boxIn_y, boxIn_z);
     
     G4RotationMatrix rotBoxIn;
     rotBoxIn.rotateZ(0.0*deg);
     G4ThreeVector  translationBoxIn(0.0,0.0,0.0);
     G4Transform3D transformBoxIn(rotBoxIn,translationBoxIn);
     G4SubtractionSolid* ceilSupportBox = new G4SubtractionSolid("ceilSupportBox", 
                                                                 boxOut, 
                                                                 boxIn, 
                                                                 transformBoxIn);
     return ceilSupportBox;	 
		 
}
	
G4SubtractionSolid* LeribssSupport::MakeCeilSupport()		
{
		
	ceilSuppBase_x = 2.94*inch;
	ceilSuppBase_y = 4.5*inch;
	ceilSuppBase_z = 0.25*inch;
	
	const G4double xyCut = 4.497*inch;
	
	const G4double ceilSuppCut_x = sqrt(xyCut*xyCut + xyCut*xyCut)/2.0;
	const G4double ceilSuppCut_y = ceilSuppCut_x;
	const G4double ceilSuppCut_z = 2.0*ceilSuppBase_z;
	
	G4Box* ceilSuppBase = new G4Box("ceilSuppBase", 
	                                 ceilSuppBase_x, 
	                                 ceilSuppBase_y, 
	                                 ceilSuppBase_z);
	G4Box* ceilSuppCut = new G4Box("ceilSuppCut", 
	                                ceilSuppCut_x, 
	                                ceilSuppCut_y, 
	                                ceilSuppCut_z);
	
	G4RotationMatrix rotceilSuppCut;
     rotceilSuppCut.rotateZ(45.0*deg);
     const G4ThreeVector translationceilSuppCut(-ceilSuppBase_x,
                                                -ceilSuppBase_y,
                                                0.0);
     G4Transform3D transformceilSuppCut(rotceilSuppCut,translationceilSuppCut);
     G4SubtractionSolid* ceilSupport = new G4SubtractionSolid("ceilSupport", 
                                                               ceilSuppBase, 
                                                               ceilSuppCut, 
                                                               transformceilSuppCut);
     
	return ceilSupport;
} 

G4UnionSolid* LeribssSupport::MatekTotalCeilingSupport(G4SubtractionSolid* ceilSupportBox, 
                                                       G4SubtractionSolid* ceilSupport)		
{

    G4RotationMatrix rot;

    G4ThreeVector translSuppL((boxOut_x - ceilSuppBase_x + 2.0*ceilBracketBase_z),
                               -(ceilSuppBase_y+boxOut_y),
                               boxOut_z+ceilSuppBase_z);
    G4Transform3D transformSuppL(rot,translSuppL);
    G4UnionSolid* railSupportL = new G4UnionSolid("railSupportL", 
                                                  ceilSupportBox, 
                                                  ceilSupport, 
                                                  transformSuppL);


    G4ThreeVector  translSuppR((boxOut_x - ceilSuppBase_x + 2.0*ceilBracketBase_z),
                              -(ceilSuppBase_y+boxOut_y),
                              -(boxOut_z+ceilSuppBase_z));
    G4Transform3D transformSuppR(rot,translSuppR);
    G4UnionSolid* railSupportR = new G4UnionSolid("railSupportR", 
                                                   railSupportL, 
                                                   ceilSupport, 
                                                   transformSuppR);
    return railSupportR;
}    
     

G4SubtractionSolid* LeribssSupport::MakeCeilingSupportBracket()
{
    ceilBracketBase_x = 1.875*inch;
	ceilBracketBase_y = 1.75*inch;
	ceilBracketBase_z = 0.25*inch;
	
	const G4double ceilBracketCutX = 2.812*inch;
	
	const G4double ceilBracketCut_x = sin(atan(ceilBracketCutX/(2.0*ceilBracketBase_y)))
	                                  *2.0*ceilBracketBase_y;
	const G4double ceilBracketCut_y = cos(atan(ceilBracketCutX/(2.0*ceilBracketBase_y)))
	                                  *2.0*ceilBracketBase_y;
	const G4double ceilBracketCut_z = 2.0*ceilBracketBase_z;
	
	G4double BracketCutAngle = atan(ceilBracketCutX/(2.0*ceilBracketBase_y))*(180.0/M_PI);
	
	G4Box* ceilBracketBase = new G4Box("ceilBracketBase", 
	                                    ceilBracketBase_x, 
	                                    ceilBracketBase_y, 
	                                    ceilBracketBase_z);
	G4Box* ceilBracketCut = new G4Box("ceilBracketCut", 
	                                   ceilBracketCut_x, 
	                                   ceilBracketCut_y, 
	                                   ceilBracketCut_z);

	G4RotationMatrix rotceilBracketCut;
    rotceilBracketCut.rotateZ(-BracketCutAngle*deg);
    const G4ThreeVector translationceilBracketCut(ceilBracketBase_x,-ceilBracketBase_y,0.0);
    G4Transform3D transformceilBracketCut(rotceilBracketCut,translationceilBracketCut);
    G4SubtractionSolid* ceilBracket = new G4SubtractionSolid("ceilBracket", 
                                                              ceilBracketBase, 
                                                              ceilBracketCut, 
                                                              transformceilBracketCut);
    return ceilBracket;
}

G4UnionSolid* LeribssSupport::MakeHPGeHolderExtra()
{

    HPGeholderBox_x = 2.975*inch;
	HPGeholderBox_y = HPGeholderBox_x;
	HPGeholderBox_z = 3.90625*inch;
	
	const G4double HPGeholderBoxCut_x = 2.6*inch;
	const G4double HPGeholderBoxCut_y = HPGeholderBoxCut_x;
	const G4double HPGeholderBoxCut_z = 2.0*HPGeholderBox_z;
	
	G4Box* HPGeholderBoxBase = new G4Box("HPGeholderBoxBase", 
	                                      HPGeholderBox_x, 
	                                      HPGeholderBox_y, 
	                                      HPGeholderBox_z);
	G4Box* HPGeholderBoxCut = new G4Box("HPGeholderBoxCut", 
	                                     HPGeholderBoxCut_x, 
	                                     HPGeholderBoxCut_y, 
	                                     HPGeholderBoxCut_z);

	G4RotationMatrix rotHPGeholderBoxCut;
    rotHPGeholderBoxCut.rotateZ(0.0*deg);
    const G4ThreeVector  translationHPGeholderBoxCut(0.0,0.0,0.0);
    G4Transform3D transformHPGeholderBoxCut(rotHPGeholderBoxCut,translationHPGeholderBoxCut);
    G4SubtractionSolid* HPGeholderBox = new G4SubtractionSolid("HPGeholderBox", 
                                                                HPGeholderBoxBase, 
                                                                HPGeholderBoxCut, 
                                                                transformHPGeholderBoxCut);
    //return HPGeholderBox;

	HPGeholderBoxExtra_x = 2.0*inch;
	HPGeholderBoxExtra_y = 0.25*inch;
	HPGeholderBoxExtra_z = HPGeholderBoxExtra_y;
	
	G4Box* HPGeholderExtra = new G4Box("HPGeholderExtra", 
	                                    HPGeholderBoxExtra_x, 
	                                    HPGeholderBoxExtra_y, 
	                                    HPGeholderBoxExtra_z);

	G4RotationMatrix rot;
    const G4ThreeVector translation1(0.0,
                                    HPGeholderBox_y+HPGeholderBoxExtra_y,
                                    -(HPGeholderBox_z-HPGeholderBoxExtra_z));
    G4Transform3D transform1(rot,translation1);
    G4UnionSolid* HPGeholderBoxExtra1 = new G4UnionSolid("HPGeholderBoxExtra1", 
                                                          HPGeholderBox, 
                                                          HPGeholderExtra, 
                                                          transform1);


     const G4ThreeVector translation2(0.0,
                                     -(HPGeholderBox_y+HPGeholderBoxExtra_y),
                                     -(HPGeholderBox_z-HPGeholderBoxExtra_z));
     G4Transform3D transform2(rot,translation2);
     G4UnionSolid* HPGeholderBoxExtra2 = new G4UnionSolid("HPGeholderBoxExtra2", 
                                                           HPGeholderBoxExtra1, 
                                                           HPGeholderExtra, 
                                                           transform2);
     return HPGeholderBoxExtra2;
}     

 
G4UnionSolid* LeribssSupport::MakeHPGeBoxRing(G4UnionSolid* HPGeholderBoxExtra2)
{
    G4double holderRingRad = 6.0*inch;
    const G4double holderRingLength = 0.375*inch;
    	
    G4double holderRingCut_x = 2.595*inch;
    G4double holderRingCut_y = holderRingCut_x;
    G4double holderRingCut_z = 2.0*holderRingLength;
    	
    G4Tubs* holderRingTube = new G4Tubs("holderRingTube", 
                                        0.0, 
                                        holderRingRad, 
                                        holderRingLength, 
                                        0.0*M_PI, 
                                        2.0*M_PI);
    G4Box* holderRingCut = new G4Box("holderRingCut", 
                                      holderRingCut_x, 
                                      holderRingCut_y, 
                                      holderRingCut_z);
    	
    G4RotationMatrix rot;
    const G4ThreeVector  translHolderRingCut(0.0,0.0,0.0);
    G4Transform3D transformHolderRingCut(rot,translHolderRingCut);
    G4SubtractionSolid* holderRing = new G4SubtractionSolid("holderRing", 
                                                             holderRingTube, 
                                                             holderRingCut, 
                                                             transformHolderRingCut);

    	

     const G4ThreeVector translHolderRing(0.0,
                                          0.0,
                                          -(HPGeholderBox_z+holderRingLength));
     G4Transform3D transformHolderRing(rot,translHolderRing);
     G4UnionSolid* HPGeholderNoHolder = new G4UnionSolid("HPGeholderNoHolder", 
                                                          HPGeholderBoxExtra2, 
                                                          holderRing, 
                                                          transformHolderRing);	
     return HPGeholderNoHolder;
	
}   


G4UnionSolid* LeribssSupport::MakeHPGeHolderPlate(G4UnionSolid* HPGeholderNoHolder)
{
	G4double holderPlate_x = 7.31*inch;
    G4double holderPlate_y = 7.0*inch;
    G4double holderPlate_z = 0.44*inch;
    	
    G4double holderPlateCut_x = 1.35*HPGeholderBox_x;
    G4double holderPlateCut_y = holderPlateCut_x;
    G4double holderPlateCut_z = 2.0*holderPlate_z;

    G4Box* holderPlateBox = new G4Box("holderPlateBox", 
                                      holderPlate_x, 
                                      holderPlate_y, 
                                      holderPlate_z);
    G4Box* holderPlateCut = new G4Box("holderPlateCut", 
                                       holderPlateCut_x, 
                                       holderPlateCut_y, 
                                       holderPlateCut_z);
    	
    G4RotationMatrix rot;

    const G4ThreeVector translationholderPlateCut(0.0,0.0,0.0);
    G4Transform3D transformholderPlateCut(rot,translationholderPlateCut);
    G4SubtractionSolid* holderPlate = new G4SubtractionSolid("holderPlate",
                                                              holderPlateBox, 
                                                              holderPlateCut, 
                                                              transformholderPlateCut);


    const G4ThreeVector translationholderPlate(0.0,
                                               0.0,
                                               -(HPGeholderBox_z-holderPlate_z));
    G4Transform3D transformholderPlate(rot,translationholderPlate);
    G4UnionSolid* HPGeholder = new G4UnionSolid("HPGeholder", 
                                                 HPGeholderNoHolder, 
                                                 holderPlate, 
                                                 transformholderPlate);
	return HPGeholder;
}


void LeribssSupport::MakeHPGeHolder(G4RotationMatrix *pRot, 
                               G4ThreeVector &tlate, 
                               const G4String &pName, 
                               G4LogicalVolume *pMotherLogical,  
                               G4int pCopyNo)
{
     //make HPGe holders
     G4UnionSolid* HPGeholderBoxExtra2 = MakeHPGeHolderExtra();

          
     // make HPGe holder box ring
     G4UnionSolid* HPGeholderNoHolder = MakeHPGeBoxRing(HPGeholderBoxExtra2);

          
     //HPGe holder plate
     G4UnionSolid* HPGeholder = MakeHPGeHolderPlate(HPGeholderNoHolder);     
     
     G4double HPGeHolderXOffset = -(15.860-(3.90625-2.0*0.44-1.0))*inch;
     
     G4LogicalVolume* HPGeholderLogic = new G4LogicalVolume(HPGeholder, 
                                                            aluminiumMaterial, 
                                                            "HPGeholderLogic");
     HPGeholderLogic->SetVisAttributes(supportVisAtt);
     const G4ThreeVector posHPGeholder1(HPGeHolderXOffset,0.0,0.0);
     G4RotationMatrix* rotHPGeHolder1 = new G4RotationMatrix();
     rotHPGeHolder1->rotateY(-90.0*deg);
	 new G4PVPlacement( rotHPGeHolder1, 
	                    posHPGeholder1, 
	                    HPGeholderLogic, 
	                    pName+"HPGeHolder1", 
	                    pMotherLogical, 
	                    0, 
	                    0 );
	 

     const G4ThreeVector posHPGeholder2(-HPGeHolderXOffset,0.0,0.0);
     G4RotationMatrix* rotHPGeHolder2 = new G4RotationMatrix();
     rotHPGeHolder2->rotateY(90.0*deg);
	 new G4PVPlacement( rotHPGeHolder2, 
	                    posHPGeholder2, 
	                    HPGeholderLogic, 
	                    pName+"HPGeHolder2",  
	                    pMotherLogical, 
	                    0, 
	                    1 );
}


void LeribssSupport::MakeSupportRing(G4RotationMatrix *pRot, 
                                     G4ThreeVector &tlate, 
                                     const G4String &pName, 
                                     G4LogicalVolume *pMotherLogical,  
                                     G4int pCopyNo)
    
{   
    const G4double radiusHole = 20.438*inch;
    G4double lengthCyl = boxDist + 2.0*half_thickness_zm;    	
	G4Tubs* cylinderHole = new G4Tubs("cylinderHole", 
	                                  0.0, 
	                                  radiusHole, 
	                                  lengthCyl, 
	                                  0.0*M_PI, 
	                                  2.0*M_PI);
	
	
	G4Box* boxM = new G4Box("boxM", half_length_xm, half_length_ym, half_thickness_zm);

	const G4double holeOffset = 0.0*inch;	
     G4SubtractionSolid* boxMHole = new G4SubtractionSolid("boxhvHole", 
                                                           boxM, 
                                                           cylinderHole, 
                                                           0, 
                                                           G4ThreeVector(0.0,holeOffset,0.0));


     //make supportRing top cuts 
     G4Box* boxUpCut = new G4Box("boxUpCut", boxUpCutX, boxUpCutY, boxUpCutZ);
     
     G4RotationMatrix rotBoxUpCut1;
     rotBoxUpCut1.rotateZ(45.0*deg);
     G4ThreeVector  translationBoxUpCut1(-half_length_xm, half_length_ym, 0.0);
     G4Transform3D transformBoxUpCut1(rotBoxUpCut1,translationBoxUpCut1);
     G4SubtractionSolid* boxUpCut1 = new G4SubtractionSolid("boxUpCut1", 
                                                            boxMHole, 
                                                            boxUpCut, 
                                                            transformBoxUpCut1);

     G4RotationMatrix rotBoxUpCut2;
     rotBoxUpCut2.rotateZ(45.0*deg);
     G4ThreeVector  translationBoxUpCut2(half_length_xm, half_length_ym, 0.0);
     G4Transform3D transformBoxUpCut2(rotBoxUpCut2,translationBoxUpCut2);
     G4SubtractionSolid* boxUpCut12 = new G4SubtractionSolid("boxUpCut12", 
                                                              boxUpCut1, 
                                                              boxUpCut, 
                                                              transformBoxUpCut2);


     //make supportRing bottom cuts    
     G4Box* boxBCut = new G4Box("boxBCut", boxBCutX, boxBCutY, boxBCutZ);   
     G4RotationMatrix rotBoxBCut1;
     rotBoxBCut1.rotateZ(-boxBCutAngle*deg);
     G4ThreeVector  translationBoxBCut1(-half_length_xm,-half_length_ym,0.0);
     G4Transform3D transformBoxBCut1(rotBoxBCut1,translationBoxBCut1);
     G4SubtractionSolid* boxBCut1 = new G4SubtractionSolid("boxBCut1", 
                                                            boxUpCut12, 
                                                            boxBCut, 
                                                            transformBoxBCut1);

     G4RotationMatrix rotBoxBCut2;
     rotBoxBCut2.rotateZ(boxBCutAngle*deg);
     G4ThreeVector  translationBoxBCut2(half_length_xm,-half_length_ym,0.0);
     G4Transform3D transformBoxBCut2(rotBoxBCut2,translationBoxBCut2);
     G4SubtractionSolid* boxBCut12 = new G4SubtractionSolid("boxBCut12", 
                                                             boxBCut1, 
                                                             boxBCut, 
                                                             transformBoxBCut2);



     G4LogicalVolume* supprotRingLog = new G4LogicalVolume(boxBCut12, 
                                                           aluminiumMaterial, 
                                                           "supprotRingLog");
     supprotRingLog->SetVisAttributes(supportVisAtt);
     const G4ThreeVector possupprotRing1(0.0,0.0,-boxDist/2.0);
	 new G4PVPlacement( 0, 
	                    possupprotRing1, 
	                    supprotRingLog, 
	                    pName, 
	                    pMotherLogical, 
	                    0, 
	                    0 );   
	 
     G4RotationMatrix* rotSupp2 = new G4RotationMatrix();
     rotSupp2->rotateY(180.0*deg);
     G4ThreeVector possupprotRing2(0.0,0.0,boxDist/2.0);
     new G4PVPlacement(rotSupp2, 
                       possupprotRing2, 
                       supprotRingLog, 
                       pName, 
                       pMotherLogical, 
                       0, 
                       1 );

}

void LeribssSupport::MakeRailSupport(G4RotationMatrix *pRot, 
                                     G4ThreeVector &tlate, 
                                     const G4String &pName, 
                                     G4LogicalVolume *pMotherLogical,  
                                     G4int pCopyNo)   
{ 
     G4SubtractionSolid* ceilSupportBox = MakeCeilingSupportBox();
     G4SubtractionSolid* ceilSupport = MakeCeilSupport();
     G4UnionSolid* railSupportR = MatekTotalCeilingSupport(ceilSupportBox, 
                                                           ceilSupport);
     G4SubtractionSolid* ceilBracket = MakeCeilingSupportBracket();

     G4RotationMatrix rotBraL;
     rotBraL.rotateY(90.*deg);
     G4ThreeVector  translationBraL(boxOut_x+ceilBracketBase_z,
                                    -(boxOut_y+ceilSuppBase_z),
                                    (boxOut_z-ceilBracketBase_x));
     G4Transform3D transformBraL(rotBraL,translationBraL);
     G4UnionSolid* railBraL = new G4UnionSolid("railBraL", 
                                                railSupportR, 
                                                ceilBracket, 
                                                transformBraL);

     const G4double railSupportX = 12.431*inch;
     const G4double railSupportY = 30.249*inch;

     
     G4LogicalVolume* railSupportLog = new G4LogicalVolume(railBraL, 
                                                           aluminiumMaterial, 
                                                           "supprotRingLog");
     railSupportLog->SetVisAttributes(supportVisAtt);
     const G4ThreeVector posRailSupport1(railSupportX,railSupportY,0);
	 new G4PVPlacement( 0, 
	                    posRailSupport1, 
	                    railSupportLog, 
	                    pName, 
	                    pMotherLogical, 
	                    0, 
	                    0 ); 
	    
	 const G4ThreeVector posRailSupport2(-railSupportX,railSupportY,0);   
	 G4RotationMatrix* rotRailSupport2 = new G4RotationMatrix();
     rotRailSupport2->rotateY(180.*deg);  
	 new G4PVPlacement(rotRailSupport2, 
	                   posRailSupport2, 
	                   railSupportLog, 
	                   pName, 
	                   pMotherLogical, 
	                   0, 
	                   1 );   
 
}


void LeribssSupport::PlaceTubes(G4RotationMatrix *pRot, 
                                G4ThreeVector &tlate, 
                                const G4String &pName, 
                                G4LogicalVolume *pMotherLogical,  
                                G4int pCopyNo)
{
	G4double tubeRad = 1.0*inch;

    G4Tubs* tube = new G4Tubs("tube", 0.0, tubeRad, tubeLength, 0.0*M_PI, 2.0*M_PI);
    G4LogicalVolume* tubeLogic = new G4LogicalVolume(tube, 
                                                     aluminiumMaterial, 
                                                     "tubeLogic");
    tubeLogic->SetVisAttributes(supportVisAtt);
    G4RotationMatrix rotTube;
    
    G4double tube1x = -23.0*inch;
    G4double tube1y =  12.5*inch;     
    G4double tube2x = -17.0*inch;
    G4double tube2y =  21.0*inch;
     
    G4double tube3x =  17.0*inch;
    G4double tube3y =  21.0*inch;     
    G4double tube4x =  23.0*inch;
    G4double tube4y =  12.5*inch;
     
    G4double tube5x =  23.0*inch;
    G4double tube5y = -11.447*inch;     
    G4double tube6x =  17.0*inch;
    G4double tube6y = -21.1*inch;
     
    G4double tube7x = -17.0*inch;
    G4double tube7y = -21.0*inch;     
    G4double tube8x = -23.0*inch;
    G4double tube8y = -11.447*inch;
     
    G4double tube9x = -15.860*inch;
    G4double tube9y = 6.582*inch;     
    G4double tube10x = tube9x;
    G4double tube10y = -tube9y;
     
    G4double tube11x = -tube9x;
    G4double tube11y = tube9y;     
    G4double tube12x = -tube9x;
    G4double tube12y = -tube9y;
    
    G4ThreeVector posTube1(tube1x,tube1y,0);
	new G4PVPlacement( 0, 
	                  posTube1, 
	                  tubeLogic, 
	                  pName + "tube0", 
	                  pMotherLogical, 
	                  0, 
	                  0 );

    G4ThreeVector  posTube2(tube2x, tube2y, 0);
	new G4PVPlacement( 0, 
	                   posTube2, 
	                   tubeLogic, 
	                   pName + "tube0", 
	                   pMotherLogical, 
	                   0, 
	                   1 );
	 
    G4ThreeVector posTube3(tube3x, tube3y, 0);
    new G4PVPlacement( 0, 
                       posTube3, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       2 );
 
    G4ThreeVector posTube4(tube4x, tube4y, 0);
    new G4PVPlacement( 0, 
                       posTube4, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       3 );

    G4ThreeVector  posTube5(tube5x, tube5y, 0);
    new G4PVPlacement( 0, 
                       posTube5, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       4 );
     
    G4ThreeVector posTube6(tube6x, tube6y, 0);
    new G4PVPlacement( 0, 
                       posTube6, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       5 );

    G4ThreeVector posTube7(tube7x, tube7y, 0);
    new G4PVPlacement( 0, 
                       posTube7, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       6 );

    G4ThreeVector posTube8(tube8x, tube8y, 0);
    new G4PVPlacement( 0, 
                       posTube8, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       7 );

    G4ThreeVector posTube9(tube9x, tube9y, 0);
    new G4PVPlacement( 0, 
                       posTube9, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       8 );

    G4ThreeVector posTube10(tube10x, tube10y, 0);
    new G4PVPlacement( 0, 
                       posTube10, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       9 );

    G4ThreeVector posTube11(tube11x, tube11y, 0);
    new G4PVPlacement( 0, 
                       posTube11, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       10 );

    G4ThreeVector posTube12(tube12x, tube12y, 0);
    new G4PVPlacement( 0, 
                       posTube12, 
                       tubeLogic, 
                       pName + "tube0", 
                       pMotherLogical, 
                       0, 
                       11 );
}


void LeribssSupport::Place(G4RotationMatrix *pRot, 
                               G4ThreeVector &tlate, 
                               const G4String &pName, 
                               G4LogicalVolume *pMotherLogical,  
                               G4int pCopyNo)
{


   aluminiumMaterial = materialsManager->GetAluminum();
   supportVisAtt = new G4VisAttributes( G4Colour(100/255.,100./255.,100./255.));
   supportVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
   supportVisAtt->SetForceSolid(true);


    //Rail Support
    MakeRailSupport(pRot, tlate, pName+"RailSupport",  pMotherLogical, pCopyNo);
 

   //make supportRing      
   MakeSupportRing(pRot, tlate, pName+"SupprotRing",  pMotherLogical, pCopyNo);
  
     
   //make Detector Plate	
   MakeDetectorPlate(pRot, tlate, pName+"DetPlate",  pMotherLogical, pCopyNo);

   //Tubes
   PlaceTubes(pRot, tlate, pName, pMotherLogical);

   //HPGeHolder
   MakeHPGeHolder(pRot, tlate, pName, pMotherLogical, pCopyNo);



}



G4UnionSolid* LeribssSupport::roundedHalfPlate(G4double height, 
                                               G4double width, 
                                               G4double thickness, 
                                               G4double radius)
{
 
  G4double half_width_xv = width/2.0-radius;    
  G4double half_height_yv = height/2.0;    
  G4double half_thickness_zv = thickness/2.0;
  
  G4Box* boxv = new G4Box("boxv", half_width_xv, half_height_yv, half_thickness_zv);
  
  G4double half_length_xh = width/2.0;   
  G4double half_length_yh = (height-radius)/2.0;    
  G4double half_thickness_zh = half_thickness_zv;
  
  G4Box* boxh = new G4Box("boxh", half_length_xh, half_length_yh, half_thickness_zh);
  
  G4RotationMatrix rotBoxH;
  rotBoxH.rotateX(0.*deg);
  G4ThreeVector  translationBoxH(0.0, -radius/2.0, 0.0);
  G4Transform3D transformBoxH(rotBoxH,translationBoxH);
  G4UnionSolid* boxvh = new G4UnionSolid("boxvh", boxv, boxh, transformBoxH);

  G4Tubs* cylinderUR = new G4Tubs("cylinderUR", 
                                   0.0, 
                                   radius, 
                                   half_thickness_zh, 
                                   0.0*M_PI, 
                                   2.0*M_PI/4.0);
  
  G4RotationMatrix rotUR;
  rotUR.rotateX(0.*deg);
  G4ThreeVector  translationUR(half_width_xv, half_height_yv - radius, 0);
  G4Transform3D transformUR(rotUR,translationUR);
  G4UnionSolid* boxhvULR = new G4UnionSolid("boxhvULR", 
                                             boxvh, 
                                             cylinderUR, 
                                             transformUR);

  G4Tubs* cylinderBR = new G4Tubs("cylinderBR", 
                                   0.0, 
                                   radius, 
                                   half_thickness_zh, 
                                   0.0*M_PI, 
                                   2.0*M_PI/4.0);
  
  G4RotationMatrix rotBR;
  rotBR.rotateY(180.*deg);
  G4ThreeVector  translationBR(-half_width_xv, half_height_yv - radius, 0);
  G4Transform3D transformBR(rotBR,translationBR);
  G4UnionSolid* rounded_half_plate = new G4UnionSolid("boxhvUBLR", 
                                                       boxhvULR, 
                                                       cylinderBR, 
                                                       transformBR);	

  return rounded_half_plate;

}
