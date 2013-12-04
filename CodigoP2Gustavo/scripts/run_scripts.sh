#!/bin/bash

# Gera os automatos a partir do WIRTH

rm -rf output
mkdir output
python3 downloadJflaps.py

# Gera as imagens dos automatos

rm -rf images
mkdir images
./run_automatic_jflaps.sh

# Gera o latex pra parte da P2

cd latex
python3 latexator.py > parte1.tex
pdflatex parte1.tex
cd ..
