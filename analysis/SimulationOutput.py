from __init__ import *

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--directory', default="../data/supercell_v2", help="Folder path")
parser.add_argument('-s', '--sensors', default=["Arapuca","SiPM1","SiPM2"], nargs='+', help="List of sensor names")
parser.add_argument('-f', '--file', default=["supercell_v2.root"], nargs='+', help="File name")
parser.add_argument('--save', action='store_true', help="Save figures")
parser.add_argument('--debug', action='store_true', help="Enable debug mode")
args = parser.parse_args()

# Print arguments
print("Arguments:")
for arg in vars(args): print(f"  {arg}: {getattr(args, arg)}")

all_data = []
for my_file in args.file: all_data.append(extract_branches(folder=args.directory, root_file=my_file, sensors=args.sensors, debug=args.debug))

# Add unitary normal vectors to each sensor
sensors_info=dict()
for sensor in args.sensors:
    sensors_info[sensor]={}
    for my_file in args.file:
        sensors_info[sensor][my_file]={}
        if (sensor=="Arapuca"): sensors_info[sensor][my_file]=np.array([ 0, 0, 1])
        elif (sensor=="RefSiPM"): sensors_info[sensor][my_file]=np.array([ 0, 0, 1])
        elif (sensor=="SiPM1"):   sensors_info[sensor][my_file]=np.array([ 0, 1, 0])
        elif (sensor=="SiPM2"):   sensors_info[sensor][my_file]=np.array([ 0,-1, 0])
        else:
            print(f"Warning: Sensor {sensor} not recognized. Please add its normal vector.")
            sensors_info[sensor][my_file]=np.array([ 0, 0, 1])
            
# Print sensors info
if args.debug:
    print("Sensors info:")
    for sensor in sensors_info:
        print(f"  {sensor}:")
        for my_file in sensors_info[sensor]:
            print(f"    {my_file}: {sensors_info[sensor][my_file]}")

# sensors_info["Arapuca"]["Area"] = 143.75*143.75 # mm²
if "Arapuca" in sensors_info: sensors_info["Arapuca"]["Area"] = 100*80 # mm²
sensors_info["RefSiPM"]["Area"] = 6*6    # mm²
sensors_info["SiPM1"]["Area"]   = 6*6    # mm²
sensors_info["SiPM2"]["Area"]   = 6*6    # mm²

for d,data in enumerate(all_data): compute_real_angles(my_data=data,sensors_info=sensors_info,debug=args.debug)

for variable, bins, logx, logy in zip(["X","Y","Z", "FixedIncidenceAngleDegree", "AccumHits"], [20,20,100,100,50], [False, False, False, False, False], [False, False, False, False, True]):
    for d, my_data in enumerate(all_data):
        if variable == "AccumHits":
            fig_ang = plot_variable_distributions(my_data,variable,stats=(True,False),bins=bins,probability=True,limits=(500,None),percentile=(0,1),log=(logx,logy),save=args.save,debug=args.debug)
        else:
            fig_ang = plot_variable_distributions(my_data,variable,stats=(True,False),bins=bins,probability=True,limits=(None,None),percentile=(0,1),log=(logx,logy),save=args.save,debug=args.debug)