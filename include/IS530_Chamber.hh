/************************************************************************
 * \file IS530_Chamber.hh
 *
 * \class IS530_Chamber   
 * \brief Design of the IS530_Chamber (	Versatile Implantation Chamber
 * 					for the Fast Timing and
 *                              	Standard beta-decay configurations) 
 *
 * \author Christophe Sotty : csotty@gmail.com
 *                            Christophe.Sotty@fys.kuleuven.be
 *
 *************************************************************************/

#ifndef IS530_Chamber_H
#define IS530_Chamber_H 1


// Include-files for detector construction
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"

// Include-files for detector readout
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "CADMesh.hh"

#ifdef G4VIS_USE
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#endif

using namespace std;

// Clover Detector Class
class IS530_Chamber : public G4VUserDetectorConstruction
{
    public:
      IS530_Chamber(G4VPhysicalVolume* p_mother, G4double p_rho, G4double p_theta, G4double p_phi, G4double p_spin);
      ~IS530_Chamber();

      G4VPhysicalVolume* Construct();

    private:
      // Solid, Physical, Logical and CADMesh  volumes of the Capsules and Carbon window
      //
      vector<G4VSolid* >		IS530_Chamber_current_sol;
      vector<CADMesh* >			mesh_IS530_Chamber_current;
      vector<G4VPhysicalVolume* >	IS530_Chamber_current_phys;
      vector<G4LogicalVolume* >		IS530_Chamber_current_log;

      // Physical and Logical volumes of the environnement   
      //
      G4VPhysicalVolume*	det_env;
      G4VPhysicalVolume*	mother;

      G4LogicalVolume*		det_env_log;
      G4LogicalVolume*		mother_log;
    
      // Position and Orientation
      //
      G4double	rho;
      G4double	phi;
      G4double	theta;
      G4double	spin;
      G4RotationMatrix	IS530_Chamber_rotation;
      G4Transform3D*	IS530_Chamber_transformation;
   
      // Visualisation
      //
      G4VisAttributes* IS530_Chamber_vis_att;
    
};

#endif
