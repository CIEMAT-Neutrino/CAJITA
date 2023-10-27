import uproot
import numpy as np
import matplotlib.pyplot as plt

def extract_branches(folder,root_file,sensors,branches={"Photons":["X","Y","Z","Phi","Theta","Sensor"],"Hits":["Sensor","AccumHits"]},debug=False):
    opened_file = uproot.open(folder+root_file)
    branch_dict = dict.fromkeys(branches.keys())
    my_data = dict.fromkeys([root_file]); my_data[root_file] = dict.fromkeys(sensors)
    for k,key in enumerate(branch_dict):
        branch_dict[key] = {}
        for branch in branches[key]: 
            branch_dict[key][branch] = opened_file[key][branch].array().to_numpy();
    for s,sensor in enumerate(sensors):
        my_data[root_file][sensor] = {}
        for k,key in enumerate(branch_dict):
            for branch in branches[key]: 
                my_data[root_file][sensor][branch] = branch_dict[key][branch][branch_dict[key]["Sensor"]==s]
    return my_data

def compute_real_angles(my_data,sensors_info,debug=False):
    for geo_file in my_data.keys():
        for sensor in my_data[geo_file].keys():
            phi   = my_data[geo_file][sensor]["Phi"]
            theta = my_data[geo_file][sensor]["Theta"]
            
            abs_vec = np.array([ np.sin(theta)*np.cos(phi), np.sin(theta)*np.sin(phi), np.cos(theta)])
            norm    = np.sum(  (abs_vec.T*sensors_info[sensor][geo_file]),axis=1 )
            inc_ang = np.arccos(norm)
            
            my_data[geo_file][sensor]["vector"]        = abs_vec
            my_data[geo_file][sensor]["norm"]          = norm
            my_data[geo_file][sensor]["inc_ang"]       = inc_ang
            if   ((np.mean(inc_ang)>np.pi/2)): my_data[geo_file][sensor]["inc_ang_fixed"] = np.pi - inc_ang
            else                             : my_data[geo_file][sensor]["inc_ang_fixed"] = inc_ang
    if debug: print(sensors_info)

def plot_angular_distributions(my_data,dpi=50):
    fig = plt.figure(dpi=dpi)
    for my_file in my_data.keys():
        plt.title(my_file.replace(".root",""))
        for sensor in my_data[my_file]:
            plt.hist (my_data[my_file][sensor]["inc_ang_fixed"]*180/np.pi,45,[0,90],alpha=.5,density=True,label=sensor)
        plt.xlabel("Incident angle (°)")
        plt.legend()
        plt.show()

def plot_photon_density(my_data,sensors_info,dpi=50):
    axes   = ["X","Y","Z"]
    labels = ["X axis", "Y axis", "Z axis"]
    fig = plt.figure(dpi=dpi)
    for my_file in my_data:
        for sensor in my_data[my_file]: 
            plt.figure()
            plt.title(my_file.replace(".root","_")+sensor)
            axes2plot = list((np.abs(sensors_info[sensor][my_file])-1)*(-1))
            axes2plot = np.where(np.array(axes2plot)==1)[0]
            plt.hist2d(my_data[my_file][sensor][axes[axes2plot[0]]],my_data[my_file][sensor][axes[axes2plot[1]]])
            plt.xlabel(labels[axes2plot[0]])
            plt.ylabel(labels[axes2plot[1]])
            plt.colorbar()
            plt.show()

def plot_acumhits(my_data,dpi=50,bins=[100,35,35],semilogy=False):
    for my_file in my_data:
        means = [];sensors = []
        for s,sensor in enumerate(my_data[my_file]): mean = np.mean(my_data[my_file][sensor]["AccumHits"]); means.append(mean); sensors.append(sensor)
        
        if np.any(np.array(means/means[0]) < 0.1) or np.any(np.array(means/means[0]) > 10): 
            fig, axs = plt.subplots(1,2,sharey=True,dpi=dpi); 
            axs[1].hist(my_data[my_file][sensors[0]]["AccumHits"], bins[0], alpha=0.5, histtype="step", color="C"+str(0),label=sensors[0]+" (Mean %0.2f)"%means[0], density=True)
            axs[1].set_xlim(means[0]-means[0]*0.8,means[0]+means[0]*1.1)
            if semilogy: axs[1].semilogy()
            for diff in list(np.where(np.array(means/means[0]) < 0.1)[0]) + list(np.where(np.array(means/means[0]) > 10)[0]):
                axs[0].hist(my_data[my_file][sensors[diff]]["AccumHits"], bins[diff], alpha=0.5, histtype="step", color="C"+str(diff),label=sensors[diff]+" (Mean %0.2f)"%means[diff], density=True)
                axs[0].set_xlim(means[diff]-means[diff]*0.8,means[diff]+means[diff]*1.6)
                if semilogy: axs[0].semilogy()
            axs[1].set_xlabel("Accumulated hits")
            axs[0].set_xlabel("Accumulated hits")
            axs[0].set_ylabel("Counts")
            
        else: 
            fig, axs = plt.subplots(1,1,sharey=True,dpi=dpi)
            for s,sensor in enumerate(my_data[my_file]):
                axs.hist(my_data[my_file][sensor]["AccumHits"], bins[s], alpha=0.5, histtype="step", color="C"+str(s),label=sensor+" (Mean %0.2f)"%means[s], density=True)
                plt.xlabel("Accumulated hits")
                plt.ylabel("Counts")
                plt.grid()

        fig.suptitle(my_file.replace(".root",""))
        fig.legend()
        fig.show()