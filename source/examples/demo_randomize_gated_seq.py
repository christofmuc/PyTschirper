from pytschirp import Rev2
import random

# Create an object to represent the Rev2 and run the autodetection
r = Rev2()
r.detect()

# This won't work if the MIDI devices are not found or the Rev2 is turned off
if not r.detected():
    print("Rev2 not found, aborting!")
    exit(-1)

def explainGated(e):
    if e["Seq Mode"].get() == 0:
        print("Gated sequencer is on in", e["Gated Seq Mode"], "mode")
        print("Seq 1: ", e["Seq 1 Dest"], ":", e["Seq Track 1"])
        print("Seq 2: ", e["Seq 2 Dest"], ":", e["Seq Track 2"])
        print("Seq 3: ", e["Seq 3 Dest"], ":", e["Seq Track 3"])
        print("Seq 4: ", e["Seq 4 Dest"], ":", e["Seq Track 4"])
        print("Running at", e["BPM Tempo"], "bpm with", e["Clock Divide"])
    else:
        print("Poly sequencer is on")

# When we reach this, the Rev2 is available, let's get the edit buffer
e = r.editBuffer()

e.Cutoff = 0

e["Seq 1 Dest"] = 41
e["Seq 4 Dest"] = 10
e["Seq 2 Dest"] = 11
e["Seq 3 Dest"] = 14
e["Seq Track 1"] = [[80, 127][random.randrange(2)] for _ in range(16)]
e["Seq Track 2"] = [random.randrange(30, 80) for _ in range(16)]
e["Seq Track 3"] = [random.randrange(0, 12) for _ in range(16)]
e["Seq Track 4"] = [random.randrange(0, 20) for _ in range(16)]
e["Gated Seq Mode"] = 1
e["Seq Mode"] = 0

e["BPM Tempo"] = 120
e["Clock Divide"] = 6

explainGated(e)
print(e["Seq Mode"] == "Gated")

