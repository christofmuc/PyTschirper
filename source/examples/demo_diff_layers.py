from pytschirp import Rev2, Rev2Patch

# Create an object to represent the Rev2 and run the autodetection
r = Rev2()

p = r.loadSysex(R'D:\Development\github\PyTschirper\examples\Rev2_Programs_v1.0.syx')

print(len(p))


def printDiffLayers(patch):
    for param in patch.parameterNames():
        if patch.layer(0)[param].get() != patch.layer(1)[param].get():
            print("%s: %s vs. %s" % (param, patch.layer(0)[param], patch.layer(1)[param]))

printDiffLayers(p[1])