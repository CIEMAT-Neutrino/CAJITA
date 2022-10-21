#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    // Adding efficiency to our detectors:
    quEff = new G4PhysicsOrderedFreeVector();

    std::ifstream datafile;
    datafile.open("PMT_qeff.dat"); //file with qefficiency given in the datasheet

    while(1)
    {
        G4double wlen, queff;

        datafile >> wlen >> queff;

        if(datafile.eof())
            break;

        // G4cout << wlen << " " << queff << std::endl; //Checking everyhting OK

        quEff->InsertValues(wlen, queff/100.); //Valores en porcentaje
    }

    datafile.close();

    quEff->SetSpline(false); //hacemos interpolacion lineal entre valores porque da menos problemas en 1a aproximacion
}

MySensitiveDetector::~MySensitiveDetector()
{}


G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // Information about the track that enters our sensitive volume
    G4Track *track = aStep->GetTrack();

    // track->SetTrackStatus(fStopAndKill); //As soon as enters the detector the track is killed and no propagated, always detected in the upper surface of the detector

    // Points where the photon enters and leaves the detector
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    // Get the position and the momentum:
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double time = preStepPoint->GetGlobalTime();
    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;

    // G4cout << "Photon position: " << posPhoton << G4endl; //In reality we dont have access to this position

    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable(); //detector position where the photons hit

    G4int copyNo = touchable->GetCopyNumber(); //the detector i+j*100 reached
    // G4cout << "Copy number: " << copyNo << G4endl;

    G4VPhysicalVolume *physVol = touchable->GetVolume(); 
    G4ThreeVector posDetector = physVol->GetTranslation(); //detector position (where the photon hits)

    #ifndef G4MULTITHREADED // uncomment to print detector position and photon wavelenght
        // G4cout << "Detector position: " << posDetector << G4endl; //funciona
        // G4cout << "Photon wavelength: " << wlen << G4endl; //funciona
    #endif
    
    // Current event ID
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Fill the Ntuple defined in run.cc and run.hh with the hits registered (photon info)
    G4AnalysisManager *man = G4AnalysisManager::Instance();
   
    man->FillNtupleIColumn(0, 0, evt); //event number vs entries (statistical fluctuation same ph hit)
    man->FillNtupleDColumn(0, 1, posPhoton[0]);
    man->FillNtupleDColumn(0, 2, posPhoton[1]);
    man->FillNtupleDColumn(0, 3, posPhoton[2]);
    man->FillNtupleDColumn(0, 4, time);
    man->FillNtupleDColumn(0, 5, wlen);
    man->FillNtupleDColumn(0, 6, momPhoton.getPhi());
    man->FillNtupleDColumn(0, 7, momPhoton.getTheta());
    man->AddNtupleRow(0);

    // >> Without Quantum Efficiency:
    man->FillNtupleIColumn(1, 0, evt);
    man->FillNtupleDColumn(1, 1, posDetector[0]);
    man->FillNtupleDColumn(1, 2, posDetector[1]);
    man->FillNtupleDColumn(1, 3, posDetector[2]);
    man->AddNtupleRow(1);

    // if(G4UniformRand() < quEff->Value(wlen))
    // {
    //     man->FillNtupleIColumn(1, 0, evt);
    //     man->FillNtupleDColumn(1, 1, posDetector[0]);
    //     man->FillNtupleDColumn(1, 2, posDetector[1]);
    //     man->FillNtupleDColumn(1, 3, posDetector[2]);
    //     man->AddNtupleRow(1);
    // }

    return true;
}