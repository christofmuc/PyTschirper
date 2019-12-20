from pytschirp import Rev2, Rev2Patch

# Create an object to represent the Rev2 and run the autodetection
r = Rev2()

p = r.loadSysex(R'D:\Development\github\PyTschirper\examples\Rev2_Programs_v1.0.syx')

print(len(p))


