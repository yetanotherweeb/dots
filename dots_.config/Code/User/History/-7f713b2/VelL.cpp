#include <iostream>
#include <cstdlib>

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
    
    int choice;
    std::cout << "Bir uygulama seçin:\n";
    std::cout << "1. AnimeciX\n";
    std::cout << "2. OpenAnime\n";
    std::cout << "Seçiminiz: ";
    std::cin >> choice;

    if (choice == 1) {
        system("./animecix-cli"); // AnimeciX uygulamasını başlat
    } else if (choice == 2) {
        system("./openanime-cli"); // OpenAnime uygulamasını başlat
    } else {
        std::cout << "Geçersiz seçim.\n";
    }

    return 0;
}
