import uproot,os,sys
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
            norm    = np.sum((abs_vec.T*sensors_info[sensor][geo_file]), axis=1)
            inc_ang = np.arccos(norm)
            
            my_data[geo_file][sensor]["Vector"]  = abs_vec
            my_data[geo_file][sensor]["Norm"]    = norm
            my_data[geo_file][sensor]["IncidenceAngle"] = inc_ang
            if   ((np.mean(inc_ang)>np.pi/2)): my_data[geo_file][sensor]["FixedIncidenceAngle"] = np.pi - inc_ang
            else                             : my_data[geo_file][sensor]["FixedIncidenceAngle"] = inc_ang
            my_data[geo_file][sensor]["FixedIncidenceAngleDegree"] = my_data[geo_file][sensor]["FixedIncidenceAngle"]*180/np.pi
    if debug: print(sensors_info)

def plot_variable_distributions(my_data,variable,stats=(False,False),bins=100,probability=False,density=False,percentile=(0.01,0.99),log=(False,False),dpi=50,save=False,debug=False):
    fig = plt.figure(dpi=dpi)
    for my_file in my_data.keys():
        plt.title(my_file.replace(".root",""))
        for sensor in my_data[my_file]:
            # Check if variable exists
            try: my_data[my_file][sensor][variable]
            except KeyError:
                print("ERROR: Variable %s not found in %s"%(variable,my_file))
                print("Available variables are: %s"%my_data[my_file][sensor].keys())
                raise KeyError
            # Compute percentile
            hist_data = np.sort(my_data[my_file][sensor][variable])
            if debug: print("%s: %i"%(variable,len(my_data[my_file][sensor][variable])))
            selected_data = hist_data[int(percentile[0]*len(hist_data)):int(percentile[1]*len(hist_data))]
            if debug: print("%s after percentile cut: %i"%(variable,len(selected_data)))
            # Compute histogram
            h, bins = np.histogram(selected_data,bins,density=density)
            if probability and not density: h = h/np.sum(h)
            bin_centers = (bins[1:]+bins[:-1])/2
            # Plot
            plt.bar(bin_centers,h,width=(bins[1]-bins[0]),alpha=.5,label=sensor)
            # Compute stats
            mean = np.mean(selected_data)
            std  = np.std(selected_data)
            if stats[0]: print("Mean %s: %0.2f +- %0.2f"%(sensor,mean,std))
            if stats[1]:
                plt.axvline(mean,color="green",label="Mean %0.2f"%mean)
                plt.axvline(mean+std,linestyle="--",color="green",label="Std %0.2f"%std)
                plt.axvline(mean-std,linestyle="--",color="green",label="Std %0.2f"%std)
        # Plot settings
        plt.xlabel(variable)
        if density: plt.ylabel("Density")
        elif probability: plt.ylabel("Probability")
        else      : plt.ylabel("Counts")
        if log[0]: plt.semilogx()
        if log[1]: plt.semilogy()
        plt.legend()
        plt.grid()
        # Save figure
        # Check if folder exists
        if not os.path.isdir("../results/"): os.mkdir("../results/")
        if save: plt.savefig("../results/"+my_file.split('.root')[0]+'_'+variable+".png")
    return fig

def plot_photon_density(my_data,sensors_info,bins=100,density=False,dpi=50,debug=False):
    axes   = ["X","Y","Z"]
    labels = ["X axis", "Y axis", "Z axis"]
    fig, axs = plt.subplots(len(my_data),len(sensors_info.keys()),dpi=dpi,)
    for idx,my_file in enumerate(my_data):
        for jdx,sensor in enumerate(my_data[my_file]): 
            axes2plot = list((np.abs(sensors_info[sensor][my_file])-1)*(-1))
            axes2plot = np.where(np.array(axes2plot)==1)[0]
            h = axs[jdx].hist2d(my_data[my_file][sensor][axes[axes2plot[0]]],my_data[my_file][sensor][axes[axes2plot[1]]],bins=bins,density=density)
            axs[jdx].set_xlabel(labels[axes2plot[0]])
            axs[jdx].set_ylabel(labels[axes2plot[1]])
            if idx==0: axs[jdx].set_title(sensor)
    fig.colorbar(h[3], ax=axs.ravel().tolist())
    return fig  

def plot_acumhits(my_data,dpi=50,bins=[100,35,35],semilogy=False,debug=False):
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
            handles, labels = axs[0].get_legend_handles_labels()
            fig.legend(handles, labels, loc='upper center')
            
        else: 
            fig, axs = plt.subplots(1,1,sharey=True,dpi=dpi)
            for s,sensor in enumerate(my_data[my_file]):
                axs.hist(my_data[my_file][sensor]["AccumHits"], bins[s], alpha=0.5, histtype="step", color="C"+str(s),label=sensor+" (Mean %0.2f)"%means[s], density=True)
                plt.xlabel("Accumulated hits")
                plt.ylabel("Counts")
                if semilogy: plt.semilogy()
            fig.legend()

        fig.suptitle(my_file.replace(".root",""))
        fig.show()