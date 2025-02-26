from __init__ import *

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--directory', default="/pc/choozdsk01/DATA/CAJITA/supercell_v2/", help="Folder path")
parser.add_argument('-s', '--sensors', default=["Arapuca","SiPM1","SiPM2"], nargs='+', help="List of sensor names")
parser.add_argument('-f', '--file', default=["supercell_box5_SiPM2.5.root"], nargs='+', help="File name")
parser.add_argument('--save', action='store_true', help="Save figures")
parser.add_argument('--debug', action='store_true', help="Enable debug mode")
args = parser.parse_args()

folder = args.directory
sensors = args.sensors
my_files = args.file
save = args.save
debug = args.debug

all_data = []
for my_file in my_files: all_data.append(extract_branches(folder=folder, root_file=my_file, sensors=sensors, debug=debug))

# Add unitary normal vectors to each sensor
sensors_info=dict()
for sensor in sensors:
    sensors_info[sensor]={}
    for my_file in my_files:
        sensors_info[sensor][my_file]={}
        if (sensor=="Arapuca"): sensors_info[sensor][my_file]=np.array([ 0, 0, 1])
        if (sensor=="SiPM1"):   sensors_info[sensor][my_file]=np.array([ 0, 1, 0])
        if (sensor=="SiPM2"):   sensors_info[sensor][my_file]=np.array([ 0,-1, 0])

# sensors_info["Arapuca"]["Area"] = 143.75*143.75 # mm²
sensors_info["Arapuca"]["Area"] = 100*80 # mm²
sensors_info["SiPM1"]["Area"]   = 6*6    # mm²
sensors_info["SiPM2"]["Area"]   = 6*6    # mm²

for d,data in enumerate(all_data): compute_real_angles(my_data=data,sensors_info=sensors_info,debug=debug)

for coordinate in ["X","Y","Z"]:
    for d,my_data in enumerate(all_data):
            fig_ang = plot_variable_distributions(my_data,coordinate,stats=(True,False),probability=False,percentile=(0,1),log=(False,True),dpi=100,save=save,debug=debug)
            # plt.show()

for d,my_data in enumerate(all_data):
    fig_ang = plot_variable_distributions(my_data,"FixedIncidenceAngleDegree",stats=(True,False),probability=True,percentile=(0,1),log=(False,False),dpi=100,save=save,debug=debug)
    # plt.show()

for d,my_data in enumerate(all_data):
    fig_hit = plot_variable_distributions(my_data,"AccumHits",stats=(True,False),probability=True,dpi=100,save=save,debug=debug)
    # fig_hit.show()