
#include "g4CMOSHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4ParticleDefinition.hh"

#include <iomanip>
using namespace CLHEP;

G4Allocator<g4CMOSHit> g4CMOSHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4CMOSHit::g4CMOSHit()
 : G4VHit(),
   fPD(0),
   fEdep(0.),
   fKE(0.),
   fPos(G4ThreeVector())
{}   

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
   
g4CMOSHit::~g4CMOSHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

g4CMOSHit::g4CMOSHit(const g4CMOSHit& right)
  : G4VHit()
{
	fPD = right.fPD;
	fEdep = right.fEdep;
  fKE = right.fKE;
	fPos = right.fPos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const g4CMOSHit& g4CMOSHit::operator=(const g4CMOSHit& right)
{
	fPD = right.fPD;
	fEdep = right.fEdep;
  fKE = right.fKE;
	fPos = right.fPos;
	
	return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int g4CMOSHit::operator==(const g4CMOSHit& right) const
{
	return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4CMOSHit::Draw()
{
	G4VVisManager * pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
	{
	G4Circle circle(fPos);
	circle.SetScreenSize(4.);
  circle.SetFillStyle(G4Circle::filled);
  G4Colour colour(1.,0.,0.);
	G4VisAttributes attribs(colour);
  circle.SetVisAttributes(attribs);
	pVVisManager->Draw(circle);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void g4CMOSHit::Print()
{
	
	G4cout 
     << "Incidence Particle Name and Kinetic Energy " 
     << fPD->GetParticleName() << " " << fKE/MeV 
     << " MeV" << G4endl;
	
	G4cout 
     << "Incidense Position in CMOS Monitor " << fPos/m << G4endl;

	G4cout 
     << "Energy Deposited " << G4BestUnit(fEdep, "Energy") << G4endl;
	
	G4cout
		 << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
