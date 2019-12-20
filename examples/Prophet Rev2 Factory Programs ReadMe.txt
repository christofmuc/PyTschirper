==========================================================================================
Prophet Rev2 Factory Program Banks ReadMe File May-17-2017
==========================================================================================

The accompanying file, Rev2_Programs_v1.0.syx, contains all of the factory program banks
for the Prophet Rev2. The data is in a MIDI System Exclusive (SysEx) file. You will need a
DAW, MIDI utility, or other application capable of opening and sending MIDI System
Exclusive (SysEx) messages. MIDI-OX (Windows) and SysEx Librarian (Mac OS) are shareware
MIDI utilities that can reliably be used to update DSI instruments. You will also need
either a USB cable (Type A to Type B connectors, like a typical USB printer cable) or a
MIDI interface to transmit the SysEx file from your computer to the Prophet Rev2.

You will find instructions for updating using MIDI-OX and SysEx Librarian below.

************************************* IMPORTANT NOTE *************************************

Be aware that loading programs overwrites any programs that are currently in memory. If
you have programs you want to keep, be sure to save them to a SysEx file first. Refer to
the manual for more information about initiating a SysEx dump from the Prophet Rev2's
front panel controls.

******************************************************************************************

__________________________________________________________________________________________
LOADING PROGRAMS FROM WINDOWS USING MIDI-OX

Before starting, close all other audio or MIDI or DAW software, and disconnect all other
MIDI devices. If necessary, download and install MIDI-OX.

http://www.midiox.com/

To prepare the Prophet Rev2 to receive system exclusive messages:

1. Connect your synthesizer to a computer using a USB cable (or MIDI cable, if you are
using a dedicated MIDI interface).

2. Press the Global button on your Rev2.

3. Use the Parameter knob to select "MIDI sysex cable" then use the Value knob to choose
either USB or MIDI, depending on which port you are using to connect to your computer.

If using USB, the Prophet Rev2 is a Class Compliant USB device. That means it does not
require any additional drivers to be installed to communicate with your computer. It will
appear in MIDI-OX and other MIDI applications as a MIDI port named "Prophet Rev2." (Under
Windows XP, it will appear as "USB Audio Device.")

If using a MIDI interface, connect the computer's MIDI out to the Prophet Rev2's MIDI in.
Depending on the type of interface, you may also need a MIDI cable.

Most MIDI interfaces or sound cards with built-in MIDI interfaces will work. However, we
have received reports that Digidesign/Avid interfaces and Native Instruments Maschine may
not transmit SysEx correctly for updates. The M-Audio Uno is an inexpensive, reliable, and
widely available MIDI interface for use when updating our instruments.

To load the programs:

 1. Run MIDI-OX.

 2. From the Options menu, choose "MIDI Devices."

 3. In the lower left quadrant of the window, select the MIDI port to which the instrument
 is connected. If connected via USB, choose "Prophet Rev2" (or "USB Audio Device" under
 Windows XP). If connected via MIDI, choose the MIDI interface. The selected MIDI port
 appears in the Port Mappings window.

 4. Click OK to close the dialog and save the settings.

 5. From the View menu, choose "SysEx."

 6. From the SysEx menu, choose "Configure."

 7. Set the Low Level Output Buffers "Num" and "Size" to 4096.

 8. Under Output Timing, make sure that "Auto-adjust Buffer Delays if necessary" is NOT
 enabled.

 9. Click OK to close the dialog and save the settings.

10. From the Command Window menu, choose "Load File." Browse to and open the Prophet Rev2
programs file.

11. From the Command Window menu, choose "Send SysEx." The programs will increment on the
Prophet Rev2 as each program file is transferred to it. They will replace any existing
programs in those memory locations on the synth

__________________________________________________________________________________________
LOADING PROGRAMS FROM A MAC USING SYSEX LIBRARIAN

Before starting, close all other audio or MIDI or DAW software, and disconnect all other
MIDI devices. If necessary, download and install SysEx Librarian.

www.snoize.com/sysexlibrarian

To prepare the Prophet Rev2 to receive system exclusive messages:

1. Connect your synthesizer to a computer using a USB cable (or MIDI cable, if you are
using a dedicated MIDI interface).

2. Press the Global button on your Rev2.

3. Use the Parameter knob to select "MIDI sysex cable" then use the Value knob to choose
either USB or MIDI, depending on which port you are using to connect to your computer.

If using USB, the Prophet Rev2 is a Class Compliant USB device. That means it does not
require any additional drivers to be installed to communicate with your computer. It will
appear in SysEx Librarian, the Mac's Audio MIDI Utility, and other MIDI applications as a
MIDI port named "Prophet Rev2."

If using a MIDI interface, connect the computer's MIDI out to the Prophet Rev2's MIDI in.
Depending on the type of interface, you may also need a MIDI cable.

Most MIDI interfaces or sound cards with built-in MIDI interfaces will work. However, we
have received reports that Digidesign/Avid interfaces and Native Instruments Maschine may
not transmit SysEx correctly for updates. The M-Audio Uno is an inexpensive, reliable, and
widely available MIDI interface for use when updating our instruments.

To load and update the OS:

1. Run SysEx Librarian.

2. From the SysEx Librarian menu, choose "Preferences."

3. Close the SysEx Librarian Preferences window to save changes.

4. Choose your MIDI interface from the "Destination" menu.

5. Click Add/+ to add the file to the file list or simply drag and drop the file on the
open SysEx Librarian window. The file name appears in the file list and should be
highlighted.

6. Click Play. The programs will increment on the Prophet Rev2 as each program file is
transferred to it. They will replace any existing programs in those memory locations on
the synth
