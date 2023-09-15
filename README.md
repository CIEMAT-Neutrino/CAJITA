# CAJITA SIMULATION GEANT4


## SUMMARY
This is a basic example for performing simulations in Geant4. The world volume is filled with LAr and we place different detectors
that can be included with given positions in a .json or manually. The output is a .root file which contains (_Photon_) NTuple with the position (X,Y,Z) of the photon hit in your detector and the incidence angle (theta, phi).

We have different headers and sources that are included in the main file (box1.cc).
Chronologically we have created the files as follows:

1.- Detector construction (construction)


2.- Physics included in the analysis (physics)


3.- Particle gun in action (action)


4.- Particle generator (generator)


5.- Inserting sensitive detector (detector) and we add in construcction.cc the detectors' volumes


6.- Store the information in a .root file (with Ntuples). Defined in ```run.cc``` and ```run.hh```. We can see the stored information:
```console
  >> root output.root
  >> new TBrowser()
  >> Hits->Draw("fX:fY", "", "colz") //to see the correlation
```

7.- Automatizating the code 
* ```vis.mac``` for visualization commands -> when creating vis.mac it is not in /build and it is not found, we move all the macro file 
  to the right directory without including absolute paths -> look in CMakeLists.txt
  
* `run.mac` -> change the momentum for each particle creation. Changes in box1.cc to accept command line inputs as files (Execute as: >>./box1 *.mac).

* `*.dat` -> store quantum/photon detection efficiencies for the different detectors.


  8th: Analysing the Ntuple.root generated with the photons' hits. We create a notebook (Analysis.ipynb) for this purpose and to plot the distributions.

We can send alpha particles (blue), electrons(red) and photons(green) are produced. 
We would need to include the energy deposition calculation that can be also stored in the NTuple.
We want to show Scintillation LAr light (see https://apc.u-paris.fr/~franco/g4doxy/html/classG4Scintillation.html)

-----------------------------------------------------------------------------------------------------------------------------------------------------

## RUN GEANT4 SIMULATION

### 1.- SETUP AND COMPILATION

Once you have cloned the repository, you can setup the enviroment with the following commands:
```console
source setup.sh
./run_build.sh
```

Before the compilation occurs you will need to choose a geometry to be used among the displayed options. Then you can run the code.

-----------------------------------------------------------------------------------------------------------------------------------------------------

### 2.- RUN YOUR CHOSEN GEOMETRY

Therer are three options of configuring the geometry that can be run as:

* option1: no json. Source information in the .mac. Compile to update changes in construction.
```console
./build/box1 configs/option1.mac data/output.root
```

* option2: fixed positions given in construcction.cc (sbnd_pds_mapping.json). Source information by .mac
```console
./build/box1 configs/option2.mac data/output.root
```

* option3: json including positions for sensors + (.mac) information
```console
./build/box1 configs/option3.json data/output.root
```

Note: each time you change the geometry you need to recompile the code. Probably the simulation output is big so if you need to run several simulation for optimize the setup it is better to allocate the output in a different folder (i.e. in ```/pc/choozdsk01/palomare/GEANT4``` or ```/pnfs/ciemat.es/neutrinos/```  where you can make your own folder ).

-----------------------------------------------------------------------------------------------------------------------------------------------------

### 3.- GEOMETRIES EXAMPLES
You can also find some of the geometries used in the IR02 setups:

* ```cajita_xa-hd.mac``` -> measurements from October 2021 to February 2022 of the X-ARAPUCA DUNE HD (to be run as option1)

* ```cajita_xa-sbnd.json``` -> measurements from Febreuary 2023 to April 2023 of the X-ARAPUCA SBND (to be run as option3). 
    There are two types of files for production (*_prod) and for visualization. 
    Moreover there were two measurements to be made with visible ligth (VIS) and with alpha source (VUV)
* ```megacell.json``` -> measurements from August 2023 to * of the MeggaCell DUNE-VD (to be run as option3).

-----------------------------------------------------------------------------------------------------------------------------------------------------
### 4.- ANALYSIS: JUPYTER NOTEBOOKS

In the analysis folder you can find some notebooks to analyse the output of the simulation.

* Analitical computations -> expected shapes of the distributions for the different detectors
* Simulation outputs -> PDE, #PE, angular distribution, etc.
-----------------------------------------------------------------------------------------------------------------------------------------------------

## Visualizing in ROOT
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

## LICENSE
[MIT](https://choosealicense.com/licenses/mit/)

## Authors (alphabetical order, please insert your name here if you contribute to this project)

* [**Alvárez-Garrote, Rodrigo**](https://github.com/LauPM)
* [**Pérez-Molina, Laura**](https://github.com/rodralva)
* [**Manthey Corchado, Sergio**](https://github.com/mantheys)
