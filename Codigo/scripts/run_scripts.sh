#!/bin/bash

# Gera os automatos a partir do WIRTH

rm -rf output
mkdir output
python3 downloadJflaps.py

# Gera as imagens dos automatos

rm -rf images
mkdir images
./run_automatic_jflaps.sh
