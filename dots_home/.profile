# Call the script that starts PipeWire and related services
#if [ -x "$HOME/.local/share/scripts/artix-pipewire.sh" ]; then
#    $HOME/.local/share/scripts/artix-pipewire.sh
#fi

# Start user runit services if not already running
#if ! pgrep -u "$USER" -f "runsvdir -P $HOME/.runit/service" > /dev/null 2>&1; then
#    runsvdir -P "$HOME/.runit/service" &
#fi
