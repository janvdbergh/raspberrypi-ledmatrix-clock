# raspberrypi-ledmatrix-clock
Clock using a Raspberry Pi displayed on a led matrix which can be controlled using the Open Sound Control (OSC) protocol.
This code requires
- rpi-rgb-led-matrix from https://github.com/hzeller/rpi-rgb-led-matrix.
- oscpack from http://www.rossbencina.com/code/oscpack

## Supported commands:
- `/time <time in minutes since Sunday 00:00>`: display a time.
- `/clear`: clear the display.
- `/brightness <value>`: sets the brightness (0 - 255).
- `/fade <value> <time>`: fades to certain brightness in specific time (in ms).
- `/forward <time> <duration>`: forwards the time to the specified end time in duration ms.
- `/showAndForward <start time> <end time> <brightness> <fade time> <forward time> <wait time>`: fades the clock in on start time, forwards it to end time, waits wait time and fades out again.
