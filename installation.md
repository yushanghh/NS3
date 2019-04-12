---
layout: page
title: Installation
permalink: /installation
---

## 1. Pre-requisites
Before proceeding you will need: 

    1. git
    2. g++
    3. make or ninja
    4. cmake
    5. unzip
    6. curl 
    7. tar
Those are basic pre-requisites for both ns-3 and VcPkg, used in this version of ns-3 for dependencies installation. The [official ns-3 branch](https://gitlab.com/nsnam/ns-3-dev/) uses Bake for dependencies installation.

## 2. Fetching the code
After installing the dependencies, clone the [ns-3 with CMake](https://github.com/Gabrielcarvfer/NS3) repository.
```
git clone https://github.com/Gabrielcarvfer/NS3.git
```

## 3. CMake configuration 
You can choose either command line tools or IDEs that support CMake projects (e.g. [Jetbrains CLion](https://www.jetbrains.com/clion/) and [Visual Studio](https://visualstudio.microsoft.com/) ).


### 3.1 CMake configuration with command line 
Navigate to the cloned NS3 folder, create a CMake cache folder, navigate to it and run [CMake](https://cmake.org/cmake/help/latest/manual/cmake.1.html) pointing to the NS3 folder.
```
cd NS3
mkdir cmake-cache
cd cmake-cache
cmake -DCMAKE_BUILD_TYPE=DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL -G"MinGW|Unix|MSYS Makefiles" ..
```
You can pass arguments to the CMake command, like the [CMAKE_BUILD_TYPE](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html) and choose [buildsystem generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html) with the -G flag.

For the ns-3 options, I recommend editting the default values of the switches in the NS3/CMakeLists.txt file.

or you can use an IDE that supports CMake projects (Jetbrains CLion is the best one), that will load and run CMake automatically after opening the project folder.

During the `cmake` command, [VcPkg](https://github.com/Microsoft/vcpkg) will be compiled and then will download and build required dependencies. This will take some time.

### 3.2 CMake configuration with CLion


## 4 Building the project
After the configuration, you will have multiple targets (for libraries and executables). Again, you can choose either command line tools or IDEs that support CMake projects to build and link those targets.


### 4.1 Building the project with command line
If you selected the Makefiles generator, you can run the `make` command to build specific targets and `make all` to build all targets.
You can also run `make clean` to remove built libraries and executables before rebuilding them.

### 4.2 Building the project with CLion


## 5. Running built executables
After building executables, they will be placed in NS3/build/bin. To run them, you can either use the command line or your prefered IDE.

### 5.1 Running executables with command line
Navigate to NS3/build/bin and run the executable.
```
cd ../build/bin
./executable_name
```
If you want to rebuild it, you will need to navigate back to the cmake-cache folder.
```
cd ../../cmake-cache
```

### 5.2 Running executables with CLion

## 6. Creating a new module
As [Waf](https://waf.io/) (the official buildsystem) was replaced with CMake, the required steps to create a new module for the ns-3 change slightly.

Instead of a wscript per module, you will need a CMakeLists.txt file. As an example, let's take the Applications module wscript and compare to the CMakeLists.txt that replaced it.

### Applications Wscript
```
def build(bld):
    module = bld.create_ns3_module('applications', ['internet', 'config-store','stats'])
    module.source = [
        'model/bulk-send-application.cc',
        ...
        ]

    applications_test = bld.create_ns3_module_test_library('applications')
    applications_test.source = [
        'test/three-gpp-http-client-server-test.cc', 
        ...
        ]

    headers = bld(features='ns3header')
    headers.module = 'applications'
    headers.source = [
        'model/bulk-send-application.h',
        ...
        ]
```

### Applications CMakeLists.txt
```


set(name applications)

set(source_files
        model/bulk-send-application.cc
        ...
        )

set(header_files
        model/bulk-send-application.h
        ...
        )

#link to dependencies
set(libraries_to_link
        ${libinternet}
        ${libconfig-store}
        ${libstats}
        )

set(test_sources
        test/udp-client-server-test.cc
        ...
    )

build_lib("${name}" "${source_files}" "${header_files}" "${libraries_to_link}" "${test_sources}")
```

After you finish working on your CMakeLists.txt, add your new module to the libs_to_build list in the NS3/CMakeLists.txt file.

Reconfigure your CMake project (step 3) to automatically copy the headers to NS3/build/NS3 and create the appropriate buildsystem files to build your module related targets.