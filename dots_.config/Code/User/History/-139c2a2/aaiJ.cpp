#include "modules/FetchOpenAnime.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <tuple>
#include <optional>
#include <future>

using json = nlohmann::json;
FetchData fetchdata;

// Ana menü seçenekleri
std::vector<std::string> main_menu_options = {
	"İzle", "Sonraki Bölüm", "Önceki Bölüm", "Bölüm Seç", "Anime Ara", "Çık"
};

std::string movie_url;
bool is_movie;

// Kullanıcıdan rofi ile giriş alacak fonksiyon
std::string getInputFromRofi(const std::string& prompt, const std::vector<std::string>& options) 
{
    // Rofi parametrelerini okumak için konfigürasyon dosyasını kontrol et
    std::string rofi_flags = "";
    std::ifstream config_file(std::string(getenv("HOME")) + "/.config/anitr-cli/rofi-flags.conf");

    if (config_file.is_open()) 
    {
        std::string line;
        while (std::getline(config_file, line)) 
        {
            // Eğer satır boş değilse ve ya başında # yoksa
            if (!line.empty() && line[0] != '#') 
            {
                rofi_flags += line + " ";  // Parametreleri birleştir
            }
        }
        
        config_file.close();
    }

    // Rofi komutunu oluştur
    std::string rofi_cmd = "{ echo '" + prompt + "\n" + 
                            "\n" + 
                            "[\n" + 
                            "  '" + prompt + "'\n" +
                            "'<back>'\n" +
                            "'<exit>'" + 
                            "']\n\n" ;

    rofi_cmd += "echo -e \"" ;

    for (const auto& option : options) 
    {
        rofi_cmd += option + "\n";
    }

    rofi_cmd += "\" | rofi -dmenu -p '" + prompt + "' " + rofi_flags + "; } 2>/dev/null";  // Çıktıyı gizle

    std::string selected;
    
    FILE* fp = popen(rofi_cmd.c_str(), "r");
    if (fp != NULL) 
    {
        char buffer[1024];
        
        if (fgets(buffer, sizeof(buffer), fp) != NULL) 
        {
            selected = buffer;
        }
        
        fclose(fp);
    }

    // Satır sonu karakterini kaldır
    if (!selected.empty() && selected.back() == '\n') 
    {
        selected.pop_back();
    }

    return selected;
}

std::vector<std::map<std::string, std::string>> queryLoop() 
{
    std::string query;
    std::vector<std::map<std::string, std::string>> results;
    
    // Anime arama prompt'unu döngüye al
    while (true) 
    {
        query = getInputFromRofi("Anime Ara", {"Çık"});

        if (query == "<exit>" || query == "exit" || query == "Çık") 
        {
            exit(0);
        }

        if (query.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos) 
        {
            continue;
        }

        results = fetchdata.fetch_anime_search_data_openani(query);

        if (results.empty()) 
        {
            std::cout << "Sonuç bulunamadı. Tekrar deneyin!" << "\n";
            continue;
        }

        break;
    }

    return results;
}


