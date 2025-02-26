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

surface = {"Arapuca": ["Z","X"], "SiPM1": ["Z","Y"], "SiPM2": ["Z","Y"]}
# surface = {"Arapuca": ["X","Z"]}
for d,my_data in enumerate(all_data): 
    fig = plot_photon_density(my_data,surface,density=True,bins=50,figsize=(20,5),dpi=150,save=save,debug=debug)

# Make font size bigger for matplotlib plots
# plt.rcParams.update({'font.size': 18})
# fig.show()
# Save figure
# fig.savefig(f"../results/images/{my_file.split('.root')[0]}_SurfaceDistribution.png",dpi=100)