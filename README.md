# CAJITA

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

6.- Store the information in a ```.root``` file (with Ntuples). Defined in ```run.cc``` and ```run.hh```. We can see the stored information:

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

8th: Analysing the Ntuple.root generated with the photons' hits. Have a look at the ```analysis``` folder.

We can send alpha particles (blue), electrons(red) and photons(green) are produced. 
We would need to include the energy deposition calculation that can be also stored in the NTuple.
We want to show Scintillation LAr light (see [G4Scintillation](https://apc.u-paris.fr/~franco/g4doxy/html/classG4Scintillation.html))

-----------------------------------------------------------------------------------------------------------------------------------------------------

## RUN GEANT4 SIMULATION

To be ran in ```gaeuidc1.ciemat.es``` (```CentOS7```, ```Geant4 v10-07-patch01```, ```Motif``` library)

For new simulations you need:
* New file in ```configs/your_geometry.json```  (check TEMPLATE.md)
* Two files in ```geometries/``` ```your_geometry.cc``` + ```your_geometry.hh```

You can try with an existing one first as follows:


### 🇬4️⃣ 1. SETUP AND COMPILATION

Once you have cloned the repository, you can setup the enviroment with the following commands:

```console
source scripts/setup.sh
./scripts/run_build.sh
```

Before the compilation occurs you will need to choose a geometry to be used among the displayed options. Then you can run the code.

![setup](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/83ecf83e-d368-4be5-b1f2-98375f88d920)
![compilation](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/89fbd956-3177-4630-9296-3bafe01e99c6)
![success](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/5eb0b15b-0b8e-4fc3-ac29-aef5298a0816)

⚠️ WARNINGS ⚠️

* You will need the library **Motif** for a successful compilation (ask ae_support to install it in you local pcae if you need it). It is already installed in ```gaeuidc1```.
* _run_build.sh_ script only runs from main folder (it will ask for your confirmation to continue with the process)
* You need to choose a geometry (_geometries_ and _configs_ folders 📂) for the compilation. Each change need to be recompiled !! (Do not include the extension of the file)

🆕 Once you have clearly chosen your geometry you can compile with ```./scripts/run_build.sh geometry``` to avoid the second question of the script.
![build](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/b5648858-3a74-4393-bf29-8cd26bd02b36)

-----------------------------------------------------------------------------------------------------------------------------------------------------

### 👾 2. RUN YOUR CHOSEN GEOMETRY

There are three options of configuring the geometry that can be run as:

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

![run_option3](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/47a9cddc-dc6e-43fb-b0b5-92d9071cc077)
![output_option3](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/0c6a1eab-9b91-479d-8438-5bf29c3d454d)

🔍 CHECK ```configs/TEMPLATE.md``` for more details. 

🗒️ Note: each time you change the geometry you need to recompile the code. Probably the simulation output is big so if you need to run several simulation for optimize the setup it is better to allocate the output in a different folder (i.e. in ```/pc/choozdsk01/DATA/CAJITA``` or ```/pnfs/ciemat.es/neutrinos/```  where you can make your own folder ).
Moreover if you need to define a geometry based on different simulations you can check production folder with examples on how to change some parameters with different ```*.json``` without compilation and run at the same time with the ```production/production.sh``` script.


🖥️ The code is prepared for paralelizing the simulation process. You can notice this when the following message appears in the building process.

```console
  Manually-specified variables were not used by the project:

    GEANT4_BUILD_MULTITHREADED
```

To choose the number of nodes go to the ```box1.cc``` file line ```runManager->SetNumberOfThreads(12); // 1 for no multithreading (default) you can change to the number of threads you want``` and change it as you need.


-----------------------------------------------------------------------------------------------------------------------------------------------------

### 🧪 3. GEOMETRIES EXAMPLES

You can also find some of the geometries used in the IR02 setups:

* ```cajita_xa-hd.mac``` -> measurements from October 2021 to February 2022 of the X-ARAPUCA DUNE HD (to be run as option1)

* ```cajita_xa-sbnd.json``` -> measurements from February 2023 to April 2023 of the X-ARAPUCA SBND (to be run as option3). 
    There are two types of files for production (*_prod) and for visualization. 
    Moreover there were two measurements to be made with visible ligth (VIS) and with alpha source (VUV)
* ```megacell_v1.json``` -> measurements of August 2023 of the MeggaCell DUNE-VD (to be run as option3).
* ```megacell_v2.json``` -> measurements from October 2023 of the MeggaCell DUNE-VD (nueva cajita; to be run as option3)
* ```megacell_v3.json``` -> measurements from December 2023 of the MeggaCell DUNE-VD (default)

-----------------------------------------------------------------------------------------------------------------------------------------------------

### 🪄 4. ANALYSIS: PYTHON SCRIPTS

In the analysis folder you can find some scripts to analyse the output of the simulation. Outputs are saved in the ```results``` folder. If the ```AccumHits`` variable is needed make sure that ```save_all = true``` in ```detector.cc``` is enabled (it is by default).


* ```AnaliticIntegral.py``` ---> expected shapes of the distributions for the different detectors
* ```SimulationHitDistribution.py``` ---> accumulated number of hits in plotly (+ the combinated distribution for SiPMs)
* ```SimulationOutput.py``` ---> #PE and angular distribution per sensor
* ```SimulationSurfaceDistribution.py``` ---> plots the photon density in a 2D histogram for each sensor

To run type ```python3 your_script.py``` in your terminal. The scripts allow some inputs with flags run ```python3 your_script.py --help``` for more info on how to change the default parameters.

If you have chosen to run the simulation with the multithreading option you will have different files you may need to combine before looking at the results. Check this command 😉:

```console
hadd -k COMBINED_ALL_OUTPUTS.root COMMOM_NAME_t*
```

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

* [**Alvárez-Garrote, Rodrigo**](https://github.com/rodralva)
* [**Pérez-Molina, Laura**](https://github.com/LauPM)
* [**Manthey Corchado, Sergio**](https://github.com/mantheys)
