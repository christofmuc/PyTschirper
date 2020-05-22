from pytschirp import Rev2

# Create an object to represent the Rev2 and run the autodetection
r = Rev2()
r.detect()

# This won't work if the MIDI devices are not found or the Rev2 is turned off
if not r.detected():
    print("Rev2 not found, aborting!")
    exit(-1)

# When we reach this, the Rev2 is available, let's get the edit buffer
e = r.editBuffer()



# Any modification done to the edit buffer is immediately send as NRPN via MIDI
e.Cutoff = 63



# Let's print the edit buffer as text, all the parameters and their values
print(e.toText())



