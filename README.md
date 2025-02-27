# CAJITA

Geant4 simulation repository to optimize IR02 setups. 

## HOW TO RUN

To be ran in ```gaeuidc1.ciemat.es``` (```CentOS7```, ```Geant4 v10-07-patch01```, ```Motif``` library)

For new simulations you need:

* New file in ```configs/your_geometry.json```
* Two files in ```geometries/``` ```your_geometry.cc``` + ```your_geometry.hh```

You can try with an existing one first as follows:

### 🇬4️⃣ 1. SETUP AND COMPILATION

Once you have cloned the repository, you can setup the enviroment with the following commands:

```console
source setup.sh
cd srcs && ./scripts/run_build.sh <geometry>
```

The geometry argument is optional, if not provided the script will ask for it.

![setup](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/83ecf83e-d368-4be5-b1f2-98375f88d920)
![compilation](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/89fbd956-3177-4630-9296-3bafe01e99c6)
![success](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/5eb0b15b-0b8e-4fc3-ac29-aef5298a0816)

⚠️ WARNINGS ⚠️

* You will need the library **Motif** for a successful compilation (ask ae_support to install it in you local pcae if you need it). It is already installed in ```gaeuidc1```.
* ```run_build.sh``` script only runs from ```srcs``` folder (it will ask for your confirmation to continue with the process)
* You need to choose a geometry (```geometries``` and ```configs``` folders 📂) for the compilation. Each change need to be recompiled !! (Do not include the extension of the file)

