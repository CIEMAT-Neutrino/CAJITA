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
    Aluminum = nist->FindOrBuildMaterial("G4_Al");
    Steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    Plastic = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    const G4int num = 2;
    G4double energy[num] = {1.239841939 * eV / 0.128, 1.239841939 * eV / 0.9}; // momentum of optical photon; conversion wavelenght(um) to energy
    G4double rindexAir[num] = {1.0, 1.0};                                      // Refraction index for propagation (for photons to propagate in Air)
    G4double rindexLAr[num] = {1.38, 1.38};                                    // not considering dispersion so assume rindex constant
    G4double reflectivityAluminum[num] = {0.9, 0.9}; // reflectivity of aluminum
    G4double reflectivitySteel[num] = {0.35, 0.35}; // reflectivity of steel
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

    G4MaterialPropertiesTable *mptPlastic = new G4MaterialPropertiesTable();
    mptPlastic->AddProperty("RINDEX", energy, rindexLAr, num);
    Plastic->SetMaterialPropertiesTable(mptPlastic);
    G4MaterialPropertiesTable *mptAluminum = new G4MaterialPropertiesTable();
    mptMetal->AddProperty("REFLECTIVITY", energy, reflectivityAluminum, num);
    Aluminum->SetMaterialPropertiesTable(mptSteel);
    G4MaterialPropertiesTable *mptSteel = new G4MaterialPropertiesTable();
    mptMetal->AddProperty("REFLECTIVITY", energy, reflectivitySteel, num);

    Steel->SetMaterialPropertiesTable(mptMetal);
}

