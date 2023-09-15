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

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction(); //constructor
    ~MyDetectorConstruction(); //destructor

    virtual G4VPhysicalVolume *Construct();

private:
    G4Box *solidWorld,  *solidCajitaOut, *solidCajitaIn, *solidSiPM;
    G4LogicalVolume *logicWorld, *logicCajitaOut,*logicCajitaIn, *logicSiPM1, *logicSiPM2, *logicPMT, *logicSC, *logicSC1, *logicSC2, *logicBoxOut, *logicBoxIn, *logicprueba;
    G4VPhysicalVolume *physWorld, *physCajitaOut, *physCajitaIn, *physSiPM1, *physSiPM2, *physPMT, *physSC,*physSC1,*physSC2, *physBoxOut, *physBoxIn;
    G4Material *Air, *LAr, *Plastic, *Metal, *Iron, *Nickel, *Copper;

    void ConstructScintillator();
    void DefineMaterials();
    virtual void ConstructSDandField(); //construct sensitive detector and E/B field
};

#endif

