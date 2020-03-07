import sys
sys.path.append(R"D:\Development\github\PyTschirper\builds\pytschirp\Debug")
import pytschirp
r = pytschirp.Rev2()
r.detect()
s = r.getGlobalSettings()