void MyDetectorConstruction::ConstructScintillator()
{
    const Json_file fjson = Json_file(fjsonName);

    // >> Set visAttributes for various components
    G4VisAttributes *logicSolidVisAtt    = new G4VisAttributes(G4Colour(0.46, 0.53, 0.6, 0.3));  // grey + alpha
    G4VisAttributes *logicHoleVisAtt     = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 1));      // black + alpha
    G4VisAttributes *logicSCVisAtt       = new G4VisAttributes(G4Colour(0.25, 0.88, 0.41, 0.5)); // green
    G4VisAttributes *logicDetectorVisAtt = new G4VisAttributes(G4Colour(1, 0, 0, 1));         // red
    G4VisAttributes *logicFiltroVisAtt   = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.7));
    G4VisAttributes *logicTestVisAtt     = new G4VisAttributes(G4Colour(0,0,1,0.5)); // blue
    logicSolidVisAtt->SetForceSolid(true);
    logicHoleVisAtt->SetForceWireframe(true);
    logicDetectorVisAtt->SetForceSolid(true);
    logicSCVisAtt->SetForceSolid(true);
    logicFiltroVisAtt->SetForceSolid(true);
    logicTestVisAtt->SetForceSolid(true);

    // >> Matriz de rotacion para lo relativo a la SC
    G4RotationMatrix *pRotX = new G4RotationMatrix();
    G4RotationMatrix *pRotY = new G4RotationMatrix();
    G4RotationMatrix *pRotZ = new G4RotationMatrix();
    pRotX->rotateX(90. * deg);
    pRotY->rotateY(90. * deg);
    pRotZ->rotateZ(90. * deg);

    // ---- Cajita (hueca) ----- //
    // >> Cajita Exterior (+grande, plastico)
    std::vector<double> outter_dim,inner_dim;
    for (double coord:fjson.json_map["big_cajita"]["outter_dim"]) outter_dim.push_back(coord);
    for (double coord:fjson.json_map["big_cajita"]["inner_dim"])  inner_dim.push_back(coord);
    auto outerBox = new G4Box("solidCajitaOut1", outter_dim[0] * mm, outter_dim[1] * mm, outter_dim[2] * mm);
    auto innerBox = new G4Box("solidCajitaOut2", inner_dim[0]  * mm, inner_dim[1]  * mm, inner_dim[2]  * mm);
    G4SubtractionSolid *solidCajitaOut = new G4SubtractionSolid("solidCajitaOut",outerBox,innerBox);
    G4LogicalVolume *logicCajitaOut;
    if (fjson.json_map["big_cajita"]["material"] == "Steel")
        logicCajitaOut = new G4LogicalVolume(solidCajitaOut, Steel, "logicCajitaOut");
    else if (fjson.json_map["big_cajita"]["material"] == "Aluminum")
        logicCajitaOut = new G4LogicalVolume(solidCajitaOut, Aluminum, "logicCajitaOut");
    else
    {
        std::cerr << "Material for big_cajita not recognized. Using Plastic as default." << std::endl;
        logicCajitaOut = new G4LogicalVolume(solidCajitaOut, Plastic, "logicCajitaOut");
    }
    G4VPhysicalVolume *physCajitaOut  = new G4PVPlacement(0, G4ThreeVector(0 * mm, inner_dim[1] * mm, 0 * mm), logicCajitaOut, "physCajitaOut", logicWorld, false, 1, true);
    logicCajitaOut->SetVisAttributes(logicSolidVisAtt); // solid+grey

    // --- SC --- //
    std::vector<double> SC_dim, SC_pos, SC_rot;
    for (double coord:fjson.json_map["arapuca"]["dim"]) SC_dim.push_back(coord);
    for (double coord:fjson.json_map["arapuca"]["pos"]) SC_pos.push_back(coord);
    for (double coord:fjson.json_map["arapuca"]["rot"]) SC_rot.push_back(coord);
    G4Box *solidSC = new G4Box("solidSC", SC_dim[0] * mm, SC_dim[1] * mm, SC_dim[2] * mm); //--<<-- 1 filtro de VD X-ARAPUCA
    logicSC = new G4LogicalVolume(solidSC, LAr, "logicSC");                     //--<<--
    logicSC->SetVisAttributes(logicSCVisAtt);
    G4VPhysicalVolume *physSC = new G4PVPlacement(0, G4ThreeVector(0 * mm,  SC_pos[1]* mm, 0 * mm), logicSC, "physSC", logicWorld, false, 1, true);

    // --- SiPM x2 --- //
    std::vector<double> SiPM_dim, SiPM1_x ,SiPM2_x ,SiPM1_x_tapa ,SiPM2_x_tapa,SiPM_rot;
    for (double coord:fjson.json_map["sipms"]["dim"])  SiPM_dim.push_back(coord);
    for (double coord:fjson.json_map["sipms"]["pos1"]) SiPM1_x.push_back(coord);
    for (double coord:fjson.json_map["sipms"]["pos2"]) SiPM2_x.push_back(coord);
    for (double coord:fjson.json_map["sipms"]["rot"])  SiPM_rot.push_back(coord);
    G4Box *solidSiPM = new G4Box("solidSiPM", SiPM_dim[0] * mm, SiPM_dim[1] * mm, SiPM_dim[2] * mm);
    logicSiPM1 = new G4LogicalVolume(solidSiPM, LAr, "logicSiPM1");
    logicSiPM1->SetVisAttributes(logicDetectorVisAtt);
    logicSiPM2 = new G4LogicalVolume(solidSiPM, LAr, "logicSiPM2");
    logicSiPM2->SetVisAttributes(logicDetectorVisAtt);
    
    G4RotationMatrix *R_SiPM1 = new G4RotationMatrix();
    G4RotationMatrix *R_SiPM2 = new G4RotationMatrix();
    
    R_SiPM1->rotateX( SiPM_rot[0]*deg);
    R_SiPM1->rotateY( SiPM_rot[1]*deg);
    R_SiPM1->rotateZ( SiPM_rot[2]*deg);

    R_SiPM2->rotateX( SiPM_rot[0]*deg);
    R_SiPM2->rotateY( SiPM_rot[1]*deg);
    R_SiPM2->rotateZ( SiPM_rot[2]*deg);
    
    G4VPhysicalVolume *physSiPM1 = new G4PVPlacement(R_SiPM1, G4ThreeVector( SiPM1_x[0] * mm, SiPM1_x[1] * mm, SiPM1_x[2] * mm), logicSiPM1, "physSiPM1", logicWorld, false, 1, true);
    G4VPhysicalVolume *physSiPM2 = new G4PVPlacement(R_SiPM2, G4ThreeVector( SiPM2_x[0] * mm, SiPM2_x[1] * mm, SiPM2_x[2] * mm), logicSiPM2, "physSiPM2", logicWorld, false, 1, true);
    
    // Insert optional components:
    std::vector<double> tower_dim, tower_buffer;
    if(fjson.json_map.contains("tower_sipms")){
        std::cout<<"---------------- INSERTING SIPMs' REBABA ----------------"<<std::endl;
        for (double coord:fjson.json_map["tower_sipms"]["dim"]) tower_dim.push_back(coord);
        for (double coord:fjson.json_map["tower_sipms"]["buffer"]) tower_buffer.push_back(coord);
        auto outerRebaba = new G4Box("solidRebabaOut1", inner_dim[0] * mm, tower_dim[1] * mm, inner_dim[2] * mm);
        auto innerRebaba = new G4Box("solidRebabaOut2", (SiPM1_x[0]+SiPM_dim[0]+tower_buffer[0]) * mm, tower_dim[1] * mm, (SiPM1_x[0]+SiPM_dim[0]+tower_buffer[0]) * mm);
        G4SubtractionSolid *solidRebabaOut = new G4SubtractionSolid("solidRebabaOut",outerRebaba,innerRebaba);
        G4LogicalVolume *logicRebabaOut = new G4LogicalVolume(solidRebabaOut, Plastic, "logicRebabaOut");
        G4VPhysicalVolume *physRebabaOut  = new G4PVPlacement(0, G4ThreeVector(0 * mm, 2*inner_dim[1]-tower_dim[1] * mm, 0 * mm), logicRebabaOut, "physRebabaOut", logicWorld, false, 1, true);
        logicRebabaOut->SetVisAttributes(logicSolidVisAtt); // blue
    }
    std::vector<double> support_dim, support_angle, support_x;
    if(fjson.json_map.contains("alpha_support")){
        std::cout<<"---------------- INSERTING ALPHA SUPPORT ----------------"<<std::endl;
        for (double coord:fjson.json_map["alpha_support"]["dim"]) support_dim.push_back(coord);
        for (double coord:fjson.json_map["alpha_support"]["angle"]) support_angle.push_back(coord);
        for (double coord:fjson.json_map["alpha_support"]["pos"]) support_x.push_back(coord);
        G4Tubs *solidFalphaOut = new G4Tubs("solidFalphaOut", support_dim[0] * mm, support_dim[1] * mm, support_dim[2] * mm, support_angle[0]*deg, support_angle[1]*deg);
        G4LogicalVolume   *logicFalphaOut = new G4LogicalVolume(solidFalphaOut, Plastic, "logicFalphaOut");
        G4VPhysicalVolume *physFalphaOut  = new G4PVPlacement(pRotX, G4ThreeVector(support_x[0] * mm, 2*(inner_dim[1]-support_dim[2]/2) * mm, support_x[2] * mm), logicFalphaOut,  "physFalphaOut", logicWorld, false, 0, 1);
        logicFalphaOut->SetVisAttributes(logicTestVisAtt);
    }
    std::vector<double> mask_dim, mask_angle, mask_x;
    if(fjson.json_map.contains("alpha_mask")){
        std::cout<<"---------------- INSERTING ALPHA MASK ----------------"<<std::endl;
        for (double coord:fjson.json_map["alpha_mask"]["dim"]) mask_dim.push_back(coord);
        for (double coord:fjson.json_map["alpha_mask"]["angle"]) mask_angle.push_back(coord);
        for (double coord:fjson.json_map["alpha_mask"]["pos"]) mask_x.push_back(coord);
        G4Tubs *solidMalphaOut = new G4Tubs("solidMalphaOut", mask_dim[0] * mm, mask_dim[1] * mm, mask_dim[2] * mm, mask_angle[0]*deg, mask_angle[1]*deg);
        G4LogicalVolume   *logicMalphaOut = new G4LogicalVolume(solidMalphaOut, Plastic, "logicMalphaOut");
        G4VPhysicalVolume *physMalphaOut  = new G4PVPlacement(pRotX, G4ThreeVector(mask_x[0] * mm, 2*(inner_dim[1]-support_dim[2]/2-mask_x[1]/2) * mm, mask_x[2] * mm), logicMalphaOut,  "physMalphaOut", logicWorld, false, 0, 1);
        logicMalphaOut->SetVisAttributes(logicTestVisAtt);
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