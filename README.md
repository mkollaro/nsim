# Nsim (N-body Simulator)

NSim is a simulator of the Solar system, or in general any system of objects
with gravitational forces between them. It can visualize the results in a 3D
animation, or provide the raw data on the command line. Instead of using
pre-computed data like Stelarium or Celestia do, it does the computation
itself and thus it's possible to create arbitrary systems of celestial bodies.

The simulator predicts the motions of objects using numerical integration,
since there is no analytic solution to a system with more than two bodies.
Gravitational interactions are computed with classical Newtonian mechanics and
the bodies are modelled as simple point masses (their volume and shape are not
taken into consideration).

## Examples

### CLI usage

    $ bin/nsim-cmd -f examples/earth-moon-sun.xml
    # time Sun_x Sun_y Sun_z Earth_x Earth_y Earth_z Moon_x Moon_y Moon_z
    0 0 0 0 128079368.922767 -80628651.5813115 -3492.12286324799 127792152.883098 -80865704.0944675 9563.00363019109
    200 0 0 0 128082444.421441 -80623629.9416063 -3492.07035570603 127795365.928979 -80860840.702465 9580.24818598272
    400 0 0 0 128085519.722891 -80618608.179338 -3492.01779332642 127798578.867124 -80855977.1132846 9597.48870185548
    ...
    
### GUI screenshot (alpha version)

![Alpha version of the GUI showing the project 'examples/earth-moon-satellite.xml'](doc/img/screenshot.png)


## Installation

You will need:

* a compiler that supports C++11 (g++ is recommended)

        $ apt-get install g++

* at least [Qt 5.4](http://www.qt.io/download-open-source/)
* OpenGL 3.1 (lower versions might work, but are untested)

Afterwards, run:

    $ qmake
    $ make

Execute:

    $ bin/nsim  # GUI
    $ bin/nsim-cmd -h

## Graphical Interface

Warning: The GUI is in an alpha stage of development, some of the elements
might not be implemented yet.
    
Load a project (a file containing the positions of objects) from the `example/`
directory and hit the play button to start the animation.

Depending on the algorithm, number of the objects in the simulation and your
hardware, the animation might be very slow.
