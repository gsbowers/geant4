#ifndef myDetectorConstruction_h
#define myDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class myDetectorConstruction : public G4VUserDetectorConstruction 
{
	public:
		myDetectorConstruction();
		virtual ~myDetectorConstruction();
		
		virtual G4VPhysicalVolume* Construct();
		void ConstructSDandField();
		
		G4LogicalVolume* GetScoringVolume() const {return fScoringVolume; }
		
	protected:
		G4LogicalVolume* fScoringVolume;
	  G4LogicalVolume* fLogicLgPl;	
};

#endif
