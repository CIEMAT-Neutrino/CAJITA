#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{

    // ‘particle guns’ used for test-beam or fixed target simulations
    // fParticleGun = new G4ParticleGun(1); //# of particles per event = 1
    fParticleGun = new G4GeneralParticleSource(); // change particle gun by GPS everywhere

    // Before this was in MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
    // The idea now is that we can overwrite this "default" parameters with .mac

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();

    // G4String particleName="proton";
    // G4ParticleDefinition *particle = particleTable ->FindParticle("proton");
    // G4String particleName="opticalphoton";
    G4ParticleDefinition *particle = particleTable ->FindParticle("opticalphoton");
    // G4String particleName="alpha";
    // G4ParticleDefinition *particle = particleTable ->FindParticle("alpha");

    //more generic particle --> placeholder without any properties
    // G4ParticleDefinition *particle = particleTable ->FindParticle("geantino");


    // G4ThreeVector pos(0., 0., 0.); // center of mother volume
    G4ThreeVector pos(0*mm, 0*mm, -20*mm); // alpha source position -> center of our boxed detector face
    G4ThreeVector mom(0., 0., 1.); // positive z direction

    fParticleGun->SetParticlePosition(pos);
    // fParticleGun->SetParticleMomentumDirection(mom);
    // fParticleGun->SetParticleMomentum(100.*GeV);
    fParticleGun->SetParticleDefinition(particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleDefinition* particle = fParticleGun->GetParticleDefinition();

    if(particle == G4Geantino::Geantino())
    {
        // Simulate the decay of the Am241 (check if coherent with activity 50-65 Bq)
        G4int Z = 95;//
        G4int A = 241; //

        G4double charge = 1. * eplus;
        G4double energy = 0. * keV;

        G4ParticleDefinition *ion =G4IonTable::GetIonTable()->GetIon(Z,A,energy);
        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleCharge(charge);

    }

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
