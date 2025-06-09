#include <iostream>
#include "modules/FetchOpenAnime.h" // FetchOpenAnime başlık dosyasını ekliyoruz

int main() {
    // FetchData sınıfından bir nesne oluşturuyoruz
    FetchData fetchData;

    // Anime araması yapmak için bir sorgu yazıyoruz
    std::string query = "Naruto";  // Arama yapmak istediğiniz anime ismi
    auto results = fetchData.fetch_anime_search_data(query);

    // Sonuçları ekrana yazdırıyoruz
    if (results.empty()) {
        std::cout << "No results found for query: " << query << std::endl;
    } else {
        std::cout << "Search results for '" << query << "':" << std::endl;
        for (const auto& anime : results) {
            std::cout << "Name: " << anime.at("name") << ", Slug: " << anime.at("slug") << std::endl;
        }
    }

    return 0;
}
