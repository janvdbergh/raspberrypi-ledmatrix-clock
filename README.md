# raspberrypi-ledmatrix-clock
Clock using a Raspberry Pi displayed on a led matrix which can be controlled using the Open Sound Control (OSC) protocol.
This code requires
- rpi-rgb-led-matrix from https://github.com/hzeller/rpi-rgb-led-matrix.
- oscpack from http://www.rossbencina.com/code/oscpack

## Supported commands:
- `/time <time in minutes since Sunday 00:00>`: display a time.
- `/clear`: clear the display.
- `/brightness <value>`: sets the brightness (0 - 255).
- `/fade <value> <duration in ms>`: fades to certain brightness in specific time.
- `/forward <time in minutes since Sunday 00:00> <duration in ms>`: forwards the time to the specified end time in duration ms.
- `/show <time in minutes since Sunday 00:00> <brightness> <fade duration in ms> <wait duration in ms>`: fades the clock in on the given time, waits during wait duration and fades out again (duration in ms).
- `/showAndForward <start time in minutes since Sunday 00:00> <end time in minutes since Sunday 00:00> <brightness> <fade duration in ms> <forward duration in ms> <wait duration in ms>`: fades the clock in on start time, forwards it to end time, waits wait time and fades out again.
