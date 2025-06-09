#include <iostream>
#include "modules/FetchOpenAnime.h"

int main() {
    FetchData fetchData;

    std::string query = "Pancreas";  // Arama yapmak istediğiniz anime ismi
    auto results = fetchData.fetch_anime_search_data_openani(query);

    if (results.empty()) {
        std::cout << "No results found for query: " << query << std::endl;
    } else {
        std::cout << "Search results for '" << query << "':" << std::endl;

        for (const auto& anime : results) {
            std::string slug = anime.at("slug");
            std::cout << "Name: " << anime.at("name") << ", Slug: " << slug << std::endl;

            auto [seasonCount, returnedSlug, malID, isMovie] = fetchData.fetch_anime_seasons_openani(slug);

            std::cout << "--- Season Info ---" << std::endl;
            std::cout << "Slug: " << returnedSlug << std::endl;
            std::cout << "Number of Seasons: " << seasonCount << std::endl;
            std::cout << "Is Movie: " << (isMovie ? "Yes" : "No") << std::endl;
            if (malID.has_value()) {
                std::cout << "malID: " << malID.value() << std::endl;
            } else {
                std::cout << "malID: Not available" << std::endl;
            }

            auto episodes = fetchData.fetch_anime_season_episodes(returnedSlug);

            if (episodes.empty()) {
                std::cout << "No episodes found." << std::endl;
            } else {
                std::cout << "--- Episodes ---" << std::endl;

                // Sadece ilk bölümü test edelim (filmse bu yeterli olur)
                const auto& [name, episodeNumber, seasonNumber] = episodes[0];

                std::cout << "Season " << seasonNumber << " - Episode " << episodeNumber << ": " << name << std::endl;

                // İzleme bağlantısı (tam video adı alınmalı)
                std::string full_watch_url = fetchData.fetch_anime_episode_watch_api_url(returnedSlug, episodeNumber, seasonNumber);
                if (!full_watch_url.empty()) {
                    std::cout << "Watch URL (tam): " << full_watch_url << std::endl;

                    // Buradan sadece video kısmını ayıklıyoruz
                    std::string video_filename;
                    size_t lastSlash = full_watch_url.find_last_of('/');
                    if (lastSlash != std::string::npos) {
                        video_filename = full_watch_url.substr(lastSlash + 1); // örnek: "1-71236...mp4?big=1"
                    }

                    std::cout << "\n--- Testing watch_url() ---" << std::endl;
                    std::string seasonStr = std::to_string(seasonNumber);

                    std::string generatedURL = fetchData.watch_url(returnedSlug, seasonStr, video_filename);
                    std::cout << "Generated watch URL: " << generatedURL << std::endl;
                } else {
                    std::cout << "No watch URL found for Episode " << episodeNumber << std::endl;
                }

                // Filmse döngü sonlanır
                if (isMovie) break;
            }
        }
    }

    return 0;
}
