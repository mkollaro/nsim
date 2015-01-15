# Solar system data from the NASA JPL

The data in this directory have been generated using the HORIZONS web
interface [1] to the NASA JPL data. They show how the chosen celestial bodies
move, starting from 1977-Aug-20 15:33:00, with a time step of 1 day, for about
10 years. These data are used to create comparisons with NSim - to see how
NSim's computations deviate from JPL's.

The first column is the time in JDCT (Epoch Julian Date, Coordinate Time), the
second is a more human readable time format, which is followed by 3 columns
consisting of the x, y and z position coordinates, centered at the sun. The
next 3 colums is the velocity vector vx, vy, vz.

## About the JPL data
JPL Horizons interface is a tool to access DE405, the NASA JPL (Jet Propulsion
Laboratory) ephemerides tables that describe the orbits of the Sun, planets,
satellites, spacecraft and other astronomical bod- ies. The positions are a
data-fits of observations and integrated into the past and future.  They are
referenced to the ICRS system (International Celestial Reference System), which
is tied to distant radio quasars.
