# ptwWORKS
#
# This is the Readme for the Pin-to-Win system, a pinball system that is capable of running itself. It uses light sensors that send analog 
# values in the range 0-255 and based on variations in this it can fire the flippers. 
# The systems uses two matrixes for this. One offset and one of true values. 
#
# Project requires Makefile to be integrated in rspi system, guide for creating new Makefile and relinking is here if needed:
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# https://www.softwaretestinghelp.com/cpp-makefile-tutorial/
# If these are outdated you will have to google around. 
# 
# Detection values are modified in the header file but should be changed and renamed. Current naming scheme was done for time and convinience.
#
# FUNCTIONS
#
# sensorReadLeft and sensorReadRight creates two matrixes and compares them. one (baseLvlMtx(R/L)) is an offset of the main matrix (i2dArr) 
# and decides whether or not to fire the left or right flipper. baseLvlMtx values should be reworked into header file.
#
# setLightLevel is no longer nescessary because of this, but is left since it may have other applications.
#
