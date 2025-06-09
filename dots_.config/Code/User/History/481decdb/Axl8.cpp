#include <iostream>
#include "modules/FetchOpenAnime.h" // FetchOpenAnime başlık dosyasını ekliyoruz

int main() {
    // FetchData sınıfından bir nesne oluşturuyoruz
    FetchData fetchData;

    // Anime araması yapmak için bir sorgu yazıyoruz
    std::string query = "Pancreas";  // Arama yapmak istediğiniz anime ismi
    auto results = fetchData.fetch_anime_search_data_openani(query);

    // Sonuçları ekrana yazdırıyoruz
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

                // İlk bölümü alıyoruz (film ise tek olur zaten)
                const auto& [name, episodeNumber, seasonNumber] = episodes[0];
                std::cout << "Season " << seasonNumber << " - Episode " << episodeNumber << ": " << name << std::endl;

                std::string watch_url = fetchData.fetch_anime_episode_watch_api_url(returnedSlug, episodeNumber, seasonNumber);
                if (!watch_url.empty()) {
                    std::cout << "Watch URL for Episode " << episodeNumber << ": " << watch_url << std::endl;
                } else {
                    std::cout << "No watch URL found for Episode " << episodeNumber << std::endl;
                }

                // watch_url() fonksiyonunu test ediyoruz
                std::cout << "\n--- Testing watch_url() ---" << std::endl;
                std::string seasonStr = std::to_string(seasonNumber);
                std::string videoStr = std::to_string(episodeNumber);
                std::string generatedURL = fetchData.watch_url(returnedSlug, seasonStr, videoStr);
                std::cout << "Generated watch URL: " << generatedURL << std::endl;
            }
        }
    }

    return 0;
}
