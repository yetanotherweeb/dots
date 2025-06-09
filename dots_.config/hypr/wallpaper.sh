#!/bin/bash

# Arka plan resimlerinin bulunduğu dizin
WALLPAPER_DIR="$HOME/Resimler/Wallpaper/"

# Arka plan resimlerinin listesini alın
WALLPAPERS=("$WALLPAPER_DIR"/*)

# Kaç tane resim varsa onu belirleyin
NUM_WALLPAPERS=${#WALLPAPERS[@]}

while true; do
  for ((i=0; i<$NUM_WALLPAPERS; i++)); do
    # Arka plan resmini değiştir
    hyprpaper wallpaper "${WALLPAPERS[$i]}"
    
    # 10 dakika bekle (600 saniye)
    sleep 10
  done
done
