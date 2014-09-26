TurtleBot
=========

## Hardware Overview

## Software Overview
### Drive Code
The drive code is designed to provide a single joystick interface for maneuvering the TurtleBot.  This code constantly polls the joysticks for new values, and calculates the proper motor outputs to effect the desired motion.  THis drive code is known as arcade style drive.  Pushing the stick full forward causes movement directly forward.  Pushing the stick to the front left causes the robot to turn to the left pivoting on the left read.  Pushing the stick directly to the left causes the robot to spin to the left in place.  Pushing the stick to the rear left causes the robot to move to the right in the reverse direction pivoting on the right tread.  full rear deflection causes the robot to move rearwards.  Right rear deflection causes the robot to turn to the left in the reverse direction while pivoting on the left tread.  Full right deflection causes the robot to turn to the right in position.  Forward right deflection causes the robot to turn to the right while pivoting on the right tread.  This control scheme allows for fully continuous driving, with no discontinuities in the motion of the robot as the joystick is manipulated.
