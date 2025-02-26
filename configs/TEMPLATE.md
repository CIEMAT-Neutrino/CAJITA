{
  ## No comments are accepted in json files. Use this as reference.
  "comments": "Position units given in mm",

  ## True if you want a pop-up window with you geometry to appear. Use only for debug, set false for production!!
  "display": true,
  
  ## Typical G4 parameters. Check the manual https://geant4.web.cern.ch/documentation/dev/prm_html/PhysicsReferenceManual/
  ## To be changed: the energy is setted for UV photons (9.7eV); the source position and shape fits the alphas source of 23mm diameter at IR02, change the centre and rotation angles; number are the number of photons simulated per event and beamOn is the number of events to be simulated. To check the geometry que do not need big amount of them.
  "G4_beam_settings": [
    "/gps/particle opticalphoton",
    "/gps/ene/type Mono",
    "/gps/ene/mono 9.7 eV",
    "/gps/pos/type Plane",
    "/gps/pos/shape Circle",
    "/gps/pos/centre 0 72.5 0 mm",
    "/gps/pos/radius  11 mm",
    "/gps/pos/rot1 0 0 90",
    "/gps/pos/rot2 90 0 0",
    "/gps/ang/type iso",
    "/gps/number 100",
    "/run/beamOn 1",
    "/run/initialize"
  ],

  ## Visualization settings
  "G4_vis_settings": [
    "/vis/viewer/set/autorefresh/true",
    "/vis/open OGL",
    "/vis/drawVolume",
    "/vis/scene/add/trajectories smooth",
    "/vis/scene/add/eventID",
    "/vis/scene/add/surfaces",
    "/vis/scene/add/axes 0 0 0 10 cm",
    "/vis/enable",
    "/vis/viewer/rebuild",
    "/vis/viewer/set/background grey",
    "/vis/scene/endOfEventAction accumulate 10"
  ],

  ## Labels for the optical sensors to be simulated in the box
  "sensors_keys": [
    "XArapuca",
    "SiPM1",
    "SiPM2"
  ],
  ## Position of the source
  "source": {
    "X": 0,
    "Y": 0,
    "Z": 0
  },

  ## From this point the geometry of each campaign is different. The following keys need to be called from geometries/your_geometry.cc and .hh. As an example in line 113 of megacell_v3.cc you find the import for big_cajita and so on.
  ## This way we do not need to compile at every change we want to test of the geometry, just re-run with a new value in the json file. Moreover, productions can be made just changing parameters from a bash script without recompiling.
  "big_cajita": {
    "outter_dim": [
      67.0,
      37.425,
      67.0
    ],
    "inner_dim": [
      66.7,
      37.125,
      66.7
    ],
    "comment": "Rotation and position should be left as they are",
    "rot": [
      0.0,
      0.0,
      0.0
    ]
  },
  "arapuca": {
    "dim": [
      71.875,
      1e-6,
      71.875
    ],
    "pos": [
      0.0,
      0.5,
      0.0
    ],
    "comment": "Rotation should follow the same as big_cajita",
    "rot": [
      0.0,
      0.0,
      0.0
    ]
  },
  "sipms": {
    "dim": [
      1e-6,
      3.0,
      3.0
    ],
    "pos1": [
      28.54,
      66.58,
      0
    ],
    "pos2": [
      -28.54,
      66.58,
      0
    ],
    "rot": [
      0.0,
      0.0,
      0.0
    ]
  },
  "tower_sipms": {
    "comment1": "X & Z coordinates constrained to SiPM1",
    "dim": [
      1e-6,
      9.215,
      1e-6
    ],
    "buffer": [
      5.3
    ],
    "comment2": "Position & rotation constrained by big_cajita"
  },
  "alpha_source": {
    "dim": [
      0,
      12.5,
      0.85
    ],
    "angle": [
      0,
      360
    ],
    "pos": [
      0.0,
      0.0,
      0.0
    ],
    "rot": [
      90.0,
      90.0,
      0.0
    ]
  },
  "alpha_support": {
    "dim": [
      12.75,
      14.5,
      0.85
    ],
    "angle": [
      0,
      360
    ],
    "comment": "Y coordinate constrained to top of big_cajita",
    "pos": [
      0.0,
      0.0,
      0.0
    ],
    "rot": [
      0.0,
      0.0,
      0.0
    ]
  }
}