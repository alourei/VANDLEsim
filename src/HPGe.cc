// $Id: HPGe.cc 17.01.2016 A Fijalkowska $
//
/// \file HPGe.cc
/// \brief Implementation of the HPGe class, mostly Sergey's Ilyushkin code
//
//
#include "HPGe.hh"

#include "G4RotationMatrix.hh"
#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

HPGe::HPGe()                
{
    SetSizes();
	SetColors();
	materialsManager = MaterialsManager::GetInstance();
	HPGeAssembly = makeHPGe();
	//HPGeAssembly =makeHPGeCrystals();

}

void HPGe::SetSizes()
{
	SetBasicSizes();
	SetFacePartSizes();
	SetBox2Sizes();
	SetBox3Sizes();
	SetCylinder4Sizes();
	SetCylinder5Sizes();
	SetCylinder6Sizes();
	SetCylinder7Sizes();
}

void HPGe::SetBasicSizes()
{
	crystalInnerRad = 0.0*mm;
  	crystalOuterRad = 25.0*mm;
  	crystalLength = 70.0*mm;
  	
  	crystalTubeCutX = 5.0*mm;
	crystalTubeCutY = 2.0*crystalOuterRad;
	crystalTubeCutZ = 2.0*crystalLength;
	
	crystalDistance = 1.0*mm;
	crystalCenterDistance = 2.0*crystalOuterRad
	                       -2.0*crystalTubeCutX
	                       +crystalDistance;
	                       	                 
}


void HPGe::SetFacePartSizes()
{
	box1_x = 50.5*mm;        
  	box1_z = 125.0*mm;
  	box1_rad = 15.5*mm;
  	box1_thickness = 1.5*mm;
	faceZOffset = box1_x+box1_thickness;
}

void HPGe::SetBox2Sizes()
{
	box2_x = 60.0*mm;        
  	box2_z = 7.5*mm;
  	box2_rad = 25.0*mm;
  	box2_thickness = 1.5*mm;
  	faceb_rad = 38.0*mm;
}

void HPGe::SetBox3Sizes()
{
	box3_x = 65.0*mm;        
  	box3_z = 85.0*mm;
  	box3_rad = 30.0*mm;
  	box3_thickness = 1.5*mm; 
}

void HPGe::SetCylinder4Sizes()
{
    cyl4_inn_rad = 35.0*mm;
  	cyl4_out_rad = faceb_rad;
  	cyl4_length = 25.0*mm;
}

void HPGe::SetCylinder5Sizes()
{
    cyl5_inn_rad = cyl4_out_rad;
  	cyl5_out_rad = 98.0*mm;
  	cyl5_length = 4.0*mm;
}

void HPGe::SetCylinder6Sizes()
{
    cyl6_inn_rad = 2.0*cyl4_out_rad;
  	cyl6_out_rad = 111.0*mm;
  	cyl6_length = 134.0*mm;
}

void HPGe::SetCylinder7Sizes()
{
    cyl7_inn_rad = 0.0*mm;
  	cyl7_out_rad = cyl6_inn_rad;
  	cyl7_length = 2.0*box3_thickness;
}

void HPGe::SetColors()
{
	HPGeVisAtt = new G4VisAttributes( G4Colour(75./255.,0./255.,130./255.));
	HPGeVisAtt->SetForceAuxEdgeVisible(true);
	HPGeVisAtt->SetForceSolid(true);
	
	HPGeFVisAtt = new G4VisAttributes( G4Colour(0x06,0x06,0x06));
	HPGeFVisAtt->SetForceAuxEdgeVisible(true);
	//HPGeFVisAtt->SetForceSolid(true);
	
}
	
