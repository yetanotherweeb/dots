#!/bin/sh

# PipeWire launcher script for Artix Linux (XDG-compliant sessions)

# Launch a new D-Bus session if not already set
if [ -z "$DBUS_SESSION_BUS_ADDRESS" ]; then
    eval "$(dbus-launch --sh-syntax --exit-with-session)"
fi

# Kill any previously running instances to avoid conflicts
pkill -u "${USER}" -x pipewire\|pipewire-pulse\|wireplumber 1>/dev/null 2>&1

# Start PipeWire core daemon
/usr/bin/pipewire & disown

# Start PulseAudio compatibility layer
/usr/bin/pipewire-pulse & disown

# Wait for the PipeWire socket to appear (max 5 seconds)
tries=0
while [ ! -e /run/user/$(id -u)/pipewire-0 ] && [ $tries -lt 50 ]; do
    sleep 0.1
    tries=$((tries + 1))
done

# Small delay to ensure services are fully ready
sleep 0.5

# Start WirePlumber session manager
/usr/bin/wireplumber & disown

exit 0
