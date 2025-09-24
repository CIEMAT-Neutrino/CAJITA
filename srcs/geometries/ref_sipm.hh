#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include "detector.hh"
#include "json_class.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MyDetectorConstruction(); //constructor
  MyDetectorConstruction(G4String jsonName); //constructor
  ~MyDetectorConstruction(); //destructor

  virtual G4VPhysicalVolume *Construct();

private:
  G4Box *solidWorld;
  G4LogicalVolume *logicWorld;
  G4VPhysicalVolume *physWorld;
  G4Material *Air, *LAr, *Plastic, *Steel, *Aluminum, *Iron, *Nickel, *Copper;
  G4MaterialPropertiesTable *mptSteel, *mptPlastic, *mptAluminum; 


  // SENSITIVE DETECTORS NEED TO BE DEFINED HERE
  G4LogicalVolume *logicRefSiPM, *logicSiPM1, *logicSiPM2;
    
  G4String fjsonName;
  void ConstructScintillator();
  void DefineMaterials();
  virtual void ConstructSDandField(); //construct sensitive detector and E/B field
};



#endif

