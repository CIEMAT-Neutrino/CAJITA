#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    std::cout << "No geometry file provided." << std::endl;
    fjsonName = "None";
}

MyDetectorConstruction::MyDetectorConstruction(G4String jsonName)
{
    // std::cout<<"Loading Geometry from: "<<jsonName<<" file."<<std::endl;
    fjsonName = jsonName;
}

MyDetectorConstruction::~MyDetectorConstruction()
{
}

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
    G4double energy[num] = {1.239841939 * eV / 0.128, 1.239841939 * eV / 0.9}; // momentum of optical photon; conversion wavelenght(um) to energy
    G4double rindexAir[num] = {1.0, 1.0};                                      // Refraction index for propagation (for photons to propagate in Air)
    G4double rindexLAr[num] = {1.38, 1.38};                                    // not considering dispersion so assume rindex constant
    G4double reflectivity[num] = {0.94, 0.94};
    // G4double efficiency[num] = {0.8, 0.1};

    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", energy, rindexAir, num);
    Air->SetMaterialPropertiesTable(mptAir);

    G4MaterialPropertiesTable *mptLAr = new G4MaterialPropertiesTable();

    std::vector<G4double> AbsLengthEnergies = {3. * eV, 4. * eV, 5. * eV, 6. * eV, 7. * eV,
                                               8. * eV, 9. * eV, 10. * eV, 11. * eV};

    std::vector<G4double> AbsLengthSpectrum = {2000., 2000., 2000., 2000., 2000., 2000., 2000., 2000., 2000.};
    std::vector<G4double> AbsLengthSpectrum_metal = {0., 000., 000., 000., 000., 000., 000., 000., 000.};

    std::vector<G4double> RIndexEnergies = {
        1.18626 * eV, 1.68626 * eV, 2.18626 * eV, 2.68626 * eV, 3.18626 * eV, 3.68626 * eV,
        4.18626 * eV, 4.68626 * eV, 5.18626 * eV, 5.68626 * eV, 6.18626 * eV, 6.68626 * eV,
        7.18626 * eV, 7.68626 * eV, 8.18626 * eV, 8.68626 * eV, 9.18626 * eV, 9.68626 * eV,
        1.01863e1 * eV, 1.06863e1 * eV, 1.11863e1 * eV};

    std::vector<G4double> RIndexSpectrum = {
        1.24664, 1.2205, 1.22694, 1.22932, 1.23124, 1.23322,
        1.23545, 1.23806, 1.24116, 1.24489, 1.24942, 1.25499,
        1.26197, 1.2709, 1.28263, 1.29865, 1.32169, 1.35747,
        1.42039, 1.56011, 2.16626};

    std::vector<G4double> RayleighEnergies = {
        1.18626 * eV, 1.68626 * eV, 2.18626 * eV, 2.68626 * eV, 3.18626 * eV, 3.68626 * eV,
        4.18626 * eV, 4.68626 * eV, 5.18626 * eV, 5.68626 * eV, 6.18626 * eV, 6.68626 * eV,
        7.18626 * eV, 7.68626 * eV, 8.18626 * eV, 8.68626 * eV, 9.18626 * eV, 9.68626 * eV,
        10.1863 * eV, 10.6863 * eV, 11.1863 * eV};
    std::vector<G4double> RayleighSpectrum = {
        1200800 * cm, 390747 * cm, 128633 * cm, 54969.1 * cm, 27191.8 * cm, 14853.7 * cm,
        8716.9 * cm, 5397.42 * cm, 3481.37 * cm, 2316.51 * cm, 1577.63 * cm, 1092.02 * cm,
        763.045 * cm, 534.232 * cm, 371.335 * cm, 252.942 * cm, 165.38 * cm, 99.9003 * cm,
        51.2653 * cm, 17.495 * cm, 0.964341 * cm};

    mptLAr->AddProperty("RINDEX"   , RIndexEnergies   , RIndexSpectrum   ); //energy=mom in relativistic approximation
    mptLAr->AddProperty("ABSLENGTH", AbsLengthEnergies, AbsLengthSpectrum);
    mptLAr->AddProperty("RAYLEIGH" , RayleighEnergies , RayleighSpectrum );
    LAr->SetMaterialPropertiesTable(mptLAr);

    Metal = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    G4MaterialPropertiesTable *mptMetal = new G4MaterialPropertiesTable();
    mptMetal->AddProperty("ABSLENGTH", AbsLengthEnergies, AbsLengthSpectrum_metal);

    Metal->SetMaterialPropertiesTable(mptMetal);
}

