# Fermentation Tank Monitor

Project to implement a monitoring solution for a fermentation tank.

[Project Overview](https://github.com/cu-ecen-aeld/final-project-rcmb/wiki/Project%E2%80%90Overview).

## Table of Contents

- [Building](#building)
- [Usage](#usage)
- [License](#license)

## Building

1.Clone repository and the buildroot submodule

`git clone --recurse-submodules https://github.com/cu-ecen-aeld/final-project-rcmb.git`

2.`cd [repository-directory]\buildroot`

3.`make BR2_EXTERNAL=[repository-directory]/ECEA5307  O=[build-directory] raspberrypi2_defconfig  menuconfig`

4.Select EXTERNAL -> SENSORS to include the sensor code into the build. This will write the config to [build-directory]/.config file on save/exit.

5.Build the image using : `make BR2_EXTERNAL=[repository-directory]/app_external/  O=[build-directory]`

## Usage

Instructions on how to use the project and any relevant examples.

## License

[MIT](https://mit-license.org/).