G4LogicalVolume* HPGe::makeSingleCrystal()
{
	G4Material* GeMaterial = materialsManager->GetGermanium();
	G4Tubs* cryTube = new G4Tubs("cryTube", 
	                              crystalInnerRad, 
	                              crystalOuterRad, 
	                              crystalLength, 
	                              0.0*M_PI, 
	                              2.0*M_PI);
    G4Box* cryTubeCut = new G4Box("cryTubeCut", 
                                   crystalTubeCutX, 
                                   crystalTubeCutY, 
                                   crystalTubeCutZ);
     
    G4RotationMatrix rotCryCut1;
  	rotCryCut1.rotateX(0.*deg);
  	G4ThreeVector  translationCryCut1(-crystalOuterRad,0.0,0.0);
  	G4Transform3D transformCryCut1(rotCryCut1,translationCryCut1);
  	G4SubtractionSolid* cryCut1 = new G4SubtractionSolid("cryCut1", 
  	                                                      cryTube, 
  	                                                      cryTubeCut, 
  	                                                      transformCryCut1);


  	G4RotationMatrix rotCryCut2;
  	rotCryCut2.rotateZ(90.*deg);
  	G4ThreeVector  translationCryCut2(0.0,-crystalOuterRad,0.0);
  	G4Transform3D transformCryCut2(rotCryCut2,translationCryCut2);
  	G4SubtractionSolid* cry1 = new G4SubtractionSolid("cry1", 
  	                                                   cryCut1, 
  	                                                   cryTubeCut, 
  	                                                   transformCryCut2); 
  	                                                    	
  	G4LogicalVolume* crystalLogVol = new G4LogicalVolume(cry1, 
  	                                                     GeMaterial, 
  	                                                     "HPGeCrystalLogic");
  	
  	
  	G4VisAttributes *crystalGeVisAtt = new G4VisAttributes( G4Colour(0xFF,0xFF,0x00));
	crystalGeVisAtt->SetForceAuxEdgeVisible(true);
	crystalGeVisAtt->SetForceSolid(true);
	crystalLogVol->SetVisAttributes(crystalGeVisAtt);
	return crystalLogVol;
}


G4AssemblyVolume* HPGe::makeAllCrystals()
{

	HPGeCrystalLogic = makeSingleCrystal();	
	G4AssemblyVolume* assemblyHPGeCry = new G4AssemblyVolume();
	
    //place 1st
  	G4ThreeVector translCry1(-crystalCenterDistance/2.0,
  	                          crystalCenterDistance/2.0,
  	                          0.0);
  	placeSingleCrystal(HPGeCrystalLogic, assemblyHPGeCry, 90.*deg, translCry1);
                              
                              
    //place 2nd
  	G4ThreeVector translCry2(crystalCenterDistance/2.0,
  	                         crystalCenterDistance/2.0,
  	                         0.0);
    placeSingleCrystal(HPGeCrystalLogic, assemblyHPGeCry, 0.*deg, translCry2);
    
    
  	//place 3nd
  	G4ThreeVector translCry3(-crystalCenterDistance/2.0,
  	                         -crystalCenterDistance/2.0,
  	                         0.0);
    placeSingleCrystal(HPGeCrystalLogic, assemblyHPGeCry, 180.*deg, translCry3);
    
    //place 4th
  	G4ThreeVector translCry4(crystalCenterDistance/2.0,
  	                         -crystalCenterDistance/2.0,
  	                         0.0);
    placeSingleCrystal(HPGeCrystalLogic, assemblyHPGeCry, -90.*deg, translCry4);

	return assemblyHPGeCry;
}

void HPGe::placeSingleCrystal(G4LogicalVolume* crystalLogVol,
                              G4AssemblyVolume* assembly, 
                              G4double rotAngle, 
                              G4ThreeVector &position)
{								  
	G4RotationMatrix rot;
  	rot.rotateZ(rotAngle);
  	G4Transform3D transform(rot,position);	
  	assembly->AddPlacedVolume(crystalLogVol,transform);							  								  								  
}




