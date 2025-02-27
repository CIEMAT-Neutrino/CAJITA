#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
// #include "gpsgenerator.hh"
#include "run.hh"
#include "event.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
public:
    MyActionInitialization(G4String out_file);
    ~MyActionInitialization();
    
    virtual void Build() const;
    G4String foutfile;
};

#endif