🆕 Once you have clearly chosen your geometry you can compile with ```cd srcs && ./scripts/run_build.sh <geometry>``` to avoid the second question of the script.
![build](https://github.com/CIEMAT-Neutrino/SIM_LAB_GEANT4/assets/80100549/b5648858-3a74-4393-bf29-8cd26bd02b36)

-----------------------------------------------------------------------------------------------------------------------------------------------------

### 👾 2. RUN YOUR CHOSEN GEOMETRY

```bash
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

In the analysis folder you can find some scripts to analyse the output of the simulation. Outputs are saved in the ```results``` folder. If the ```AccumHits``` variable is needed make sure that ```save_all = true``` in ```detector.cc``` is enabled (it is by default).

* ```AnaliticIntegral.py``` ---> expected shapes of the distributions for the different detectors
* ```SimulationHitDistribution.py``` ---> accumulated number of hits in plotly (+ the combinated distribution for SiPMs)
* ```SimulationOutput.py``` ---> #PE and angular distribution per sensor
* ```SimulationSurfaceDistribution.py``` ---> plots the photon density in a 2D histogram for each sensor

To run type ```python3 your_script.py``` in your terminal. The scripts allow some inputs with flags run ```python3 your_script.py --help``` for more info on how to change the default parameters.

If you have chosen to run the simulation with the multithreading option you will have different files you may need to combine before looking at the results. Check this command 😉:

```console
hadd -k COMBINED_ALL_OUTPUTS.root COMMOM_NAME_t*
```


## LEGACY SUMMARY

We started in 2021 with a very basic example of a light propagating LAr simulation in Geant4 to optimize IR02 setups. The world volume is filled with LAr and we place different detectors that can be included with given positions in a .json or manually. The output is a ```.root``` file which contains (_Photon_) NTuple with the position (X,Y,Z) of the photon hit in your detector and the incidence angle (theta, phi).

We have different headers and sources that are included in the main file (```box1.cc```).
Chronologically we have created the files as follows:

1.- Detector construction (construction)

2.- Physics included in the analysis (physics)

3.- Particle gun in action (action)

4.- Particle generator (generator)

5.- Inserting sensitive detector (detector) and we add in ```construcction.cc``` the detectors' volumes

6.- Store the information in a ```.root``` file (with Ntuples). Defined in ```run.cc``` and ```run.hh```. We can see the stored information:

```bash
  >> root output.root
  >> new TBrowser()
  >> Hits->Draw("fX:fY", "", "colz") //to see the correlation
```

7.- Automatizating the code

* ```vis.mac``` for visualization commands -> when creating ```vis.mac``` it is not in ```/build``` and it is not found, we move all the macro file to the right directory without including absolute paths -> look in ```CMakeLists.txt```.

  <details>
    <summary>vis.mac</summary>
      
    ```mac
    #visualization properties
    /run/initialize
    /vis/open OGL
    #/vis/scene/add/axes 0 0 0 1 mm
    /vis/drawVolume
    /vis/viewer/set/autoRefresh true
    /vis/scene/add/trajectories smooth
    #/vis/scene/add/axes
    /vis/scene/add/eventID
    /vis/enable
    /vis/viewer/rebuild
    /vis/scene/endOfEventAction accumulate 100
    /vis/viewer/set/background white
    ```

  </details>
  
* ```run.mac``` -> change the momentum for each particle creation. Changes in ```box1.cc``` to accept command line inputs as files (Execute as: ```>>./box1 *.mac```).

  <details>
    <summary>run.mac</summary>
      
    ```mac
    #initialise the geometry and the physics tables
  /run/initialize

  #set the beam parameters
  /gun/momentumAmp 1 GeV
  /gun/particle opticalphoton
  /gun/position 0 0 -1.9 cm

  #run the code
  /run/beamOn 100
    ```

  </details>

* ```*.dat``` -> store quantum/photon detection efficiencies for the different detectors.
  <details>
    <summary>PMT_qeff.dat</summary>
      
    ```dat
    116.4   18.72
    118.0	20.26
    121.6	23.04
    125.4	23.39
    135.4	26.46
    140.3	26.99
    144.1	26.85
    148.7	26.33
    154.5	23.32
    160.8	28.00
    164.8	29.64
    170.0	31.51
    175.0	33.31
    182.3	34.70
    187.9	33.85
    193.7	32.18
    200.0	31.52
    210.0	30.18
    220.0	28.87
    230.0	28.42
    240.0	28.54
    250.0	28.90
    260.0	28.54
    270.0	28.39
    280.0	28.80
    290.0	29.23
    300.0	29.75
    310.0	30.10
    320.0	30.22
    330.0	30.13
    340.0	29.96
    350.0	29.74
    360.0	29.32
    370.0	28.81
    380.0	28.65
    390.0	28.27
    400.0	27.70
    410.0	27.04
    420.0	26.33
    430.0	25.52
    440.0	24.64
    450.0	23.60
    460.0	22.41
    470.0	21.15
    480.0	20.01
    490.0	18.99
    500.0	18.05
    510.0	17.07
    520.0	15.95
    530.0	14.63
    540.0	13.18
    550.0	11.71
    560.0	10.28
    570.0	8.92
    580.0	7.96
    590.0	6.64
    600.0	5.75
    610.0	5.01
    620.0	4.36
    630.0	3.76
    640.0	3.20
    650.0	2.68
    660.0	2.20
    670.0	1.77
    680.0	1.38
    690.0	1.06
    700.0	0.79
    710.0	0.57
    720.0	0.40
    730.0	0.27
    740.0	0.18
    750.0	0.12
    760.0	0.07
    770.0	0.05
    780.0	0.03
    790.0	0.02

    ```

  </details>

8.- Analysing the Ntuple.root generated with the photons' hits. Have a look at the ```analysis``` folder.

We can send alpha particles (blue), electrons(red) and photons(green) are produced.
We would need to include the energy deposition calculation that can be also stored in the NTuple.
We want to show Scintillation LAr light (see [G4Scintillation](https://apc.u-paris.fr/~franco/g4doxy/html/classG4Scintillation.html))

-----------------------------------------------------------------------------------------------------------------------------------------------------

## LEARNED LESSONS

We went through different options for running the code until we reach the current configuration. We can summarize this in three options of configuring the geometry that can be run as:

### OPTION 1: no json. Source information in a ```.mac```. Compile each time to update changes in construction.

```bash
./build/box1 configs/option1.mac data/output.root
```

<details>
  <summary>configs/option1.mac</summary>
    
  ```mac
  #initialise the geometry and the physics tables
  /run/initialize
  /control/verbose 0
  /tracking/verbose 0
  /event/verbose 0

  #set the beam parameters
  /gps/verbose 2
  /gps/particle opticalphoton
  /gps/ene/type Mono

  # --> Energy equiv. to 128nm
  /gps/ene/mono 9.7 eV

  # --> Energy equiv. to 413nm
  # /gps/ene/mono 3. eV

  # /gps/polarization 1 1 1

  # --> Extense Source
  /gps/pos/type Plane
  /gps/pos/shape Rectangle
  /gps/pos/halfx 0.1 m
  /gps/pos/halfy 0.1 m

  /gps/pos/centre 0 0 0.5 m

  #/gps/pos/confine DetectorVolume
  #/gps/pos/halfx 1 mm
  #/gps/pos/halfy 1 mm 

  #/gps/direction 0 0 -1
  /gps/ang/type iso
  /gps/number 1

  #run the code
  /run/beamOn 1000
  ```

</details>


### OPTION 2: fixed positions given in construcction.cc (```sbnd_pds_mapping.json```). Source information by ```.mac```

```bash
./build/box1 configs/option2.mac data/output.root
```
You can pick the Geant4 configurations with the following examples:
<details>
  <summary>configs/option2.mac</summary>

  ```mac
  #initialise the geometry and the physics tables
  /run/initialize
  /control/verbose 0
  /tracking/verbose 0
  /event/verbose 0

  #set the beam parameters
  /gps/verbose 0
  /gps/particle opticalphoton
  /gps/ene/type User

  # TPB Energy Spectrum
  # TpbEmmisionEnergies: [0.05,1.0,1.5, 2.25, 2.481, 2.819, 2.952,2.988,3.024, 3.1, 3.14,3.1807, 3.54, 5.5, 50.39]
  # TpbEmmisionSpectrum: [0.0, 0.0, 0.0, 0.0588,0.235, 0.853, 1.0,1.0,0.9259,0.704,0.0296,0.011, 0.0,0.0, 0.     ]

  #Geant4 default units are MeV, add extra E-6 to the bins.

  /gps/hist/type energy
  /gps/hist/point 0.0E-6     0.0
  /gps/hist/point 1.0E-6     0.0
  /gps/hist/point 1.5E-6     0.0
  /gps/hist/point 2.25E-6    0.0588
  /gps/hist/point 2.481E-6   0.235
  /gps/hist/point 2.819E-6   0.853
  /gps/hist/point 2.952E-6   1.0
  /gps/hist/point 2.988E-6   1.0
  /gps/hist/point 3.024E-6   0.9259
  /gps/hist/point 3.1E-6     0.704
  /gps/hist/point 3.14E-6    0.0296
  /gps/hist/point 3.1807E-6  0.011
  /gps/hist/point 3.54E-6    0.0
  /gps/hist/point 5.5E-6     0.0


  # /gps/polarization 1 1 1


  # --> Extense Source
  /gps/pos/type Plane
  /gps/pos/shape Rectangle
  /gps/pos/halfx 2.5 m
  /gps/pos/halfy 2 m

  /gps/pos/centre 0 0 1 m

  #/gps/pos/confine DetectorVolume
  #/gps/pos/halfx 1 mm
  #/gps/pos/halfy 1 mm 

  #/gps/direction 0 0 -1
  /gps/ang/type iso
  /gps/number 1

  #run the code

  /run/beamOn 10000

  #30seg ~14k detected photons
  #/run/beamOn 10000000
  #6min/140k
  #/run/beamOn 100000000

  #~1h/1.5M
  #/run/beamOn 1000000000


  #/run/beamOn 191800
  ```

</details>


<details>
  <summary>sbnd_pds_mapping.json</summary>

  ```json
  [
  {
    "channel": 0,
    "pd_type": "xarapuca_vis",
    "pds_box": 0,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 20.2542
  },
  {
    "channel": 1,
    "pd_type": "xarapuca_vis",
    "pds_box": 1,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 20.2542
  },
  {
    "channel": 2,
    "pd_type": "xarapuca_vis",
    "pds_box": 2,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 20.2542
  },
  {
    "channel": 3,
    "pd_type": "xarapuca_vis",
    "pds_box": 3,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 20.2542
  },
  {
    "channel": 4,
    "pd_type": "xarapuca_vis",
    "pds_box": 4,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 20.2542
  },
  {
    "channel": 5,
    "pd_type": "xarapuca_vis",
    "pds_box": 5,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 20.2542
  },
  {
    "channel": 6,
    "pd_type": "pmt_coated",
    "pds_box": 0,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -175,
    "z": 27.8742
  },
  {
    "channel": 7,
    "pd_type": "pmt_coated",
    "pds_box": 1,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -175,
    "z": 27.8742
  },
  {
    "channel": 8,
    "pd_type": "pmt_coated",
    "pds_box": 0,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -95,
    "z": 27.8742
  },
  {
    "channel": 9,
    "pd_type": "pmt_coated",
    "pds_box": 1,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -95,
    "z": 27.8742
  },
  {
    "channel": 10,
    "pd_type": "pmt_coated",
    "pds_box": 2,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -40,
    "z": 27.8742
  },
  {
    "channel": 11,
    "pd_type": "pmt_coated",
    "pds_box": 3,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -40,
    "z": 27.8742
  },
  {
    "channel": 12,
    "pd_type": "pmt_coated",
    "pds_box": 2,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 40,
    "z": 27.8742
  },
  {
    "channel": 13,
    "pd_type": "pmt_coated",
    "pds_box": 3,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 40,
    "z": 27.8742
  },
  {
    "channel": 14,
    "pd_type": "pmt_coated",
    "pds_box": 4,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 95,
    "z": 27.8742
  },
  {
    "channel": 15,
    "pd_type": "pmt_coated",
    "pds_box": 5,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 95,
    "z": 27.8742
  },
  {
    "channel": 16,
    "pd_type": "pmt_coated",
    "pds_box": 4,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 175,
    "z": 27.8742
  },
  {
    "channel": 17,
    "pd_type": "pmt_coated",
    "pds_box": 5,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 175,
    "z": 27.8742
  },
  {
    "channel": 18,
    "pd_type": "xarapuca_vuv",
    "pds_box": 0,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 35.4942
  },
  {
    "channel": 19,
    "pd_type": "xarapuca_vuv",
    "pds_box": 1,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 35.4942
  },
  {
    "channel": 20,
    "pd_type": "xarapuca_vuv",
    "pds_box": 2,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 35.4942
  },
  {
    "channel": 21,
    "pd_type": "xarapuca_vuv",
    "pds_box": 3,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 35.4942
  },
  {
    "channel": 22,
    "pd_type": "xarapuca_vuv",
    "pds_box": 4,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 35.4942
  },
  {
    "channel": 23,
    "pd_type": "xarapuca_vuv",
    "pds_box": 5,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 35.4942
  },
  {
    "channel": 24,
    "pd_type": "xarapuca_vis",
    "pds_box": 0,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -175,
    "z": 50.2542
  },
  {
    "channel": 25,
    "pd_type": "xarapuca_vis",
    "pds_box": 1,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -175,
    "z": 50.2542
  },
  {
    "channel": 26,
    "pd_type": "xarapuca_vis",
    "pds_box": 0,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -95,
    "z": 50.2542
  },
  {
    "channel": 27,
    "pd_type": "xarapuca_vis",
    "pds_box": 1,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -95,
    "z": 50.2542
  },
  {
    "channel": 28,
    "pd_type": "xarapuca_vis",
    "pds_box": 2,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -40,
    "z": 50.2542
  },
  {
    "channel": 29,
    "pd_type": "xarapuca_vis",
    "pds_box": 3,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -40,
    "z": 50.2542
  },
  {
    "channel": 30,
    "pd_type": "xarapuca_vis",
    "pds_box": 2,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 40,
    "z": 50.2542
  },
  {
    "channel": 31,
    "pd_type": "xarapuca_vis",
    "pds_box": 3,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 40,
    "z": 50.2542
  },
  {
    "channel": 32,
    "pd_type": "xarapuca_vis",
    "pds_box": 4,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 95,
    "z": 50.2542
  },
  {
    "channel": 33,
    "pd_type": "xarapuca_vis",
    "pds_box": 5,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 95,
    "z": 50.2542
  },
  {
    "channel": 34,
    "pd_type": "xarapuca_vis",
    "pds_box": 4,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 175,
    "z": 50.2542
  },
  {
    "channel": 35,
    "pd_type": "xarapuca_vis",
    "pds_box": 5,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 175,
    "z": 50.2542
  },
  {
    "channel": 36,
    "pd_type": "pmt_uncoated",
    "pds_box": 0,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -135,
    "z": 57.8742
  },
  {
    "channel": 37,
    "pd_type": "pmt_uncoated",
    "pds_box": 1,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -135,
    "z": 57.8742
  },
  {
    "channel": 38,
    "pd_type": "pmt_uncoated",
    "pds_box": 2,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 0,
    "z": 57.8742
  },
  {
    "channel": 39,
    "pd_type": "pmt_uncoated",
    "pds_box": 3,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 0,
    "z": 57.8742
  },
  {
    "channel": 40,
    "pd_type": "pmt_uncoated",
    "pds_box": 4,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 135,
    "z": 57.8742
  },
  {
    "channel": 41,
    "pd_type": "pmt_uncoated",
    "pds_box": 5,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 135,
    "z": 57.8742
  },
  {
    "channel": 42,
    "pd_type": "xarapuca_vuv",
    "pds_box": 0,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -175,
    "z": 65.4942
  },
  {
    "channel": 43,
    "pd_type": "xarapuca_vuv",
    "pds_box": 1,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -175,
    "z": 65.4942
  },
  {
    "channel": 44,
    "pd_type": "xarapuca_vuv",
    "pds_box": 0,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -95,
    "z": 65.4942
  },
  {
    "channel": 45,
    "pd_type": "xarapuca_vuv",
    "pds_box": 1,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -95,
    "z": 65.4942
  },
  {
    "channel": 46,
    "pd_type": "xarapuca_vuv",
    "pds_box": 2,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -40,
    "z": 65.4942
  },
  {
    "channel": 47,
    "pd_type": "xarapuca_vuv",
    "pds_box": 3,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -40,
    "z": 65.4942
  },
  {
    "channel": 48,
    "pd_type": "xarapuca_vuv",
    "pds_box": 2,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 40,
    "z": 65.4942
  },
  {
    "channel": 49,
    "pd_type": "xarapuca_vuv",
    "pds_box": 3,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 40,
    "z": 65.4942
  },
  {
    "channel": 50,
    "pd_type": "xarapuca_vuv",
    "pds_box": 4,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 95,
    "z": 65.4942
  },
  {
    "channel": 51,
    "pd_type": "xarapuca_vuv",
    "pds_box": 5,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 95,
    "z": 65.4942
  },
  {
    "channel": 52,
    "pd_type": "xarapuca_vuv",
    "pds_box": 4,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 175,
    "z": 65.4942
  },
  {
    "channel": 53,
    "pd_type": "xarapuca_vuv",
    "pds_box": 5,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 175,
    "z": 65.4942
  },
  {
    "channel": 54,
    "pd_type": "xarapuca_vis",
    "pds_box": 0,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 80.2542
  },
  {
    "channel": 55,
    "pd_type": "xarapuca_vis",
    "pds_box": 1,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 80.2542
  },
  {
    "channel": 56,
    "pd_type": "xarapuca_vis",
    "pds_box": 2,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 80.2542
  },
  {
    "channel": 57,
    "pd_type": "xarapuca_vis",
    "pds_box": 3,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 80.2542
  },
  {
    "channel": 58,
    "pd_type": "xarapuca_vis",
    "pds_box": 4,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 80.2542
  },
  {
    "channel": 59,
    "pd_type": "xarapuca_vis",
    "pds_box": 5,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 80.2542
  },
  {
    "channel": 60,
    "pd_type": "pmt_coated",
    "pds_box": 0,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -175,
    "z": 87.8742
  },
  {
    "channel": 61,
    "pd_type": "pmt_coated",
    "pds_box": 1,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -175,
    "z": 87.8742
  },
  {
    "channel": 62,
    "pd_type": "pmt_coated",
    "pds_box": 0,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -95,
    "z": 87.8742
  },
  {
    "channel": 63,
    "pd_type": "pmt_coated",
    "pds_box": 1,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -95,
    "z": 87.8742
  },
  {
    "channel": 64,
    "pd_type": "pmt_coated",
    "pds_box": 2,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -40,
    "z": 87.8742
  },
  {
    "channel": 65,
    "pd_type": "pmt_coated",
    "pds_box": 3,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -40,
    "z": 87.8742
  },
  {
    "channel": 66,
    "pd_type": "pmt_coated",
    "pds_box": 2,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 40,
    "z": 87.8742
  },
  {
    "channel": 67,
    "pd_type": "pmt_coated",
    "pds_box": 3,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 40,
    "z": 87.8742
  },
  {
    "channel": 68,
    "pd_type": "pmt_coated",
    "pds_box": 4,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 95,
    "z": 87.8742
  },
  {
    "channel": 69,
    "pd_type": "pmt_coated",
    "pds_box": 5,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 95,
    "z": 87.8742
  },
  {
    "channel": 70,
    "pd_type": "pmt_coated",
    "pds_box": 4,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 175,
    "z": 87.8742
  },
  {
    "channel": 71,
    "pd_type": "pmt_coated",
    "pds_box": 5,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 175,
    "z": 87.8742
  },
  {
    "channel": 72,
    "pd_type": "xarapuca_vuv",
    "pds_box": 0,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 95.4942
  },
  {
    "channel": 73,
    "pd_type": "xarapuca_vuv",
    "pds_box": 1,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 95.4942
  },
  {
    "channel": 74,
    "pd_type": "xarapuca_vuv",
    "pds_box": 2,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 95.4942
  },
  {
    "channel": 75,
    "pd_type": "xarapuca_vuv",
    "pds_box": 3,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 95.4942
  },
  {
    "channel": 76,
    "pd_type": "xarapuca_vuv",
    "pds_box": 4,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 95.4942
  },
  {
    "channel": 77,
    "pd_type": "xarapuca_vuv",
    "pds_box": 5,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 95.4942
  },
  {
    "channel": 78,
    "pd_type": "xarapuca_vis",
    "pds_box": 6,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 153.538
  },
  {
    "channel": 79,
    "pd_type": "xarapuca_vis",
    "pds_box": 7,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 153.538
  },
  {
    "channel": 80,
    "pd_type": "xarapuca_vis",
    "pds_box": 8,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 153.538
  },
  {
    "channel": 81,
    "pd_type": "xarapuca_vis",
    "pds_box": 9,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 153.538
  },
  {
    "channel": 82,
    "pd_type": "xarapuca_vis",
    "pds_box": 10,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 153.538
  },
  {
    "channel": 83,
    "pd_type": "xarapuca_vis",
    "pds_box": 11,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 153.538
  },
  {
    "channel": 84,
    "pd_type": "pmt_coated",
    "pds_box": 6,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -175,
    "z": 161.158
  },
  {
    "channel": 85,
    "pd_type": "pmt_coated",
    "pds_box": 7,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -175,
    "z": 161.158
  },
  {
    "channel": 86,
    "pd_type": "pmt_coated",
    "pds_box": 6,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -95,
    "z": 161.158
  },
  {
    "channel": 87,
    "pd_type": "pmt_coated",
    "pds_box": 7,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -95,
    "z": 161.158
  },
  {
    "channel": 88,
    "pd_type": "pmt_coated",
    "pds_box": 8,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -40,
    "z": 161.158
  },
  {
    "channel": 89,
    "pd_type": "pmt_coated",
    "pds_box": 9,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -40,
    "z": 161.158
  },
  {
    "channel": 90,
    "pd_type": "pmt_coated",
    "pds_box": 8,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 40,
    "z": 161.158
  },
  {
    "channel": 91,
    "pd_type": "pmt_coated",
    "pds_box": 9,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 40,
    "z": 161.158
  },
  {
    "channel": 92,
    "pd_type": "pmt_coated",
    "pds_box": 10,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 95,
    "z": 161.158
  },
  {
    "channel": 93,
    "pd_type": "pmt_coated",
    "pds_box": 11,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 95,
    "z": 161.158
  },
  {
    "channel": 94,
    "pd_type": "pmt_coated",
    "pds_box": 10,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 175,
    "z": 161.158
  },
  {
    "channel": 95,
    "pd_type": "pmt_coated",
    "pds_box": 11,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 175,
    "z": 161.158
  },
  {
    "channel": 96,
    "pd_type": "xarapuca_vuv",
    "pds_box": 6,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 168.778
  },
  {
    "channel": 97,
    "pd_type": "xarapuca_vuv",
    "pds_box": 7,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 168.778
  },
  {
    "channel": 98,
    "pd_type": "xarapuca_vuv",
    "pds_box": 8,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 168.778
  },
  {
    "channel": 99,
    "pd_type": "xarapuca_vuv",
    "pds_box": 9,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 168.778
  },
  {
    "channel": 100,
    "pd_type": "xarapuca_vuv",
    "pds_box": 10,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 168.778
  },
  {
    "channel": 101,
    "pd_type": "xarapuca_vuv",
    "pds_box": 11,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 168.778
  },
  {
    "channel": 102,
    "pd_type": "xarapuca_vis",
    "pds_box": 6,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -175,
    "z": 183.538
  },
  {
    "channel": 103,
    "pd_type": "xarapuca_vis",
    "pds_box": 7,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -175,
    "z": 183.538
  },
  {
    "channel": 104,
    "pd_type": "xarapuca_vis",
    "pds_box": 6,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -95,
    "z": 183.538
  },
  {
    "channel": 105,
    "pd_type": "xarapuca_vis",
    "pds_box": 7,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -95,
    "z": 183.538
  },
  {
    "channel": 106,
    "pd_type": "xarapuca_vis",
    "pds_box": 8,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -40,
    "z": 183.538
  },
  {
    "channel": 107,
    "pd_type": "xarapuca_vis",
    "pds_box": 9,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -40,
    "z": 183.538
  },
  {
    "channel": 108,
    "pd_type": "xarapuca_vis",
    "pds_box": 8,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 40,
    "z": 183.538
  },
  {
    "channel": 109,
    "pd_type": "xarapuca_vis",
    "pds_box": 9,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 40,
    "z": 183.538
  },
  {
    "channel": 110,
    "pd_type": "xarapuca_vis",
    "pds_box": 10,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 95,
    "z": 183.538
  },
  {
    "channel": 111,
    "pd_type": "xarapuca_vis",
    "pds_box": 11,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 95,
    "z": 183.538
  },
  {
    "channel": 112,
    "pd_type": "xarapuca_vis",
    "pds_box": 10,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 175,
    "z": 183.538
  },
  {
    "channel": 113,
    "pd_type": "xarapuca_vis",
    "pds_box": 11,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 175,
    "z": 183.538
  },
  {
    "channel": 114,
    "pd_type": "pmt_uncoated",
    "pds_box": 6,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -135,
    "z": 191.158
  },
  {
    "channel": 115,
    "pd_type": "pmt_uncoated",
    "pds_box": 7,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -135,
    "z": 191.158
  },
  {
    "channel": 116,
    "pd_type": "pmt_uncoated",
    "pds_box": 8,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 0,
    "z": 191.158
  },
  {
    "channel": 117,
    "pd_type": "pmt_uncoated",
    "pds_box": 9,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 0,
    "z": 191.158
  },
  {
    "channel": 118,
    "pd_type": "pmt_uncoated",
    "pds_box": 10,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 135,
    "z": 191.158
  },
  {
    "channel": 119,
    "pd_type": "pmt_uncoated",
    "pds_box": 11,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 135,
    "z": 191.158
  },
  {
    "channel": 120,
    "pd_type": "xarapuca_vuv",
    "pds_box": 6,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -175,
    "z": 198.778
  },
  {
    "channel": 121,
    "pd_type": "xarapuca_vuv",
    "pds_box": 7,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -175,
    "z": 198.778
  },
  {
    "channel": 122,
    "pd_type": "xarapuca_vuv",
    "pds_box": 6,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -95,
    "z": 198.778
  },
  {
    "channel": 123,
    "pd_type": "xarapuca_vuv",
    "pds_box": 7,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -95,
    "z": 198.778
  },
  {
    "channel": 124,
    "pd_type": "xarapuca_vuv",
    "pds_box": 8,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -40,
    "z": 198.778
  },
  {
    "channel": 125,
    "pd_type": "xarapuca_vuv",
    "pds_box": 9,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -40,
    "z": 198.778
  },
  {
    "channel": 126,
    "pd_type": "xarapuca_vuv",
    "pds_box": 8,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 40,
    "z": 198.778
  },
  {
    "channel": 127,
    "pd_type": "xarapuca_vuv",
    "pds_box": 9,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 40,
    "z": 198.778
  },
  {
    "channel": 128,
    "pd_type": "xarapuca_vuv",
    "pds_box": 10,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 95,
    "z": 198.778
  },
  {
    "channel": 129,
    "pd_type": "xarapuca_vuv",
    "pds_box": 11,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 95,
    "z": 198.778
  },
  {
    "channel": 130,
    "pd_type": "xarapuca_vuv",
    "pds_box": 10,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 175,
    "z": 198.778
  },
  {
    "channel": 131,
    "pd_type": "xarapuca_vuv",
    "pds_box": 11,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 175,
    "z": 198.778
  },
  {
    "channel": 132,
    "pd_type": "xarapuca_vis",
    "pds_box": 6,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 213.538
  },
  {
    "channel": 133,
    "pd_type": "xarapuca_vis",
    "pds_box": 7,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 213.538
  },
  {
    "channel": 134,
    "pd_type": "xarapuca_vis",
    "pds_box": 8,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "apsaia",
    "x": -213.75,
    "y": 0,
    "z": 213.538
  },
  {
    "channel": 135,
    "pd_type": "xarapuca_vis",
    "pds_box": 9,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "apsaia",
    "x": 213.75,
    "y": 0,
    "z": 213.538
  },
  {
    "channel": 136,
    "pd_type": "xarapuca_vis",
    "pds_box": 10,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 213.538
  },
  {
    "channel": 137,
    "pd_type": "xarapuca_vis",
    "pds_box": 11,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 213.538
  },
  {
    "channel": 138,
    "pd_type": "pmt_coated",
    "pds_box": 6,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -175,
    "z": 221.158
  },
  {
    "channel": 139,
    "pd_type": "pmt_coated",
    "pds_box": 7,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -175,
    "z": 221.158
  },
  {
    "channel": 140,
    "pd_type": "pmt_coated",
    "pds_box": 6,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -95,
    "z": 221.158
  },
  {
    "channel": 141,
    "pd_type": "pmt_coated",
    "pds_box": 7,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -95,
    "z": 221.158
  },
  {
    "channel": 142,
    "pd_type": "pmt_coated",
    "pds_box": 8,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -40,
    "z": 221.158
  },
  {
    "channel": 143,
    "pd_type": "pmt_coated",
    "pds_box": 9,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -40,
    "z": 221.158
  },
  {
    "channel": 144,
    "pd_type": "pmt_coated",
    "pds_box": 8,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 40,
    "z": 221.158
  },
  {
    "channel": 145,
    "pd_type": "pmt_coated",
    "pds_box": 9,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 40,
    "z": 221.158
  },
  {
    "channel": 146,
    "pd_type": "pmt_coated",
    "pds_box": 10,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 95,
    "z": 221.158
  },
  {
    "channel": 147,
    "pd_type": "pmt_coated",
    "pds_box": 11,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 95,
    "z": 221.158
  },
  {
    "channel": 148,
    "pd_type": "pmt_coated",
    "pds_box": 10,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 175,
    "z": 221.158
  },
  {
    "channel": 149,
    "pd_type": "pmt_coated",
    "pds_box": 11,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 175,
    "z": 221.158
  },
  {
    "channel": 150,
    "pd_type": "xarapuca_vuv",
    "pds_box": 6,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "apsaia",
    "x": -213.75,
    "y": -135,
    "z": 228.778
  },
  {
    "channel": 151,
    "pd_type": "xarapuca_vuv",
    "pds_box": 7,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "apsaia",
    "x": 213.75,
    "y": -135,
    "z": 228.778
  },
  {
    "channel": 152,
    "pd_type": "xarapuca_vuv",
    "pds_box": 8,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "apsaia",
    "x": -213.75,
    "y": 0,
    "z": 228.778
  },
  {
    "channel": 153,
    "pd_type": "xarapuca_vuv",
    "pds_box": 9,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "apsaia",
    "x": 213.75,
    "y": 0,
    "z": 228.778
  },
  {
    "channel": 154,
    "pd_type": "xarapuca_vuv",
    "pds_box": 10,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "apsaia",
    "x": -213.75,
    "y": 135,
    "z": 228.778
  },
  {
    "channel": 155,
    "pd_type": "xarapuca_vuv",
    "pds_box": 11,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "apsaia",
    "x": 213.75,
    "y": 135,
    "z": 228.778
  },
  {
    "channel": 156,
    "pd_type": "xarapuca_vis",
    "pds_box": 12,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "apsaia",
    "x": -213.75,
    "y": -135,
    "z": 280.622
  },
  {
    "channel": 157,
    "pd_type": "xarapuca_vis",
    "pds_box": 13,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "apsaia",
    "x": 213.75,
    "y": -135,
    "z": 280.622
  },
  {
    "channel": 158,
    "pd_type": "xarapuca_vis",
    "pds_box": 14,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "apsaia",
    "x": -213.75,
    "y": 0,
    "z": 280.622
  },
  {
    "channel": 159,
    "pd_type": "xarapuca_vis",
    "pds_box": 15,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "apsaia",
    "x": 213.75,
    "y": 0,
    "z": 280.622
  },
  {
    "channel": 160,
    "pd_type": "xarapuca_vis",
    "pds_box": 16,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "apsaia",
    "x": -213.75,
    "y": 135,
    "z": 280.622
  },
  {
    "channel": 161,
    "pd_type": "xarapuca_vis",
    "pds_box": 17,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "apsaia",
    "x": 213.75,
    "y": 135,
    "z": 280.622
  },
  {
    "channel": 162,
    "pd_type": "pmt_coated",
    "pds_box": 12,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -175,
    "z": 288.242
  },
  {
    "channel": 163,
    "pd_type": "pmt_coated",
    "pds_box": 13,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -175,
    "z": 288.242
  },
  {
    "channel": 164,
    "pd_type": "pmt_coated",
    "pds_box": 12,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -95,
    "z": 288.242
  },
  {
    "channel": 165,
    "pd_type": "pmt_coated",
    "pds_box": 13,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -95,
    "z": 288.242
  },
  {
    "channel": 166,
    "pd_type": "pmt_coated",
    "pds_box": 14,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -40,
    "z": 288.242
  },
  {
    "channel": 167,
    "pd_type": "pmt_coated",
    "pds_box": 15,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -40,
    "z": 288.242
  },
  {
    "channel": 168,
    "pd_type": "pmt_coated",
    "pds_box": 14,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 40,
    "z": 288.242
  },
  {
    "channel": 169,
    "pd_type": "pmt_coated",
    "pds_box": 15,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 40,
    "z": 288.242
  },
  {
    "channel": 170,
    "pd_type": "pmt_coated",
    "pds_box": 16,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 95,
    "z": 288.242
  },
  {
    "channel": 171,
    "pd_type": "pmt_coated",
    "pds_box": 17,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 95,
    "z": 288.242
  },
  {
    "channel": 172,
    "pd_type": "pmt_coated",
    "pds_box": 16,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 175,
    "z": 288.242
  },
  {
    "channel": 173,
    "pd_type": "pmt_coated",
    "pds_box": 17,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 175,
    "z": 288.242
  },
  {
    "channel": 174,
    "pd_type": "xarapuca_vuv",
    "pds_box": 12,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 295.862
  },
  {
    "channel": 175,
    "pd_type": "xarapuca_vuv",
    "pds_box": 13,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 295.862
  },
  {
    "channel": 176,
    "pd_type": "xarapuca_vuv",
    "pds_box": 14,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "apsaia",
    "x": -213.75,
    "y": 0,
    "z": 295.862
  },
  {
    "channel": 177,
    "pd_type": "xarapuca_vuv",
    "pds_box": 15,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "apsaia",
    "x": 213.75,
    "y": 0,
    "z": 295.862
  },
  {
    "channel": 178,
    "pd_type": "xarapuca_vuv",
    "pds_box": 16,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 295.862
  },
  {
    "channel": 179,
    "pd_type": "xarapuca_vuv",
    "pds_box": 17,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 295.862
  },
  {
    "channel": 180,
    "pd_type": "xarapuca_vis",
    "pds_box": 12,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -175,
    "z": 310.622
  },
  {
    "channel": 181,
    "pd_type": "xarapuca_vis",
    "pds_box": 13,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -175,
    "z": 310.622
  },
  {
    "channel": 182,
    "pd_type": "xarapuca_vis",
    "pds_box": 12,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -95,
    "z": 310.622
  },
  {
    "channel": 183,
    "pd_type": "xarapuca_vis",
    "pds_box": 13,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -95,
    "z": 310.622
  },
  {
    "channel": 184,
    "pd_type": "xarapuca_vis",
    "pds_box": 14,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -40,
    "z": 310.622
  },
  {
    "channel": 185,
    "pd_type": "xarapuca_vis",
    "pds_box": 15,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -40,
    "z": 310.622
  },
  {
    "channel": 186,
    "pd_type": "xarapuca_vis",
    "pds_box": 14,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 40,
    "z": 310.622
  },
  {
    "channel": 187,
    "pd_type": "xarapuca_vis",
    "pds_box": 15,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 40,
    "z": 310.622
  },
  {
    "channel": 188,
    "pd_type": "xarapuca_vis",
    "pds_box": 16,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 95,
    "z": 310.622
  },
  {
    "channel": 189,
    "pd_type": "xarapuca_vis",
    "pds_box": 17,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 95,
    "z": 310.622
  },
  {
    "channel": 190,
    "pd_type": "xarapuca_vis",
    "pds_box": 16,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 175,
    "z": 310.622
  },
  {
    "channel": 191,
    "pd_type": "xarapuca_vis",
    "pds_box": 17,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 175,
    "z": 310.622
  },
  {
    "channel": 192,
    "pd_type": "pmt_uncoated",
    "pds_box": 12,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -135,
    "z": 318.242
  },
  {
    "channel": 193,
    "pd_type": "pmt_uncoated",
    "pds_box": 13,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -135,
    "z": 318.242
  },
  {
    "channel": 194,
    "pd_type": "pmt_uncoated",
    "pds_box": 14,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 0,
    "z": 318.242
  },
  {
    "channel": 195,
    "pd_type": "pmt_uncoated",
    "pds_box": 15,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 0,
    "z": 318.242
  },
  {
    "channel": 196,
    "pd_type": "pmt_uncoated",
    "pds_box": 16,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 135,
    "z": 318.242
  },
  {
    "channel": 197,
    "pd_type": "pmt_uncoated",
    "pds_box": 17,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 135,
    "z": 318.242
  },
  {
    "channel": 198,
    "pd_type": "xarapuca_vuv",
    "pds_box": 12,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -175,
    "z": 325.862
  },
  {
    "channel": 199,
    "pd_type": "xarapuca_vuv",
    "pds_box": 13,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -175,
    "z": 325.862
  },
  {
    "channel": 200,
    "pd_type": "xarapuca_vuv",
    "pds_box": 12,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -95,
    "z": 325.862
  },
  {
    "channel": 201,
    "pd_type": "xarapuca_vuv",
    "pds_box": 13,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -95,
    "z": 325.862
  },
  {
    "channel": 202,
    "pd_type": "xarapuca_vuv",
    "pds_box": 14,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -40,
    "z": 325.862
  },
  {
    "channel": 203,
    "pd_type": "xarapuca_vuv",
    "pds_box": 15,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -40,
    "z": 325.862
  },
  {
    "channel": 204,
    "pd_type": "xarapuca_vuv",
    "pds_box": 14,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 40,
    "z": 325.862
  },
  {
    "channel": 205,
    "pd_type": "xarapuca_vuv",
    "pds_box": 15,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 40,
    "z": 325.862
  },
  {
    "channel": 206,
    "pd_type": "xarapuca_vuv",
    "pds_box": 16,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 95,
    "z": 325.862
  },
  {
    "channel": 207,
    "pd_type": "xarapuca_vuv",
    "pds_box": 17,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 95,
    "z": 325.862
  },
  {
    "channel": 208,
    "pd_type": "xarapuca_vuv",
    "pds_box": 16,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 175,
    "z": 325.862
  },
  {
    "channel": 209,
    "pd_type": "xarapuca_vuv",
    "pds_box": 17,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 175,
    "z": 325.862
  },
  {
    "channel": 210,
    "pd_type": "xarapuca_vis",
    "pds_box": 12,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 340.622
  },
  {
    "channel": 211,
    "pd_type": "xarapuca_vis",
    "pds_box": 13,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 340.622
  },
  {
    "channel": 212,
    "pd_type": "xarapuca_vis",
    "pds_box": 14,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 340.622
  },
  {
    "channel": 213,
    "pd_type": "xarapuca_vis",
    "pds_box": 15,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 340.622
  },
  {
    "channel": 214,
    "pd_type": "xarapuca_vis",
    "pds_box": 16,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 340.622
  },
  {
    "channel": 215,
    "pd_type": "xarapuca_vis",
    "pds_box": 17,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 340.622
  },
  {
    "channel": 216,
    "pd_type": "pmt_coated",
    "pds_box": 12,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -175,
    "z": 348.242
  },
  {
    "channel": 217,
    "pd_type": "pmt_coated",
    "pds_box": 13,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -175,
    "z": 348.242
  },
  {
    "channel": 218,
    "pd_type": "pmt_coated",
    "pds_box": 12,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -95,
    "z": 348.242
  },
  {
    "channel": 219,
    "pd_type": "pmt_coated",
    "pds_box": 13,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -95,
    "z": 348.242
  },
  {
    "channel": 220,
    "pd_type": "pmt_coated",
    "pds_box": 14,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -40,
    "z": 348.242
  },
  {
    "channel": 221,
    "pd_type": "pmt_coated",
    "pds_box": 15,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -40,
    "z": 348.242
  },
  {
    "channel": 222,
    "pd_type": "pmt_coated",
    "pds_box": 14,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 40,
    "z": 348.242
  },
  {
    "channel": 223,
    "pd_type": "pmt_coated",
    "pds_box": 15,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 40,
    "z": 348.242
  },
  {
    "channel": 224,
    "pd_type": "pmt_coated",
    "pds_box": 16,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 95,
    "z": 348.242
  },
  {
    "channel": 225,
    "pd_type": "pmt_coated",
    "pds_box": 17,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 95,
    "z": 348.242
  },
  {
    "channel": 226,
    "pd_type": "pmt_coated",
    "pds_box": 16,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 175,
    "z": 348.242
  },
  {
    "channel": 227,
    "pd_type": "pmt_coated",
    "pds_box": 17,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 175,
    "z": 348.242
  },
  {
    "channel": 228,
    "pd_type": "xarapuca_vuv",
    "pds_box": 12,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 355.862
  },
  {
    "channel": 229,
    "pd_type": "xarapuca_vuv",
    "pds_box": 13,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 355.862
  },
  {
    "channel": 230,
    "pd_type": "xarapuca_vuv",
    "pds_box": 14,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 355.862
  },
  {
    "channel": 231,
    "pd_type": "xarapuca_vuv",
    "pds_box": 15,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 355.862
  },
  {
    "channel": 232,
    "pd_type": "xarapuca_vuv",
    "pds_box": 16,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 355.862
  },
  {
    "channel": 233,
    "pd_type": "xarapuca_vuv",
    "pds_box": 17,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 355.862
  },
  {
    "channel": 234,
    "pd_type": "xarapuca_vis",
    "pds_box": 18,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 413.906
  },
  {
    "channel": 235,
    "pd_type": "xarapuca_vis",
    "pds_box": 19,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 413.906
  },
  {
    "channel": 236,
    "pd_type": "xarapuca_vis",
    "pds_box": 20,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 413.906
  },
  {
    "channel": 237,
    "pd_type": "xarapuca_vis",
    "pds_box": 21,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 413.906
  },
  {
    "channel": 238,
    "pd_type": "xarapuca_vis",
    "pds_box": 22,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 413.906
  },
  {
    "channel": 239,
    "pd_type": "xarapuca_vis",
    "pds_box": 23,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 413.906
  },
  {
    "channel": 240,
    "pd_type": "pmt_coated",
    "pds_box": 18,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -175,
    "z": 421.526
  },
  {
    "channel": 241,
    "pd_type": "pmt_coated",
    "pds_box": 19,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -175,
    "z": 421.526
  },
  {
    "channel": 242,
    "pd_type": "pmt_coated",
    "pds_box": 18,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -95,
    "z": 421.526
  },
  {
    "channel": 243,
    "pd_type": "pmt_coated",
    "pds_box": 19,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -95,
    "z": 421.526
  },
  {
    "channel": 244,
    "pd_type": "pmt_coated",
    "pds_box": 20,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -40,
    "z": 421.526
  },
  {
    "channel": 245,
    "pd_type": "pmt_coated",
    "pds_box": 21,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -40,
    "z": 421.526
  },
  {
    "channel": 246,
    "pd_type": "pmt_coated",
    "pds_box": 20,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 40,
    "z": 421.526
  },
  {
    "channel": 247,
    "pd_type": "pmt_coated",
    "pds_box": 21,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 40,
    "z": 421.526
  },
  {
    "channel": 248,
    "pd_type": "pmt_coated",
    "pds_box": 22,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 95,
    "z": 421.526
  },
  {
    "channel": 249,
    "pd_type": "pmt_coated",
    "pds_box": 23,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 95,
    "z": 421.526
  },
  {
    "channel": 250,
    "pd_type": "pmt_coated",
    "pds_box": 22,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 175,
    "z": 421.526
  },
  {
    "channel": 251,
    "pd_type": "pmt_coated",
    "pds_box": 23,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 175,
    "z": 421.526
  },
  {
    "channel": 252,
    "pd_type": "xarapuca_vuv",
    "pds_box": 18,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 429.146
  },
  {
    "channel": 253,
    "pd_type": "xarapuca_vuv",
    "pds_box": 19,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 429.146
  },
  {
    "channel": 254,
    "pd_type": "xarapuca_vuv",
    "pds_box": 20,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 429.146
  },
  {
    "channel": 255,
    "pd_type": "xarapuca_vuv",
    "pds_box": 21,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 429.146
  },
  {
    "channel": 256,
    "pd_type": "xarapuca_vuv",
    "pds_box": 22,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 429.146
  },
  {
    "channel": 257,
    "pd_type": "xarapuca_vuv",
    "pds_box": 23,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 429.146
  },
  {
    "channel": 258,
    "pd_type": "xarapuca_vis",
    "pds_box": 18,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -175,
    "z": 443.906
  },
  {
    "channel": 259,
    "pd_type": "xarapuca_vis",
    "pds_box": 19,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -175,
    "z": 443.906
  },
  {
    "channel": 260,
    "pd_type": "xarapuca_vis",
    "pds_box": 18,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -95,
    "z": 443.906
  },
  {
    "channel": 261,
    "pd_type": "xarapuca_vis",
    "pds_box": 19,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -95,
    "z": 443.906
  },
  {
    "channel": 262,
    "pd_type": "xarapuca_vis",
    "pds_box": 20,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -40,
    "z": 443.906
  },
  {
    "channel": 263,
    "pd_type": "xarapuca_vis",
    "pds_box": 21,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -40,
    "z": 443.906
  },
  {
    "channel": 264,
    "pd_type": "xarapuca_vis",
    "pds_box": 20,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 40,
    "z": 443.906
  },
  {
    "channel": 265,
    "pd_type": "xarapuca_vis",
    "pds_box": 21,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 40,
    "z": 443.906
  },
  {
    "channel": 266,
    "pd_type": "xarapuca_vis",
    "pds_box": 22,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 95,
    "z": 443.906
  },
  {
    "channel": 267,
    "pd_type": "xarapuca_vis",
    "pds_box": 23,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 95,
    "z": 443.906
  },
  {
    "channel": 268,
    "pd_type": "xarapuca_vis",
    "pds_box": 22,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 175,
    "z": 443.906
  },
  {
    "channel": 269,
    "pd_type": "xarapuca_vis",
    "pds_box": 23,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 175,
    "z": 443.906
  },
  {
    "channel": 270,
    "pd_type": "pmt_uncoated",
    "pds_box": 18,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -135,
    "z": 451.526
  },
  {
    "channel": 271,
    "pd_type": "pmt_uncoated",
    "pds_box": 19,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -135,
    "z": 451.526
  },
  {
    "channel": 272,
    "pd_type": "pmt_uncoated",
    "pds_box": 20,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 0,
    "z": 451.526
  },
  {
    "channel": 273,
    "pd_type": "pmt_uncoated",
    "pds_box": 21,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 0,
    "z": 451.526
  },
  {
    "channel": 274,
    "pd_type": "pmt_uncoated",
    "pds_box": 22,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 135,
    "z": 451.526
  },
  {
    "channel": 275,
    "pd_type": "pmt_uncoated",
    "pds_box": 23,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 135,
    "z": 451.526
  },
  {
    "channel": 276,
    "pd_type": "xarapuca_vuv",
    "pds_box": 18,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -175,
    "z": 459.146
  },
  {
    "channel": 277,
    "pd_type": "xarapuca_vuv",
    "pds_box": 19,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -175,
    "z": 459.146
  },
  {
    "channel": 278,
    "pd_type": "xarapuca_vuv",
    "pds_box": 18,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -95,
    "z": 459.146
  },
  {
    "channel": 279,
    "pd_type": "xarapuca_vuv",
    "pds_box": 19,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -95,
    "z": 459.146
  },
  {
    "channel": 280,
    "pd_type": "xarapuca_vuv",
    "pds_box": 20,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -40,
    "z": 459.146
  },
  {
    "channel": 281,
    "pd_type": "xarapuca_vuv",
    "pds_box": 21,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -40,
    "z": 459.146
  },
  {
    "channel": 282,
    "pd_type": "xarapuca_vuv",
    "pds_box": 20,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 40,
    "z": 459.146
  },
  {
    "channel": 283,
    "pd_type": "xarapuca_vuv",
    "pds_box": 21,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 40,
    "z": 459.146
  },
  {
    "channel": 284,
    "pd_type": "xarapuca_vuv",
    "pds_box": 22,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 95,
    "z": 459.146
  },
  {
    "channel": 285,
    "pd_type": "xarapuca_vuv",
    "pds_box": 23,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 95,
    "z": 459.146
  },
  {
    "channel": 286,
    "pd_type": "xarapuca_vuv",
    "pds_box": 22,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 175,
    "z": 459.146
  },
  {
    "channel": 287,
    "pd_type": "xarapuca_vuv",
    "pds_box": 23,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 175,
    "z": 459.146
  },
  {
    "channel": 288,
    "pd_type": "xarapuca_vis",
    "pds_box": 18,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 473.906
  },
  {
    "channel": 289,
    "pd_type": "xarapuca_vis",
    "pds_box": 19,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 473.906
  },
  {
    "channel": 290,
    "pd_type": "xarapuca_vis",
    "pds_box": 20,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 473.906
  },
  {
    "channel": 291,
    "pd_type": "xarapuca_vis",
    "pds_box": 21,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 473.906
  },
  {
    "channel": 292,
    "pd_type": "xarapuca_vis",
    "pds_box": 22,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 473.906
  },
  {
    "channel": 293,
    "pd_type": "xarapuca_vis",
    "pds_box": 23,
    "sensible_to_vuv": false,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 473.906
  },
  {
    "channel": 294,
    "pd_type": "pmt_coated",
    "pds_box": 18,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -175,
    "z": 481.526
  },
  {
    "channel": 295,
    "pd_type": "pmt_coated",
    "pds_box": 19,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -175,
    "z": 481.526
  },
  {
    "channel": 296,
    "pd_type": "pmt_coated",
    "pds_box": 18,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -95,
    "z": 481.526
  },
  {
    "channel": 297,
    "pd_type": "pmt_coated",
    "pds_box": 19,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -95,
    "z": 481.526
  },
  {
    "channel": 298,
    "pd_type": "pmt_coated",
    "pds_box": 20,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": -40,
    "z": 481.526
  },
  {
    "channel": 299,
    "pd_type": "pmt_coated",
    "pds_box": 21,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": -40,
    "z": 481.526
  },
  {
    "channel": 300,
    "pd_type": "pmt_coated",
    "pds_box": 20,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 40,
    "z": 481.526
  },
  {
    "channel": 301,
    "pd_type": "pmt_coated",
    "pds_box": 21,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 40,
    "z": 481.526
  },
  {
    "channel": 302,
    "pd_type": "pmt_coated",
    "pds_box": 22,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 95,
    "z": 481.526
  },
  {
    "channel": 303,
    "pd_type": "pmt_coated",
    "pds_box": 23,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 95,
    "z": 481.526
  },
  {
    "channel": 304,
    "pd_type": "pmt_coated",
    "pds_box": 22,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 0,
    "sampling": "",
    "x": -213.4,
    "y": 175,
    "z": 481.526
  },
  {
    "channel": 305,
    "pd_type": "pmt_coated",
    "pds_box": 23,
    "sensible_to_vuv": true,
    "sensible_to_vis": true,
    "tpc": 1,
    "sampling": "",
    "x": 213.4,
    "y": 175,
    "z": 481.526
  },
  {
    "channel": 306,
    "pd_type": "xarapuca_vuv",
    "pds_box": 18,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": -135,
    "z": 489.146
  },
  {
    "channel": 307,
    "pd_type": "xarapuca_vuv",
    "pds_box": 19,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": -135,
    "z": 489.146
  },
  {
    "channel": 308,
    "pd_type": "xarapuca_vuv",
    "pds_box": 20,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 0,
    "z": 489.146
  },
  {
    "channel": 309,
    "pd_type": "xarapuca_vuv",
    "pds_box": 21,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 0,
    "z": 489.146
  },
  {
    "channel": 310,
    "pd_type": "xarapuca_vuv",
    "pds_box": 22,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 0,
    "sampling": "daphne",
    "x": -213.75,
    "y": 135,
    "z": 489.146
  },
  {
    "channel": 311,
    "pd_type": "xarapuca_vuv",
    "pds_box": 23,
    "sensible_to_vuv": true,
    "sensible_to_vis": false,
    "tpc": 1,
    "sampling": "daphne",
    "x": 213.75,
    "y": 135,
    "z": 489.146
  }
]
  ```
</details>


### OPTION 3: General ```.json``` including positions for sensors + Geant4 information

<details>
  <summary>configs/option3.json</summary>
    
  ```json
  {
    "comments":"Position units given in mm",

    "display": true,
    "G4_beam_settings": 
    [
      "/gps/particle opticalphoton",
      "/gps/ene/type Mono",
      "/gps/ene/mono 9.7 eV",
      "/gps/pos/type Plane",
      "/gps/pos/shape Circle",
      "/gps/pos/radius  11.5 mm",
      "/gps/pos/rot1 1 0 0",
      "/gps/pos/rot2 0 1 0",
      "/gps/pos/centre 0 55 98 mm",
      "/gps/direction 0 0 -1",
      "/gps/ang/type iso",
      "/gps/number 1",
      "/run/beamOn 100",
      "/run/initialize"
    ],

    "G4_vis_settings":[
      "/vis/viewer/set/autorefresh/true",
      "/vis/open OGL",
      "/vis/drawVolume",
      "/vis/scene/add/trajectories smooth",
      "/vis/scene/add/eventID",
      "/vis/enable",
      "/vis/viewer/rebuild",
      "/vis/viewer/set/background grey",
      "/vis/scene/add/axes",
      "/vis/scene/endOfEventAction accumulate 1000"
    ],
                  
    "sensors_keys": [
      "XArapuca",
      "SiPM1",
      "SiPM2"
    ],
    "source": {
      "X": 0,
      "Y": 0,
      "Z": 0
    },
    "SiPM1":      {    "X": [ 35, 50, 53]  },
    "SiPM2":      {    "X": [-35, 50, 53]  },
    "SiPM1_tapa": {    "X": [ 36, 50, 53]  },
    "SiPM2_tapa": {    "X": [-36, 50, 53]  },
    "XArapuca":      {    "X": [0,-2, 53]  },
    "SiPM_rot": [0.0 , 0.0 , 0.0]
  }
  ```

</details>

<details>
  <summary>configs/option3_prod.json</summary>

  ```json
  {
    "comments":"Position units given in mm",

    "display": false,
    "G4_beam_settings": 
    [
      "/gps/particle opticalphoton",
      "/gps/ene/type Mono",
      "/gps/ene/mono 9.7 eV",
      "/gps/pos/type Plane",
      "/gps/pos/shape Circle",
      "/gps/pos/radius  11.5 mm",
      "/gps/pos/rot1 1 0 0",
      "/gps/pos/rot2 0 1 0",
      "/gps/pos/centre 0 55 98 mm",
      "/gps/direction 0 0 -1",
      "/gps/ang/type iso",
      "/gps/number 1",
      "/run/beamOn 20000000",
      "/run/initialize"
    ],

    "G4_vis_settings":[
      "/vis/viewer/set/autorefresh/true",
      "/vis/open OGL",
      "/vis/drawVolume",
      "/vis/scene/add/trajectories smooth",
      "/vis/scene/add/eventID",
      "/vis/enable",
      "/vis/viewer/rebuild",
      "/vis/viewer/set/background grey",
      "/vis/scene/add/axes",
      "/vis/scene/endOfEventAction accumulate 1000"
    ],
                  
    "sensors_keys": [
      "XArapuca",
      "SiPM1",
      "SiPM2"
    ],
    "source": {
      "X": 0,
      "Y": 0,
      "Z": 0
    },
    "SiPM1":      {    "X": [ 35, 50, 53]  },
    "SiPM2":      {    "X": [-35, 50, 53]  },
    "SiPM1_tapa": {    "X": [ 36, 50, 53]  },
    "SiPM2_tapa": {    "X": [-36, 50, 53]  },
    "SiPM_rot": [0.0 , 0.0 , 0.0]
  }
  ```
</details>


-----------------------------------------------------------------------------------------------------------------------------------------------------

## Visualizing in ROOT

```bash
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
```

### GetEntries

```bash
  >> Photons->Draw("fZ:fX>>SC","-15<fZ && fZ<15 && -15<fX && fX<15", "colZ")
  >> Photons->Draw("fY:fX>>SiPM1","fX>15 && fY<15", "colZ")
  >> Photons->Draw("fY:fX>>SiPM2","fX<-15 && fY<15", "colZ")

  >> TH1F *PMT= (TH1F*)gDirectory->Get("PMT")
  >> PMT->GetEntries()
```

## LICENSE

[MIT](https://choosealicense.com/licenses/mit/)

## Authors (alphabetical order, please insert your name here if you contribute to this project)

* [**Alvárez-Garrote, Rodrigo**](https://github.com/rodralva)
* [**Pérez-Molina, Laura**](https://github.com/LauPM)
* [**Manthey Corchado, Sergio**](https://github.com/mantheys)