void HPGe::makeHPGeFace(G4AssemblyVolume* assemblyHPGe)
{
	G4Material* aluminiumMaterial = materialsManager->GetAluminium();
  	  	
  	//side
	G4UnionSolid* box1b = roundedBox(box1_z,box1_x,box1_rad,box1_thickness);
	G4LogicalVolume* box1b_Logical = new G4LogicalVolume(box1b, 
	                                                     aluminiumMaterial, 
	                                                     "box1b_Logical");
	box1b_Logical->SetVisAttributes(HPGeVisAtt);
	G4RotationMatrix rotBox1b;
	rotBox1b.rotateY(0.0*degree);
	G4ThreeVector posBox1b(0.0,0.0,0.0);
	G4Transform3D transformBox1b(rotBox1b,posBox1b);
	assemblyHPGe->AddPlacedVolume(box1b_Logical,transformBox1b);
	
		
	//front - window 
	G4double face1_x = box1_x;
  	G4double face1_rad = box1_rad;
	G4UnionSolid* box1f = roundedFrontFace(face1_x,face1_rad, box1_thickness);
	G4LogicalVolume* box1f_Logical = new G4LogicalVolume(box1f, 
	                                                     aluminiumMaterial, 
	                                                     "box1f_Logical");
	box1f_Logical->SetVisAttributes(HPGeFVisAtt);		
	G4RotationMatrix rotBox1f;
	rotBox1f.rotateY(90*degree);
	const G4ThreeVector posBox1f(box1_z-box1_thickness,0.0,faceZOffset);
	G4Transform3D transformBox1f(rotBox1f,posBox1f);
	assemblyHPGe->AddPlacedVolume(box1f_Logical,transformBox1f);

		
}

void HPGe::placeCrystals(G4AssemblyVolume* assemblyHPGe)
{
	G4AssemblyVolume* HPGeCrystals = makeAllCrystals();  
	G4RotationMatrix rotCry;
	rotCry.rotateX(0.0*degree);
	rotCry.rotateY(90.0*degree);
	rotCry.rotateZ(0.0*degree);
    const G4ThreeVector posCry(crystalCenterDistance+10.0*mm,0.0,faceZOffset);//X is arbitrary
    G4Transform3D transformCry(rotCry,posCry);
	assemblyHPGe->AddPlacedAssembly(HPGeCrystals,transformCry);
	
}


void HPGe::makeHPGeBox2(G4AssemblyVolume* assemblyHPGe)
{
	G4Material* aluminiumMaterial = materialsManager->GetAluminium();
	
	//side
	G4UnionSolid* box2b = roundedBox(box2_z,box2_x,box2_rad,box2_thickness);
    G4LogicalVolume* box2b_Logical = new G4LogicalVolume(box2b, 
                                                         aluminiumMaterial, 
                                                         "box2b_Logical");
    box2b_Logical->SetVisAttributes(HPGeVisAtt);
    G4RotationMatrix rotBox2b;
	rotBox2b.rotateY(0.0*degree);
	const G4ThreeVector posBox2b(-(box1_z+box2_z),0.0,box1_x-box2_x);
	G4Transform3D transformBox2b(rotBox2b,posBox2b);
	assemblyHPGe->AddPlacedVolume(box2b_Logical,transformBox2b);
    
    //front
    G4double face2_x = box2_x;
  	G4double face2_rad = box2_rad;
	G4UnionSolid* box2f = roundedFrontFaceHole(face2_x,face2_rad, box2_thickness,faceb_rad);
	G4LogicalVolume* box2f_Logical = new G4LogicalVolume(box2f, 
	                                                     aluminiumMaterial, 
	                                                     "box2f_Logical");
	box2f_Logical->SetVisAttributes(HPGeFVisAtt);
	G4RotationMatrix rotBox2f;
	rotBox2f.rotateY(90*degree);
	const G4ThreeVector posBox2f(-(box1_z+box2_thickness),0.0,faceZOffset);
	G4Transform3D transformBox2f(rotBox2f,posBox2f);
	assemblyHPGe->AddPlacedVolume(box2f_Logical,transformBox2f);
}

