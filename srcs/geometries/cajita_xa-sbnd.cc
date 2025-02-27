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
    G4VisAttributes *logicSolidVisAtt = new G4VisAttributes(G4Colour(0.46, 0.53, 0.6, 0.3));   // grey + alpha
    G4VisAttributes *logicHoleVisAtt  = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 1));        // black + alpha
    G4VisAttributes *logicSCVisAtt    = new G4VisAttributes(G4Colour(0.25, 0.88, 0.41, 0.5));     // green
    G4VisAttributes *logicDetectorVisAtt = new G4VisAttributes(G4Colour(0.25, 0.41, 0.88, 1)); // blue
    G4VisAttributes *logicFiltroVisAtt   = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.7));
    logicSolidVisAtt->SetForceSolid(true);
    logicHoleVisAtt->SetForceWireframe(true);
    logicDetectorVisAtt->SetForceSolid(true);
    logicSCVisAtt->SetForceSolid(true);
    logicFiltroVisAtt->SetForceSolid(true);

    // >> Matriz de rotacion para lo relativo a la SC
    G4RotationMatrix *pRotX = new G4RotationMatrix();
    G4RotationMatrix *pRotY = new G4RotationMatrix();
    pRotX->rotateX(90. * deg);
    pRotY->rotateY(90. * deg);

    // ---- Cajita (hueca) ----- //
    // >> Cajita Exterior (+grande, plastico)
    auto outerBox = new G4Box("solidCajitaOut1", 44   * mm, 51   * mm, 55.5 * mm);
    auto innerBox = new G4Box("solidCajitaOut2", 37.5 * mm, 50.5 * mm, 49   * mm);
    G4SubtractionSolid *solidCajitaOut = new G4SubtractionSolid("solidCajitaOut",outerBox,innerBox);
    logicCajitaOut = new G4LogicalVolume(solidCajitaOut, Plastic, "logicCajitaOut");
    physCajitaOut  = new G4PVPlacement(0, G4ThreeVector(0 * mm, 44 * mm, 53 * mm), logicCajitaOut, "physCajitaOut", logicWorld, false, 1, true);
    logicCajitaOut->SetVisAttributes(logicSolidVisAtt); // solid+grey

    // --- SC --- //
    G4Box *solidSC = new G4Box("solidSC", 37.5 * mm, 2 * mm, 49 * mm); //--<<-- 1 filtro de SBND X-ARAPUCA
    logicSC = new G4LogicalVolume(solidSC, LAr, "logicSC");            //--<<--
    // physSC = new G4PVPlacement(0, G4ThreeVector(0 * mm, -2 * mm, 53 * mm), logicSC, "physSC", logicWorld, false, 1, true);
    G4LogicalVolume *logicSCblind  = new G4LogicalVolume(solidSC, LAr, "logicSCblind");
    G4VPhysicalVolume *physSCBlind = new G4PVPlacement(0, G4ThreeVector(0 * mm, -2 * mm, -53 * mm), logicSCblind, "physSCblind", logicWorld, false, 1, true); // d=21mm + 2mm (altura ejey SC)
    logicSC->SetVisAttributes(logicSCVisAtt);
    logicSCblind->SetVisAttributes(logicSCVisAtt);

    // --- SiPM x2 --- //
    solidSiPM = new G4Box("solidSiPM", 0.5 * mm, 3 * mm, 3 * mm);
    logicSiPM1 = new G4LogicalVolume(solidSiPM, LAr, "logicSiPM1");
    logicSiPM2 = new G4LogicalVolume(solidSiPM, LAr, "logicSiPM2");
    logicSiPM1->SetVisAttributes(logicDetectorVisAtt);
    logicSiPM2->SetVisAttributes(logicDetectorVisAtt);

    logicSiPM3 = new G4LogicalVolume(solidSiPM, LAr, "logicSiPM3");
    logicSiPM4 = new G4LogicalVolume(solidSiPM, LAr, "logicSiPM4");
    logicSiPM3->SetVisAttributes(logicDetectorVisAtt);
    logicSiPM4->SetVisAttributes(logicDetectorVisAtt);


    // --- Tapas --- //
    G4Box *solidSiPM_tapa = new G4Box("solidSiPM_tapa", 0.5 * mm, 3 * mm, 3 * mm);
    logicSiPM1_tapa = new G4LogicalVolume(solidSiPM_tapa, Metal, "logicSiPM1_tapa");
    logicSiPM2_tapa = new G4LogicalVolume(solidSiPM_tapa, Metal, "logicSiPM2_tapa");
    logicSiPM1_tapa->SetVisAttributes(logicSolidVisAtt);
    logicSiPM2_tapa->SetVisAttributes(logicSolidVisAtt);

    G4LogicalVolume *logicSiPM3_tapa = new G4LogicalVolume(solidSiPM_tapa, Metal, "logicSiPM3_tapa");
    G4LogicalVolume *logicSiPM4_tapa = new G4LogicalVolume(solidSiPM_tapa, Metal, "logicSiPM4_tapa");
    logicSiPM3_tapa->SetVisAttributes(logicSolidVisAtt);
    logicSiPM4_tapa->SetVisAttributes(logicSolidVisAtt);
    // --- tapa --- //
    // G4Box *solidSC_tapa = new G4Box("solidSC", 37.5 * mm, 2 * mm, 110 * mm); //--<<-- 1 filtro de SBND X-ARAPUCA
    // logicSC_tapa = new G4LogicalVolume(solidSC_tapa, Metal, "logicSC");            //--<<--
    // physSC_tapa = new G4PVPlacement(0, G4ThreeVector(0 * mm, -6 * mm, 0 * mm), logicSC_tapa, "physSC_tapa", logicWorld, false, 1, true);
    // logicSC_tapa->SetVisAttributes(logicSolidVisAtt);

    // ---- Rebaba ----- //
    G4Tubs *solidFalphaOut = new G4Tubs("solidFalphaOut", 12.5 * mm, 13.5 * mm, 5.75 * mm, 0*deg, 360*deg); //rebaba //poner mas alante :)
    G4LogicalVolume   *logicFalphaOut = new G4LogicalVolume(solidFalphaOut, Metal, "logicFalphaOut");
    G4VPhysicalVolume *physFalphaOut  = new G4PVPlacement(pRotY, G4ThreeVector(-31.75 * mm, 55 * mm, 52 * mm), logicFalphaOut,  "physFalphaOut", logicWorld, false, 0, 1);
    logicFalphaOut->SetVisAttributes(logicSolidVisAtt); 
    
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
        
        physSiPM1      = new G4PVPlacement(R_SiPM1, G4ThreeVector(     SiPM1_x[0] * mm,      SiPM1_x[1] * mm,      SiPM1_x[2] * mm), logicSiPM1,      "physSiPM1",      logicWorld, false, 1, true);
        physSiPM1_tapa = new G4PVPlacement(R_SiPM1, G4ThreeVector(SiPM1_x_tapa[0] * mm, SiPM1_x_tapa[1] * mm, SiPM1_x_tapa[2] * mm), logicSiPM1_tapa, "physSiPM1_tapa", logicWorld, false, 1, true);

        physSiPM2      = new G4PVPlacement(R_SiPM2, G4ThreeVector(     SiPM2_x[0] * mm,      SiPM2_x[1] * mm,      SiPM2_x[2] * mm), logicSiPM2,      "physSiPM2",      logicWorld, false, 1, true);
        physSiPM2_tapa = new G4PVPlacement(R_SiPM2, G4ThreeVector(SiPM2_x_tapa[0] * mm, SiPM2_x_tapa[1] * mm, SiPM2_x_tapa[2] * mm), logicSiPM2_tapa, "physSiPM2_tapa", logicWorld, false, 1, true);
        
            if(fjson.json_map.contains("XArapuca"))
            {
                std::vector<double> XA;
                for (double coord:fjson.json_map["XArapuca"]["X"]) XA.push_back(coord);
                std::cout<<"---------------- XArapuca position entered in json ----------------"<<std::endl;
                physSC = new G4PVPlacement(0, G4ThreeVector(XA[0] * mm, XA[1] * mm, XA[2] * mm), logicSC, "physSC", logicWorld, false, 1, true);
            }
            else
            {
                std::cout<<"---------------- XArapuca default position ----------------"<<std::endl;
                physSC = new G4PVPlacement(0, G4ThreeVector(0 * mm, -2 * mm, 53 * mm), logicSC, "physSC", logicWorld, false, 1, true);
            }
        
        //tapa girada (solo 3ª geometría)
            if(fjson.json_map.contains("tapa_girada"))
            {
                G4Box *tapa_girada = new G4Box("tapa_girada", 8 * cm, 0.5 * mm, 8 * cm);
                G4LogicalVolume *logic_tapa_girada  = new G4LogicalVolume(tapa_girada, Metal, "logic_tapa_girada");
                logic_tapa_girada ->SetVisAttributes(logicSolidVisAtt);

                std::vector<double> tapa_x,tapa_rot;
                for (double coord:fjson.json_map["tapa_girada"]["x"])   tapa_x.push_back(coord);
                for (double coord:fjson.json_map["tapa_girada"]["rot"]) tapa_rot.push_back(coord);

                G4RotationMatrix *R_tapa      = new G4RotationMatrix();
                R_tapa->rotateX(tapa_rot[0]*deg);
                R_tapa->rotateY(tapa_rot[1]*deg);
                R_tapa->rotateZ(tapa_rot[2]*deg);

                G4VPhysicalVolume *phys_tapa_girada;
                phys_tapa_girada = new G4PVPlacement(R_tapa, G4ThreeVector(tapa_x[0] * mm, tapa_x[1] * mm, tapa_x[2] * mm), logic_tapa_girada, "phys_tapa_girada", logicWorld, false, 1, true);

            }
            
            // FINAL GEOMETRY --> 4SiPMs and VUV 23mm closer to the source//
            if(fjson.json_map.contains("rebaba_sipms"))
            {
                std::cout<<"---------------- INSERTING SIPMs' REBABA (default dimensions 23x8x16 mm3 - recompile and change if necessary) ----------------"<<std::endl;
                // --- Rebaba para acercar los SiPMs a la fuente (Geo1_ancho) --- //
                G4Box *solidSiPMRebaba = new G4Box("solidSiPMOut", 5.75*mm, 12.5*mm, 17.5*mm); //11.5 sobresalir en el ejeX
                G4LogicalVolume *logicSiPMRebaba = new G4LogicalVolume(solidSiPMRebaba, Plastic, "logicSiPMOut");
                logicSiPMRebaba->SetVisAttributes(logicSolidVisAtt);

                std::vector<double> rebaba_x,rebaba_rot;
                for (double coord:fjson.json_map["rebaba_sipms"]["x"])   rebaba_x.push_back(coord);
                for (double coord:fjson.json_map["rebaba_sipms"]["rot"]) rebaba_rot.push_back(coord);

                G4RotationMatrix *R_rebaba      = new G4RotationMatrix();
                R_rebaba->rotateX(rebaba_rot[0]*deg);
                R_rebaba->rotateY(rebaba_rot[1]*deg);
                R_rebaba->rotateZ(rebaba_rot[2]*deg);

                G4VPhysicalVolume *physSiPMRebaba = new G4PVPlacement(R_rebaba, G4ThreeVector(rebaba_x[0] * mm, rebaba_x[1] * mm, rebaba_x[2] * mm), logicSiPMRebaba, "physSiPMRebaba", logicWorld, false, 1, true);
            }

            if(fjson.json_map.contains("SiPM3") && fjson.json_map.contains("SiPM4"))
            {
                std::cout<<"---------------- INSERTING Vis SIPMs ----------------"<<std::endl;

                std::vector<double> SiPM3_x , SiPM4_x, SiPM3_x_tapa, SiPM4_x_tapa, SiPM_rot2;

                for (double coord:fjson.json_map["SiPM3"]     ["X"]) SiPM3_x.push_back(coord);
                for (double coord:fjson.json_map["SiPM4"]     ["X"]) SiPM4_x.push_back(coord);
                for (double coord:fjson.json_map["SiPM3_tapa"]["X"]) SiPM3_x_tapa.push_back(coord);
                for (double coord:fjson.json_map["SiPM4_tapa"]["X"]) SiPM4_x_tapa.push_back(coord);
                for (double coord:fjson.json_map["SiPM_rot2"]) SiPM_rot2.push_back(coord);
                
                
                G4RotationMatrix *R_SiPM3 = new G4RotationMatrix();
                G4RotationMatrix *R_SiPM4 = new G4RotationMatrix();
                
                R_SiPM3->rotateX( SiPM_rot2[0]*deg);
                R_SiPM3->rotateY( SiPM_rot2[1]*deg);
                R_SiPM3->rotateZ( SiPM_rot2[2]*deg);

                R_SiPM4->rotateX(-SiPM_rot2[0]*deg);
                R_SiPM4->rotateY(-SiPM_rot2[1]*deg);
                R_SiPM4->rotateZ(-SiPM_rot2[2]*deg);
                
                G4VPhysicalVolume *physSiPM3      = new G4PVPlacement(R_SiPM3, G4ThreeVector(     SiPM3_x[0] * mm,      SiPM3_x[1] * mm,      SiPM3_x[2] * mm), logicSiPM3,      "physSiPM3",      logicWorld, false, 1, true);
                G4VPhysicalVolume *physSiPM3_tapa = new G4PVPlacement(R_SiPM3, G4ThreeVector(SiPM3_x_tapa[0] * mm, SiPM3_x_tapa[1] * mm, SiPM3_x_tapa[2] * mm), logicSiPM3_tapa, "physSiPM3_tapa", logicWorld, false, 1, true);
                
                G4VPhysicalVolume *physSiPM4      = new G4PVPlacement(R_SiPM4, G4ThreeVector(     SiPM4_x[0] * mm,      SiPM4_x[1] * mm,      SiPM4_x[2] * mm), logicSiPM4,      "physSiPM4",      logicWorld, false, 1, true);
                G4VPhysicalVolume *physSiPM4_tapa = new G4PVPlacement(R_SiPM4, G4ThreeVector(SiPM4_x_tapa[0] * mm, SiPM4_x_tapa[1] * mm, SiPM4_x_tapa[2] * mm), logicSiPM4_tapa, "physSiPM4_tapa", logicWorld, false, 1, true);

            }

    }
    
    else
    {
        physSiPM1 = new G4PVPlacement(0, G4ThreeVector( 35 * mm, 55 * mm, 49 * mm), logicSiPM1, "physSiPM1", logicWorld, false, 1, true); // 18.5 //18 antes
        physSiPM2 = new G4PVPlacement(0, G4ThreeVector(-35 * mm, 55 * mm, 49 * mm), logicSiPM2, "physSiPM2", logicWorld, false, 1, true); //-17.5
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
    // G4Box *solidRadiator = new G4Box("solidRadiator", 0.01*cm, 0.01*cm, 0.01*cm); //radiator --> smaller than world volume
    // G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, LAr, "logicRadiatior");
    // G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(2*cm, 0*cm, 0*cm), logicRadiator, "physRadiator", logicWorld, false, 0, true);

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
    MySensitiveDetector *sensSiPM3 = new MySensitiveDetector("SensitiveSiPM3");
    MySensitiveDetector *sensSiPM4 = new MySensitiveDetector("SensitiveSiPM4");
    
    logicSC->SetSensitiveDetector(sensSC);
    if (check_is_file_type(fjsonName) && check_json_file(fjsonName))
    { 
        const Json_file fjson = Json_file(fjsonName);
        if(fjson.json_map.contains("vuv"))
        {
            logicSiPM1->SetSensitiveDetector(sensSiPM1);
            logicSiPM2->SetSensitiveDetector(sensSiPM2);
        }
        
        if(fjson.json_map.contains("vis"))
        {
            logicSiPM3->SetSensitiveDetector(sensSiPM3);
            logicSiPM4->SetSensitiveDetector(sensSiPM4);
        }
    }

}