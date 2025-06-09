#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <vector>

// Yardım menüsü
void printHelp() 
{
    std::cout << "anitr-cli kullanımı:\n"
              << "  --help, -h: Bu yardım menüsünü gösterir\n"
              << "  --gen-config: rofi-flags.conf dosyasını oluşturur\n"
              << "\n";
}

// rofi.flags.conf dosyasını oluşturacak funksiyon
void generateConfigFile() 
{
    std::string configDir = std::string(getenv("HOME")) + "/.config/anitr-cli";
    std::string configFile = configDir + "/rofi-flags.conf";

    // Klasörü oluştur (varsa atla)
    std::filesystem::create_directories(configDir);

    // Konfigürasyon dosyası varsa, uyarı göster ve çık
    if (std::filesystem::exists(configFile)) 
    {
        std::cout << "Konfigürasyon dosyası zaten var: " << configFile << "\n";
        return;
    }

    // Varsayılan parametrelerle dosya oluştur
    std::ofstream config(configFile);
    if (config.is_open()) 
    {
        config << "";
        config.close();
        std::cout << "Konfigürasyon dosyası oluşturuldu: " << configFile << "\n";
    } 
    
    else 
    {
        std::cerr << "Konfigürasyon dosyası oluşturulamıyor: " << configFile << "\n";
    }
}


int main(int argc, char* argv[]) {
    // Komut satırı parametrelerini kontrol et
    for (int i = 1; i < argc; ++i) 
    {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") 
        {
            printHelp();
            return 0;
        } 
        
        else if (arg == "--gen-config") 
        {
            generateConfigFile();
            return 0;
        }
    }

    std::vector<std::string> main_menu_options = {
	    "İzle", "Sonraki Bölüm", "Önceki Bölüm", "Bölüm Seç", "Anime Ara", "Çık"
    };

    return 0;
}
