/************************************************************************
 * \file Cubic_Chamber.hh
 *
 * \class Cubic_Chamber   
 * \brief Design of the Cubic_Chamber (Implantation Chamber for the future
 *                                     cubic frame) 
 *
 * \author Christophe Sotty : csotty@gmail.com
 *                            Christophe.Sotty@fys.kuleuven.be
 *
 *************************************************************************/

#include "Cubic_Chamber.hh"
#include "CADMesh.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include <TText.h>

using namespace std;



Cubic_Chamber::Cubic_Chamber(G4VPhysicalVolume* p_mother, G4double p_rho, G4double p_theta, G4double p_phi, G4double p_spin)
  :mother(p_mother), rho(p_rho), theta(p_theta), phi(p_phi), spin(p_spin)
{
	G4cout	<<"\nCubic_Chamber (Cubic Implantation Chamber) #######################\n"	<<flush
		<<"\nRho =\t\t"		<<rho/mm	<< " mm"	<<flush
		<<"\nTheta =\t\t"	<<theta/deg	<< " deg" 	<<flush
		<<"\nPhi =\t\t"		<<phi/deg	<< " deg"       <<flush
		<<"\nSpin =\t\t"	<<spin/deg	<< " deg"       <<flush<<G4endl;
}

Cubic_Chamber::~Cubic_Chamber()
{;}


