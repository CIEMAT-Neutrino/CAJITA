# BasicSimulation_Geant4


## What is this?
This is a basic example for performing simulations in Geant4. The world volume is filled with LAr and we place different detectors
that can be included with given positions in a .json or manually. The output is a .root file which contains (_Photon_) NTuple with the position (X,Y,Z) of the photon hit in your detector and the incidence angle (theta, phi).

We have different headers and sources that are included in the main file (box1.cc).
Chronologically we have created the files as follows:

  1st: Detector construction (construction)


  2nd: Physics included in the analysis (physics)


  3rd: Particle gun in action (action)


  4rd: Particle generator (generator)


  5th: Inserting sensitive detector (detector) and we add in construcction.cc the detectors' volumes


  6th: Store the information in a .root file (with Ntuples). Defined in run.cc and run.hh.
      We can see the stored information:
          >> root output.root
          >> new TBrowser()
          >> Hits->Draw("fX:fY", "", "colz") //to see the correlation


  7th: Automatizating the code 

    1. vis.mac for visualization commands -> when creating vis.mac it is not in /build and it is not found, we move all the macro file 
      to the right directory without including absolute paths -> look in CMakeLists.txt)
      
    2. run.mac -> change the momentum for each particle creation. Changes in box1.cc to accept command line inputs as files (Execute as: >>./box1 *.mac).
    
    3. *.dat -> store quantum/photon detection efficiencies for the different detectors.


  8th: Analysing the Ntuple.root generated with the photons' hits. We create a notebook (Analysis.ipynb) for this purpose and to plot the distributions.

We can send alpha particles (blue), electrons(red) and photons(green) are produced. 
We would need to include the energy deposition calculation that can be also stored in the NTuple.
We want to show Scintillation LAr light (see https://apc.u-paris.fr/~franco/g4doxy/html/classG4Scintillation.html)

## Running

(Keep an eye on the operational system/arquitecture you are using)
  1. We are running without local installation of genat4 neither gcc so we make a source to them:
  ```console
  source /cvmfs/sft.cern.ch/lcg/views/LCG_100/x86_64-centos7-gcc8-opt/setup.sh
  source /cvmfs/sft.cern.ch/lcg/views/LCG_100/x86_64-centos7-gcc8-opt/bin/geant4.sh
  ```

  2. New folder for building (delete if cloning the repository)
  ```console
  mkdir build
  cd build 
  ```

  3. Prepare compilation (each time there are new files also)
  ```console
  cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt ../
  ```

  4. Compile
  ```console
  make -j 2
  ```

  5. Execute (if everything is going well)
  ```console
  ./box1
  ```

  6. To introduce the particle momentum stored in a macro file:
  ```console
  ./box1 run.mac
  ```


## Visualizing
  ```console
  >> root NTuple1.root

  >> Photons->Show(10)
  ======> EVENT:10
  fEvent          = 125
  fX              = 6.68858
  fY              = 19.8176
  fZ              = -9.35483
  fT              = 0.106023
  fWlen           = 430.053
  >> Photons->Print()
  ******************************************************************************
  *Tree    :Photons   : Photons                                                *
  *Entries :      680 : Total =           50496 bytes  File  Size =      26396 *
  *        :          : Tree compression factor =   1.00                       *
  ******************************************************************************
    ...
  *............................................................................*

  ### GetEntries

  >> Photons->Draw("fY:fX>>PMT","-15<fX && fX<15 && -15<fY && fY<15", "colZ")

  >> Photons->Draw("fZ:fX>>SC","-15<fZ && fZ<15 && -15<fX && fX<15", "colZ")

  >> Photons->Draw("fY:fZ>>SiPMs","", "colZ")
  >> Photons->Draw("fY:fX>>SiPM1","fX>15 && fY<15", "colZ")
  >> Photons->Draw("fY:fX>>SiPM2","fX<-15 && fY<15", "colZ")

  >> #TH1F *PMT= (TH1F*)gDirectory->Get("PMT")

  >> #PMT->GetEntries()
```