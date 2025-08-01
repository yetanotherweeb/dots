location=""

for arg in "$@"; do
    if [[ "$arg" =~ ^location=(.*)$ ]]; then
        # location değerini ayıkla
        location="${BASH_REMATCH[1]}"
    fi
done

ROFI_SCRIPTS="$HOME/.config/rofi/scripts"
echo "$location"
bash $ROFI_SCRIPTS/menus "$1" "location=$location"
