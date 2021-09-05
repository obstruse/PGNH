
# PolarGraph Network Hack

_A modified version of:_<br>
**Polargraph**: ___An open source art project <http://www.polargraph.co.uk/>___

![Cat](/images/IMG_1561-C2r2.png)

A [PolarshieldSD V3 ESP32](http://www.polargraph.co.uk/), with [polargraphserver](https://github.com/euphy/polargraph_server_polarshield_esp32) and [polargraphcontroller](https://github.com/euphy/polargraphcontroller), modified to use WiFi network for communication between Controller and Server.

# The Polargraph Software

![COMMS](/images/comms.png)

The Doxygen documentation for the polargraphserver can be found here: <https://obstruse.github.io/PGNH>

## Installation

To compile and upload, use VSCode/PlatformIO.  For the first upload, use the `[env:TTY0]` Project Environment (edit platformio.ini to match your serial port).

The first time it boots it will create a Soft Access Point with **SSID**: `polargraph` and **PSK**: `carbondioxide`. <br>
Open <http://polargraph.local/config> to configure the WiFi

For uploads after the first, use the `[env:OTA]` Project Environment (edit platformio.ini for machine host/IP)

PolargraphController requires Processing-2.2.1.  Use _File -> Export Application_ from the IDE to create the application, or from the command line: `$ processing-java --export --output=build --sketch=polargraphcontroller`


## Changes

- Network
    - WiFi connection between server and controller on port 12345
    - Web server for configuration and status
    - Firmware update via OTA
- Multi-task
    - WiFi Task
    - HTTP Task
    - Comms Read Task
    - Comms Execute Task
    - LCD Task
    - SD Plot Task (created when needed)
    - Loop Task (empty and paused)
    - All tasks run in parallel at same priority with no conflicts.
- Speed Improvements
    - Instead of reading one character every 20msec, it reads all when available
    - Begins buffering the next command while the current command is executing
    - Comms plot speed equal to SD plot speed

## Travelling Salesman Problem

**noun**: *travelling salesman problem*

    a mathematical problem in which one tries to find the shortest route that passes 
    through each of a set of points once and only once.

    The problem can be solved by analyzing every round-trip route to determine the shortest one. 
    However, as the number of destinations increases, the corresponding number of roundtrips 
    surpasses the capabilities of even the fastest computers. 

![TSPwiki](/images/TSPwiki.png)

    With 10 destinations, there can be more than 300,000 roundtrip permutations and combinations. 
    With 15 destinations, the number of possible routes could exceed 87 billion.

A TSP "solution" for a 15,000 point map:

![MOM-4](/images/MOM-4.png)

### Create the Vector File

Make a Grayscale Image

    Open your image in GIMP
    Turn the color information monochrome “Image -> Mode -> Grayscale“
    Open the color levels “Colors -> Levels“
    Wash out the image by adjusting “All Channels” in the pop-up window to some value to 200 
    ((Values between 180 and 245 should work well))

Make a Stippled Image

    Turn the washed out grayscale image into a stippled image “Create Image->Mode->Indexed“
    Select these options in the pop-up window
        Use black and white (1-bit) palette): Checked
        Remove unused colors from colormap: Checked
        Color dithering: Floyd-Steinberg (normal)
    Click “Convert“
    Save the file in the “Portable Bit Map” format “File->Save As” and saving the file with the extension “.pbm“
        If given the option, choose to save the PBM file in the “Raw” format

Convert PBM to Vector

Use `tspart.py` from [tsp_art_tools](https://github.com/evil-mad/EggBot/tree/master/other/TSP-stipple/tsp_art_tools)

### Run polargraphcontroller

Setup machine parameters and enter Network Host:

![Network Host](/images/controllerNH.png)

Home the machine; home the controller; start the queue<br>
Load/Resize/Move Vector; Draw Vectors

![Vector Commands](/images/controllerVECTOR.png)




