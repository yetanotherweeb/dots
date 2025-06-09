from typing import Any
from kitty.boss import Boss  # type: ignore
from kitty.window import Window  # type: ignore

def set_padding(window: Window, padding: int = 0):
    window.set_padding(padding, padding, padding, padding)

def on_resize(boss: Boss, window: Window, data: dict[str, Any]) -> None:
    with open("/tmp/kitty_padding_debug.txt", "a") as f:
        f.write(f"resize: {window.id}\n")
        try:
            set_padding(window, 0)
            f.write(f"{window.id} için padding=0\n")
        except Exception as e:
            f.write(f"resize HATA: {e}\n")
