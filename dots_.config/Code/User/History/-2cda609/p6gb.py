from typing import Any
from kitty.boss import Boss  # type: ignore
from kitty.window import Window  # type: ignore

def on_load(boss: Boss, data: dict[str, Any]) -> None:
    for tab in boss.tabs:
        for window in tab.windows:
            set_padding(window)

def set_padding(window: Window) -> None:
    window.set_padding(0, 0, 0, 0)