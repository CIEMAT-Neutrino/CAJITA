#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fT");
    man->CreateNtupleDColumn("fWlen");
    man->CreateNtupleDColumn("fPhi");
    man->CreateNtupleDColumn("fTheta");
    man->FinishNtuple(0);

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX"); //column of doubles
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(1); 
    
    // man->CreateNtuple("Scoring", "Scoring");
    // man->CreateNtupleDColumn("fEdep");
    // man->FinishNtuple(2);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // man->OpenFile("output.root");

    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("data/output"+strRunID.str()+".root"); //trying not to overwrite all the output files
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}