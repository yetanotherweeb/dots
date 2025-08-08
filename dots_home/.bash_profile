# ~/.bash_profile

# Start DBus if not already running
if [ -z "$DBUS_SESSION_BUS_ADDRESS" ]; then
    eval "$(dbus-launch --sh-syntax)"
fi

# Source .profile if it exists
[ -f ~/.profile ] && . ~/.profile

# Start Hyprland if it's not already running
pgrep -x Hyprland >/dev/null || exec Hyprland
