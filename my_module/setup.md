# Run qemu
>  runqemu qemux86-64 nographic

As password use the user password of the host system.

# Build the SDK
> bitbake core
-Image-sato -c populate-sdk



# Develop out-of-tree kernel module
## Setup the host machine


## Obtain the kernel sources
### A) Download and build the mainline kernel
Clone the mainline kernel:
> cd
> git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/ ./mainline_kernel

Install the build dependencies:
> sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev

Configure the kernel with default config options:
> make defconfig

Execute the necessary steps to ba able to compile a module with this kernel source:
>  make -j modules_prepare

In order to get a full symvers file one has to build the complete kernel,
otherwise the module cannot contain the exact version numbers of the kernel
it was built for. Therefore it is advised the completely build the kernel before
starting the module development.

### B) Use a yocto project SDK
See above

### C) Download using a package manager
> apt-get install linux-headers-`uname -r`

## Create build environment for the module

### Kbuild and makefile
To build a module one needs a makefile and a Kbuild file. Before the widespread usage of the Kbuild system
a single makefile was used to build a module, however the modern approach is to separate the 
module description from the make build instructions.

The official module developer guide can help with the creation of these 2 files:
https://www.kernel.org/doc/html/latest/kbuild/modules.html

### Setup the VSCode developement environment.
Clean the kernel module build artifacts.
> make clean

Run the make command in verbose mode and send the output into a file:
> make V=1 > module_build.log

The output file contains in the example 2 gcc calls, since the KBuild file first instructed the build system to create an object file from the main source code, and then to create a module *.ko file from the object files - in our case one one object file.

As an example here is the first gcc call, that compiles the *_main.c file into an object file:

> gcc -Wp,-MMD,/home/tibi/module_dev/my_module/.my_module_main.o.d  -nostdinc 
>-I./arch/x86/include 
>-I./arch/x86/include/generated  
>-I./include 
>-I./arch/x86/include/uapi 
>-I./arch/x86/include/generated/uapi 
>-I./include/uapi 
>-I./include/generated/uapi 
>-Include ./include/linux/compiler-version.h 
>-Include ./include/linux/kconfig.h 
>-Include ./include/linux/compiler_types.h 
>-D__KERNEL__ 
>-Werror -fmacro-prefix-map=./= -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE -Werror=implicit-function-declaration -Werror=implicit -Int -Werror=return-type -Wno-format-security -funsigned-char -std=gnu11 -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fcf-protection=branch -fno-jump-tables -m64 -falign-jumps=1 -falign-loops=1 -mno-80387 -mno-fp-ret -In-387 -mpreferred-stack-boundary=3 -mskip-rax-setup -mtune=generic -mno-red-zone -mcmodel=kernel -Wno-sign-compare -fno-asynchronous-unwind-tables -mindirect-branch=thunk-extern -mindirect-branch-register -mfunction-return=thunk-extern -fno-jump-tables -fpatchable-function-entry=16,16 -fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member -O2 --param=allow-store-data-races=0 -Wframe-larger-than=2048 -fstack-protector-strong -Wno-main -Wno-unused-but-set-variable -Wno-unused-const-variable -fomit-frame-pointer -fno-stack-clash-protection -falign-functions=16 -Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wcast-function-type -Wno-stringop-truncation -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 -fno-strict-overflow -fno-stack-check -fconserve-stack -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated -Init -Wno-packed-not-aligned 
>-I/home/tibi/module_dev/my_module  
>-DMODULE  -DKBUILD_BASENAME='"my_module_main"' -DKBUILD_MODNAME='"my_module"' -D__KBUILD_MODNAME=kmod_my_module 
>-c -o /home/tibi/module_dev/my_module/my_module_main.o /home/tibi/module_dev/my_module/my_module_main.c  



Newertheless the include paths:
>     -I./arch/x86/include 
>     -I./arch/x86/include/generated  
>     -I./include 
>     -I./arch/x86/include/uapi 
>     -I./arch/x86/include/generated/uapi 
>     -I./include/uapi 
>     -I./include/generated/uapi 
>     -I/home/tibi/module_dev/my_module  

Distinct include files that were included for every source file additionally:
>     -Include ./include/linux/compiler-version.h 
>     -Include ./include/linux/kconfig.h 
>     -Include ./include/linux/compiler_types.h 

Define macros:
>     -D__KERNEL__ 
>     -DMODULE 
>     -DKBUILD_BASENAME='"my_module.mod"'
>     -DKBUILD_MODNAME='"my_module"'
>     -D__KBUILD_MODNAME=kmod_my_module 

We can add these config parameters to the c_cpp_properties.json file so that the C/C++ instellisense can work correctly with the module source files."

Define the following environement variables:
>     "env": {
>         "KDIR" : "/home/tibi/mainline_kernel",
>         "ARCH" : "x86",
>         "MODNAME" : "my_module"
>     },

Define the following include directories:
>     "includePath": [
>         "${workspaceFolder}/**",                
>         // Kernel source dir include paths
>         "${KDIR}/arch/${ARCH}/include",
>         "${KDIR}/arch/${ARCH}/include/generated",
>         "${KDIR}/include",
>         "${KDIR}/arch/${ARCH}/include/uapi",
>         "${KDIR}/arch/${ARCH}/include/generated/uapi",
>         "${KDIR}/include/uapi",
>         "${KDIR}/include/generated/uapi"
>     ],

Add the following forced include files:
>     "forcedInclude": [
>         "${KDIR}/include/linux/compiler-version.h",
>         "${KDIR}/include/linux/kconfig.h",
>         "${KDIR}/include/linux/compiler_types.h"
>     ],

Add the following defines:
>     "defines": [
>         "__KERNEL__",
>         "MODULE",
>         "KBUILD_BASENAME='\"${MODNAME}.mod\"'",
>         "KBUILD_MODNAME='\"${MODNAME}\"'",
>         "__KBUILD_MODNAME=kmod_${MODNAME}"
>     ],