#include "run.hh"

MyRunAction::MyRunAction(G4String output_Name="None")
{
    foutput_Name=output_Name;
    
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
    man->CreateNtupleDColumn("fsensor");  //0 arapuca, 1/2 SiPM
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
    
    std::cout<<"-------- WWWW ------- Message-START -------- WWWW -------"<<std::endl;
    std::cout<<"Output file path: "+foutput_Name<<std::endl;
    std::cout<<"-------- WWWW ------- Message-END   -------- WWWW -------"<<std::endl;
    man->OpenFile(foutput_Name); 

    // G4int runID = run->GetRunID();
    // std::stringstream strRunID;
    // strRunID << runID;
    // man->OpenFile("output.root");
    // man->OpenFile("data/output"+strRunID.str()+".root"); //trying not to overwrite all the output files
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}