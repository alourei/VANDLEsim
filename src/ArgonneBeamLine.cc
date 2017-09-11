
// $Id: ArgonneBeamLine.cc 10.05.2017 A Fijalkowska $
//
/// \file ArgonneBeamLine.cc
/// \brief Implementation of the ArgonneBeamLine class, S Ilushkin code
//
//

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

#include "ArgonneBeamLine.hh"

ArgonneBeamLine::ArgonneBeamLine()
{
    materialsManager = MaterialsManager::GetInstance();
    SetBasicSizes();

} 

ArgonneBeamLine::~ArgonneBeamLine()
{

}

void ArgonneBeamLine::SetBasicSizes()
{
    inch = 2.54*cm;
	
}



void ArgonneBeamLine::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{

	G4Material* aluminiumMaterial = materialsManager->GetAluminum();
	
	G4double BeamLineTh = 0.12*inch;

    G4double BeamLine1_out_rad = 50.8*mm;
	G4double BeamLine1_inn_rad = BeamLine1_out_rad-BeamLineTh; 	
  	G4double BeamLine1_length = 9.2*inch;

  	G4double BeamLine1cut_out_rad = BeamLine1_inn_rad*0.9;
	G4double BeamLine1cut_inn_rad = 0.0*inch;
  	G4double BeamLine1cut_length = BeamLine1_length*inch;

    G4Tubs* BeamLine1 = new G4Tubs("BeamLine1", 
                                    BeamLine1_inn_rad, 
                                    BeamLine1_out_rad, 
                                    BeamLine1_length, 
                                    0.0*M_PI, 
                                    2.0*M_PI);
    G4Tubs* BeamLine1cut = new G4Tubs("BeamLine1cut", 
                                       BeamLine1cut_inn_rad, 
                                       BeamLine1cut_out_rad, 
                                       BeamLine1cut_length, 
                                       0.0*M_PI, 
                                       2.0*M_PI);



  	G4double BeamLine2_out_rad = BeamLine1_out_rad;	
	G4double BeamLine2_inn_rad = BeamLine1_inn_rad;
  	G4double BeamLine2_length = 4.0*cm;

  	G4double BeamLine2cut_out_rad = BeamLine2_inn_rad*0.9;
  	G4double BeamLine2cut_inn_rad = 0.0*inch;
  	G4double BeamLine2cut_length = BeamLine2_length;

    G4Tubs* BeamLine2 = new G4Tubs("BeamLine2", 
                                    BeamLine2_inn_rad, 
                                    BeamLine2_out_rad, 
                                    BeamLine2_length, 
                                    0.0*M_PI, 
                                    2.0*M_PI);
    G4Tubs* BeamLine2cut = new G4Tubs("BeamLine2cut", 
                                       BeamLine2cut_inn_rad, 
                                       BeamLine2cut_out_rad, 
                                       BeamLine2cut_length, 
                                       0.0*M_PI, 
                                       2.0*M_PI);



  	G4double BeamLine3_out_rad = 0.8*BeamLine1_inn_rad;//2.5*cm;
	G4double BeamLine3_inn_rad = BeamLine3_out_rad-BeamLineTh;
  	G4double BeamLine3_length = 0.5*m;
    G4Tubs* BeamLine3 = new G4Tubs("BeamLine3", 
                                    BeamLine3_inn_rad, 
                                    BeamLine3_out_rad, 
                                    BeamLine3_length, 
                                    0.0*M_PI, 
                                    2.0*M_PI); 
    
    
  	G4double Flange1_out_rad = 2.3*inch;
  	G4double Flange1_inn_rad = BeamLine3_out_rad;
  	G4double Flange1_length = 0.3*inch;
    G4Tubs* Flange1 = new G4Tubs("Flange1", 
                                  Flange1_inn_rad, 
                                  Flange1_out_rad, 
                                  Flange1_length, 
                                  0.0*M_PI, 
                                  2.0*M_PI);
    
    
  	G4double Flange2_out_rad = Flange1_out_rad;
  	G4double Flange2_inn_rad = 0.0;//BeamLine2_inn_rad;
  	G4double Flange2_length = 0.3*inch;
    G4Tubs* Flange2 = new G4Tubs("Flange2", 
                                  Flange2_inn_rad, 
                                  Flange2_out_rad, 
                                  Flange2_length, 
                                  0.0*M_PI, 
                                  2.0*M_PI);
    

    G4RotationMatrix rotB1Flange1;
    rotB1Flange1.rotateZ(90.*deg);
    G4ThreeVector translB1Flange1(0.0,0.0,BeamLine1_length+Flange1_length);
    G4Transform3D transfB1Flange1(rotB1Flange1,translB1Flange1);
    G4UnionSolid* BeamLine1Flange1 = new G4UnionSolid("BeamLine1Flange1", 
                                                        BeamLine1, 
                                                        Flange1, 
                                                        transfB1Flange1);
     
    G4RotationMatrix rotB1Flange2;
    rotB1Flange2.rotateZ(90.*deg);
    G4ThreeVector translnB1Flange2(0.0,0.0,-(BeamLine1_length+Flange1_length));
    G4Transform3D transfB1Flange2(rotB1Flange2,translnB1Flange2);
    G4UnionSolid* BeamLine1Flange2 = new G4UnionSolid("BeamLine1Flange2", 
                                                        BeamLine1Flange1, 
                                                        Flange1, 
                                                        transfB1Flange2);
    
    G4RotationMatrix rotB2Flange1;
    rotB2Flange1.rotateZ(90.*deg);
    G4ThreeVector  translationB2Flange1(0.0,0.0,BeamLine2_length+Flange2_length);
    G4Transform3D transformB2Flange1(rotB2Flange1,translationB2Flange1);
    G4UnionSolid* BeamLine2Flange1 = new G4UnionSolid("BeamLine2Flange1", 
                                                       BeamLine2, 
                                                       Flange2, 
                                                       transformB2Flange1);
     
  
    G4double beamline2Offset = -5.07*inch; //18.4/2. - 4.13 = 5.07

    G4RotationMatrix rotBeamLine2;
    rotBeamLine2.rotateX(90.*deg);
    G4ThreeVector translBeamLine2(0.0*cm,-BeamLine2_length,beamline2Offset);
    G4Transform3D transfBeamLine2(rotBeamLine2,translBeamLine2);
    G4UnionSolid* BeamLine12 = new G4UnionSolid("BeamLineU", 
                                                 BeamLine1Flange2, 
                                                 BeamLine2Flange1, 
                                                 transfBeamLine2);
     
    G4RotationMatrix rotBeamLineCut1;
  	rotBeamLineCut1.rotateX(0.*deg);
  	G4ThreeVector translBeamLineCut1(0.0,0.0,0.0);
  	G4Transform3D transfBeamLineCut1(rotBeamLineCut1,translBeamLineCut1);
  	G4SubtractionSolid* BeamLineCut1 = new G4SubtractionSolid("BeamLineCut1", 
  	                                                           BeamLine12, 
  	                                                           BeamLine1cut, 
  	                                                           transfBeamLineCut1);
  	
    G4RotationMatrix rotBeamLineCut2;
  	rotBeamLineCut2.rotateX(90.*deg);
  	G4ThreeVector translBeamLineCut2(0.0,-BeamLine2_length,beamline2Offset);
  	G4Transform3D transfBeamLineCut2(rotBeamLineCut2,translBeamLineCut2);
  	G4SubtractionSolid* BeamLineCut2 = new G4SubtractionSolid("BeamLineCut2", 
  	                                                          BeamLineCut1, 
  	                                                          BeamLine2cut, 
  	                                                          transfBeamLineCut2);

//put attachments  	
    G4RotationMatrix rotBeamLine3;
  	rotBeamLine3.rotateX(0.*deg);
  	G4ThreeVector translBeamLine3(0.0,
  	                              0.0,
  	                              BeamLine1_length+Flange1_length+BeamLine3_length);
  	G4Transform3D transfBeamLine3(rotBeamLine3,translBeamLine3);
  	G4UnionSolid* BeamLineB3 = new G4UnionSolid("BeamLineB3", 
  	                                             BeamLineCut2, 
  	                                             BeamLine3, 
  	                                             transfBeamLine3);
  	
    G4RotationMatrix rotBeamLine4;
  	rotBeamLine4.rotateX(0.*deg);
  	G4ThreeVector translBeamLine4(0.0,
  	                              0.0,
  	                              -(BeamLine1_length+Flange1_length+BeamLine3_length));
  	G4Transform3D transfBeamLine4(rotBeamLine4,translBeamLine4);
  	G4UnionSolid* BeamLine = new G4UnionSolid("BeamLine", 
  	                                           BeamLineB3, 
  	                                           BeamLine3, 
  	                                           transfBeamLine4);
 
    G4LogicalVolume* beamLineLogVol = new G4LogicalVolume(BeamLine, 
                                                          aluminiumMaterial, 
                                                          "beamLineLogVol");

     
    G4VisAttributes* visAtt = new G4VisAttributes( G4Colour(0.,1.,0.5));
	visAtt->SetForceAuxEdgeVisible(true);
	//BeamLineVisAtt->SetForceSolid(true);
	beamLineLogVol->SetVisAttributes(visAtt);
	
	const G4double blXOffset = 15.*cm;
	
	G4RotationMatrix rotBeamLine;
  	rotBeamLine.rotateY(90.*deg);
  	G4ThreeVector  translationBeamLine(-blXOffset,0.0,0.0);
  	G4Transform3D transformBeamLine(rotBeamLine,translationBeamLine);
  	G4String name = pName+"_beamLine";
  	new G4PVPlacement(transformBeamLine, beamLineLogVol, name, pMotherLogical, 0, pCopyNo);
	
	
}