void HPGe::makeHPGeBox3(G4AssemblyVolume* assemblyHPGe)
{
	G4Material* aluminiumMaterial = materialsManager->GetAluminium();
	
	//side
	G4UnionSolid* box3b = roundedBox(box3_z,box3_x,box3_rad,box3_thickness);
	G4LogicalVolume* box3b_Logical = new G4LogicalVolume(box3b, 
	                                                     aluminiumMaterial, 
	                                                     "box3b_Logical");
	box3b_Logical->SetVisAttributes(HPGeVisAtt);                                                           	
	G4RotationMatrix rotBox3b;
	rotBox3b.rotateY(0.0*degree);
	const G4ThreeVector posBox3b(-(box1_z+2.0*box2_z+box3_z),0.0,box1_x-box3_x);
	G4Transform3D transformBox3b(rotBox3b,posBox3b);
	assemblyHPGe->AddPlacedVolume(box3b_Logical,transformBox3b);

	//first front
	G4double face3_x = box3_x;
  	G4double face3_rad = box3_rad;
  	G4UnionSolid* box3f = roundedFrontFaceHole(face3_x,
  	                                           face3_rad,
  	                                           box3_thickness,
  	                                           faceb_rad);
  	G4LogicalVolume* box3f_Logical = new G4LogicalVolume(box3f, 
  	                                                     aluminiumMaterial, 
  	                                                     "box3f_Logical");
  	box3f_Logical->SetVisAttributes(HPGeFVisAtt);
	G4RotationMatrix rotBox3f;
	rotBox3f.rotateY(90*degree);
	const G4ThreeVector posBox3f(-(box1_z+2.0*box2_z+box3_thickness),
	                             0.0,
	                             faceZOffset);
	G4Transform3D transformBox3f(rotBox3f,posBox3f);
	assemblyHPGe->AddPlacedVolume(box3f_Logical,transformBox3f);
	  	
	//second front
	G4UnionSolid* box3fb = roundedFrontFaceHole(face3_x,
	                                            face3_rad,
	                                            box3_thickness,
	                                            faceb_rad);
	G4LogicalVolume* box3fb_Logical = new G4LogicalVolume(box3fb, 
	                                                      aluminiumMaterial,
	                                                      "box3fb_Logical");
	box3fb_Logical->SetVisAttributes(HPGeVisAtt);
	G4RotationMatrix rotBox3fb;
	rotBox3fb.rotateY(90*degree);
	const G4ThreeVector posBox3fb(-(box1_z+2.0*box2_z+2.0*box3_z-box3_thickness),
	                              0.0,
	                              faceZOffset);
	G4Transform3D transformBox3fb(rotBox3fb,posBox3fb);
	assemblyHPGe->AddPlacedVolume(box3fb_Logical,transformBox3fb);
	
}

void HPGe::makeHPGeCylinder4(G4AssemblyVolume* assemblyHPGe)
{
	G4Material* aluminiumMaterial = materialsManager->GetAluminium();
	G4Tubs* cyl4 = new G4Tubs("cyl4", 
	                           cyl4_inn_rad, 
	                           cyl4_out_rad, 
	                           cyl4_length, 
	                           0.0*M_PI, 
	                           2.0*M_PI);
    G4LogicalVolume* cyl4_Logical = new G4LogicalVolume(cyl4, 
                                                        aluminiumMaterial, 
                                                        "cyl4_Logical");
    cyl4_Logical->SetVisAttributes(HPGeVisAtt);
     
    G4RotationMatrix rotCyl4;
    rotCyl4.rotateY(90*degree);
    const G4ThreeVector posCyl4(-(box1_z+2.0*box2_z+2.0*box3_z+cyl4_length-box3_thickness),
                                0.0,
                                faceZOffset);
    G4Transform3D transformCyl4(rotCyl4,posCyl4);
	assemblyHPGe->AddPlacedVolume(cyl4_Logical,transformCyl4);
}

void HPGe::makeHPGeCylinder5(G4AssemblyVolume* assemblyHPGe)
{
	G4Material* aluminiumMaterial = materialsManager->GetAluminium();
    G4Tubs* cyl5 = new G4Tubs("cyl5", 
                               cyl5_inn_rad, 
                               cyl5_out_rad, 
                               cyl5_length, 
                               0.0*M_PI, 
                               2.0*M_PI);
    G4LogicalVolume* cyl5_Logical = new G4LogicalVolume(cyl5, 
                                                        aluminiumMaterial, 
                                                        "cyl5_Logical");
    cyl5_Logical->SetVisAttributes(HPGeVisAtt);
     
    G4RotationMatrix rotCyl5;
	rotCyl5.rotateY(90*degree);
	G4double xPos = -(box1_z+2.0*box2_z+2.0*box3_z+2.0*cyl4_length-box3_thickness-cyl5_length);
    const G4ThreeVector posCyl5(xPos,
                                0.0,
                                faceZOffset);
    G4Transform3D transformCyl5(rotCyl5,posCyl5);
	assemblyHPGe->AddPlacedVolume(cyl5_Logical,transformCyl5);
}

