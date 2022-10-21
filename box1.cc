// Template for any Geant4 applications
# include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"


int main(int argc, char** argv)
{
    G4UIExecutive *ui = 0;

    G4RunManager *runManager = new G4RunManager();

    // Use the geometry we define in construction.cc
    runManager->SetUserInitialization(new MyDetectorConstruction());
    // Use the physics we define in physics.cc
    runManager->SetUserInitialization(new MyPhysicsList());
    //ActionInitialization
    runManager->SetUserInitialization(new MyActionInitialization());


    runManager->Initialize();
    
    if(argc == 1) //number of commands arguments is 1 (i.e no arguments in the command line)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();
    
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if(ui)
    {
        UImanager->ApplyCommand("/vis/open OGL");
        UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");;
        UImanager->ApplyCommand("/vis/scene/add/axes");
        UImanager->ApplyCommand("/vis/drawVolume");
        // // Draw the trajectory and autorefresh :)
        UImanager->ApplyCommand("/vis/viewer/set/autorefresh/true");
        UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
        // // p+ ->blue, e- -> red
        UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");

    //     // The comented lines are now stored in vis.mac     
        UImanager->ApplyCommand("/control/execute vis.mac");
        UImanager->ApplyCommand("/vis/viewer/set/background grey");
        ui->SessionStart();
    }

    // In the case we type a macro file after ./box1 (i.e ./box1 run.mac) it will look for it and take the input parameters 
   else
    {
        // ui = new G4UIExecutive(argc, argv);
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);

        // UImanager->ApplyCommand("/vis/open OGL");
        // // UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");;
        // UImanager->ApplyCommand("/vis/scene/add/axes");
        // UImanager->ApplyCommand("/vis/drawVolume");
        // // Draw the trajectory and autorefresh :)
        // UImanager->ApplyCommand("/vis/viewer/set/autorefresh/true");
        // UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");

        // UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate 1000");  
        // UImanager->ApplyCommand("/vis/viewer/set/background grey");

        // ui->SessionStart();
    }

    return 0;
}
