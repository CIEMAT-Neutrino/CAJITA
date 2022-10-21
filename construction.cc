#include "construction.hh"
#include "G4GDMLParser.hh"
#include "G4SystemOfUnits.hh"
#include "G4SystemOfUnits.hh"
#include "json_class.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    // We need to specify the material before the detector is defined:
    // Some are already predefined so we pick the ones we need
    G4NistManager *nist = G4NistManager::Instance();

    Air = nist->FindOrBuildMaterial("G4_AIR");
    LAr = nist->FindOrBuildMaterial("G4_lAr");
    Plastic = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    Metal = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    
    const G4int num = 2;  
    G4double energy[num] = {1.239841939*eV/0.128, 1.239841939*eV/0.9}; //momentum of optical photon; conversion wavelenght(um) to energy 
    G4double rindexAir[num] = {1.0, 1.0}; // Refraction index for propagation (for photons to propagate in Air)
    G4double reflectivity[num] = {0.94, 0.94};

    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", energy, rindexAir, num);
    Air->SetMaterialPropertiesTable(mptAir);
    
    G4MaterialPropertiesTable *mptLAr = new G4MaterialPropertiesTable();
    
    
    std::vector<G4double> AbsLengthEnergies ={ 3. *eV, 4. *eV,  5. *eV,  6. *eV, 7. *eV,
                                               8. *eV, 9. *eV, 10. *eV, 11. *eV 
                                               };

    std::vector<G4double> AbsLengthSpectrum ={ 2000., 2000., 2000., 2000., 2000., 2000., 2000., 2000., 2000. };

    std::vector<G4double>  RIndexEnergies={ 
          1.18626 *eV,   1.68626 *eV,   2.18626 *eV, 2.68626 *eV, 3.18626 *eV, 3.68626 *eV, 
          4.18626 *eV,   4.68626 *eV,   5.18626 *eV, 5.68626 *eV, 6.18626 *eV, 6.68626 *eV, 
          7.18626 *eV,   7.68626 *eV,   8.18626 *eV, 8.68626 *eV, 9.18626 *eV, 9.68626 *eV, 
        1.01863e1 *eV, 1.06863e1 *eV, 1.11863e1 *eV
    };

    std::vector<G4double> RIndexSpectrum={
        1.24664 , 1.2205 ,1.22694 ,1.22932 ,1.23124 ,1.23322 ,
        1.23545 ,1.23806 ,1.24116 ,1.24489 ,1.24942 ,1.25499 ,
        1.26197 , 1.2709 ,1.28263 ,1.29865 ,1.32169 ,1.35747 ,
        1.42039 ,1.56011 ,2.16626 
    };

    std::vector<G4double> RayleighEnergies= {
        1.18626 *eV, 1.68626 *eV, 2.18626 *eV, 2.68626 *eV, 3.18626 *eV, 3.68626 *eV, 
        4.18626 *eV, 4.68626 *eV, 5.18626 *eV, 5.68626 *eV, 6.18626 *eV, 6.68626 *eV,  
        7.18626 *eV, 7.68626 *eV, 8.18626 *eV, 8.68626 *eV, 9.18626 *eV, 9.68626 *eV, 
        10.1863 *eV, 10.6863 *eV, 11.1863 *eV 
        };
    std::vector<G4double> RayleighSpectrum= { 
                        1200800 *cm,  390747 *cm,   128633 *cm, 54969.1 *cm, 27191.8 *cm, 14853.7 *cm, 
                         8716.9 *cm, 5397.42 *cm,  3481.37 *cm, 2316.51 *cm, 1577.63 *cm, 1092.02 *cm, 
                        763.045 *cm, 534.232 *cm,  371.335 *cm, 252.942 *cm,  165.38 *cm, 99.9003 *cm, 
                        51.2653 *cm,  17.495 *cm, 0.964341 *cm 
                        };

    mptLAr->AddProperty("RINDEX"   , RIndexEnergies   , RIndexSpectrum   ); //energy=mom in relativistic approximation
    mptLAr->AddProperty("ABSLENGTH", AbsLengthEnergies, AbsLengthSpectrum);
    mptLAr->AddProperty("RAYLEIGH" , RayleighEnergies , RayleighSpectrum );
    LAr->SetMaterialPropertiesTable(mptLAr);

}

