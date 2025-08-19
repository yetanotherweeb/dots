local map = vim.keymap.set -- Daha kısa yazım için

-- Normal modda kopyala/yapıştır
map("n", "<C-c>", '"+y') -- Ctrl+C ile satırı kopyala (system clipboard)
map("v", "<C-c>", '"+y') -- Seçili alanı kopyala
map("n", "<C-v>", '"+p') -- Ctrl+V ile yapıştır
map("v", "<C-v>", '"+p')

-- Undo/Redo
map("n", "<C-z>", "u") -- Undo
map("n", "<C-y>", "<C-r>") -- Redo

-- Satır taşımak (normal mod)
map("n", "<C-Up>", ":m .-2<CR>==")
map("n", "<C-Down>", ":m .+1<CR>==")

-- Neovim’den çıkmak
map("n", "<C-q>", ":q<CR>")
map("v", "<C-q>", ":q<CR>")
map("i", "<C-q>", "<Esc>:q<CR>")

-- Alt + left/right ile split arasında geçiş
map("n", "<A-Left>", "<C-w>h")
map("n", "<A-Right>", "<C-w>l")

-- VSCode benzeri eklemeler:

-- Insert modda Ctrl+C kopyala / Ctrl+V yapıştır
map("i", "<C-c>", '<Esc>"+y') -- insert moddan kopyala
map("i", "<C-v>", "<C-r>+") -- insert modda yapıştır

-- Dosya kaydetmek
map("n", "<C-s>", ":w<CR>") -- Normal modda Ctrl+S
map("i", "<C-s>", "<Esc>:w<CR>a") -- Insert modda Ctrl+S

-- Tümünü seçmek (Visual mod benzeri)
map("n", "<C-a>", "ggVG") -- Normal modda Ctrl+A tümünü seç

-- Buffer/geçiş kolaylığı
map("n", "<C-Tab>", ":bnext<CR>") -- sonraki buffer
map("n", "<C-S-Tab>", ":bprevious<CR>") -- önceki buffer

-- Yeni split açmak
map("n", "<C-\\>", ":vsplit<CR>") -- Ctrl+\ ile dikey split
map("n", "<C-->", ":split<CR>") -- Ctrl+- ile yatay split

-- Seçili text ile indent/unindent (Visual mod)
map("v", "<Tab>", ">gv") -- Seçili alanı sağa kaydır
map("v", "<S-Tab>", "<gv") -- Seçili alanı sola kaydır

-- Visual modda seçili alanı silmek
map("v", "<BS>", '"_d') -- Backspace ile seçili text’i sil (yeni register’a gitmeden)
map("v", "<Del>", '"_d') -- Delete ile seçili text’i sil
