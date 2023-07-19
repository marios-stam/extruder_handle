# Extruder_Code
Drone extrusion

~~Currently some new bug in the code, there is something wrong with the serial communication part.~~ Disregard the text in the email about a bug, it's fixed now. 


Documentation: 

The extrude function can either be called directly in the Arduino code as extrude(DISTANCE, SPEED) where distance is how far the gray platform travels in millimeters, and SPEED is the velocity at which the gray platform travels, in millimeters per second. The extrude function can also be called over serial, as EnnSmmmD+/1, where nn = distance in mm and mmm = speed in mm/s where the third digit is one-tenth of a mm/s. Without modifying the code, distance needs to be two digits and speed three digits. So for a distance of 99 mm, it would be E99, for a distance of 40 mm it would be E40. For a speed of 25.6 mm/s it would be S256, for a speed of 15.0 mm/s it would be S150. The direction of the extrusion is determined by either a + (towards the side of the syringes) or - (towards the Arduino/stepper side)

Example: Extrude 25 mm, at 32.6 mm/s forwards: E25S326D+ over serial. 


The extrusion speed should be limited to a minimum of 5mm/s (but that could probably be lowered, it gets progressively more and more noisy the slower it is however) and a maximum of 35mm/s, beyond which the motor stalls when supplied with about 1.8 amps at 12V with syringes. These values can be changed via “MaximumSpeed” and “MinimumSpeed” at the top of the code. 

Constants: 

RPMconv: Constant scaling conversion factor. Converts the received speed value into rotations per minute of the stepper, which the stepper library requires. Derived from experimental measurements. Would likely have to be changed if the stepper motor is replaced. 

Steps: Total amount of steps in the stepper motors rotation. Might have to be changed if another stepper motor was installed with more or fewer steps. 

PWMA and PWB disable/enable the motors (LOW = disabled, HIGH = enabled). Currently, the motors are only enabled during extrusion, by disabling them the extruding platform can turn freely (although it does take a lot of pressure to move) but pull no current. Enabled while not extruding pulls a significant amount of power, heating the motor driver too much. By changing the digitalWrite on lines 98 and 99 to HIGH, and removing all other digitalWrite(PWMA/PWMB, HIGH/LOW) lines the steppers would be enabled constantly. 

Limit switches are active low, normally open. 
