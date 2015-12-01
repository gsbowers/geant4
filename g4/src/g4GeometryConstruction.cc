//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file eventgenerator/exgps/src/g4GeometryConstruction.cc
/// \brief Implementation of the g4GeometryConstruction class
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//
#include "g4GeometryConstruction.hh"
#include "g4CMOS.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh"
#include "G4UserLimits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

g4GeometryConstruction::g4GeometryConstruction()
: physWorld(0), fStepLimit(NULL) 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

g4GeometryConstruction::~g4GeometryConstruction()
{
  delete fStepLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* g4GeometryConstruction::Construct()
{
  SetupGeometry();
  return physWorld;
}

void g4GeometryConstruction::SetupGeometry() 
{

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4Material *Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material *Plastic = 
     nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material *NaI = 
     nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  G4Material *Vacuum = 
     nist->FindOrBuildMaterial("G4_Galactic");
  //G4Material *Pyrex = 
   //  nist->FindOrBuildMaterial("G4_Pyrex_Glass");

  //option to swtich on/off checking of volume overlaps
  G4bool checkOverlaps = false;

  // Envelope size
  G4double env_hx = 20.*cm;
  G4double env_hy = 20.*cm;
  G4double env_hz = 20.*cm;

  G4double env_pRmax = 100.*cm;

  //
  // World
  //

  G4double world_hx = 1.2*env_hx;
  G4double world_hy = 1.2*env_hy;
  G4double world_hz = 1.2*env_hz;

  G4double world_pRmax = 1.2 * env_pRmax;
  
  //G4Material* world_mat = Air;
  G4Material* world_mat = Vacuum;

  //G4Box* solidWorld 
  //   = new G4Box("World",                 //its name
  //               world_hx,                //its size  
  //               world_hy,
  //               world_hz);

  G4Orb* solidWorld
      = new G4Orb("World",                   //its name
                   world_pRmax);

  G4LogicalVolume* logicWorld
      = new G4LogicalVolume(solidWorld,      //its solid
                             world_mat,      //its material 
                             "World");       //its name 

  //G4VPhysicalVolume* physWorld
  physWorld
    = new G4PVPlacement(0,                //no rotation
                        G4ThreeVector(),  //at (0,0,0)
                        logicWorld,       //its logical volume
                        "World",          //its name
                        0,                //its mother volume
                        false,            //no boolean operation
                        0,                //its copy number
                        checkOverlaps);   //overlaps checking 
 

  //
  // Envelope
  //  

  G4Material *env_mat = Vacuum;
  //G4Material *env_mat = Air;

  //G4Box* solidEnv
  //  = new G4Box("Envelope",                //its name
  //              env_hx,                    //its size 
  //              env_hy,
  //              env_hz);

  G4Orb* solidEnv
    = new G4Orb("Envelope",                //its name
                env_pRmax);                //its size 

  G4LogicalVolume* logicEnv
    = new G4LogicalVolume(solidEnv,        //its solid
                          env_mat,         //its material
                          "Envelope");     //its name 


  //G4VPhysicalVolume* physEnv = 
      new G4PVPlacement(0,                 //no rotation
                        G4ThreeVector(),   //translation (0,0,0)
                        logicEnv,          //its logical volume
                        "Envelope",        //its name
                        logicWorld,        //its mother (logical) volume
                        false,             //no boolean operations
                        0,                 //its copy number 
                        checkOverlaps);    //checkOverlap


  //a physical world volume has been created and within that 
  //world volume a physical envelope volume has been placed.

  //
  //  Sensitive Detectors
  //    

	//
	//  Large NaI
	//

  //G4String CMOSName = "g4/CMOS";
  G4String CMOSName = "LgNaI";
  G4VSensitiveDetector* CMOS=new g4CMOS(CMOSName,"g4CMOSHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector( CMOS );

  //!!!!!!!!!!!!!!!! Half length Values!!!!!
  //G4double cmos_hx = 0.5 * 2.54 *cm;
  //G4double cmos_hy = 0.5 * 2.54 *cm;
  //G4double cmos_hz = 1.0 * 2.54 *cm;

	G4double large_pRmin = 0.0;
	G4double large_pRmax = 2.5 * 2.54 * cm;
	G4double large_pDz =   2.5 * 2.54 * cm;
	G4double large_pSPhi =  0.0; 
	G4double large_pDPhi =  2.0*pi;

  //G4Material *cmos_mat = Plastic;
  G4Material *LgNaI_mat = NaI; 

  //G4Box* solidCMOS
  //  = new G4Box("CMOS",                    //its name
  //              cmos_hx,                   //its size 
  //              cmos_hy,
  //              cmos_hz);
  
  G4Tubs* solidCMOS
    = new G4Tubs("LgNaI",                    //its name
                large_pRmin,                //its size 
                large_pRmax,
                large_pDz,
                large_pSPhi,
                large_pDPhi);

  G4LogicalVolume* logicCMOS
    = new G4LogicalVolume(solidCMOS,       //its solid
                          LgNaI_mat,        //its material
                          "LgNaI");         //its name 

  logicCMOS->SetSensitiveDetector( CMOS );

      new G4PVPlacement(0,                     //no rotation
                       G4ThreeVector(0,0,0),   //translation (0,0,0)
                        logicCMOS,             //its logical volume
                        "LgNaI",                //its name
                        logicEnv,              //its mother (logical) volume
                        false,                 //no boolean operations
                        0);//,                 //its copy number 
                        //checkOverlaps);      //checkOverlap

  //
  // sets a max step length in the tracker region

  //G4double maxStep = cmos_hx/2;
  G4double maxStep = large_pDz/16;
  fStepLimit = new G4UserLimits(maxStep); 
  logicCMOS->SetUserLimits(fStepLimit);

  //set colours of regions
  G4VisAttributes* aVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicCMOS->SetVisAttributes(aVisAtt);
  G4VisAttributes* bVisAtt= new G4VisAttributes(G4Colour(0,0,1.0));
  logicEnv->SetVisAttributes(bVisAtt);
  G4VisAttributes* cVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicWorld->SetVisAttributes(cVisAtt);
  //G4VisAttributes* dVisAtt= new G4VisAttributes(G4Colour(0,1.0,0));

  
  //return physWorld; 
  //instead, define physWorld as Private Member of g4GeometryConstruction
  //class and access there... why not? 

}


void g4GeometryConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
