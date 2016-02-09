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

	G4double a, z;
	G4double density;
	G4int ncomponents, natoms;

	G4Element* La = new G4Element("Lanthanum","La",z=57.,a=138.9057*g/mole);
	G4Element* Br = new G4Element("Bromium","Br",z=35.,a=79.904*g/mole);
	G4Element* Ce = new G4Element("Cerium","Ce",z=58.,a=140.116*g/mole);

	//LaBr3
	G4Material* LaBr3 = new G4Material("LaBr3", density = 5.07*g/cm3, ncomponents=2);
	LaBr3->AddElement(La, natoms=1);
	LaBr3->AddElement(Br, natoms=3);

	//LaBr3_Ce
	G4Material* LaBr3_Ce = new G4Material("LaBr3_Ce", density = 5.08*g/cm3, ncomponents=2);
	LaBr3_Ce->AddMaterial(LaBr3, 99.5*perCent);
	LaBr3_Ce->AddElement(Ce, 0.5*perCent);

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  //G4Material *Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material *Plastic = 
     nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  //G4Material *NaI = 
   //  nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  G4Material *Vacuum = 
     nist->FindOrBuildMaterial("G4_Galactic");
  G4Material *Aluminum = 
     nist->FindOrBuildMaterial("G4_Al");
	
  //G4Material *Pyrex = 
   //  nist->FindOrBuildMaterial("G4_Pyrex_Glass");

  //option to swtich on/off checking of volume overlaps
  G4bool checkOverlaps = true;

	// Aircraft Shell size
  G4Material *shell_mat = Aluminum;

	G4double shell_pRmax = 50.0 * cm;
	G4double shell_pRmin = (50.0 - 3.9) * cm;
	G4double shell_pSPhi = 0; 
	G4double shell_pDPhi = 2.0*pi;
	G4double shell_pSTheta = 0; 
	G4double shell_pDTheta = 1.0*pi;

	// World Size
  //G4double world_pRmax = 1.2*shell_pRmax;
  G4double world_pRmax = 100.0 * cm;

  // Envelope size
  G4double env_pRmax = shell_pRmin;

  //
  // World
  //

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
  //  Aircraft and surronding shell material
  //  

  //G4Material *shell_mat = Aluminum;

  G4Sphere* solidShell
    = new G4Sphere("Shell",                 //its name
                shell_pRmin,                //dimensions  
                shell_pRmax,
                shell_pSPhi,
                shell_pDPhi,
                shell_pSTheta,
                shell_pDTheta);               

  G4LogicalVolume* logicShell
    = new G4LogicalVolume(solidShell,       //its solid
                          shell_mat,        //its material
                          "Shell");         //its name 


  //G4VPhysicalVolume* physEnv = 
      new G4PVPlacement(0,                 //no rotation
                        G4ThreeVector(),   //translation (0,0,0)
                        logicShell,        //its logical volume
                        "Shell",           //its name
                        logicWorld,        //its mother (logical) volume
                        false,             //no boolean operations
                        0,                 //its copy number 
                        checkOverlaps);    //checkOverlap

  //
  //  Sensitive Detectors
  //    

  //!!!!!!!!!!!!!!!! Half length Values!!!!!
  //G4double cmos_hx = 0.5 * 2.54 *cm;
  //G4double cmos_hy = 0.5 * 2.54 *cm;
  //G4double cmos_hz = 1.0 * 2.54 *cm;

	G4double large_pRmin = 0.0;
	G4double large_pRmax = 2.5 * 2.54 * cm;
	G4double large_pDz =   2.5 * 2.54 * cm;
	G4double large_pSPhi =  0.0; 
	G4double large_pDPhi =  2.0*pi;

	G4double small_pRmin = 0.0;
	G4double small_pRmax = 0.5 * 2.54 * cm;
	G4double small_pDz =   0.5 * 2.54 * cm;
	G4double small_pSPhi =  0.0; 
	G4double small_pDPhi =  2.0*pi;

	G4double labr3_pRmin = 0.0;
	G4double labr3_pRmax = 1.5 * 2.54 * cm;
	G4double labr3_pDz =   1.5 * 2.54 * cm;
	G4double labr3_pSPhi =  0.0; 
	G4double labr3_pDPhi =  2.0*pi;

	//
	//  Large Plastic (5"x5")
	//

  //G4String CMOSName = "g4/CMOS";
  G4VSensitiveDetector* LgPl=new g4CMOS("LgPl","LgPlHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector( LgPl );

  //G4Material *cmos_mat = Plastic;
  G4Material *LgPl_mat = Plastic; 

  //G4Box* solidCMOS
  //  = new G4Box("CMOS",                    //its name
  //              cmos_hx,                   //its size 
  //              cmos_hy,
  //              cmos_hz);
  
  G4Tubs* solidLgPl
    = new G4Tubs("LgPl",                    //its name
                large_pRmin,                //its size 
                large_pRmax,
                large_pDz,
                large_pSPhi,
                large_pDPhi);

  G4LogicalVolume* logicLgPl
    = new G4LogicalVolume(solidLgPl,       //its solid
                          LgPl_mat,        //its material
                          "LgPl");         //its name 

  logicLgPl->SetSensitiveDetector( LgPl );

      new G4PVPlacement(0,                     //no rotation
                       G4ThreeVector(0,-3.5*2.54*cm,0),   //translation (0,0,0)
                        logicLgPl,             //its logical volume
                        "LgPl",                //its name
                        logicEnv,              //its mother (logical) volume
                        false,                 //no boolean operations
                        0);//,                 //its copy number 
                        //checkOverlaps);      //checkOverlap

	//
	//  Small Plastic (1"x1")
	//

  G4VSensitiveDetector* SmPl=new g4CMOS("SmPl","SmPlHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector( SmPl );

  //G4Material *cmos_mat = Plastic;
  G4Material *SmPl_mat = Plastic; 

  
  G4Tubs* solidSmPl
    = new G4Tubs("SmPl",                    //its name
                small_pRmin,                //its size 
                small_pRmax,
                small_pDz,
                small_pSPhi,
                small_pDPhi);

  G4LogicalVolume* logicSmPl
    = new G4LogicalVolume(solidSmPl,       //its solid
                          SmPl_mat,        //its material
                          "SmPl");         //its name 

  logicSmPl->SetSensitiveDetector( SmPl );

      new G4PVPlacement(0,                     //no rotation
                       G4ThreeVector(-1.*2.54*cm,0.5*2.54*cm,3.5*2.54*cm),   //translation (0,0,0)
                        logicSmPl,             //its logical volume
                        "SmPl",                //its name
                        logicEnv,              //its mother (logical) volume
                        false,                 //no boolean operations
                        0);//,                 //its copy number 
                        //checkOverlaps);      //checkOverlap

	//
	//  Lanthinum Bromide (3"x3")
	//

  G4VSensitiveDetector* MdLaBr3 = new g4CMOS("MdLaBr3","MdLaBr3HitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector( MdLaBr3 );

  //G4Material *cmos_mat = Plastic;
  G4Material *MdLaBr3_mat = LaBr3_Ce;

  
  G4Tubs* solidMdLaBr3
    = new G4Tubs("MdLaBr3",                    //its name
                labr3_pRmin,                //its size 
                labr3_pRmax,
                labr3_pDz,
                labr3_pSPhi,
                labr3_pDPhi);

  G4LogicalVolume* logicMdLaBr3
    = new G4LogicalVolume(solidMdLaBr3,      //its solid
                          MdLaBr3_mat,       //its material
                          "MdLaBr3");         //its name 

  logicMdLaBr3->SetSensitiveDetector( MdLaBr3 );

      new G4PVPlacement(0,                     //no rotation
                       G4ThreeVector(0,3.5*2.54*cm,1*2.54*cm),   //translation (0,0,0)
                        logicMdLaBr3,             //its logical volume
                        "MdLaBr3",                //its name
                        logicEnv,              //its mother (logical) volume
                        false,                 //no boolean operations
                        0);//,                 //its copy number 
                        //checkOverlaps);      //checkOverlap

  //
  // sets a max step length in the tracker region

  //G4double maxStep = cmos_hx/2;
  G4double maxStep = small_pDz/16;
  fStepLimit = new G4UserLimits(maxStep); 
  logicSmPl->SetUserLimits(fStepLimit);
  logicLgPl->SetUserLimits(fStepLimit);
  logicMdLaBr3->SetUserLimits(fStepLimit);

  //set colours of regions
  G4VisAttributes* LgPlVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicLgPl->SetVisAttributes(LgPlVisAtt);
  G4VisAttributes* SmPlVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicSmPl->SetVisAttributes(SmPlVisAtt);
  G4VisAttributes* LaBr3VisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  logicMdLaBr3->SetVisAttributes(LaBr3VisAtt);
  //G4VisAttributes* bVisAtt= new G4VisAttributes(G4Colour(0,0,1.0));
  G4VisAttributes* bVisAtt= new G4VisAttributes(G4Colour(0,0,0.0));
  logicEnv->SetVisAttributes(bVisAtt);
  //G4VisAttributes* cVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  G4VisAttributes* cVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,0.0));
  logicWorld->SetVisAttributes(cVisAtt);

  G4VisAttributes* dVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  logicShell->SetVisAttributes(dVisAtt);

  
  //return physWorld; 
  //instead, define physWorld as Private Member of g4GeometryConstruction
  //class and access there... why not? 

}


void g4GeometryConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
