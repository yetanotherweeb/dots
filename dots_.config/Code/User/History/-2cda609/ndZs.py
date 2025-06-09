from kitty.boss import Boss  # type: ignore
from kitty.window import Window  # type: ignore
from typing import Any

def on_load(boss: Boss, data: dict[str, Any]) -> None:
    with open("/tmp/kitty_padding_debug.txt", "a") as f:
        f.write("on_load çalıştı\n")
    
    # Tüm açık pencerelere padding uygula
    for os_window in boss.windows:
        try:
            os_window.set_padding(0, 0, 0, 0)
            f.write(f"{os_window.id} için padding 0 ayarlandı\n")
        except Exception as e:
            f.write(f"Hata: {str(e)}\n")
