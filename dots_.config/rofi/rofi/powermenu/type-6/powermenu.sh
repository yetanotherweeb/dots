#!/usr/bin/env bash

dir="$1"
theme="$2"

# CMDs
lastlogin="$(last $USER | head -n1 | tr -s ' ' | cut -d' ' -f5,6,7)"
uptime="$(uptime -p | sed -e 's/up //g')"
host=$(hostname)

# Options
hibernate=''
shutdown=''
reboot=''
lock=''
suspend=''
logout=''
yes=''
no=''

location=0

for arg in "$@"; do
    if [[ "$arg" =~ ^location=(.*)$ ]]; then
        location="${BASH_REMATCH[1]}"
    fi
done

# Rofi CMD
rofi_cmd() {
    rofi -dmenu \
        -p " $USER@$host" \
        -mesg " Uptime: $uptime" \
        -theme ${dir}/${theme}.rasi \
        -location $location
}

# Confirmation CMD
confirm_cmd() {
    rofi -theme-str 'window {anchor: center; fullscreen: false; width: 350px;}' \
        -theme-str 'mainbox {orientation: vertical; children: [ "message", "listview" ];}' \
        -theme-str 'listview {columns: 2; lines: 1;}' \
        -theme-str 'element-text {horizontal-align: 0.5;}' \
        -theme-str 'textbox {horizontal-align: 0.5;}' \
        -dmenu \
        -p 'Confirmation' \
        -mesg 'Are you Sure?' \
        -theme ${dir}/${theme}.rasi \
        -location $location
}

# Ask for confirmation
confirm_exit() {
    echo -e "$yes\n$no" | confirm_cmd
}

# Pass variables to rofi dmenu
run_rofi() {
    echo -e "$lock\n$suspend\n$logout\n$hibernate\n$reboot\n$shutdown" | rofi_cmd
}

# Execute Command
run_cmd() {
    selected="$(confirm_exit)"
    if [[ "$selected" == "$yes" ]]; then
        if [[ $1 == '--shutdown' ]]; then
            systemctl poweroff
        elif [[ $1 == '--reboot' ]]; then
            systemctl reboot
        elif [[ $1 == '--hibernate' ]]; then
            systemctl hibernate
        elif [[ $1 == '--suspend' ]]; then
            systemctl suspend
        elif [[ $1 == '--logout' ]]; then
            hyprctl dispatch exit
        fi
    else
        exit 0
    fi
}

# Actions
chosen="$(run_rofi)"
case ${chosen} in
$shutdown)
    run_cmd --shutdown
    ;;
$reboot)
    run_cmd --reboot
    ;;
$hibernate)
    run_cmd --hibernate
    ;;
$lock)
    hyprlock
    ;;
$suspend)
    run_cmd --suspend
    ;;
$logout)
    run_cmd --logout
    ;;
esac