void HPGe::makeHPGeCylinder6(G4AssemblyVolume* assemblyHPGe)
{
	G4Material* aluminiumMaterial = materialsManager->GetAluminium();
	G4Tubs* cyl6 = new G4Tubs("cyl6", 
	                           cyl6_inn_rad, 
	                           cyl6_out_rad, 
	                           cyl6_length, 
	                           0.0*M_PI, 
	                           2.0*M_PI);
    G4LogicalVolume* cyl6_Logical = new G4LogicalVolume(cyl6, 
                                                        aluminiumMaterial, 
                                                        "cyl6_Logical");
    cyl6_Logical->SetVisAttributes(HPGeVisAtt);
     
    G4RotationMatrix rotCyl6;
	rotCyl6.rotateY(90*degree);
	G4double xPos = -(box1_z+2.0*box2_z+2.0*box3_z+2.0*cyl4_length-box3_thickness+cyl6_length);
    const G4ThreeVector posCyl6(xPos,0.0,faceZOffset);
    G4Transform3D transformCyl6(rotCyl6,posCyl6);
	assemblyHPGe->AddPlacedVolume(cyl6_Logical,transformCyl6);
}

void HPGe::makeHPGeCylinder7(G4AssemblyVolume* assemblyHPGe)
{
	    G4Material* aluminiumMaterial = materialsManager->GetAluminium();
	G4Tubs* cyl7 = new G4Tubs("cyl7", 
                               cyl7_inn_rad, 
                               cyl7_out_rad, 
                               cyl7_length, 
                               0.0*M_PI, 
                               2.0*M_PI);
    G4LogicalVolume* cyl7_Logical = new G4LogicalVolume(cyl7, 
                                                        aluminiumMaterial, 
                                                        "cyl7_Logical");
    cyl7_Logical->SetVisAttributes(HPGeVisAtt);
     
    G4RotationMatrix rotCyl7;
	rotCyl7.rotateY(90*degree);
	G4double xPos = -(box1_z+2.0*box2_z+2.0*box3_z+2.0*cyl4_length-box3_thickness+2.0*cyl6_length-cyl7_length);
    const G4ThreeVector posCyl7(xPos,0.0,faceZOffset);
    G4Transform3D transformCyl7(rotCyl7,posCyl7);
	assemblyHPGe->AddPlacedVolume(cyl7_Logical,transformCyl7);
}

G4AssemblyVolume* HPGe::makeHPGe()
{
 
  	G4AssemblyVolume* assemblyHPGe = new G4AssemblyVolume();
	makeHPGeFace(assemblyHPGe);
    placeCrystals(assemblyHPGe);
    makeHPGeBox2(assemblyHPGe);
    makeHPGeBox3(assemblyHPGe);
    makeHPGeCylinder4(assemblyHPGe);
    makeHPGeCylinder5(assemblyHPGe);
    makeHPGeCylinder6(assemblyHPGe);
    makeHPGeCylinder7(assemblyHPGe);	
	
	return assemblyHPGe;
}

G4UnionSolid* HPGe::roundedBox(G4double length,  
                               G4double thickness,
                               G4double radius, 
                               G4double zz)
                               
