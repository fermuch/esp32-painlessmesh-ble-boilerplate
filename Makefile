#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp32-painlessmesh-ble
IDF_PATH		 := ./vendor/esp-idf/

# add xtensa to the $PATH
PATH  := $(PATH):$(PWD)/vendor/xtensa-esp32-elf/bin
# for older shells, set the correct $PATH
SHELL := env PATH=$(PATH) /bin/bash


include $(IDF_PATH)/make/project.mk