void MyDetectorConstruction::ConstructScintillator()
{
// >> COLORS:
//    white = G4Colour(1.0, 1.0, 1.0);
//    pink = G4Colour(0.94, 0.5, 0.5);
//    grey = G4Colour(0.46, 0.53, 0.6);
//    yellow = G4Colour(1.0, 1.0, 0.);
//    blue = G4Colour(0.25,0.41, 0.88 );
//    lightBlue = G4Colour(0.28, 0.82, 0.8);
//    green = G4Colour(0., 1., 0.);
//    brown = G4Colour(0.5, 0.5, 0.);
//    purple = G4Colour(0.85,0.44,0.84);
//    red = G4Colour(1.0, 0.0, 0.0);
//    orange = G4Colour(1.,0.5,0.); 
//    black =  G4Colour(0.,0.,0.); 

    // >> Set visAttributes for various components
    G4VisAttributes* logicSolidVisAtt  = new G4VisAttributes(G4Colour(0.46, 0.53, 0.6, 0.3)); //grey + alpha
    G4VisAttributes* logicHoleVisAtt   = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 1)); //black + alpha
    G4VisAttributes* logicDetectorVisAtt = new G4VisAttributes(G4Colour(0.25,0.41, 0.88, 1)); //blue
    G4VisAttributes* logicSCVisAtt = new G4VisAttributes(G4Colour(0.25,0.88, 0.41, 0.5)); //green
    logicSolidVisAtt->SetForceSolid(true);
    logicHoleVisAtt->SetForceWireframe(true);
    logicDetectorVisAtt->SetForceSolid(true);
    logicSCVisAtt->SetForceSolid(true);

    // >> Matriz de rotacion para lo relativo a la SC
    G4RotationMatrix* pRot = new G4RotationMatrix();
    pRot->rotateX(90.*deg);

    // Surfaces
    G4OpticalSurface* OpSurface = new G4OpticalSurface("OpSurface");
    OpSurface -> SetModel(glisur);
    OpSurface -> SetType(dielectric_metal);
    OpSurface -> SetFinish(polished);
    // G4LogicalBorderSurface* Surface = new G4LogicalBorderSurface("Surface", physSurfaceHole, physSiPM1, OpSurface); //physCajitaIn
    // G4LogicalSkinSurface* Surface = new G4LogicalSkinSurface("Surface",logicSurfaceHole,OpSurface);

    const G4int num = 2;  
    G4double specularspike[num] = {1, 1};
    G4double energy[num] = {1.239841939*eV/0.128, 1.239841939*eV/0.9}; //momentum of optical photon; conversion wavelenght(um) to energy 
    G4double reflectivity[num] = {0.9,0.9};
    // G4double efficiency[num] = {0.8, 0.1};

    G4MaterialPropertiesTable *mptSurface = new G4MaterialPropertiesTable();
    mptSurface->AddProperty("REFLECTIVITY", energy, reflectivity, num);
    // mptSurface->AddProperty("EFFICIENCY", energy, efficiency, num);

    OpSurface -> SetMaterialPropertiesTable(mptSurface); //propiedades de reflectividad


    // >> Small SC - SBND (insensitive bar in the middle)
    // G4Box *solidFrame = new G4Box("solidFrame", 40*mm, 105*mm, 1*mm); 
    // G4LogicalVolume *logicFrame = new G4LogicalVolume(solidFrame, Plastic, "logicFrame");
    // G4VPhysicalVolume *physFrame = new G4PVPlacement(0, G4ThreeVector(0*mm, 0.*mm, -999*mm), logicFrame, "physFrame", logicWorld, false, 0, 1);
    // logicFrame->SetVisAttributes(logicSolidVisAtt);

    // >> 2x SBND SC
    G4Box *solidSC = new G4Box("solidSC", 37.5*mm, 49*mm, 2*mm); //200x75 mm a 5.5 cm de la fuente (172 medidas)
    logicSC1 = new G4LogicalVolume(solidSC, LAr, "logicSC1");
    logicSC2 = new G4LogicalVolume(solidSC, LAr, "logicSC2");
    physSC1 = new G4PVPlacement(0, G4ThreeVector(0*mm,  51*mm, -1*m), logicSC1, "physSC1", logicWorld, false, 1, true); 
    physSC2 = new G4PVPlacement(0, G4ThreeVector(0*mm, -51*mm, -1*m), logicSC2, "physSC2", logicWorld, false, 1, true); 
    logicSC1->SetVisAttributes(logicSCVisAtt);
    logicSC2->SetVisAttributes(logicSCVisAtt);

    //SBND positions from json file:
    const Run_map pds_map=Run_map("sbnd_pds_mapping.json");
    G4String logic_SC_name="logicSC_";
    G4String physc_SC_name="physcSC_";

    int counter=0;
    for (auto ch:pds_map.json_map) 
    {   
        if ((ch["pd_type"]=="xarapuca_vis") && (ch["tpc"]==0))
        {
            std::cout<<"  ------------------ "<<ch["x"]<<std::endl;
            double y=ch["y"];
            double z=ch["z"];

            auto aux_logic1=new G4LogicalVolume(solidSC, LAr, logic_SC_name+std::to_string(counter));
            auto aux_logic2=new G4LogicalVolume(solidSC, LAr, logic_SC_name+std::to_string(counter+1));
            aux_logic1->SetVisAttributes(logicSCVisAtt);
            aux_logic2->SetVisAttributes(logicSCVisAtt);
            
            auto aux_physc1=new G4PVPlacement(0, G4ThreeVector((z-254.5)*cm, (y+5.1)*cm, -1*m), aux_logic1,physc_SC_name+std::to_string(counter), logicWorld, false, 1, true);
            auto aux_physc2=new G4PVPlacement(0, G4ThreeVector((z-254.5)*cm, (y-5.1)*cm, -1*m), aux_logic2,physc_SC_name+std::to_string(counter+1), logicWorld, false, 1, true);
            
            
            logicSC_v.push_back(aux_logic1);
            logicSC_v.push_back(aux_logic2);
            physcSC_v.push_back(aux_physc1); 
            physcSC_v.push_back(aux_physc2); 
            
            ++counter;
            ++counter;
        };
    }

    // SBND from xml ~not working for now
    // G4GDMLParser parser;
    // parser.Read("gdml/sbnd_v02_00.gdml");
    // G4VPhysicalVolume *w=parser.GetWorldVolume();
    
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    DefineMaterials();

    solidWorld = new G4Box("solidWorld", 2.5*m, 2*m, 1*m); //(name, half of the length)-> SBND case is 5x4x4, one half is 5x4x2 (2.5x2x1 since G4 doubles space)
    logicWorld = new G4LogicalVolume(solidWorld, LAr, "logicWorld"); //(solid, material, name)
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //(rotation, center coordinates, mothervolume, name, ..., true=checks overlaps ALWAYS)
    logicWorld->SetVisAttributes(new G4VisAttributes(G4Colour(0.,0.,0.)));

    ConstructScintillator();
    
    return physWorld; // return highest mother volume
}


void MyDetectorConstruction::ConstructSDandField()
{
    // >> Example. one XARAPUCA:
    // MySensitiveDetector *sensSC = new MySensitiveDetector("SensitiveSC");
    // logicSC->SetSensitiveDetector(sensSC);


    // >> Make all XARAPUCAs sensitive 
    std::vector< MySensitiveDetector * > sensSC_v;
    G4String sens_label="SensitiveSC_";

    for (int i=0;i<int(logicSC_v.size());i++) 
    {
        MySensitiveDetector *aux_sen=new MySensitiveDetector(sens_label+std::to_string(i));
        logicSC_v[i]->SetSensitiveDetector(aux_sen);
        sensSC_v.push_back(aux_sen);
    }

}