from pytschirp import Rev2, Rev2Patch
import random

r = Rev2()
r.detect()
if not r.detected():
    print("Rev2 not found, aborting!")
    exit(-1)

# When we reach this, the Rev2 is available, let's get the edit buffer
e = r.editBuffer()

# Clear the poly sequencer, all 6 tracks
for track in range(1,6):
    e['Poly Seq Note %d' % track] = [60] * 64
    e['Poly Seq Vel %d' % track] = [0] * 64

print('Poly Sequencer tracks 1 - 6 cleared in edit buffer\n')

print(e.toText())
