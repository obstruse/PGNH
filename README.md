
# PolarGraph Network Hack

![Cat](/images/IMG_1561-C2r2.png)

# The Software

![COMMS](/images/comms.png)

## Installation

## Changes

- Network
- Multi-task
- Speed Improvements
# Travelling Salesman Problem

**noun**: *travelling salesman problem*

    a mathematical problem in which one tries to find the shortest route that passes 
    through each of a set of points once and only once.

    The problem can be solved by analyzing every round-trip route to determine the shortest one. 
    However, as the number of destinations increases, the corresponding number of roundtrips 
    surpasses the capabilities of even the fastest computers. 

![TSPwiki](/images/TSPwiki.png)

    With 10 destinations, there can be more than 300,000 roundtrip permutations and combinations. 
    With 15 destinations, the number of possible routes could exceed 87 billion.

The following is a TSP "solution" for a 27,000 point map:

![MOM-4](/images/MOM-4.png)

## Create the Vector File

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

## Run polargraphcontroller

Setup machine parameters and enter Network Host:

![Network Host](/images/controllerNH.png)

Home the machine; home the controller; start the queue<br>
Load/Resize/Move Vector; Draw Vectors

![Vector Commands](/images/controllerVECTOR.png)




