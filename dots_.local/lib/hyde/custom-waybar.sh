#!/usr/bin/env bash

# Toggle durumu kaydedilen dosya
state_file="$HOME/.config/waybar/.waybar_toggle"

# Scriptin mutlak yolu (kendini yeniden çağırmak için)
script_path="$(realpath "$0")"

# Eğer argüman verilmişse doğrudan çalıştır
if [ "$1" == "e" ]; then
    killall waybar 2>/dev/null
    waybar --config ~/.config/waybar/custom-config.jsonc --style ~/.config/waybar/custom-style.css &
    echo "waybar --config ~/.config/waybar/custom-config.jsonc --style ~/.config/waybar/custom-style.css &" > ~/.local/lib/hyde/waybar.sh
    chmod +x ~/.local/lib/hyde/waybar.sh
    exit 0
elif [ "$1" == "d" ]; then
    killall waybar 2>/dev/null
    waybar --config ~/.config/waybar/config.jsonc --style ~/.config/waybar/style.css &
    echo "waybar --config ~/.config/waybar/config.jsonc --style ~/.config/waybar/style.css &" > ~/.local/lib/hyde/waybar.sh
    chmod +x ~/.local/lib/hyde/waybar.sh
    exit 0
fi

# Argüman yoksa: toggle modu
# Dosya yoksa "e" ile başla
if [ ! -f "$state_file" ]; then
    echo "e" > "$state_file"
fi

state=$(cat "$state_file")

if [ "$state" == "e" ]; then
    "$script_path" d
    echo "d" > "$state_file"
else
    "$script_path" e
    echo "e" > "$state_file"
fi