void MyDetectorConstruction::ConstructScintillator()
{

    // >> Set visAttributes for various components
    G4VisAttributes *logicSolidVisAtt = new G4VisAttributes(G4Colour(0.46, 0.53, 0.6, 0.3));  // grey + alpha
    G4VisAttributes *logicHoleVisAtt  = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 1));      // black + alpha
    G4VisAttributes *logicSCVisAtt    = new G4VisAttributes(G4Colour(0.25, 0.88, 0.41, 0.5)); // green
    G4VisAttributes *logicDetectorVisAtt = new G4VisAttributes(G4Colour(1, 0, 0, 1));         // red
    G4VisAttributes *logicFiltroVisAtt   = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.7));
    logicSolidVisAtt->SetForceSolid(true);
    logicHoleVisAtt->SetForceWireframe(true);
    logicDetectorVisAtt->SetForceSolid(true);
    logicSCVisAtt->SetForceSolid(true);
    logicFiltroVisAtt->SetForceSolid(true);

    // >> Matriz de rotacion para lo relativo a la SC
    G4RotationMatrix *pRotX = new G4RotationMatrix();
    G4RotationMatrix *pRotY = new G4RotationMatrix();
    G4RotationMatrix *pRotZ = new G4RotationMatrix();
    pRotX->rotateX(90. * deg);
    pRotY->rotateY(90. * deg);
    pRotZ->rotateZ(90. * deg);

    // ---- Cajita (hueca) ----- //
    // >> Cajita Exterior (+grande, plastico)
    // auto outerBox = new G4Box("solidCajitaOut1", 88.375 * mm, 25   * mm, 78.375 * mm);
    auto outerBox = new G4Box("solidCajitaOut1", 78.375 * mm, 53.325   * mm, 78.375 * mm);
    auto innerBox = new G4Box("solidCajitaOut2", 71.875 * mm, 52.325 * mm, 71.875 * mm);
    // auto innerBox = new G4Box("solidCajitaOut2", 81.875 * mm, 24.5 * mm, 71.875 * mm);
    G4SubtractionSolid *solidCajitaOut = new G4SubtractionSolid("solidCajitaOut",outerBox,innerBox);
    G4LogicalVolume *logicCajitaOut = new G4LogicalVolume(solidCajitaOut, Plastic, "logicCajitaOut");
    // physCajitaOut  = new G4PVPlacement(0, G4ThreeVector(10 * mm, 25 * mm, 0 * mm), logicCajitaOut, "physCajitaOut", logicWorld, false, 1, true);
    G4VPhysicalVolume *physCajitaOut  = new G4PVPlacement(0, G4ThreeVector(0 * mm, 52.025 * mm, 0 * mm), logicCajitaOut, "physCajitaOut", logicWorld, false, 1, true);
    logicCajitaOut->SetVisAttributes(logicSolidVisAtt); // solid+grey


    // ---- Rebaba ----- //
    // G4Tubs *solidFalphaOut = new G4Tubs("solidFalphaOut", 12.5 * mm, 13.5 * mm, 0.85 * mm, 65*deg, 230*deg); //rebaba
    // G4LogicalVolume   *logicFalphaOut = new G4LogicalVolume(solidFalphaOut, Metal, "logicFalphaOut");
    // G4VPhysicalVolume *physFalphaOut  = new G4PVPlacement(pRotX, G4ThreeVector(0 * mm, 103.2 * mm, 0* mm), logicFalphaOut,  "physFalphaOut", logicWorld, false, 0, 1);
    // logicFalphaOut->SetVisAttributes(logicSolidVisAtt);


    // --- SC --- //
    G4Box *solidSC = new G4Box("solidSC", 71.875 * mm, 0.01 * mm, 71.875 * mm); //--<<-- 1 filtro de VD X-ARAPUCA
    logicSC = new G4LogicalVolume(solidSC, LAr, "logicSC");            //--<<--
    logicSC->SetVisAttributes(logicSCVisAtt);

    // --- SiPM x2 --- //
    G4Box *solidSiPM = new G4Box("solidSiPM", 0.5 * mm, 3 * mm, 3 * mm);
    logicSiPM1 = new G4LogicalVolume(solidSiPM, LAr, "logicSiPM1");
    logicSiPM1->SetVisAttributes(logicDetectorVisAtt);
    logicSiPM2 = new G4LogicalVolume(solidSiPM, LAr, "logicSiPM2");
    logicSiPM2->SetVisAttributes(logicDetectorVisAtt);

    // --- Tapas --- //
    G4Box *solidSiPM_tapa = new G4Box("solidSiPM_tapa", 0.5 * mm, 3 * mm, 3 * mm);
    G4LogicalVolume *logicSiPM1_tapa = new G4LogicalVolume(solidSiPM_tapa, Metal, "logicSiPM1_tapa");
    G4LogicalVolume *logicSiPM2_tapa = new G4LogicalVolume(solidSiPM_tapa, Metal, "logicSiPM2_tapa");
    logicSiPM1_tapa->SetVisAttributes(logicSolidVisAtt);
    logicSiPM2_tapa->SetVisAttributes(logicSolidVisAtt);

    if (check_is_file_type(fjsonName) && check_json_file(fjsonName))
    { // load geometry from the json file instead:
        const Json_file fjson = Json_file(fjsonName);
        std::vector<double> SiPM1_x ,SiPM2_x ,SiPM1_x_tapa ,SiPM2_x_tapa,SiPM_rot;

        for (double coord:fjson.json_map["SiPM1"]["X"]) SiPM1_x.push_back(coord);
        for (double coord:fjson.json_map["SiPM2"]["X"]) SiPM2_x.push_back(coord);
        for (double coord:fjson.json_map["SiPM1_tapa"]["X"]) SiPM1_x_tapa.push_back(coord);
        for (double coord:fjson.json_map["SiPM2_tapa"]["X"]) SiPM2_x_tapa.push_back(coord);
        for (double coord:fjson.json_map["SiPM_rot"]) SiPM_rot.push_back(coord);
        
        G4RotationMatrix *R_SiPM1 = new G4RotationMatrix();
        G4RotationMatrix *R_SiPM2 = new G4RotationMatrix();
        
        R_SiPM1->rotateX( SiPM_rot[0]*deg);
        R_SiPM1->rotateY( SiPM_rot[1]*deg);
        R_SiPM1->rotateZ( SiPM_rot[2]*deg);

        R_SiPM2->rotateX(-SiPM_rot[0]*deg);
        R_SiPM2->rotateY(-SiPM_rot[1]*deg);
        R_SiPM2->rotateZ(-SiPM_rot[2]*deg);
        
        G4VPhysicalVolume *physSiPM1      = new G4PVPlacement(R_SiPM1, G4ThreeVector(     SiPM1_x[0] * mm,      SiPM1_x[1] * mm,      SiPM1_x[2] * mm), logicSiPM1,      "physSiPM1",      logicWorld, false, 1, true);
        G4VPhysicalVolume *physSiPM1_tapa = new G4PVPlacement(R_SiPM1, G4ThreeVector(SiPM1_x_tapa[0] * mm, SiPM1_x_tapa[1] * mm, SiPM1_x_tapa[2] * mm), logicSiPM1_tapa, "physSiPM1_tapa", logicWorld, false, 1, true);

        G4VPhysicalVolume *physSiPM2      = new G4PVPlacement(R_SiPM2, G4ThreeVector(     SiPM2_x[0] * mm,      SiPM2_x[1] * mm,      SiPM2_x[2] * mm), logicSiPM2,      "physSiPM2",      logicWorld, false, 1, true);
        G4VPhysicalVolume *physSiPM2_tapa = new G4PVPlacement(R_SiPM2, G4ThreeVector(SiPM2_x_tapa[0] * mm, SiPM2_x_tapa[1] * mm, SiPM2_x_tapa[2] * mm), logicSiPM2_tapa, "physSiPM2_tapa", logicWorld, false, 1, true);
        
            if(fjson.json_map.contains("rebaba_sipms"))
            {
                std::cout<<"---------------- INSERTING SIPMs' REBABA ----------------"<<std::endl;
                // --- Rebaba para acercar los SiPMs a la fuente (Geo1_ancho) --- //
                std::vector<double> rebaba_dim,rebaba_x,rebaba_rot;
                for (double coord:fjson.json_map["rebaba_sipms"]["dim"]) rebaba_dim.push_back(coord);
                for (double coord:fjson.json_map["rebaba_sipms"]["x"])   rebaba_x.push_back(coord);
                for (double coord:fjson.json_map["rebaba_sipms"]["rot"]) rebaba_rot.push_back(coord);

                G4RotationMatrix *R_rebaba = new G4RotationMatrix();
                R_rebaba->rotateX(rebaba_rot[0]*deg);
                R_rebaba->rotateY(rebaba_rot[1]*deg);
                R_rebaba->rotateZ(rebaba_rot[2]*deg);

                G4Box *solidSiPMRebaba = new G4Box("solidSiPMOut", rebaba_dim[0] * mm, rebaba_dim[1] * mm, rebaba_dim[2] * mm);
                G4LogicalVolume *logicSiPMRebaba1 = new G4LogicalVolume(solidSiPMRebaba, Plastic, "logicSiPMOut1");
                G4LogicalVolume *logicSiPMRebaba2 = new G4LogicalVolume(solidSiPMRebaba, Plastic, "logicSiPMOut2");
                logicSiPMRebaba1->SetVisAttributes(logicSolidVisAtt);
                logicSiPMRebaba2->SetVisAttributes(logicSolidVisAtt);
                
                G4VPhysicalVolume *physSiPMRebaba1 = new G4PVPlacement(R_rebaba, G4ThreeVector( rebaba_x[0] * mm, rebaba_x[1] * mm, rebaba_x[2] * mm), logicSiPMRebaba1, "physSiPMRebaba1", logicWorld, false, 1, true);
                G4VPhysicalVolume *physSiPMRebaba2 = new G4PVPlacement(R_rebaba, G4ThreeVector(-rebaba_x[0] * mm, rebaba_x[1] * mm, rebaba_x[2] * mm), logicSiPMRebaba2, "physSiPMRebaba2", logicWorld, false, 1, true);
            }
            if(fjson.json_map.contains("XArapuca"))
            {
                std::vector<double> XA;
                for (double coord:fjson.json_map["XArapuca"]["X"]) XA.push_back(coord);
                std::cout<<"---------------- XArapuca position entered in json ----------------"<<std::endl;
                G4VPhysicalVolume *physSC = new G4PVPlacement(0, G4ThreeVector(XA[0] * mm, XA[1] * mm, XA[2] * mm), logicSC, "physSC", logicWorld, false, 1, true);
            }
            else
            {
                std::cout<<"---------------- XArapuca default position ----------------"<<std::endl;
                G4VPhysicalVolume *physSC = new G4PVPlacement(0, G4ThreeVector(0 * mm, 0.5 * mm, 0 * mm), logicSC, "physSC", logicWorld, false, 1, true);
            }
    }
    
    else
    {
        G4VPhysicalVolume *physSC    = new G4PVPlacement(0, G4ThreeVector(0 * mm, 0.5 * mm, 0 * mm)            , logicSC,    "physSC",    logicWorld, false, 1, true);
        G4VPhysicalVolume *physSiPM2 = new G4PVPlacement(0, G4ThreeVector(  70.875 * mm, 99.15 * mm, 0 * mm), logicSiPM2, "physSiPM2", logicWorld, false, 1, true); 
        G4VPhysicalVolume *physSiPM1 = new G4PVPlacement(0, G4ThreeVector( -70.875 * mm, 99.15 * mm, 0 * mm), logicSiPM1, "physSiPM1", logicWorld, false, 1, true); 
    }   
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    DefineMaterials();

    // We also need al WORLD volume to place the detector there.
    // Every volume contains three parts:
    // Solid -> volume; Logical -> material;
    // Physical -> places the volume in G4 space together with rotation, translation ...
    solidWorld = new G4Box("solidWorld", 200 * mm, 200 * mm, 200 * mm);                                      //(name, half of the length)
    logicWorld = new G4LogicalVolume(solidWorld, LAr, "logicWorld"); //(solid, material, name)
    physWorld  = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //(rotation, center coordinates, mothervolume, name, ..., true=checks overlaps ALWAYS)
    logicWorld->SetVisAttributes(new G4VisAttributes(G4Colour(0.,0.,0.)));
    ConstructScintillator();

    return physWorld; // return highest mother volume
}

void MyDetectorConstruction::ConstructSDandField()
{
    // In detector.cc we include the sensitivity of the detectors.
    // Need to upgrade to include the 3 detectors sensitivity separately
    MySensitiveDetector *sensSC    = new MySensitiveDetector("SensitiveSC");
    MySensitiveDetector *sensSiPM1 = new MySensitiveDetector("SensitiveSiPM1");
    MySensitiveDetector *sensSiPM2 = new MySensitiveDetector("SensitiveSiPM2");
    
    // check geometry.hh and include the logic of the detector you need to be sensitive
    logicSC->SetSensitiveDetector(sensSC);
    if (check_is_file_type(fjsonName) && check_json_file(fjsonName))
    { 
        logicSiPM1->SetSensitiveDetector(sensSiPM1);
        logicSiPM2->SetSensitiveDetector(sensSiPM2);
    }

}