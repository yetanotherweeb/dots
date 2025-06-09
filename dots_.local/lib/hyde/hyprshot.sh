#!/bin/bash

selected_area=$(slurp)

grim -g "$selected_area" - | swappy -f - -o ~/Pictures/Screenshots/$(date '+%Y-%m-%d_%H-%M-%S').png