int main() {

    curl_global_init(CURL_GLOBAL_DEFAULT);

    std::vector<std::tuple<std::string, int, int>> anime_episodes;
    std::tuple<int, std::string, std::optional<int>, bool> anime_seasons;
    int selected_episode_index = 0;

    // Anime arama prompt'u
    std::string query;

    // Arama sonuçlarını al
    std::vector<std::map<std::string, std::string>> results = queryLoop();
    

    std::vector<std::string> anime_names;
    for (const auto& item : results) 
    {
        anime_names.push_back(item.at("name"));
    }

    anime_names.push_back("Çık");

    std::string selected_anime_name;

    while (true) 
    {
        // Anime seçimi
        selected_anime_name = getInputFromRofi("Anime Seç", anime_names);

        // Eğer arama kısmına <exit> ya da exit yazılırsa çık
        if (selected_anime_name == "<exit>" || selected_anime_name == "exit") exit(0);

        // Şartlar
        bool is_not_in_anime_names = std::find(anime_names.begin(), anime_names.end(), selected_anime_name) == anime_names.end();
        bool is_not_exit_command = selected_anime_name != "Çık";
        
        // Eğer selected_anime_name, anime_names içerisindeki bir öğe değilse ve Çık değilse döngüye devam et
        if (is_not_in_anime_names && is_not_exit_command) 
        {
            continue;
        }

        // Döngüden çık
        break;

    }

    // Seçilen animeyi bul
    std::map<std::string, std::string> selected_anime;
    for (const auto& item : results) 
    {
        if (item.at("name") == selected_anime_name) 
        {
            selected_anime = item;
            break;
        }
    }

    /*
    // Seçilen animenin bölümlerini al
    std::string slug = selected_anime.at("slug");
    anime_seasons = fetchdata.fetch_anime_seasons_openani(slug);
    anime_episodes = fetchdata.fetch_anime_season_episodes(slug);

    if (anime_episodes.empty()) {
        std::cout << "No episodes found." << "\n";
    }

    // Eğer animede herhangi bir bölüm bulunamadıysa
    is_movie = std::get<3>(anime_seasons);std::get<3>(anime_seasons);



    // Animeye ait toplam sezon sayısını almak için
    int total_seasons = std::get<2>(anime_episodes.back());
            
    // API URL'lerini ve watch_url'leri saklamak için vektör
    std::vector<std::tuple<std::string, int, int>> all_watch_urls;
        
    for (int season = 1; season <= total_seasons; ++season) {
        for (const auto& ep : anime_episodes) {
            int ep_number = std::get<1>(ep);
            int ep_season = std::get<2>(ep);
            if (ep_season == season) {
                std::string api_url = fetchdata.fetch_anime_episode_watch_api_url(slug, ep_number, ep_season);
                std::string watch_url = fetchdata.watch_url(slug, std::to_string(season), api_url);
            
                if (!watch_url.empty()) {
                    all_watch_urls.emplace_back(watch_url, ep_number, ep_season);
                } else {
                    std::cerr << "Watch URL alınamadı - Sezon: " << season << ", Bölüm: " << ep_number << "\n";
                }
            }
        }
    }
    */

    

    std::string slug = selected_anime.at("slug");

    // Sezon ve bölüm verilerini al
    anime_seasons = fetchdata.fetch_anime_seasons_openani(slug);
    anime_episodes = fetchdata.fetch_anime_season_episodes(slug);

    if (anime_episodes.empty()) {
        std::cout << "No episodes found." << "\n";
    }

    // Movie mi?
    is_movie = std::get<3>(anime_seasons);

    // Tüm bölümler için paralel olarak API ve watch_url verilerini çek
    std::vector<std::future<std::optional<std::tuple<std::string, int, int>>>> futures;

    for (const auto& ep : anime_episodes) {
        int ep_number = std::get<1>(ep);
        int ep_season = std::get<2>(ep);

        futures.push_back(std::async(std::launch::async, [&, ep_number, ep_season]() {
            std::string api_url = fetchdata.fetch_anime_episode_watch_api_url(slug, ep_number, ep_season);
            std::string watch_url = fetchdata.watch_url(slug, std::to_string(ep_season), api_url);

            if (!watch_url.empty()) {
                return std::make_optional(std::make_tuple(watch_url, ep_number, ep_season));
            } else {
                std::cerr << "Watch URL alınamadı - Sezon: " << ep_season << ", Bölüm: " << ep_number << "\n";
                return std::optional<std::tuple<std::string, int, int>>{};
            }
        }));
    }

    // Sonuçları topla
    std::vector<std::tuple<std::string, int, int>> all_watch_urls;
    for (auto& fut : futures) {
        auto result = fut.get();
        if (result.has_value()) {
            all_watch_urls.push_back(result.value());
        }
    }        

    while (true) 
    {
		// Ana menüye bölümü de ekle
        if (!is_movie)
        {
            main_menu_options = {
			    "İzle", "Sonraki Bölüm", "Önceki Bölüm", "Bölüm Seç", "Anime Ara", "Çık"
		    };
        }

        else if (is_movie) {
            main_menu_options = {
                "İzle", "Anime Ara", "Çık"
            };
        }

        // Ana menüyü göster
        std::string main_menu_choice = getInputFromRofi("Ana Menü", main_menu_options);

        // Eğer çık seçeneği seçildiyse    
        if (main_menu_choice == "Çık") 
        {
            exit(0);
            break;
        } 
        
        // Eğer anime ara seçeneği seçildiyse
        else if (main_menu_choice == "Anime Ara") 
        {
            // Önceki verileri temizle
            selected_episode_index = 0;
            anime_names.clear();
            anime_seasons = {};
            anime_episodes = {};
            all_watch_urls.clear();
        
            // Yeni anime arama
            std::vector<std::map<std::string, std::string>> results = queryLoop();
        
            for (const auto& item : results) {
                anime_names.push_back(item.at("name"));
            }
            anime_names.push_back("Çık");
        
            selected_anime_name = getInputFromRofi("Anime Seç", anime_names);
        
            if (selected_anime_name == "<exit>" || selected_anime_name == "exit") {
                exit(0);
            }
        
            bool is_not_in_anime_names = std::find(anime_names.begin(), anime_names.end(), selected_anime_name) == anime_names.end();
            bool is_not_exit_command = selected_anime_name != "Çık";
        
            if (is_not_in_anime_names && is_not_exit_command) {
                continue;
            }
        
            std::map<std::string, std::string> selected_anime;
            for (const auto& item : results) {
                if (item.at("name") == selected_anime_name) {
                    selected_anime = item;
                    break;
                }
            }
        
            // Slug al ve sezon/bölüm verilerini çek
            slug = selected_anime.at("slug");
            anime_seasons = fetchdata.fetch_anime_seasons_openani(slug);
            anime_episodes = fetchdata.fetch_anime_season_episodes(slug);
        
            if (anime_episodes.empty()) {
                std::cout << "No episodes found." << "\n";
                continue;
            }
        
            is_movie = std::get<3>(anime_seasons);
        
            int total_seasons = std::get<2>(anime_episodes.back());
        
            // all_watch_urls'ü yeniden oluştur
            for (int season = 1; season <= total_seasons; ++season) {
                for (const auto& ep : anime_episodes) {
                    int ep_number = std::get<1>(ep);
                    int ep_season = std::get<2>(ep);
                
                    if (ep_season == season) {
                        std::string api_url = fetchdata.fetch_anime_episode_watch_api_url(slug, ep_number, ep_season);
                        std::string watch_url = fetchdata.watch_url(slug, std::to_string(season), api_url);
                    
                        if (!watch_url.empty()) {
                            all_watch_urls.emplace_back(watch_url, ep_number, ep_season);
                        } else {
                            std::cerr << "Watch URL alınamadı - Sezon: " << season << ", Bölüm: " << ep_number << "\n";
                        }
                    }
                }
            }
        }

        // Eğer izle seçeneği seçildiyse
        else if (main_menu_choice == "İzle") 
        {
            if (!all_watch_urls.empty()) 
            {   
                std::string watch_url = std::get<0>(all_watch_urls[selected_episode_index]);
            
                std::cout << "İzleniyor: " << selected_anime_name << " "
                          << "Sezon " << std::get<2>(all_watch_urls[selected_episode_index])
                          << " - Bölüm " << std::get<1>(all_watch_urls[selected_episode_index]) << "\n";

                std::string command = "mpv --fullscreen \"" + watch_url + "\" > /dev/null 2>&1";
                system(command.c_str());

            } 
            else 
            {
                std::cerr << "Bölüm URL'leri alınamadı" << "\n";
            }

        }
        
        // Eğer sonraki bölüm seçeneği seçildiyse
        else if (main_menu_choice == "Sonraki Bölüm") 
        {
            if (selected_episode_index < all_watch_urls.size() - 1) 
            {
                selected_episode_index++;
            
                std::string watch_url = std::get<0>(all_watch_urls[selected_episode_index]);
            
                std::cout << "İzleniyor: " << selected_anime_name << " "
                          << "Sezon " << std::get<2>(all_watch_urls[selected_episode_index])
                          << " - Bölüm " << std::get<1>(all_watch_urls[selected_episode_index]) << "\n";
            
                std::string command = "mpv --fullscreen \"" + watch_url + "\" > /dev/null 2>&1";
                system(command.c_str());
            } 
            else 
            {
                std::cout << "Zaten en son bölümdesiniz.\n";
            }
        }

        
        // Eğer önceki bölüm seçeneği seçildiyse
        else if (main_menu_choice == "Önceki Bölüm") 
        {
            if (selected_episode_index > 0) 
            {
                selected_episode_index--;
            
                std::string watch_url = std::get<0>(all_watch_urls[selected_episode_index]);
            
                std::cout << "İzleniyor: " << selected_anime_name << " "
                          << "Sezon " << std::get<2>(all_watch_urls[selected_episode_index])
                          << " - Bölüm " << std::get<1>(all_watch_urls[selected_episode_index]) << "\n";
            
                std::string command = "mpv --fullscreen \"" + watch_url + "\" > /dev/null 2>&1";
                system(command.c_str());
            } 
            else 
            {
                std::cout << "Zaten ilk bölümdesiniz\n";
            }
        }

        
        // Eğer bölüm seç seçeneği seçildiyse
        else if (main_menu_choice == "Bölüm Seç") 
        {
            // Rofi menüsünde gösterilecek başlıklar
            std::vector<std::string> episode_titles;
        
            // all_watch_urls'den başlıkları oluştur
            for (const auto& episode_data : all_watch_urls) {
                int episode = std::get<1>(episode_data);
                int season = std::get<2>(episode_data);
            
                std::string title = std::to_string(season) + ". Sezon, " + std::to_string(episode) + ". Bölüm";
                episode_titles.push_back(title);
            }
        
            // Rofi ile kullanıcıdan seçim al
            std::string selected_title = getInputFromRofi("Bölüm Seç", episode_titles);
        
            // Seçim iptal edildiyse çık
            if (selected_title.empty() || selected_title == "<exit>") return 0;
        
            // Seçilen başlık hangi indexten geliyor, bul
            for (int i = 0; i < episode_titles.size(); ++i) {
                if (episode_titles[i] == selected_title) {
                    selected_episode_index = i;
                    break;
                }
            }
        }

    }

    curl_global_cleanup();
    return 0;
}