G4VPhysicalVolume* Cubic_Chamber::Construct()
{
    // Naming
    //
    G4String name="/Cubic_Chamber";
    
    // Material
    //
    G4NistManager* NISTmgr = G4NistManager::Instance();

    G4Material* Vacuum          = NISTmgr->FindOrBuildMaterial("G4_Galactic");
    G4Material* Air             = NISTmgr->FindOrBuildMaterial("G4_AIR");
    G4Material* Al_mat          = NISTmgr->FindOrBuildMaterial("G4_Al");
    
    G4Element*  H = new G4Element("Hydrogen" ,  "H",  1 ,   1.01*g/mole);
    G4Element*  C = new G4Element("Carbone"  ,  "C",  6 ,  12.01*g/mole);
    G4Element*  O = new G4Element("Oxygene"  ,  "O",  8 ,  16.00*g/mole);
    G4Element* Cu = new G4Element("Copper"   , "Cu", 29.,  63.54*g/mole); 
    G4Element* Zn = new G4Element("Zinc"     , "Zn", 30.,  65.39*g/mole);
    G4Element* Mn = new G4Element("Manganse" , "Mn", 25., 54.938*g/mole);

    G4Material* Mylar = new G4Material("Mylar",1.365*g/cm3,3); //
    Mylar->AddElement(H, 8);
    Mylar->AddElement(C,10);
    Mylar->AddElement(O, 4);

    G4Material* Brass = new G4Material("Brass", 8.87*g/cm3, 3); 
    Brass->AddElement(Cu, 70.0*perCent);
    Brass->AddElement(Zn, 28.8*perCent);
    Brass->AddElement(Mn,  1.2*perCent);

    vector<G4Material* > Mat;
    Mat.push_back(Brass);
    Mat.push_back(Mylar);


    // Visualisation
    // 
    vector<G4VisAttributes*> CrysVisAttM;
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,0.0,1.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,1.0,0.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(1.0,0.0,0.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,1.0,1.0)));

    // Visualisation
    //
    vector<G4VisAttributes*> VisAttM;
    Cubic_Chamber_vis_att		= new G4VisAttributes(G4Colour(0.7, 0.4, 0.1));		// Brown
    G4VisAttributes* Al_vis_att 	= new G4VisAttributes(G4Colour(0.65,0.65,0.65));	// Grey
    VisAttM.push_back(new G4VisAttributes(G4Colour(0.7, 0.4, 0.1)));		// Brown
    VisAttM.push_back(new G4VisAttributes(G4Colour(1.0, 1.0, 0.0,0.1)));		// Yellow


    // Position and Orientation
    //
    if(Cubic_Chamber_transformation==NULL){ 
      Cubic_Chamber_rotation.rotateY(90.*deg);
      //HPGeCloverQuad_rotation.rotateX(M_PI);
      Cubic_Chamber_rotation.rotateY(theta);
      Cubic_Chamber_rotation.rotateZ(phi);
      
      ////distance from the origin to the center of the detector
      //G4double trans_mag = rho + MoveToDetectorFrontClover();
      //G4double trans_mag = rho + (7.5 - 0.2548)*cm;
      G4double trans_mag = 0.*cm;

      //G4ThreeVector HPGeCloverQuad_translation(0., 0., trans_mag);
      G4ThreeVector Cubic_Chamber_translation(0., 0., trans_mag);
      Cubic_Chamber_translation.rotateY(theta);
      Cubic_Chamber_translation.rotateZ(phi);
      
      Cubic_Chamber_transformation     = new G4Transform3D(Cubic_Chamber_rotation, Cubic_Chamber_translation);
    }
    if(spin != 0){
      Cubic_Chamber_rotation = Cubic_Chamber_transformation->getRotation();
      G4ThreeVector Cubic_Chamber_translation = Cubic_Chamber_transformation->getTranslation();
      G4RotationMatrix spinCubic_Chamber_rotation;
      spinCubic_Chamber_rotation.rotateX(spin);
      
      Cubic_Chamber_rotation *= spinCubic_Chamber_rotation;
      
      Cubic_Chamber_transformation = new G4Transform3D(Cubic_Chamber_rotation, Cubic_Chamber_translation);
    }

    // Environnement
    G4double detsize_y	= 50.5*mm;
    G4double detsize_z	= 50.5*mm;
    G4double detsize_x	= 50.5*mm;
    G4Box* det_env_sol	= new G4Box(name+"/det_env", detsize_x, detsize_y, detsize_z);
    //det_env_log		= new G4LogicalVolume(det_env_sol, Vacuum, name+"/det_env_log");
    //det_env_log		-> SetVisAttributes(G4VisAttributes::Invisible);
    //det_env		= new G4PVPlacement(*Cubic_Chamber_transformation, name+"/env", det_env_log, mother, false, 0);
    

    // Detector Construction
    //
    vector<char* > Cubic_Chamber_name;
    //Cubic_Chamber_name.push_back("../STL_export/Cubic_Frame/Cubic_Chamber.stl");
    
    //try different names
    Cubic_Chamber_name.push_back("../STL_export/IDS_5/IDS_5_Bara filetata M12_127.stl");
    //Bucharest_Chamber_IS530_capac_leon
    
    

    // Meshing and Logical Volumes
    for(int i = 0; i<Cubic_Chamber_name.size(); i++){ 
      G4cout<< Cubic_Chamber_name.at(i) << G4endl;
      mesh_Cubic_Chamber_current.push_back( new CADMesh(Cubic_Chamber_name.at(i), mm, G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm), false));
      G4cout << "mesh_Cubic_Chamber_current" << G4endl;
      Cubic_Chamber_current_sol.push_back( mesh_Cubic_Chamber_current.at(i)->TessellatedMesh());
      G4cout << "Cubic_Chamber_current_sol" << G4endl;
      Cubic_Chamber_current_log.push_back( new G4LogicalVolume(Cubic_Chamber_current_sol.at(i), Mat.at(i), name+Form("Cubic_Chamber_%i_log", i )));
      G4cout << "Cubic_Chamber_current_log" << G4endl;
      Cubic_Chamber_current_log.at(i)	->SetVisAttributes(Al_vis_att);
      G4cout << "SetVisAttributes" << G4endl;
      Cubic_Chamber_current_phys.push_back( new G4PVPlacement(*Cubic_Chamber_transformation, name+Form("/Cubic_Chamber_%i_phys",i), Cubic_Chamber_current_log.at(i), mother, false, 0));
      //Cubic_Chamber_current_phys.push_back( new G4PVPlacement(0, G4ThreeVector(0, 0, 0), name+Form("/Cubic_Chamber_%i_phys",i), Cubic_Chamber_current_log.at(i), mother, false, 0));
      G4cout << "Cubic_Chamber_current_phys" << G4endl;
    }

    return(mother);

}

