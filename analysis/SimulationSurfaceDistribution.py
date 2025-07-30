from __init__ import *

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--directory', default="../data/supercell_v2", help="Folder path")
parser.add_argument('-s', '--sensors', default=["Arapuca","SiPM1","SiPM2"], nargs='+', help="List of sensor names")
parser.add_argument('-f', '--file', default=["supercell_v2.root"], nargs='+', help="File name")
parser.add_argument('--save', action='store_true', help="Save figures")
parser.add_argument('--debug', action='store_true', help="Enable debug mode")
args = parser.parse_args()


all_data = []
for my_file in args.file: all_data.append(extract_branches(folder=args.directory, root_file=my_file, sensors=args.sensors, debug=args.debug))

surface = {"Arapuca": ["Z","X"], "SiPM1": ["Z","Y"], "SiPM2": ["Z","Y"]}
# surface = {"Arapuca": ["X","Z"]}
for d,my_data in enumerate(all_data): 
    fig = plot_photon_density(my_data,surface,density=True,bins=50,figsize=(20,5),dpi=150,save=args.save,debug=args.debug)

# Make font size bigger for matplotlib plots
# plt.rcParams.update({'font.size': 18})
# fig.show()
# Save figure
# fig.savefig(f"../results/images/{my_file.split('.root')[0]}_SurfaceDistribution.png",dpi=100)