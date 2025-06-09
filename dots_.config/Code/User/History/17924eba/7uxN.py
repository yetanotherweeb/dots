import os
import time
import openai
from tqdm import tqdm

# === AYARLAR ===
SOURCE_FILE = "input.txt"
OUTPUT_FILE = "output.txt"
CHUNK_SIZE = 3500  # karakter sınırı
TARGET_LANG = "TR"

# === API Anahtarı ===
openai.api_key = os.getenv("OPENAI_API_KEY")  # Güvenli kullanım için çevre değişkeninden alınır

# === Yardımcı Fonksiyonlar ===
def read_chunks(filename, max_len=CHUNK_SIZE):
    with open(filename, "r", encoding="utf-8") as f:
        text = f.read()
    chunks = []
    while len(text) > max_len:
        split_point = text.rfind("\n", 0, max_len)
        if split_point == -1:
            split_point = max_len
        chunks.append(text[:split_point])
        text = text[split_point:].lstrip()
    chunks.append(text)
    return chunks

def translate_with_gpt(text, target="TR"):
    system_msg = f"Translate the following text into {target} in a literary and accurate way."
    try:
        response = openai.chat.completions.create(
            model="gpt-4",
            messages=[
                {"role": "system", "content": system_msg},
                {"role": "user", "content": text}
            ],
            temperature=0.4
        )
        return response.choices[0].message.content.strip()
    except Exception as e:
        print("[!] GPT Çeviri Hatası:", e)
        return "[HATA]"

# === Ana Çalışma Fonksiyonu ===
def translate_file():
    chunks = read_chunks(SOURCE_FILE)
    print(f"[+] Toplam {len(chunks)} parça bulundu.")

    with open(OUTPUT_FILE, "w", encoding="utf-8") as out:
        for i, chunk in enumerate(tqdm(chunks, desc="Çeviri")):
            time.sleep(1)  # API rate limit için bekleme
            translated = translate_with_gpt(chunk, TARGET_LANG)
            out.write(translated + "\n\n")

    print(f"[✓] Çeviri tamamlandı: {OUTPUT_FILE}")

# === Çalıştır ===
if __name__ == "__main__":
    translate_file()
