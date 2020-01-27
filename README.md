# AbstractVM
42 school project: Basic virtual machine in **C++**


![alt text](../media/avm_demo.gif?raw=true)


## Overview

AVM is composed of a series of instructions, but have limited types. The AVM program has a complete lexical and parsing analysis. It will display errors with unknown command or bad semantic. You can use it as a calculator or to display text with display instructions.

It includes a lot of features:
- Interactive mode
- Verbose mode
- Several types of instructions
- 5 differents types
- Error diagnostics

### Instructions
Here is a list of the availables instructions:

#### Stack instructions
```
push TYPE(VALUE)
pop
```
#### Arithmetic instructions
```
add
sub
mul
div
mod
```

#### Logical instructions
```
and
or
xor
not
assert TYPE(VALUE)
min
max
```
#### Display instructions
```
dump
print
```

### Types

Availables types are:
```
int8
int16
int32
float
double
```
Int{X..XX} creates a {X..XX}-bit integer.

## Installation

On UNIX-like systems, use the autotools based build system. Run the following commands from the top directory (containing this file) to configure, build and install:

```console
AVMFolder $> make
```

### Usage

```console
AVMFolder $> ./avm [-i | --interactive] [-v | --verbose] [file]
```
File parameter is optional, then avm will read on standard input.

## Examples

To save a float value and display it, you can use those instructions:
```
> push float(4.2)
> dump
4.2
```

