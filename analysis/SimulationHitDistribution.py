from __init__ import *

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--directory', default="../data/supercell_v2", help="Folder path")
parser.add_argument('-s', '--sensors', default=["Arapuca","SiPM1","SiPM2"], nargs='+', help="List of sensor names")
parser.add_argument('-f', '--file', default="supercell_v2.root", help="File name")
parser.add_argument('--save', action='store_true', help="Save figures")
parser.add_argument('--debug', action='store_true', help="Enable debug mode")
args = parser.parse_args()

# Extract branches
print("Extracting branches...")
my_data = extract_branches(folder=args.directory, root_file=args.file, sensors=args.sensors, debug=args.debug)

filename = list(my_data.keys())[0]
data = {
  "SiPM1": my_data[filename]["SiPM1"]["AccumHits"],
  "SiPM2": my_data[filename]["SiPM2"]["AccumHits"],
  "COMBI": my_data[filename]["SiPM1"]["AccumHits"] + my_data[filename]["SiPM2"]["AccumHits"],
  "XA": my_data[filename]["Arapuca"]["AccumHits"]
}

fig = make_subplots(rows=2, cols=1, subplot_titles=("SiPM1 + SiPM2", "Arapuca"), shared_xaxes=False, shared_yaxes=True, vertical_spacing=0.1)

for (idx, (key, value)), xbins in zip(enumerate(data.items()), [np.linspace(0, 2000, 100), np.linspace(0, 2000, 100), np.linspace(0, 2000, 100), np.linspace(0, 2000, 100)]):  
  if idx == 3:
    bins, edges = np.histogram(value, bins=10*xbins, density=True)
    bins = bins/np.max(bins)
    fig.add_trace(go.Bar(x=edges, y=bins, name=key, opacity=0.8), row=2, col=1)
  
  else:
    bins, edges = np.histogram(value, bins=xbins, density=True)
    bins = bins/np.max(bins)
    fig.add_trace(go.Bar(x=edges, y=bins, name=key, opacity=0.8), row=1, col=1)

fig.update_layout(title_text=filename,
  barmode="overlay", bargap=0, template="presentation", height=800, width=800, showlegend=True,
  xaxis2_title_text="#Photons",
  legend_title_text="Sensor",
  legend=dict(x=.01, y=.01, xanchor="left", yanchor="bottom", orientation="v"))

fig.update_xaxes(showline=True, showgrid=True, mirror=True)
fig.update_yaxes(title_text="Norm (a.u.)", ticks="outside", showline=True, showgrid=True, mirror=True)
fig.show()
# Save figure
if args.save:
  print(f"Saving figure to ../results/images/{filename.split('.root')[0]}_AccumHitsDistribution.png")
  if not os.path.exists('../results/images/'):
    os.makedirs('../results/images/')
  fig.write_image(f"../results/images/{filename.split('.root')[0]}_AccumHitsDistribution.png", width=800, height=800, scale=2)