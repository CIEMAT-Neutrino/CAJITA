Production folder used for defining the final geometry of each setup.
This files can be removed once the final geometry is decided and copied to configs (to use it later)

RUN script: sh production/production.sh
time ./build/box1 config_prod.json /pc/choozdsk01/palomare/GEANT4/your_folder/output_name.root

You will need to configure it with the production files yo want to test and the path to allocate the outputs.root
Be careful AFS space is limited, if you require more space you can try in pc/choozdsk01/palomare/GEANT4/your_folder of /pnfs/ciemat.es/data/neutrinos/your_folder