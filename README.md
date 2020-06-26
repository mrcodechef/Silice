# Silice

Silice aims at simplifying writing code for FPGAs. It compiles to and inter-operates with Verilog. Silice is not meant to hide the HDL, but rather to complement it, making it more enjoyable to write parallel code and algorithms utilizing the FPGA architecture. 

To setup Silice, see the [getting started](GetStarted.md) guide. 
To start writing a first program, see [writing your first design](WriteAFirstDesign.md).
You don't even need an FPGA: designs and their outputs (e.g. VGA signal) can be simulated and visualized.

#### A first example:

##### Code:
```c
1  algorithm main(output uint8 led) {   
2    uint28 counter = 0;      // a 28 bits unsigned integer
3    led := counter[20,8];    // LEDs track the 8 most significant bits  
4    while (1) {              // forever
5      counter = counter + 1; // increment counter
6    }  
7  }
```

##### Compile:
```
silice first_example.ice -f frameworks/mojo_led.v -o Mojo-Project/src/mojo_top.v
```

##### Enjoy!

![First example in action on a Mojo v3](docs/figures/first_example.gif)

##### Explanations

Line 1 is the entry point of any Silice hardware: the main algorithm. Line 2 we define
a 28 bits unsigned int, initialized to 0. Initializers are mandatory and are always constants.
Line 3 we request that the output led tracks the eight most significant bits of the counter variable.
The syntax [20,8] means 8 bits wide starting from bit 20. The assignement to led
uses the := operator which is a continuous assignement: led is now automatically 
tracking changes to counter at each clock cycle. Continuous assignements always 
directly follow variable declarations in an algorithm.

Finally, lines 4-6 define the infinite loop that increments the counter. Of course the
28 bit counter will ultimately overflow and go back to 0, hence the cyclic LED light pattern.
In this case, the loop takes exactly one cycle to execute: we have one increment per cycle 
at 50 MHz (the clock frequency of the Mojo v3).

We then compile with silice. The -f parameter indicates which framework to use: this is a
FPGA plateform dependent wrapper code. Here we are using the Mojo framework with LEDs only.
Several other frameworks are provided, and it is easy to write your own. 

The -o parameter indicates where to write the Verilog output. In this example we overwrite 
the main file of a pre-existing project, which is then compiled using Xilinx ISE toolchain.
We also have examples working with [project icestorm](http://www.clifford.at/icestorm/)!

## Design principles

Silice does not attempt to abstract away the hardware: the programmer remains in control and very close to hardware features. However, Silice makes it much easier to reason in terms of execution flow and operation sequences than when using Verilog directly. But when Verilog makes more sense, simply import Verilog directly into Silice!

Silice is reminiscent of high performance programming in the late 90s (in the demo scene in particular): the then considered high-level C language was commonly interfaced with time-critical ASM routines. This enabled a best-of-both-worlds situation, with C being used for the overall program flow and ASM used only on carefully optimized hardware dependent routines.

Silice does the same, providing a programmer friendly C-inspired layer on top of Verilog, while allowing to call low level Verilog modules whenever needed. Silice also favors parallelism and performance everywhere, allowing to fully benefits from the specificities of FPGA architectures.

The main features are:
- Prioritize combinational over sequential execution. Parallelism comes first!
- Clearly defined rules regarding clock cycle consumption.
- Explicit clock domains and reset signals.
- Inter-operates easily with Verilog, allowing to import and reuse existing modules.
- Familiar C-like syntax.
- Powerful LUA-based pre-processor.

## Getting started with Silice

See the [getting started](GetStarted.md) guide. Silice runs great both on Windows and Linux!

## Building the examples

All examples are in the *projects* directory. This directory also contains a *build* subdirectory, with one entry for each currently supported framework. This includes both simulation (icarus, verilator) and FPGA hardware (icestick, mojo v3, de10nano, etc.).

To build a project, go into projects/build/*architecture* where *architecture* is your target framework. This directory contains shell scripts that take as parameter the project source file. Let's take an example! We will build the 'divint bare' demo for simulation with icarus. Do the following:

*Note:* under Windows please use a MinGW32 shell, please refer to the [getting started](GetStarted.md) guide.

```
cd silice/projects/build/icarus
./icarus_bare.sh ../../projects/divint_bare/main.ice
```
If everthing goes well you should see in the last console output:
```
20043 /   -817 =    -24
```
Yes, the hardware division is working!


