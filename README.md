# Tricopter

DIY Arduino Uno Tricopter using Brushless ESCs. NOTE: This was a beginner project, and is here more for documentation and reference rather than to serve as instructions on how to build a successful one.
I have a youtube video documenting this project [here](https://www.youtube.com/watch?v=mtm9PQwioOE).

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

There is a detailed parts list in an excel file. The RC transmitter used was a Turnigy 9X transmitter.

PinChangeInt
https://playground.arduino.cc/Main/PinChangeInt

Arduino PID Library
https://github.com/br3ttb/Arduino-PID-Library/blob/master/PID_v1.h

```
#include <PinChangeInt.h>
#include <PID_v1.h>
```

### Navigating the Repository

The code for the Tricopter is in the folder Tricopter_2017.

In the main repo I also included a parts/specs list and some data from static test experiments. I also included some diagrams, references, and manuals used.

Let me know if you have any questions. Email me at stef.layanto@gmail.com

## Built With

* [PinChangeInt](https://playground.arduino.cc/Main/PinChangeInt) - Used to read RC signals
* [Arduino PID Library](https://github.com/br3ttb/Arduino-PID-Library/blob/master/PID_v1.h) - Used for PID computation

## Author

* **Stefan Layanto** - *Everything*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details

## Acknowledgments

* Susannah Layanto was super clutch in helping me conduct thrust tests and motivating me to finish
* Joshua Hodiny created the youtube [video](https://www.youtube.com/watch?v=mtm9PQwioOE) documenting the project
