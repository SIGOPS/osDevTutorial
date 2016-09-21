# osDevTutorial
OS Development Tutorials shown at UIUC SIGOPS Meetings

# Setup for deveoping this OS

Setup is quite simple and can be adapted for a number of environments, however, it is highly reccomended that a Ubuntu Linux environment is used. All instructions in the tutorials will assume that the user is using a Ubuntu environment (tutorials tested on Ubuntu 16.10)

## Required Packages

To install the required packages, run this command:

```
sudo apt-get install build-essential nasm genisoimage bochs bochs-sdl --fix-missing
```

This installs:
  - nasm
  - genisoimage
  - bochs
  - bochs visual
  - any required libraries

Those are the basic packages required for development. 
