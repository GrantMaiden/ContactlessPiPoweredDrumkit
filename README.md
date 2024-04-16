
# [<img src="./docs/Instagram_icon.png.webp" width="35"/>](https://www.instagram.com/byte.thebeat/) (byte)this.beat  
Welcome to the github landing page for the first prototype of the **(byte)this.beat** drumkit!

(byte)this.beat is an innovative, contactless, raspberry pi-powered drumkit being developed for  Real Time Embedded Programming (ENG5228) at the University of Glasgow.  The contactless drumkit aims to be a simple, easy-to-use interface that accommodates entry level users---being a introductory device to jam out on---as well as a device for professional musicians, conforming to sub 20ms latency requirements and high quality, 48000khz drum samples.

Please download the latest release to access the most recent stable version of the source code. Keep in mind that hardware is required to run this project!

<img src="./docs/ByteThisBeat.jpg" alt="Alt text" title="(byte)this.beat; PCB">

## So what's the tempo?

* The challenge set for this course was to develop any real time embedded project, using a Raspberry Pi 3 and a £45 budget.  
* Selecting an appropriate sensor that can be used in a real-time system that is affordable, has a high sample rate, and great data resolution was challenging. After a thorough amount of market research, the [VL53L4CD](https://www.st.com/en/imaging-and-photonics-solutions/vl53l4cd.html) ranging sensor was selected as the most suitable ic for our project. Turning them into "invisible" drum modules so that you can jam out in thin air seemed a perfect task!  
* The distance values measured by the sensors every 10ms are used to calculate the velocity of any objects directly above the sensors. A detection algorithm is derived by being a combination of when the velocity changes direction, and what the previous 80ms of velocity was measured as. Thus, it is possible to use the sensor data to not only detect when a drum hit has been detected, but also at what intensity!

## Team Collaboration and Project Management

Project management was done by utilising a teams channel for collaboration. At the start of our group project, keeping in mind an agile team development environment, a Gantt chart was created in excel and hosted on our teams channel. Our team had two meetings each week throughout the semester; the first of which was primarily for project planning and updates, and the second meeting was for development and one-on-one problem solving, if necessary. Github issues were utilised as a means to give feedback as our project development continued. All Project Discussions, meeting notes, and development feasibility studies can be found at our [sharepoint](https://gla.sharepoint.com/:o:/s/ENG5228RTEP/Er4JzmCRMUZCnUjhNUSQIA8BrX5IVSQI6c456dHyLfZf0w?e=HDqj4z).  
This link is accessible by anyone with a University of Glasgow account.

## Project Setup and Initialisation
### Hardware Requirements
* Assembled (byte)this.beat board
* Raspberry Pi updated with latest Raspbian image
* Speaker attached to Audio Jack
### Software Setup
* Complete project setup instructions located in the [Embedded Directory](https://github.com/GrantMaiden/ContactlessPiPoweredDrumkit/tree/main/embedded) on your local raspberry pi
* Download the latest release of the firmware, and compile the source code in the embedded directory by following the [cmake build instructions](https://github.com/GrantMaiden/ContactlessPiPoweredDrumkit/blob/main/embedded/cmake_build_instructions.txt)

## Hardware Development
In order to minimise costs associated with the (byte)this.beat project, a custom Hardware design was developed. After a cost analysis completed at the beginning of the project timeline, it was concluded that designing a custom solution, rather than purchasing independent breakout boards, would allow our contactless drumkit to have a larger scope. Thus accommodating more sensors/functionality by more easily staying within our £45 budget. The discussions pertaining to analysis and cost can be seen at our [onenote](https://gla.sharepoint.com/:o:/s/ENG5228RTEP/Er4JzmCRMUZCnUjhNUSQIA8BrX5IVSQI6c456dHyLfZf0w?e=HDqj4z).  

As always, the goal with any project is to learn as much as possible while designing, thus we decided to use this opportunity to get familiar with KiCad for schematic entry. A prototype design was completed early into the semester, giving our team plenty of tolerance on receiving the fabricated boards. The fabricated board can also easily accommodate many more traces and connections than a breadboard solution could, allowing for the design scope to increase. The schematic can be seen [here](https://github.com/GrantMaiden/ContactlessPiPoweredDrumkit/blob/main/hardware/ContactlessPiPoweredDrumkit/Schematic/byteThisBeatREVA.pdf).

<img src="./docs/Schematic overview.png" alt="Alt text" title="(byte)this.beat; Schematic overview">

The drumkit had a portion of the budget left over after the primary sensor selection, so we decided to use the rest of the funds on purchasing LEDs that would aid in giving the drummer feedback during hit detection. As seen in the block diagram below, the raspberry pi communicates with 6 distance sensors, outputs to the on-board audio jack, and outputs serialised data to the LEDs.

<img src="./docs/Hardware Block Diagram.JPG" alt="Alt text" title="(byte)this.beat; Hardware Diagram">

The LEDs that were chosen are the same type of addressable LEDs that can be found on LED strips. They are affordable, and plenty of options are available. A few design challenges had to be faced regarding the decision to use these leds, however. The LEDs use 5V logic for communication, so a level shifter had to be added to translate the rpi's 3.3V logic to 5V. Secondly, the LEDs use a strict 800khz transmission rate for each bit. Serialised bits need to be given to the LEDs over a single trace, essentially the LEDs act as specialised shift registers, only forwarding their to each succeeding LED after 24bits of data has been received. The data sheet for the LEDs, along with more information regarding the protocol, can be found [here](https://github.com/GrantMaiden/ContactlessPiPoweredDrumkit/blob/main/docs/datasheets/IN-PI55TAT(X)R(X)G(X)B_v1.5.pdf). 


Due to the strict timing requirement of the LEDs, we had to use a hardware bus to send the LED bitstream, given that bitbanging at 800khz would not be consistent, and would cause too much overhead. Initially, the PCB was designed to use the uart interface for driving the LEDs, but after getting the boards in-hand and beginning development, it was found that the uart bus could not support the baudrate required (800khz*(8bits + 1stopbit) = 7.2mhz), a hardware rework was completed to change the LED control over to the SPI bus, which could accommodate the necessary frequency.

For more information regarding the manufacturing of the (byte)this.beat board, please refer to the [hardware directory](https://github.com/GrantMaiden/ContactlessPiPoweredDrumkit/tree/main/hardware/ContactlessPiPoweredDrumkit), which includes a BOM for manufacture. The board file can also be seen in this directory. Layout was completed using KiCad, with a two-layer PCB being designed. After meeting with the EE technicians at the University of Glasgow, we found that they were unable to fabricate our design, so an overseas location was chosen for fab and assembly.

<img src="./docs/PCB_Layout.JPG" alt="Alt text" title="(byte)this.beat; Schematic overview">

## Software 

The embedded code, being the main focus of our project, was approached with several goals for implementation:
* Real Time Coding - code is event driven and utilises hardware inputs or timers to strictly control code behavior
* Event Handling - real time events are coded with clearly defined callbacks and other async operations
* Class Encapsulation - code is written within separate classes that have clear responsibilities, obey the Liskov Substitution Principle, and are segregated, with dependency inversion
* Documentation - Verbose function headers and additional documentation generated with Doxygen to supply the user information
* Header File Constants - many constants included to aid in program control and easily modify behavior

Below, please see a diagram regarding how the major classes interact with each other within our application.

<img src="./docs/Initialisation Flowchart.JPG" alt="Alt text" title="(byte)this.beat; Init Flowchart">

The controller class is initialised by the Main function, which on program boot creates the initial controller class. When the controller class is initialised, it will bring up the other portions of the program, which configures any necessary peripherals, and performs the bring-up procedure of the ranging sensors. This initialisation also configures asynchronous processes that use callback processes to create real-time operation of our system. The LED class uses a timer, and updates all LEDs every 2.5ms. The VL53L4CD class leaves an abstract function that is defined within the Controller class. When a hardware interrupt occurs, the VL53L4CD class will enact a callback via the virtual function.

<img src="./docs/Class Diagram.JPG" alt="Alt text" title="(byte)this.beat; class diagram">

### Bus Protocols
I2C was used as the communication protocol for each sensor. All sensors share the same bus, but are initialised with the same address. An initialization procedure had to be crafted to disable all sensors on program start, then bring-up each sensor sequentially, reprogramming the I2C bus address for the sensor, and configuring it for our high sample rate requirements.

<img src="./docs/VL53L4CD bringup.png" alt="Alt text" title="(byte)this.beat; ranging bringup">

The SPI protocol uses custom bitshift operations to drive 6*24bits of RGB data to our addressable LEDs. The GPIO controller class handles the sending of the SPI data.


## Unit Testing Framework
A custom unit testing framework was designed to accommodate the needs of our team collaborative efforts. When the application launches, the first code that is ran is a search to see if an input arguments are supplied. The input arguments are the inputs to our custom unit testing framework; it was deemed necessary to have a more advanced testing methodology in place as the many different blocks of our project required separate entry points depending on use case. In example, a unit test was designed to benchmark and initialise the LEDs. After the unit test was completed, the functions designed during test were integrated into the rest of the project. This design methodology was enacted in all facets of our software development. 

A list of all unit tests, their associated functions, and more specific usage instructions can be found [here](https://github.com/GrantMaiden/ContactlessPiPoweredDrumkit/blob/main/embedded/unit_tests.txt).


## Authors

* Grant Maiden  
* Peter O'Hanlon  
* Ethan Travers  
* Lucas Zehner  
