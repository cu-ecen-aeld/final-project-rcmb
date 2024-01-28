#!/bin/sh
# Shared definitions for buildroot scripts

# The defconfig from the buildroot directory we use for Rasp2 builds
RASP2_DEFCONFIG=configs/raspberrypi2_defconfig
# The place we store customizations to the Rasp2 configuration
MODIFIED_RASP2_DEFCONFIG=app_external/configs/sensor_rasp2_defconfig
# The defconfig from the buildroot directory we use for the project
SENSOR_DEFAULT_DEFCONFIG=${RASP2_DEFCONFIG}
SENSOR_MODIFIED_DEFCONFIG=${MODIFIED_RASP2_DEFCONFIG}
SENSOR_MODIFIED_DEFCONFIG_REL_BUILDROOT=../${SENSOR_MODIFIED_DEFCONFIG}