{
  //G4double half_length_x = length*mm;  
  //G4double half_length_y = (thickness-radius)*mm;   
  G4double half_length_x = length;  
  G4double half_length_y = (thickness-radius);    
  G4double half_thickness_z = zz;
  G4double inner_radius = radius;
  G4double outer_radius = radius+zz*2.0;
  
  G4Box* boxu = new G4Box("boxu", 
                           half_length_x, 
                           half_length_y, 
                           half_thickness_z);  
  G4Box* boxb = new G4Box("boxb", 
                           half_length_x, 
                           half_length_y, 
                           half_thickness_z);
  G4Box* boxl = new G4Box("boxl", 
                          half_length_x, 
                          half_length_y, 
                          half_thickness_z);
  G4Box* boxr = new G4Box("boxr", 
                           half_length_x, 
                           half_length_y, 
                           half_thickness_z);
  
  G4RotationMatrix rotUB;
  rotUB.rotateX(0.0*deg);
  G4ThreeVector  translationUB(0.0, 0.0, 2.0*(thickness+zz));
  G4Transform3D transformUB(rotUB,translationUB);  
  G4UnionSolid* boxub = new G4UnionSolid("boxub", 
                                          boxu, 
                                          boxb, 
                                          transformUB);
  
  G4RotationMatrix rotL;
  rotL.rotateX(90.0*deg);
  //G4ThreeVector  translationL(0.0, -(thickness+zz), thickness);
  G4ThreeVector  translationL(0.0, -(thickness+zz), thickness+zz);
  G4Transform3D transformL(rotL,translationL);  
  G4UnionSolid* boxubl = new G4UnionSolid("boxubl", boxub, boxl, transformL);
  
  G4RotationMatrix rotR;
  rotR.rotateX(90.0*deg);
  G4ThreeVector  translationR(0.0, thickness+zz, thickness+zz);
  G4Transform3D transformR(rotR,translationR);  
  G4UnionSolid* boxublr = new G4UnionSolid("boxublr", boxubl, boxr, transformR);
   
    
    
  G4Tubs* cylUL = new G4Tubs("cylUL", inner_radius, outer_radius, 
                             length, 0.0*M_PI/4.0, 2.0*M_PI/4.0);
  G4Tubs* cylUR = new G4Tubs("cylUR", inner_radius, outer_radius, 
                             length, 0.0*M_PI/4.0, 2.0*M_PI/4.0);
  G4Tubs* cylBL = new G4Tubs("cylBL", inner_radius, outer_radius, 
                             length, 0.0*M_PI/4.0, 2.0*M_PI/4.0);
  G4Tubs* cylBR = new G4Tubs("cylBR", inner_radius, outer_radius, 
                             length, 0.0*M_PI/4.0, 2.0*M_PI/4.0);
  
  G4RotationMatrix rotCUL;
  rotCUL.rotateX(90.0*deg);
  rotCUL.rotateY(180.0*deg);
  rotCUL.rotateZ(90.0*deg);
  G4ThreeVector  translationCUL(0.0, -(thickness-radius), radius+zz);
  //G4RotationMatrix InvRot= yRot45deg;
  //yRot45deg.invert();                            
  // or else InvRot.yRotate(-M_PI/4.0*rad);
  G4Transform3D transformCUL(rotCUL,translationCUL);     
  G4UnionSolid* boxCylUL = new G4UnionSolid("boxhvUL", 
                                             boxublr, 
                                             cylUL, 
                                             transformCUL);				
  
  G4RotationMatrix rotCUR;
  rotCUR.rotateX(0.0*deg);
  rotCUR.rotateY(90.0*deg);
  rotCUR.rotateZ(0.0*deg);
  G4ThreeVector  translationCUR(0.0, (thickness-radius), radius+zz);
  G4Transform3D transformCUR(rotCUR,translationCUR);     
  G4UnionSolid* boxCylULR = new G4UnionSolid("boxhvULR", 
                                              boxCylUL, 
                                              cylUR, 
                                              transformCUR);
  
  G4RotationMatrix rotCBL;
  rotCBL.rotateX(90.0*deg);
  rotCBL.rotateY(-90.0*deg);
  rotCBL.rotateZ(90.0*deg);
  G4ThreeVector  translationCBL(0.0, 
                                -(thickness-radius), 
                                (half_length_y*2.0)+radius+(zz*1.0));
  G4Transform3D transformCBL(rotCBL,translationCBL);     
  G4UnionSolid* boxhvULRBL = new G4UnionSolid("boxhvULRBL", 
                                               boxCylULR, 
                                               cylBL, 
                                               transformCBL);
  
  G4RotationMatrix rotCBR;
  rotCBR.rotateX(0.0*deg);
  rotCBR.rotateY(-90.0*deg);
  rotCBR.rotateZ(0.0*deg);
  G4ThreeVector  translationCBR(0.0, 
                                thickness-radius, 
                                (half_length_y*2.0)+radius+(zz*1.0));
  G4Transform3D transformCBR(rotCBR,translationCBR);     
  G4UnionSolid* rounded_box = new G4UnionSolid("rounded_box", 
                                                boxhvULRBL, 
                                                cylBR, 
                                                transformCBR);

  return rounded_box;

}

