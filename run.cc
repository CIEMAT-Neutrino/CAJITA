#include "run.hh"

MyRunAction::MyRunAction(G4String output_Name="None")
{
    foutput_Name=output_Name;
    
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleIColumn("Event");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->CreateNtupleDColumn("T");
    man->CreateNtupleDColumn("Wlen");
    man->CreateNtupleDColumn("Phi");
    man->CreateNtupleDColumn("Theta");
    man->CreateNtupleDColumn("Sensor");
    man->FinishNtuple(0);

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("Event");
    man->CreateNtupleDColumn("X"); //column of doubles
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->CreateNtupleDColumn("Sensor");
    man->CreateNtupleDColumn("AccumHits");  //0 arapuca, 1/2 SiPM
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