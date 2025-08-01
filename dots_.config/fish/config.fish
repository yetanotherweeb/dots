set -g fish_greeting ""
source ~/.aliases

set -Ux PULSARSHIP_SHELL fish
set -Ux PULSARSHIP_CONFIG ~/.config/pulsarship/pulsarship.toml
pulsarship init fish | source

/usr/bin/fastfetch --config arch

if status is-interactive
    # Commands to run in interactive sessions can go here
end

# bun
set --export BUN_INSTALL "$HOME/.bun"
set --export PATH $BUN_INSTALL/bin $PATH