G4UnionSolid* HPGe::roundedFrontFace(G4double thickness, 
                                     G4double radius, 
                                     G4double zz)
{
 
  G4double half_length_xh = (thickness-radius);    
  G4double half_length_yh = thickness;    
  G4double half_thickness_zh = zz;
  
  G4Box* boxh = new G4Box("boxh", 
                           half_length_xh, 
                           half_length_yh, 
                           half_thickness_zh);
  
  G4double half_length_xv = half_length_yh;   
  G4double half_length_yv = half_length_xh;    
  G4double half_thickness_zv = half_thickness_zh;
  
  G4Box* boxv = new G4Box("boxv", 
                           half_length_xv, 
                           half_length_yv, 
                           half_thickness_zv);
  
  G4UnionSolid* boxhv = new G4UnionSolid("boxhv", boxh, boxv);
    
  G4Tubs* cylinderUL = new G4Tubs("cylinderUL", 
                                  0.0, 
                                  radius, 
                                  half_thickness_zh, 
                                  0.0*M_PI/4.0, 
                                  2.0*M_PI/4.0);
  
  G4RotationMatrix rotUL;   // Rotates X and Z axes only
  //yRot45deg.rotateY(M_PI/4.*rad);
  rotUL.rotateX(180.*deg);
  G4ThreeVector  translationUL(half_length_xh, -half_length_xh, 0);
  //G4RotationMatrix InvRot= yRot45deg;
  //yRot45deg.invert();                            
  // or else InvRot.yRotate(-M_PI/4.0*rad);
  G4Transform3D transformUL(rotUL,translationUL);     
  G4UnionSolid* boxhvUL = new G4UnionSolid("boxhvUL", 
                                            boxhv, 
                                            cylinderUL, 
                                            transformUL);
  
  G4Tubs* cylinderUR = new G4Tubs("cylinderUR", 
                                  0.0, radius, 
                                  half_thickness_zh, 
                                  0.0*M_PI, 
                                  2.0*M_PI/4.0);
  
  G4RotationMatrix rotUR;
  rotUR.rotateX(0.*deg);
  G4ThreeVector  translationUR(half_length_xh, half_length_xh, 0);
  G4Transform3D transformUR(rotUR,translationUR);
  G4UnionSolid* boxhvULR = new G4UnionSolid("boxhvULR", 
                                             boxhvUL, 
                                             cylinderUR, 
                                             transformUR);
  
  G4Tubs* cylinderBL = new G4Tubs("cylinderBL", 
                                  0.0, 
                                  radius, 
                                  half_thickness_zh, 
                                  0.0*M_PI, 
                                  2.0*M_PI/4.0);
  
  G4RotationMatrix rotBL;
  rotBL.rotateX(180.*deg);
  rotBL.rotateY(180.*deg);
  G4ThreeVector  translationBL(-half_length_xh, -half_length_xh, 0);
  G4Transform3D transformBL(rotBL,translationBL);
  G4UnionSolid* boxhvULRBL = new G4UnionSolid("boxhvULRBL", 
                                               boxhvULR, 
                                               cylinderBL, 
                                               transformBL);
  
  G4Tubs* cylinderBR = new G4Tubs("cylinderBR", 
                                   0.0, 
                                   radius, 
                                   half_thickness_zh, 
                                   0.0*M_PI, 
                                   2.0*M_PI/4.0);
  
  G4RotationMatrix rotBR;
  rotBR.rotateY(180.*deg);
  G4ThreeVector  translationBR(-half_length_xh, half_length_xh, 0);
  G4Transform3D transformBR(rotBR,translationBR);
  G4UnionSolid* rounded_front_face = new G4UnionSolid("boxhvUBLR", 
                                                       boxhvULRBL, 
                                                       cylinderBR, 
                                                       transformBR);	

  return rounded_front_face;

}

