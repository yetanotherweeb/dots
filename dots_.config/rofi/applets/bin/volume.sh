type="$1"
style="$2.rasi"
theme="$type/$style"

location=0

for arg in "$@"; do
    if [[ "$arg" =~ ^location=(.*)$ ]]; then
        location="${BASH_REMATCH[1]}"
    fi
done

# Volume Info
mixer="$(amixer info Master | grep 'Mixer name' | cut -d':' -f2 | tr -d \',' ')"
speaker="$(amixer get Master | tail -n1 | awk -F ' ' '{print $5}' | tr -d '[]')"
mic="$(amixer get Capture | tail -n1 | awk -F ' ' '{print $5}' | tr -d '[]')"

active=""
urgent=""

# Speaker Info
amixer get Master | grep '\[on\]' &>/dev/null
if [[ "$?" == 0 ]]; then
    #active="-a 1"
    stext='Mute'
    sstatus='Unmuted'
    sicon=''
else
    #active="-u 1"
    stext='Unmute'
    sstatus='Muted'
    sicon=''
fi

# Microphone Info
amixer get Capture | grep '\[on\]' &>/dev/null
if [[ "$?" == 0 ]]; then
    mtext='Mute'
    mstatus='Unmuted'
    micon=''
else
    mtext='Unmute'
    mstatus='Muted'
    micon=''
fi

# Theme Elements
prompt="S:${sstatus}, M:${mstatus}"
mesg="$mixer - Speaker: $speaker, Mic: $mic"

if [[ "$theme" == *'type-1'* ]]; then
    list_col='1'
    list_row='5'
    win_width='400px'
elif [[ "$theme" == *'type-3'* ]]; then
    list_col='1'
    list_row='5'
    win_width='120px'
elif [[ "$theme" == *'type-5'* ]]; then
    list_col='1'
    list_row='5'
    win_width='520px'
elif [[ ("$theme" == *'type-2'*) || ("$theme" == *'type-4'*) ]]; then
    list_col='5'
    list_row='1'
    win_width='670px'
fi

# Options
layout=$(cat ${theme} | grep 'USE_ICON' | cut -d'=' -f2)
if [[ "$layout" == 'NO' ]]; then
    option_1=" Increase"
    option_2=" Decrease"
    option_3="$sicon $stext"
    option_4="$micon $mtext"
    option_5=" Settings"
else
    option_1=""
    option_2=""
    option_3="$sicon"
    option_4="$micon"
    option_5=""
fi

# Rofi CMD
rofi_cmd() {
    rofi -theme-str "window {width: $win_width;}" \
        -theme-str "listview {columns: $list_col; lines: $list_row;}" \
        -theme-str 'textbox-prompt-colon {str: "";}' \
        -dmenu \
        -p "$prompt" \
        -mesg "$mesg" \
        ${active} ${urgent} \
        -markup-rows \
        -theme ${theme} \
        -location $location
}

# Pass variables to rofi dmenu
run_rofi() {
    echo -e "$option_1\n$option_2\n$option_3\n$option_4\n$option_5" | rofi_cmd
}

# Execute Command
run_cmd() {
    if [[ "$1" == '--opt1' ]]; then
        amixer -Mq set Master,0 5%+ unmute
    elif [[ "$1" == '--opt2' ]]; then
        amixer -Mq set Master,0 5%- unmute
    elif [[ "$1" == '--opt3' ]]; then
        amixer set Master toggle
    elif [[ "$1" == '--opt4' ]]; then
        amixer set Capture toggle
    elif [[ "$1" == '--opt5' ]]; then
        pavucontrol
    fi
}

# Actions
chosen="$(run_rofi)"
case ${chosen} in
$option_1)
    run_cmd --opt1
    ;;
$option_2)
    run_cmd --opt2
    ;;
$option_3)
    run_cmd --opt3
    ;;
$option_4)
    run_cmd --opt4
    ;;
$option_5)
    run_cmd --opt5
    ;;
esac
