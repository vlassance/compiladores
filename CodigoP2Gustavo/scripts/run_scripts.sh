#!/bin/bash

rm -rf output
mkdir output
python3 downloadJflaps.py

rm -rf images
mkdir images
./run_automatic_jflaps.sh
