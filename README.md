`cmake_run` Documentation
## Description

cmake_run is a tool that simplifies the CMake build process for executables. This tool allows you to define a CMake project, set target directories, and initiate the build process with simple commands. It also supports automatic creation of target directories and CMakeLists.txt files.

## Installation

To install `cmake_run`, follow these steps:

1. Download `cmake_run` from the GitHub repository.

```bash
git clone https://github.com/your_username/cmake_run.git
```

2. Navigate to the `cmake_run` directory:

```bash
cd cmake_run
```

3. Run the installation script

```bash
./install.sh
```

## Usage
### Basic Usage

To build an executable using cmake_run, use the following command:

```bash
crun
```

This command will automatically look for a CMakeLists.txt file in the current directory and initiate the build process.

### Specifying Target Directory

You can specify a target directory using the -t or --target option. If the specified directory exists, cmake_run will change to that directory; otherwise, it will create the directory and set it as the target.

```bash
crun -t /path/to/target_directory
```

### Additional Argument

You can pass an additional argument to cmake_run. This argument will be used as a folder name, and the tool will change to that folder before initiating the build process. If the folder does not exist, cmake_run will create it and set it as the target directory.

```bash
crun my_project
```

In this example, my_project is treated as the folder name, and cmake_run will look for or create a folder with that name and set it as the target directory.