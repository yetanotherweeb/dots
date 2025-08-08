if status is-interactive
    # Commands to run in interactive sessions can go here
    fastfetch --config ascii-art.jsonc
    set fish_greeting

end

#starship init fish | source
set -Ux PULSARSHIP_CONFIG ~/.config/pulsarship/pulsarship.toml
pulsarship init fish | source
if test -f ~/.local/state/quickshell/user/generated/terminal/sequences.txt
    cat ~/.local/state/quickshell/user/generated/terminal/sequences.txt
end

source ~/.aliases
alias pamcan pacman
alias ls 'eza --icons --all'
alias clear "printf '\033[2J\033[3J\033[1;1H'"
alias q 'qs -c ii'
