#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    // We need to specify the material before the detector is defined:
    // Some are already predefined so we pick the ones we need
    G4String name, symbol;
    G4double density;  
    G4int natoms,nel;
    G4double z; 
    // Define Elements
    G4Element*   H = new G4Element ("Hydrogen","H",1.,1.01*g/mole);
    G4Element*   C = new G4Element ("Carbon","C",6.,12.01*g/mole);
    G4Element*   O = new G4Element ("Oxygen","O",8.,16.0*g/mole);
    G4Element*  Al = new G4Element(name="Aluminium",symbol="Al",z=13.,26.98*g/mole);
    G4Element*  Fe = new G4Element(name="Iron",symbol="Fe",z=26.,55.85*g/mole);
    G4Element*  Ni = new G4Element(name="Niquel",symbol="Ni",z=28.,58.6934*g/mole);
    G4Element*  Si = new G4Element(name="Silicon",symbol="Si",z=14.,28.085*g/mole);
    G4Element*  Cr = new G4Element(name="Chromium",symbol="Cr",z=24.,51.9961*g/mole);
    G4NistManager *nist = G4NistManager::Instance();

    G4Material* StainlessSteel = new G4Material(name="StainlessSteel",7.93*g/cm3,nel=4);//STEEL_STAINLESS_Fe7Cr2Ni
        StainlessSteel->AddElement(C,  0.0010);
        StainlessSteel->AddElement(Cr, 0.1792);
        StainlessSteel->AddElement(Fe, 0.7298);
        StainlessSteel->AddElement(Ni, 0.0900); 

    G4Material* G10 = new G4Material(name="G10",1.7*g/cm3,nel=4);
        G10->AddElement(Si, 0.2805);
        G10->AddElement(O,  0.3954);
        G10->AddElement(C,  0.2990);
        G10->AddElement(H,  0.0251);

     G4Material* Acrylic = new G4Material("acrylic", 1.18*g/cm3,3); //acrylic
        Acrylic->AddElement (C, 5);
        Acrylic->AddElement (O, 2);
        Acrylic->AddElement (H, 8);  
    
    // fDefaultMaterial = env_mat;
    // fBase = base_mat;
    // facrylic = acrylic;
    // fSteel = StainlessSteel;
    // fAluminium = Aluminium;
    // fG10 = G10;
    // DISPLAY MATERIALS
    // G4cout << G4endl << *(G4Material::GetMaterialTable()) << G4endl;

    Air = nist->FindOrBuildMaterial("G4_AIR");
    LAr = nist->FindOrBuildMaterial("G4_lAr");
    Plastic = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    Metal = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    
    const G4int num = 2;  
    G4double energy[num] = {1.239841939*eV/0.128, 1.239841939*eV/0.9}; //momentum of optical photon; conversion wavelenght(um) to energy 
    G4double rindexAir[num] = {1.0, 1.0}; // Refraction index for propagation (for photons to propagate in Air)
    G4double rindexMetal[num] = {1.0, 1.0};
    G4double rindexLAr[num] = {1.38, 1.38}; //not considering dispersion so assume rindex constant
    G4double reflectivity[num] = {0.94, 0.94};
    // G4double efficiency[num] = {0.8, 0.1};

    // G4MaterialPropertiesTable *mptAcrylic = new G4MaterialPropertiesTable();
    // mptAcrylic->AddProperty("RINDEX", energy, rindexAcrylic, num);
    // Acrylic->SetMaterialPropertiesTable(mptAcrylic);

    G4MaterialPropertiesTable *mptMetal = new G4MaterialPropertiesTable();
    mptMetal->AddProperty("RINDEX", energy, rindexMetal, num);
    Metal->SetMaterialPropertiesTable(mptMetal);

    G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", energy, rindexAir, num);
    Air->SetMaterialPropertiesTable(mptAir);

    G4MaterialPropertiesTable *mptLAr = new G4MaterialPropertiesTable();
    mptLAr->AddProperty("RINDEX", energy, rindexLAr, num); //energy=mom in relativistic approximation
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
    G4VisAttributes* logicSolidVisAtt   = new G4VisAttributes(G4Colour(0.46, 0.53, 0.6, 0.3)); //grey + alpha
    G4VisAttributes* logicHoleVisAtt   = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 1)); //black + alpha
    G4VisAttributes* logicDetectorVisAtt = new G4VisAttributes(G4Colour(0.25,0.41, 0.88, 1)); //blue 
    G4VisAttributes* logicSCVisAtt = new G4VisAttributes(G4Colour(0.25,0.88, 0.41, 0.5)); //green
    G4VisAttributes* logicFiltroVisAtt = new G4VisAttributes(G4Colour(0.0,0.0, 0.0, 0.7)); 
    logicSolidVisAtt->SetForceSolid(true);
    logicHoleVisAtt->SetForceWireframe(true);
    logicDetectorVisAtt->SetForceSolid(true);
    logicSCVisAtt->SetForceSolid(true);
    logicFiltroVisAtt->SetForceSolid(true);

    // >> Matriz de rotacion para lo relativo a la SC
    G4RotationMatrix* pRot = new G4RotationMatrix();
    pRot->rotateX(90.*deg);


    // ---- Cajita (hueca) ----- //
    // >> Cajita Exterior (+grande, plastico)
    solidCajitaOut = new G4Box("solidCajitaOut", 20*mm, 20*mm, 20*mm); 
    logicCajitaOut = new G4LogicalVolume(solidCajitaOut, Plastic, "logicCajitaOut");
    physCajitaOut = new G4PVPlacement(0, G4ThreeVector(0*mm, 0*mm, 0*mm), logicCajitaOut, "physCajitaOut", logicWorld, false, 1, true); 
    logicCajitaOut->SetVisAttributes(logicSolidVisAtt); //solid+grey
    // // >> Cajita Interior (+pequeña, aire)
    solidCajitaIn = new G4Box("solidCajitaIn", 18*mm, 18*mm, 18*mm); 
    logicCajitaIn = new G4LogicalVolume(solidCajitaIn, LAr, "logicCajitaIn");
    physCajitaIn = new G4PVPlacement(0, G4ThreeVector(0*mm, 0*mm, 0*mm), logicCajitaIn, "physCajitaIn", logicWorld, false, 1, true); 
    logicCajitaIn->SetVisAttributes(logicHoleVisAtt);


    // ---- Rebabas (plastic) ----- //
    // // >> Cylinders:
    G4Tubs *solidCylinderOut = new G4Tubs("solidCylinderOut", 15*mm, 16*mm, 5*mm, 0*deg, 360*deg); // rebaba+
    G4LogicalVolume *logicCylinderOut = new G4LogicalVolume(solidCylinderOut, Plastic, "logicCylinderOut");
    G4VPhysicalVolume *physCylinderOut = new G4PVPlacement(0, G4ThreeVector(0*mm, 0.*mm, 25.*mm), logicCylinderOut, "physCylinderOut", logicWorld, false, 0, 1);
    logicCylinderOut->SetVisAttributes(logicSolidVisAtt);

    G4Tubs *solidFalphaOut = new G4Tubs("solidFalphaOut", 12.5*mm, 13.5*mm, 1*mm, 0*deg, 360*deg); //rebaba
    G4LogicalVolume *logicFalphaOut = new G4LogicalVolume(solidFalphaOut, Plastic, "logicFalphaOut");
    // //G4LogicalVolume *logicSCOut = new G4LogicalVolume(solidCylinderOut, Plastic, "logicSCOut"); //no tiene rebaba
    G4VPhysicalVolume *physFalphaOut = new G4PVPlacement(0, G4ThreeVector(0*mm, 0.*mm, -21.*mm), logicFalphaOut, "physFalphaOut", logicWorld, false, 0, 1);
    // //G4VPhysicalVolume *physSCOut = new G4PVPlacement(pRot, G4ThreeVector(0*mm, 21.*mm, 0.*mm), logicSCOut, "physSCOut", logicWorld, false, 0, 1);
    logicFalphaOut->SetVisAttributes(logicSolidVisAtt); 
    // //logicSCOut->SetVisAttributes(logicSolidVisAtt);

    // // >> Cajitas:
    G4Box *solidSiPMOut = new G4Box("solidSiPMOut", 1*mm, 4*mm, 4*mm); 
    G4LogicalVolume *logicSiPM1Out = new G4LogicalVolume(solidSiPMOut, Plastic, "logicSiPMOut");
    G4LogicalVolume *logicSiPM2Out = new G4LogicalVolume(solidSiPMOut, Plastic, "logicSiPMOut");
    G4VPhysicalVolume *physSiPM1Out = new G4PVPlacement(0, G4ThreeVector(21*mm, 0.*mm, 0.*mm), logicSiPM1Out, "physSiPM1Out", logicWorld, false, 0, 1);
    G4VPhysicalVolume *physSiPM2Out = new G4PVPlacement(0, G4ThreeVector(-21*mm, 0.*mm, 0.*mm), logicSiPM2Out, "physSiPM2Out", logicWorld, false, 0, 1);
    logicSiPM1Out->SetVisAttributes(logicSolidVisAtt);
    logicSiPM2Out->SetVisAttributes(logicSolidVisAtt);


    // ---- Holes (air) ----- //
    // >> PMT, SC, Falpha (Tubes)
    G4Tubs *solidHole = new G4Tubs("solidHole", 0.*mm, 11.5*mm, 1.1*mm, 0*deg, 360*deg);
    G4LogicalVolume *logicPMTHole = new G4LogicalVolume(solidHole, LAr, "logicPMTHole");
    G4LogicalVolume *logicSCHole = new G4LogicalVolume(solidHole, LAr, "logicSCHole");
    G4LogicalVolume *logicFalphaHole = new G4LogicalVolume(solidHole, LAr, "logicFalphaHole");
    G4VPhysicalVolume *physPMTHole = new G4PVPlacement(0, G4ThreeVector(0., 0., 19*mm), logicPMTHole, "physPMTHole", logicWorld, false, 0, 1);
    G4VPhysicalVolume *physSCHole = new G4PVPlacement(pRot, G4ThreeVector(0., 19.1*mm, 0.), logicSCHole, "physSCHole", logicWorld, false, 0, 1);
    G4VPhysicalVolume *physFalphaHole = new G4PVPlacement(0, G4ThreeVector(0., 0., -19.1*mm), logicFalphaHole, "physSCHole", logicWorld, false, 0, 1);
    logicPMTHole->SetVisAttributes(logicHoleVisAtt);
    logicSCHole->SetVisAttributes(logicHoleVisAtt);
    logicFalphaHole->SetVisAttributes(logicHoleVisAtt);

    // // >> SiPMs (Boxes)
    G4Box *solidSiPMHole = new G4Box("solidSiPMHole", 2.2*mm, 3*mm, 3*mm); 
    G4LogicalVolume *logicSiPM1Hole = new G4LogicalVolume(solidSiPMHole, LAr, "logicSiPMHole");
    G4LogicalVolume *logicSiPM2Hole = new G4LogicalVolume(solidSiPMHole, LAr, "logicSiPMHole");
    G4VPhysicalVolume *physSiPM1Hole = new G4PVPlacement(0, G4ThreeVector(20.1*mm, 0.*mm, 0.*mm), logicSiPM1Hole, "physSiPM1Hole", logicWorld, false, 0, 1); //19.5 refelja
    G4VPhysicalVolume *physSiPM2Hole = new G4PVPlacement(0, G4ThreeVector(-20.1*mm, 0.*mm, 0.*mm), logicSiPM2Hole, "physSiPM2Hole", logicWorld, false, 0, 1);
    logicSiPM1Hole->SetVisAttributes(logicHoleVisAtt);
    logicSiPM2Hole->SetVisAttributes(logicHoleVisAtt);

    // G4Box *solidSurfaceHole = new G4Box("solidSurfaceHole", 0.25*mm, 3*mm, 3*mm); 
    // G4LogicalVolume *logicSurfaceHole = new G4LogicalVolume(solidSurfaceHole, LAr, "logicSurfaceHole");
    // G4VPhysicalVolume *physSurfaceHole = new G4PVPlacement(0, G4ThreeVector(17.25*mm, 0.*mm, 0.*mm), logicSurfaceHole, "physSurfaceHole", logicWorld, false, 0, 1);


    // // >> ¿? Create hollow box: ¿?; prueba de sustraer geometrias, de momento fallida
    // G4SubtractionSolid *solidBoxtry = new G4SubtractionSolid("solidBoxtry", solidCajitaOut, solidHoleOut, 0, G4ThreeVector(0*mm, 0*mm, 20*mm));
    // G4SubtractionSolid *solidBoxtry = new G4SubtractionSolid("solidBoxtry", solidCajitaOut, solidCajitaIn);
    // logicBoxOut = new G4LogicalVolume(solidBoxtry, Air, "logicBoxOut");
    // physBoxOut = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBoxOut, "physBoxOut", logicWorld, false, 0, 1);

    // G4UnionSolid *solidBoxFAlphaOut = new G4UnionSolid("solidBoxFAlphaOut", solidCajitaOut, solidFalphaOut, 0, G4ThreeVector(0*mm, 0*mm, -25*mm)); //G4SubtractionSolid("Absorber-Hole",theAbsorber, theHole, theRotation, theTranslation);
    // G4UnionSolid *solidBoxCylinder1Out = new G4UnionSolid("solidBoxCylinder1Out", solidBoxFAlphaOut, solidCylinderOut, 0, G4ThreeVector(0*mm, 0*mm, 21*mm)); 
    // G4UnionSolid *solidBoxCylinder2Out = new G4UnionSolid("solidBoxCylinder2Out", solidBoxCylinder1Out, solidCylinderOut, pRot, G4ThreeVector(0*mm, 21*mm, 0*mm)); 
    // G4SubtractionSolid *solidBoxPMTOut = new G4SubtractionSolid("solidBoxPMTOut", solidBoxCylinder2Out, solidHoleOut, 0, G4ThreeVector(0*mm, 0*mm, 20*mm));
    // G4SubtractionSolid *solidBoxSCOut = new G4SubtractionSolid("solidBoxSCOut", solidBoxPMTOut, solidHoleOut, pRot, G4ThreeVector(0*mm, 20*mm, 0*mm)); 
    // G4SubtractionSolid *solidBoxSiPM1Out = new G4SubtractionSolid("solidBoxSiPM1Out", solidBoxSCOut, solidBoxOut, 0, G4ThreeVector(20*mm, 0*mm, 0*mm)); 
    // G4SubtractionSolid *solidBoxSiPM2Out = new G4SubtractionSolid("solidBoxSiPM2Out", solidBoxSiPM1Out, solidBoxOut, 0, G4ThreeVector(-20*mm, 0*mm, 0*mm)); 
    // logicBoxOut = new G4LogicalVolume(solidBoxSiPM2Out, Plastic, "logicBoxOut");
    // physBoxOut = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBoxOut, "physBoxOut", logicWorld, false, 0, 1);


    // --- SiPM x2 --- //
    // >> Matriz de rotacion para los SiPMs
    G4RotationMatrix* s1Rot = new G4RotationMatrix();
    G4RotationMatrix* s2Rot = new G4RotationMatrix();
    s1Rot->rotateY(5.*deg);
    s2Rot->rotateY(-5.*deg);

    // // >> Simple Box:
    solidSiPM = new G4Box("solidSiPM", 0.5*mm, 3*mm, 3*mm); 
    logicSiPM1 = new G4LogicalVolume(solidSiPM, Metal, "logicSiPM1");
    logicSiPM2 = new G4LogicalVolume(solidSiPM, Metal, "logicSiPM2");
    // physSiPM1 = new G4PVPlacement(s1Rot, G4ThreeVector(18*mm, 0*mm, 0*mm), logicSiPM1, "physSiPM1", logicWorld, false, 1, true); 
    physSiPM1 = new G4PVPlacement(0, G4ThreeVector(18.0*mm, 0*mm, 0*mm), logicSiPM1, "physSiPM1", logicWorld, false, 1, true); //18.5 //18 antes
    // physSiPM2 = new G4PVPlacement(s2Rot, G4ThreeVector(-18*mm, 0*mm, 0*mm), logicSiPM2, "physSiPM2", logicWorld, false, 1, true);
    physSiPM2 = new G4PVPlacement(0, G4ThreeVector(-18.0*mm, 0*mm, 0*mm), logicSiPM2, "physSiPM2", logicWorld, false, 1, true); //-17.5
    logicSiPM1->SetVisAttributes(logicDetectorVisAtt);
    logicSiPM2->SetVisAttributes(logicDetectorVisAtt);

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


    // >> Pixelated: Box full of small boxes
    // solidSiPM = new G4Box("solidSiPM", 0.1*mm, 3*mm, 3*mm); 
    // logicSiPM1 = new G4LogicalVolume(solidSiPM, Air, "logicSiPM1");
    // logicSiPM2 = new G4LogicalVolume(solidSiPM, Air, "logicSiPM2");
    // for(G4int i = 0; i<150; i++)
    // {
    //     for(G4int j = 0; j<150; j++)
    //     {
    //         physSiPM1 = new G4PVPlacement(0, G4ThreeVector(1.99*cm, -6*mm + (i+75) * mm/25, -6*mm + (j+75) * mm/25), logicSiPM1, "physSiPM1", logicWorld, false, i+j*150, false); //checked no overlaps, false save time
    //         physSiPM2 = new G4PVPlacement(0, G4ThreeVector(-1.99*cm, -6*mm + (i+75) * mm/25, -6*mm + (j+75) * mm/25), logicSiPM2, "physSiPM2", logicWorld, false, j+i*150, false);
    //     }
    // }


    // --- PMT --- //
   // >> Simple Cylynder:
   // PMT (name, inner radius, outer radius, Z half length, starting Phi, segment angle)
    G4Tubs *solidPMT = new G4Tubs("solidPMT", 0*mm, 11.5*mm, 46*mm, 0*deg, 360*deg); //mide de largo 92
    logicPMT = new G4LogicalVolume(solidPMT, Metal, "logicPMT",0,0,0);
    physPMT = new G4PVPlacement(0, G4ThreeVector(0*mm, 0*mm, 66*mm), logicPMT, "physPMT", logicWorld, false, 1, true); //46+20
    logicPMT->SetVisAttributes(logicDetectorVisAtt);

    // >> Cylynder full of small cylinders; Hay overlaps pero se ve bien (creo que no es necesario para el PMT, revisar)
    // G4Tubs *solidPMT = new G4Tubs("solidPMT", 0*mm, 0.1*mm, 0.1*mm, 0*deg, 360*deg);
    // logicPMT = new G4LogicalVolume(solidPMT, Air, "logicPMT",0,0,0);
    /*    
        for(G4int i = 0; i<35; i++) //r
    {
        for(G4int j = 0; j<180; j++) //theta
        {
            //polar coordinates: x=rcos(theta), y=rsin(theta)
            physPMT = new G4PVPlacement(0, G4ThreeVector((11.5*i/35)*cos(j/0.5*deg)*mm, (11.5*i/35)*sin(j/0.5*deg)*mm, 1.99*cm), logicPMT, "physPMT", logicWorld, false, 11.5*(i+1)/(18.3*0.63), false); 
        }
    }
    //*/


    // --- SC --- //

    // >> Cubierta del filtro << : //
    G4Box *solidFiltro = new G4Box("solidFiltro", 19.75*mm, 0.2*mm, 15.75*mm); 
    G4LogicalVolume *logicFiltro = new G4LogicalVolume(solidFiltro, Plastic, "logicFiltro");
    G4VPhysicalVolume *physFiltro = new G4PVPlacement(0, G4ThreeVector(0*mm, 20.80*mm, 0*mm), logicFiltro, "physFiltro", logicWorld, false, 0, 1);
    logicFiltro->SetVisAttributes(logicFiltroVisAtt);

    G4Tubs *solidHoleFiltro = new G4Tubs("solidHoleFiltro", 0*mm, 11.5*mm, 0.3*mm, 0*deg, 360*deg);
    G4LogicalVolume *logicHoleFiltro = new G4LogicalVolume(solidHoleFiltro, LAr, "logicHoleFiltro",0,0,0);
    G4VPhysicalVolume *physHoleFiltro = new G4PVPlacement(pRot, G4ThreeVector(0*cm, 20.75*mm, 0*cm), logicHoleFiltro, "physHoleFiltro", logicWorld, false, 1, true); 
    logicHoleFiltro->SetVisAttributes(logicHoleVisAtt);


    // >> Simple Cylynder:
    // // SC (name, inner radius, outer radius, Z half length, starting Phi, segment angle)
    // G4Tubs *solidSC = new G4Tubs("solidSC", 0*mm, 11.5*mm, 2*mm, 0*deg, 360*deg);
    // logicSC = new G4LogicalVolume(solidSC, LAr, "logicSC",0,0,0);
    // physSC = new G4PVPlacement(pRot, G4ThreeVector(0*cm, 23*mm, 0*cm), logicSC, "physSC", logicWorld, false, 1, true); //21mm + 2mm = altura/2 (23mm ideal case)
    // logicSC->SetVisAttributes(logicSCVisAtt);

    // >> Boxed shaped
    // G4Box *solidSC = new G4Box("solidSC", 112*mm, 10*mm, 476*mm); 
    // G4Box *solidSC = new G4Box("solidSC", 50*mm, 2*mm, 70*mm); 
    // G4Box *solidSC = new G4Box("solidSC", 59*mm, 2*mm, 261.5*mm); //--<<-- 1XA 118mm x 523mm
    G4Box *solidSC = new G4Box("solidSC", 19.75*mm, 2*mm, 15.75*mm); //--<<-- solo 1 de los 6 filtros --<-- (4mm de ancho en realidad!)
    // G4Box *solidSC = new G4Box("solidSC", 20*mm, 2*mm, 40*mm); 
    logicSC = new G4LogicalVolume(solidSC, LAr, "logicSC");//--<<--
    physSC = new G4PVPlacement(0, G4ThreeVector(0*mm, 23*mm, 0*mm), logicSC, "physSC", logicWorld, false, 1, true); //d=21mm + 2mm (altura ejey SC)
    logicSC->SetVisAttributes(logicSCVisAtt);









    // >> Carla small SC:
    // G4Box *solidSC = new G4Box("solidSC", 37.5*mm, 100*mm, 2*mm); //200x75 mm a 5.5 cm de la fuente (172 medidas)
    // // G4Box *solidSC = new G4Box("solidSC", 46.5*mm, 240*mm, 2*mm); //480x93 mm a 5.5 cm de la fuente (172 medidas)
    // logicSC = new G4LogicalVolume(solidSC, LAr, "logicSC");
    // physSC = new G4PVPlacement(0, G4ThreeVector(0*mm, 0*mm, 0*mm), logicSC, "physSC", logicWorld, false, 1, true); 
    // logicSC->SetVisAttributes(logicSCVisAtt);


    // >> Carla small SC (insensitive bar in the middle)
    //  G4Box *solidFrame = new G4Box("solidFrame", 40*mm, 105*mm, 1*mm); 
    // G4LogicalVolume *logicFrame = new G4LogicalVolume(solidFrame, Plastic, "logicFrame");
    // G4VPhysicalVolume *physFrame = new G4PVPlacement(0, G4ThreeVector(0*mm, 0.*mm, 1*mm), logicFrame, "physFrame", logicWorld, false, 0, 1);
    // logicFrame->SetVisAttributes(logicSolidVisAtt);

    // >> SC Frame (small)
    // G4Box *solidSC = new G4Box("solidSC", 37.5*mm, 49*mm, 2*mm); //200x75 mm a 5.5 cm de la fuente (172 medidas)
    // logicSC1 = new G4LogicalVolume(solidSC, LAr, "logicSC1");
    // logicSC2 = new G4LogicalVolume(solidSC, LAr, "logicSC2");
    // physSC1 = new G4PVPlacement(0, G4ThreeVector(0*mm, 51*mm, 0*mm), logicSC1, "physSC1", logicWorld, false, 1, true); 
    // physSC2 = new G4PVPlacement(0, G4ThreeVector(0*mm, -51*mm, 0*mm), logicSC2, "physSC2", logicWorld, false, 1, true); 
    // logicSC1->SetVisAttributes(logicSCVisAtt);
    // logicSC2->SetVisAttributes(logicSCVisAtt);


    // >> Prueba con un detector esferico para ver si detecta todos los fotones que emitimos (sí)
    // G4Sphere *solidprueba = new G4Sphere("solidprueba",95*mm, 100*mm, 0*deg, 360*deg, 0*deg, 360*deg); //11.5
    // logicprueba = new G4LogicalVolume(solidprueba, LAr, "logicprueba",0,0,0);
    // G4VPhysicalVolume *physprueba = new G4PVPlacement(0, G4ThreeVector(0*cm, 0*mm, 0*cm), logicprueba, "physprueba", logicWorld, false, 1, true);
    // logicprueba->SetVisAttributes(logicDetectorVisAtt);

    // >> Prueba con un detector rectangular para ver los fotones que perdemos
    // G4Box *solidprueba = new G4Box("solidprueba", 100*mm, 2*mm, 100*mm);
    // logicprueba = new G4LogicalVolume(solidprueba, LAr, "logicprueba");//--<<--
    // G4VPhysicalVolume *physprueba = new G4PVPlacement(0, G4ThreeVector(0*mm, 35*mm, 0*mm), logicprueba, "physprueba", logicWorld, false, 1, true); //d=21mm + 2mm altura ejey SC
    // logicprueba->SetVisAttributes(logicSCVisAtt);
    //23 (1); 29 (6); 35 (12)

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    DefineMaterials();

    // We also need al WORLD volume to place the detector there.
    // Every volume contains three parts: 
    // Solid -> volume; Logical -> material; 
    // Physical -> places the volume in G4 space together with rotation, translation ...
    solidWorld = new G4Box("solidWorld", 200*mm, 200*mm, 200*mm); //(name, half of the length)
    logicWorld = new G4LogicalVolume(solidWorld, LAr, "logicWorld"); //(solid, material, name)
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); //(rotation, center coordinates, mothervolume, name, ..., true=checks overlaps ALWAYS)
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
    MySensitiveDetector *sensSiPM1 = new MySensitiveDetector("SensitiveSiPM1");
    MySensitiveDetector *sensSiPM2 = new MySensitiveDetector("SensitiveSiPM2");
    MySensitiveDetector *sensPMT = new MySensitiveDetector("SensitivePMT");
    MySensitiveDetector *sensSC = new MySensitiveDetector("SensitiveSC");
    

    logicSiPM1->SetSensitiveDetector(sensSiPM1);
    logicSiPM2->SetSensitiveDetector(sensSiPM2);
    logicPMT->SetSensitiveDetector(sensPMT);
    logicSC->SetSensitiveDetector(sensSC);

    // >> Dos ventanas carla separadas por centro insensible
    // MySensitiveDetector *sensSC1 = new MySensitiveDetector("SensitiveSC1");
    // MySensitiveDetector *sensSC2 = new MySensitiveDetector("SensitiveSC2");
    // logicSC1->SetSensitiveDetector(sensSC1);
    // logicSC2->SetSensitiveDetector(sensSC2);

    // >> Prueba con un detector esferico para ver si detecta todos los fotones que emitimos (sí)
    // MySensitiveDetector *sensprueba = new MySensitiveDetector("Sensitiveprueba");
    // logicprueba->SetSensitiveDetector(sensprueba);


}