G4UnionSolid* HPGe::roundedFrontFaceHole(G4double thickness, 
                                         G4double radius, 
                                         G4double zz, 
                                         G4double radiusHole)
{
 
  G4double half_length_xh = (thickness-radius)*mm;    
  G4double half_length_yh = thickness*mm;    
  G4double half_thickness_zh = zz;
  
  G4Box* boxh = new G4Box("boxh", 
                          half_length_xh, 
                          half_length_yh, 
                          half_thickness_zh);
  
  G4double half_length_xv = half_length_yh;   
  G4double half_length_yv = half_length_xh;    
  G4double half_thickness_zv = half_thickness_zh;
  
  G4Box* boxv = new G4Box("boxv", 
                           half_length_xv, 
                           half_length_yv, 
                           half_thickness_zv);
  
  G4UnionSolid* boxhv = new G4UnionSolid("boxhv", boxh, boxv);
  
  G4Tubs* cylinderHole = new G4Tubs("cylinderHole", 
                                     0.0, 
                                     radiusHole, 
                                     10.0, 
                                     0.0*M_PI,
                                     2.0*M_PI);
  
  G4SubtractionSolid* boxhvSubCyl = new G4SubtractionSolid("boxhvSubCyl", 
                                                           boxhv, 
                                                           cylinderHole,
                                                           0, 
                                                           G4ThreeVector(0.0,0.0,0.0));
    
  G4Tubs* cylinderUL = new G4Tubs("cylinderUL", 
                                   0.0, 
                                   radius, 
                                   half_thickness_zh, 
                                   0.0*M_PI/4.0, 
                                   2.0*M_PI/4.0); 
  
  G4RotationMatrix rotUL;   // Rotates X and Z axes only
  //yRot45deg.rotateY(M_PI/4.*rad);
  rotUL.rotateX(180.*deg);
  G4ThreeVector  translationUL(half_length_xh, -half_length_xh, 0);
  //G4RotationMatrix InvRot= yRot45deg;
  //yRot45deg.invert();                            
  // or else InvRot.yRotate(-M_PI/4.0*rad);
  G4Transform3D transformUL(rotUL,translationUL);     
  //G4UnionSolid* boxhvUL = new G4UnionSolid("boxhvUL", boxhv, cylinderUL, transformUL);
  G4UnionSolid* boxhvUL = new G4UnionSolid("boxhvUL", 
                                            boxhvSubCyl, 
                                            cylinderUL, 
                                            transformUL);
  
  G4Tubs* cylinderUR = new G4Tubs("cylinderUR", 
                                   0.0, 
                                   radius, 
                                   half_thickness_zh, 
                                   0.0*M_PI, 
                                   2.0*M_PI/4.0);
  
  G4RotationMatrix rotUR;
  rotUR.rotateX(0.*deg);
  G4ThreeVector  translationUR(half_length_xh, half_length_xh, 0);
  G4Transform3D transformUR(rotUR,translationUR);
  G4UnionSolid* boxhvULR = new G4UnionSolid("boxhvULR", 
                                             boxhvUL, 
                                             cylinderUR, 
                                             transformUR);
  
  G4Tubs* cylinderBL = new G4Tubs("cylinderBL", 
                                   0.0, 
                                   radius, 
                                   half_thickness_zh, 
                                   0.0*M_PI, 
                                   2.0*M_PI/4.0);
  
  G4RotationMatrix rotBL;
  rotBL.rotateX(180.*deg);
  rotBL.rotateY(180.*deg);
  G4ThreeVector  translationBL(-half_length_xh, -half_length_xh, 0);
  G4Transform3D transformBL(rotBL,translationBL);
  G4UnionSolid* boxhvULRBL = new G4UnionSolid("boxhvULRBL", 
                                               boxhvULR, 
                                               cylinderBL, 
                                               transformBL);
  
  G4Tubs* cylinderBR = new G4Tubs("cylinderBR", 
                                   0.0, 
                                   radius, 
                                   half_thickness_zh, 
                                   0.0*M_PI, 
                                   2.0*M_PI/4.0);
  
  G4RotationMatrix rotBR;
  rotBR.rotateY(180.*deg);
  G4ThreeVector  translationBR(-half_length_xh, half_length_xh, 0);
  G4Transform3D transformBR(rotBR,translationBR);
  G4UnionSolid* rounded_front_face = new G4UnionSolid("boxhvUBLR", 
                                                       boxhvULRBL, 
                                                       cylinderBR, 
                                                       transformBR);	

  return rounded_front_face;

}
