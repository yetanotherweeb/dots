#!/bin/bash

PACMAN_FILE="pacman-pkgs.txt"
AUR_FILE="aur-pkgs.txt"

# Function to install Pacman packages
install_pacman_packages() {
    echo "Installing Pacman packages..."
    if [[ -f $PACMAN_FILE ]]; then
        sudo pacman -S --needed - < "$PACMAN_FILE"
    else
        echo "Error: $PACMAN_FILE not found!"
    fi
}

# Function to install AUR packages
install_aur_packages() {
    echo "Installing AUR packages..."
    if [[ -f $AUR_FILE ]]; then
        yay -S --needed - < "$AUR_FILE"
    else
        echo "Error: $AUR_FILE not found!"
    fi
}

# Main menu
echo "Please select an option:"
echo "1. Install Pacman packages"
echo "2. Install AUR packages"
echo "3. Install all packages"

read -rp "Enter your choice (1-3): " choice

case $choice in
    1)
        install_pacman_packages
        ;;
    2)
        install_aur_packages
        ;;
    3)
        echo "Starting with Pacman packages..."
        install_pacman_packages
        echo "Now installing AUR packages..."
        install_aur_packages
	echo "Finished!"
        ;;
    *)
        echo "Invalid option. Exiting."
        exit 1
        ;;
esac

