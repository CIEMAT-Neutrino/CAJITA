#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "g4root.hh"

class MyRunAction : public G4UserRunAction
{
public:
    // MyRunAction();
    MyRunAction(G4String output_Name);
    ~MyRunAction();
    
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    
    G4String foutput_Name="None";
};

#endif