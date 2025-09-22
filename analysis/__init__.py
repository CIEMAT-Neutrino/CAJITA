import uproot,os,sys
import argparse
import numpy             as np
import pandas            as pd
import awkward           as ak
import plotly.express    as px
import matplotlib.pyplot as plt
import plotly.graph_objs as go
from plotly.subplots import make_subplots

# Set matplotlib font size
plt.rcParams.update({'font.size': 14})

def extract_branches(folder, root_file,sensors, branches={"Photons":["X","Y","Z","Phi","Theta","Sensor"],"Hits":["Sensor","AccumHits"]}, debug=False):
    if not os.path.isdir(folder):
        print(f"ERROR: Folder {folder} does not exist.")
        sys.exit(1)
    if folder.endswith("/"): folder = folder[:-1]  # Remove trailing slash if present
    if not os.path.isfile(f"{folder}/{root_file}"):
        print(f"ERROR: File {root_file} does not exist in folder {folder}.")
        sys.exit(1)
    opened_file = uproot.open(f"{folder}/{root_file}")
    branch_dict = dict.fromkeys(branches.keys())
    my_data = dict.fromkeys([root_file]); my_data[root_file] = dict.fromkeys(sensors)
    for k,key in enumerate(branch_dict):
        branch_dict[key] = {}
        for branch in branches[key]: 
            if debug: print(f"Extracting branch {branch} from {key} in {root_file}")
            if key not in opened_file:
                print(f"ERROR: Branch {key} not found in file {root_file}. Available branches: {opened_file.keys()}")
                raise KeyError
            if debug:
                # Print type of the branch
                print(f"\tBranch {branch} type: {type(opened_file[key][branch])}")
            branch_dict[key][branch] = ak.to_numpy(opened_file[key][branch].array())
            if debug:
                # Print shape of the branch
                print(f"\tBranch {branch} saved as: {type(branch_dict[key][branch])}")

    for s,sensor in enumerate(sensors):
        my_data[root_file][sensor] = {}
        for k,key in enumerate(branch_dict):
            for branch in branches[key]: 
                my_data[root_file][sensor][branch] = branch_dict[key][branch][branch_dict[key]["Sensor"]==s]
    return my_data

def compute_real_angles(my_data, sensors_info, debug:bool=False):
    for geo_file in my_data.keys():
        for sensor in my_data[geo_file].keys():
            phi   = my_data[geo_file][sensor]["Phi"]
            theta = my_data[geo_file][sensor]["Theta"]
            
            abs_vec = np.array([ np.sin(theta)*np.cos(phi), np.sin(theta)*np.sin(phi), np.cos(theta)])
            norm    = np.sum((abs_vec.T*sensors_info[sensor][geo_file]), axis=1)
            inc_ang = np.arccos(norm)
            
            my_data[geo_file][sensor]["Vector"]  = abs_vec
            my_data[geo_file][sensor]["Norm"]    = norm
            my_data[geo_file][sensor]["IncidenceAngle"] = inc_ang
            if   ((np.mean(inc_ang)>np.pi/2)): my_data[geo_file][sensor]["FixedIncidenceAngle"] = np.pi - inc_ang
            else                             : my_data[geo_file][sensor]["FixedIncidenceAngle"] = inc_ang
            my_data[geo_file][sensor]["FixedIncidenceAngleDegree"] = my_data[geo_file][sensor]["FixedIncidenceAngle"]*180/np.pi
    if debug: print(sensors_info)

def plot_variable_distributions(my_data,variable,stats=(False,False),bins=50,probability=False,density=False,limits=(1,None),percentile=(0.01,0.99),log=(False,False),figsize=(10,10),dpi=300,save=False,debug=False):
    fig = plt.figure(dpi=dpi)
    for my_file in my_data.keys():
        plt.title(my_file.replace(".root",""))
        if not os.path.isdir("../results/stats/"): os.mkdir("../results/stats/")
        with open("../results/stats/"+my_file.split('.root')[0]+'_'+variable+"_stats.txt","w") as f:
            if limits[0] is not None:
                plt.axvline(limits[0],color="grey",linestyle="--",label="Min limit %.0f"%limits[0])
            for sensor in my_data[my_file]:
                # Check if variable exists
                try: my_data[my_file][sensor][variable]
                except KeyError:
                    print("ERROR: Variable %s not found in %s"%(variable,my_file))
                    print("Available variables are: %s"%my_data[my_file][sensor].keys())
                    raise KeyError
                # Compute percentile
                hist_data = np.sort(my_data[my_file][sensor][variable])
                if limits[0] is not None: hist_data = hist_data[hist_data>=limits[0]]
                if limits[1] is not None: hist_data = hist_data[hist_data<=limits[1]]
                if debug: print("%s: %i"%(variable,len(my_data[my_file][sensor][variable])))
                selected_data = hist_data[int(percentile[0]*len(hist_data)):int(percentile[1]*len(hist_data))]
                if debug: print("%s after percentile cut: %i"%(variable,len(selected_data)))
                # Compute histogram
                if variable == "AccumHits":
                    h, bin_edges = np.histogram(selected_data,np.arange(0, np.max(selected_data)+bins, bins),density=density)
                else:
                    h, bin_edges = np.histogram(selected_data,bins,density=density)
                if probability and not density:
                    h = h/np.sum(h)

                mean = np.mean(selected_data)
                std  = np.std(selected_data)
                median = np.median(selected_data)
                bin_centers = 0.5*(bin_edges[1:]+bin_edges[:-1])
                plt.hist(bin_centers, bins=bin_edges, weights=h, histtype="step", color="C"+str(list(my_data[my_file].keys()).index(sensor)), label=sensor+" (%.0f +- %.0f)"%(mean, std), lw=1.5)    
                
                if stats[0]:
                    print("Mean %s: %.0f +- %.0f"%(sensor,mean,std))
                    f.write("Mean %s: %.0f +- %.0f\n"%(sensor,mean,std))

                if stats[1]:
                    plt.axvline(mean,color="green",label="Mean %.0f"%mean)
                    plt.axvline(mean+std,linestyle="--",color="green",label="Std %.0f"%std)
                    plt.axvline(mean-std,linestyle="--",color="green",label="Std %.0f"%std)
        
        # Plot settings
        plt.xlabel(variable)
        if density: plt.ylabel("Density")
        elif probability: plt.ylabel("Probability")
        else: plt.ylabel("Counts")
        if log[0]: plt.semilogx()
        if log[1]: plt.semilogy()
        # Figuresize
        if figsize[0] is not None and figsize[1] is not None:
            fig.set_figwidth(figsize[0])
            fig.set_figheight(figsize[1])
        plt.legend()
        plt.grid()
        # Save figure
        if not os.path.isdir("../results/images/"): os.mkdir("../results/images/")
        if save: 
            plt.savefig("../results/images/"+my_file.split('.root')[0]+'_'+variable+".png", bbox_inches='tight', dpi=dpi)
            if debug: print("Saving figure to ../results/images/"+my_file.split('.root')[0]+'_'+variable+".png")
    return fig

