This repository contains a small, simple example Visual Studio Code environment that can be used to develop linux kernel modules.

It builds on the C/C++ VSCode extension, and configures the the intellisense to work with a kernel source code.
The location of the cloned and built kernel source can be configured in the .vscode/c_cpp_properties.json file
as the value of the "KDIR" environment variable.

The intellisense and the build files are tested with the latest 6.0 mainline kernel source.
The way how this environment was created is documented in the setup.md file.

