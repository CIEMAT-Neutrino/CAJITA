//We want to implement the particle gun --> two new files Generator

#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::Build() const
{
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    // MyGPSPrimaryGeneratorAction *generator = new MyGPSPrimaryGeneratorAction();
    SetUserAction(generator);
    
    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);

    MyEventAction *eventAction = new MyEventAction(runAction);
    SetUserAction(eventAction);
}