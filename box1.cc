// Template for any Geant4 applications
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

// C++
#include <iostream>
// #include <fstream>

// Other
#include "json_class.hh"

int main(int argc, char **argv)
{
    G4UIExecutive *ui = 0;
    G4RunManager *runManager = new G4RunManager();


    G4String second_arg = "none";
    G4String third_arg  = "none";

    if (argc == 1) // number of commands arguments is 1 (i.e no arguments in the command line)
    {
        ui = new G4UIExecutive(argc, argv);
    }
    else if(argc == 2)
    {
        second_arg = argv[1];
    }
    else if(argc == 3)
    {
        second_arg = argv[1];
        third_arg  = argv[2];
    }

    G4String out_file="None";
    if(check_is_file_type(second_arg,".root")||check_is_file_type(third_arg,".root"))
    {
        out_file=check_is_file_type(second_arg,".root") ? second_arg:third_arg;
    }else
    {//sanitycheck
        throw std::runtime_error("WARNING MESSAGE: \n No output rootfile provided from terminal, please, give one!");
    }

    // Use the geometry we define in construction.cc
    runManager->SetUserInitialization(new MyDetectorConstruction(second_arg));
    // Use the physics we define in physics.cc
    runManager->SetUserInitialization(new MyPhysicsList());
    // ActionInitialization
    runManager->SetUserInitialization(new MyActionInitialization(out_file));

    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (ui)
    {

        UImanager->ApplyCommand("/vis/scene/add/axes");
        // // The comented lines are now stored in vis.mac
        UImanager->ApplyCommand("/control/execute configs/vis.mac");
        UImanager->ApplyCommand("/vis/viewer/set/background grey");

        ui->SessionStart();
    }
    else if (check_is_file_type(second_arg)) // .json file initialization
    {

        std::cout << "---------------Json conf file provided-------------:" << second_arg << std::endl;

        // Check fjson file exists:
        check_json_file(second_arg);

        // Read Json
        const Json_file fjson = Json_file(second_arg);

        // With display:
        if (fjson.json_map["display"].get<bool>())
        {
            ui = new G4UIExecutive(argc, argv);
            for (auto line : fjson.json_map["G4_vis_settings"])
            {
                G4String g4_line = line.get<std::string>();
                UImanager->ApplyCommand(g4_line);
            }
            for (auto line : fjson.json_map["G4_beam_settings"])
            {
                G4String g4_line = line.get<std::string>();
                // std::cout << g4_line << std::endl;
                UImanager->ApplyCommand(g4_line);
            }
            ui->SessionStart();
        }
        else
        { // only beam
            for (auto line : fjson.json_map["G4_beam_settings"])
            {
                G4String g4_line = line.get<std::string>();
                // std::cout << g4_line << std::endl;
                UImanager->ApplyCommand(g4_line);
            }
        }
    }
    else // .mac file initialization
    {
        ui = new G4UIExecutive(argc, argv);
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);

        UImanager->ApplyCommand("/vis/open OGL");
        // UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");;
        UImanager->ApplyCommand("/vis/scene/add/axes");
        UImanager->ApplyCommand("/vis/drawVolume");
        // Draw the trajectory and autorefresh :)
        UImanager->ApplyCommand("/vis/viewer/set/autorefresh/true");
        UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");

        UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate 1000");
        UImanager->ApplyCommand("/vis/viewer/set/background grey");

        ui->SessionStart();
    }

    return 0;
}