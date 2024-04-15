# [<img src="./docs/Instagram_icon.png.webp" width="35"/>](https://www.instagram.com/byte.thebeat/) byte_theBeat
**byte_theBeat** is a contactless, Pi-powered drumkit, being developed for the Real Time Embedded Programming course (ENG5228) at the University of Glasgow.

<img src="./docs/ByteThisBeat.jpg" alt="Alt text" title="(byte)this.beat; PCB">

## So what's the tempo?

 - The challenge set for this course was to develop any real time embedded project, using a Raspberry Pi 3 and a £50 budget.  
 - We found these awesome time-of-flight sensors from ST: [VL53L4CD](https://www.st.com/en/imaging-and-photonics-solutions/vl53l4cd.html), and decided to turn them into "invisible" drum modules so that you can jam out in thin air!  
 - The distance values measured by the sensors are used to calculate a velocity - which in turn is used for "hit detection", so the timing and intensity of each drum hit can be determined.
## Hardware required
 - [ ] Raspberry Pi 3B
 - [ ] 6x [VL53L4CD](https://www.st.com/en/imaging-and-photonics-solutions/vl53l4cd.html) time-of-flight sensors **or** [breakout boards*](https://www.st.com/en/ecosystems/satel-vl53l4cd.html)
 - [ ] [Custom PCB with components**](https://github.com/GrantMaiden/ContactlessPiPoweredDrumkit/tree/main/hardware/ContactlessPiPoweredDrumkit)
 - [ ] [3D printed case](https://github.com/GrantMaiden/ContactlessPiPoweredDrumkit/tree/main/hardware/ContactlessPiPoweredDrumkit/CAD) (optional)

## Software documentation
Check out the complete documentation at: 

## tl;dr
- byte_theBeat has **six** distance sensors installed, meaning up to **six** different "drums" can be added per kit. 
- Each drum has **independent hit detection**
- Hit detection works by using the **distance measurement** from a given sensor, and calculating the **velocity** from successive measurements.
- If the program detects that an object is moving **towards** the sensor, it calculates the **acceleration** in order to determine a **strength/hardness** value.
## Team docs

All Project Discussions, meeting notes, and development feasibility studies can be found at our [sharepoint](https://gla.sharepoint.com/:o:/s/ENG5228RTEP/Er4JzmCRMUZCnUjhNUSQIA8BrX5IVSQI6c456dHyLfZf0w?e=HDqj4z).  
This link is accessible by anyone with a University of Glasgow account.

## Authors

* Grant Maiden  
* Peter O'Hanlon  
* Ethan Travers  
* Lucas Zehner  

## Acknowledgments

> Written with [StackEdit](https://stackedit.io/).
