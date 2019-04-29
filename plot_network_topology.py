import json
import numpy
import matplotlib
matplotlib.use('Qt5Agg')


import matplotlib.pyplot as plt
from matplotlib.offsetbox import OffsetImage, AnnotationBbox
import matplotlib.lines as mlines

plt.rcParams.update({'font.size': 12,'lines.markersize': 8})

def getImage(path,zoom=0.1):
    return OffsetImage(plt.imread(path),zoom=zoom)

def plot_network_topology(baseDir):
    simulationModel = {}

    with open(baseDir+"\\simulationParameters.json", "r", encoding="utf-8") as file:
        simulationModel = json.load(file)


    fig, ax = plt.subplots(nrows=1,figsize=(10,10))

    annotationPosition=(1200,2200)
    plt.xlim([0, 100000])
    plt.ylim([0, 100000])

    i = 0
    for ue in simulationModel["UE"].values():
        pos = ue["position"][0:2]
        ax.scatter(*pos, color="blue", marker="s", label="UE%d"%i)
        ax.annotate("UE%d"%i, numpy.subtract(pos,annotationPosition))
        i+=1
        #ab = AnnotationBbox(getImage(path), (ue["position"][0], ue["position"][1]), frameon=False)
        #ax.add_artist(ab)

    i = 0
    for eNB in simulationModel["eNB"].values():
        pos = eNB["position"][0:2]
        ax.scatter(*pos, color="blue", marker="^", label="gNB%d"%i)
        ax.annotate("gNB%d"%i, numpy.subtract(pos,annotationPosition))

        i+=1

    i = 0
    for pu in simulationModel["PU"].values():
        pos = pu["position"][0:2]
        ax.scatter(*pos, color="red", marker="^", label="PU%d"%i)
        ax.annotate("PU%d"%i, pos, numpy.subtract(pos,annotationPosition))
        l = mlines.Line2D([50000, pos[0]],[50000, pos[1]])
        ax.add_line(l)
        i+=1

    c = plt.Circle((50000, 50000), 50000.0, alpha=0.2, linewidth=1, fill=False)#,fill=False)
    ax.add_patch(c)
    #plt.legend()
    #plt.show()
    plt.tight_layout()
    fig.savefig(baseDir + "topology.png")

    return

if __name__ == "__main__":
    import sys
    plot_network_topology(sys.argv[1])
    pass