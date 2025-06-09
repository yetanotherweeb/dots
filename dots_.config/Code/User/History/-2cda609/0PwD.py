from typing import Any
from kitty.boss import Boss  # type: ignore
from kitty.window import Window  # type: ignore

def set_padding(window: Window, padding: int):
    try:
        window.set_padding(padding, padding, padding, padding)
    except Exception as e:
        with open("/tmp/kitty_padding_debug.txt", "a") as f:
            f.write(f"HATA set_padding: {e}\n")

def on_set_user_var(boss: Boss, window: Window, data: dict[str, Any]) -> None:
    with open("/tmp/kitty_padding_debug.txt", "a") as f:
        f.write(f"on_set_user_var: {data}\n")

    if data["key"] == "in_editor":
        value = data.get("value", "").lower()
        if value == "true":
            set_padding(window, 0)
        else:
            set_padding(window, 8)
