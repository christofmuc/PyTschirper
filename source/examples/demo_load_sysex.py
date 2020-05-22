from pytschirp import Rev2
import os
from pathlib import Path

# Very Windows-specific path calculation
demo_file = Path(os.environ['LOCALAPPDATA']) / Path('PyTschirperExamples/Rev2_Programs_v1.0.syx')

# Create an object to represent the Rev2 and run the autodetection
r = Rev2()

# Load the 512 Rev2 Factory patchs, to have something to play with
p = r.loadSysex(demo_file.as_posix())

print(len(p), "patches loaded, access them using e.g. p[0]")


