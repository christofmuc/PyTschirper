from pytschirp import Rev2
import os
from pathlib import Path

# Create an object to represent the Rev2 and run the autodetection
r = Rev2()

# Very Windows-specific path calculation
demo_file = Path(os.environ['LOCALAPPDATA']) / Path('PyTschirperExamples/Rev2_Programs_v1.0.syx')

# Load the 512 Rev2 Factory patchs, to have something to play with
p = r.loadSysex(demo_file.as_posix())

print(len(p))

def printPatch(patch):
    for param in patch.parameterNames():
        print("%s: %s" % (param, patch[param]))

printPatch(p[0])