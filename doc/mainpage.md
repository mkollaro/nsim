@mainpage NSim: N-body Simulator

@section intro Introduction

NSim is a simulator of the Solar system, or in general any system of objects
with gravitational forces between them. It can visualize the results in a 3D
animation, or provide the raw data on the command line. Instead of using
pre-computed data like Stelarium or Celeestia do, it does the computation
itself and thus it's possible to create arbitrary systems of celestial bodies.

@section core Collaboration of the Main Components
The main components of the application are the simulation and animation
objects, which are communicating trough a data buffer of computation results
(see image below). Both are controlled by the user actions in the GUI. For
simplicity, only one thread is used — Qt’s timer events create the illusion of
parallelism. One timer in the OpenGL visualization animates the bodies and
another timer runs the simulation for a few steps when there are no other
pending events. The animation reads the buffer and displays the data. It pauses
for a few seconds when there are no more data in the buffer, waiting for the
simulation to make further computations. No direct communication is required
between the simulation and animation, as they are controlled by the users
actions. The simulation may continue computing in the background when the
animation is paused, so that data are available after it is resumed. This way,
the user with a slower computer can wait for the computation and continue when
the results are ready. The data buffer holds a list of simulation results, both
positions and velocities. The simulation may jump to a specified time, load
data from the data buffer as initial conditions and continue computing using a
different algorithm or time

@image latex core.eps
@image html core.png "Collaboration diagram" width=10cm

@section strategy Numerical Integration

For the application to be able to change the numerical integration algorithm,
it uses the Strategy design pattern (also known as Policy).
@dotfile strategy.dot Strategy Design Pattern
