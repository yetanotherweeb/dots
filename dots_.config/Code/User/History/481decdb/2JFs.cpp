#include <iostream>
#include "modules/FetchOpenAnime.h" // FetchOpenAnime başlık dosyasını ekliyoruz

int main() {
    // FetchData sınıfından bir nesne oluşturuyoruz
    FetchData fetchData;

    // Anime araması yapmak için bir sorgu yazıyoruz
    std::string query = "Talentless Nana";  // Arama yapmak istediğiniz anime ismi
    auto results = fetchData.fetch_anime_search_data_openani(query);

    // Sonuçları ekrana yazdırıyoruz
    if (results.empty()) {
        std::cout << "No results found for query: " << query << std::endl;
    } else {
        std::cout << "Search results for '" << query << "':" << std::endl;

        for (const auto& anime : results) {
            std::cout << "Name: " << anime.at("name") << ", Slug: " << anime.at("slug") << std::endl;
        }

        // İlk sonucu al ve slug ile sezon bilgilerini getir
        std::string selected_slug = results[0].at("slug");

        // Sezon bilgilerini çekiyoruz
        auto [season_count, slug, mal_id_opt] = fetchData.fetch_anime_seasons_openani(selected_slug);

        std::cout << "\n--- Season Info ---" << std::endl;
        std::cout << "Slug: " << slug << std::endl;
        std::cout << "Number of Seasons: " << season_count << std::endl;

        if (mal_id_opt.has_value()) {
            std::cout << "malID: " << mal_id_opt.value() << std::endl;
        } else {
            std::cout << "malID: Not available" << std::endl;
        }
    }

    return 0;
}