def plot_photon_density(my_data, surface, bins=100, density=False, figsize=(None,None), dpi=50, save=False, debug=False):
    fig, axs = plt.subplots(len(my_data),len(surface.keys()),dpi=dpi,)
    for idx,my_file in enumerate(my_data):
        for jdx,sensor in enumerate(surface.keys()): 
            try:
                my_data[my_file][sensor]
            except KeyError:
                print("ERROR: Sensor %s not found in surface dictionary"%sensor)
                print("Available sensors are: %s"%surface.keys())
                raise KeyError
            x_axis = surface[sensor][0]
            y_axis = surface[sensor][1]
            h = axs[jdx].hist2d(my_data[my_file][sensor][x_axis],my_data[my_file][sensor][y_axis],bins=bins,density=density)
            axs[jdx].set_xlabel(x_axis+" [mm]")
            axs[jdx].set_ylabel(y_axis+" [mm]")
            if idx==0: axs[jdx].set_title(sensor)
    fig.colorbar(h[3], ax=axs.ravel().tolist())
    fig.set_figwidth(figsize[0])
    fig.set_figheight(figsize[1])
    if not os.path.isdir("../results/images/"): os.mkdir("../results/images/")
    if save:
        plt.savefig("../results/images/"+my_file.split('.root')[0]+"_PhotonDensity.png")
        if debug: print("Saving figure to ../results/images/"+my_file.split('.root')[0]+"_PhotonDensity.png")
    return fig  

def plot_acumhits(my_data, bins=[100,35,35], semilogy=False, dpi=50, debug=False):
    for my_file in my_data:
        means = [];sensors = []
        for s,sensor in enumerate(my_data[my_file]): mean = np.mean(my_data[my_file][sensor]["AccumHits"]); means.append(mean); sensors.append(sensor)
        
        if np.any(np.array(means/means[0]) < 0.1) or np.any(np.array(means/means[0]) > 10): 
            fig, axs = plt.subplots(1,2,sharey=True,dpi=dpi); 
            axs[1].hist(my_data[my_file][sensors[0]]["AccumHits"], bins[0], alpha=0.5, histtype="step", color="C"+str(0),label=sensors[0]+" (%0.2f)"%means[0], density=True)
            axs[1].set_xlim(means[0]-means[0]*0.8,means[0]+means[0]*1.1)
            if semilogy: axs[1].semilogy()
            for diff in list(np.where(np.array(means/means[0]) < 0.1)[0]) + list(np.where(np.array(means/means[0]) > 10)[0]):
                axs[0].hist(my_data[my_file][sensors[diff]]["AccumHits"], bins[diff], alpha=0.5, histtype="step", color="C"+str(diff),label=sensors[diff]+" (%0.2f)"%means[diff], density=True)
                axs[0].set_xlim(means[diff]-means[diff]*0.8,means[diff]+means[diff]*1.6)
                if semilogy: axs[0].semilogy()
            axs[1].set_xlabel("Accumulated hits")
            axs[0].set_xlabel("Accumulated hits")
            axs[0].set_ylabel("Counts")
            handles, labels = axs[0].get_legend_handles_labels()
            fig.legend(handles, labels, loc='upper center')
            
        else: 
            fig, axs = plt.subplots(1,1,sharey=True,dpi=dpi)
            for s,sensor in enumerate(my_data[my_file]):
                axs.hist(my_data[my_file][sensor]["AccumHits"], bins[s], alpha=0.5, histtype="step", color="C"+str(s),label=sensor+" (%0.2f)"%means[s], density=True)
                plt.xlabel("Accumulated hits")
                plt.ylabel("Counts")
                if semilogy: plt.semilogy()
            fig.legend()

        fig.suptitle(my_file.replace(".root",""))
        fig.show()