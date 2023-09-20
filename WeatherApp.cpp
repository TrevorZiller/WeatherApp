#include <iostream>
#include <curl/curl.h>
#include <json/json.h>

// Replace with your OpenWeatherMap API key
const string API_KEY = 'ff298185519418a711d126880a970b21';

// The OpenWeatherMap API URL
const string API_URL = "http://api.openweathermap.org/data/2.5/weather?q=";

// Callback function to handle the HTTP response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, ststring* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

int main() {
    using namespace std;
    CURL* curl;

    CURLcode res;
    string response;

    // Location for weather data
    string location = "Atlanta,US";

    // Construct the complete API URL
    string url = API_URL + location + "&appid=" + API_KEY;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            cerr << "Curl error: " << curl_easy_strerror(res) << endl;
            return 1;
        }

        // Parse the JSON response using a library like jsoncpp
        Json::CharReaderBuilder builder;
        Json::CharReader* reader = builder.newCharReader();
        Json::Value root;
        string errors;

        if (Json::parseFromStream(builder, response, &root, &errors)) {
            // Extract weather information from the JSON object and display it
            string cityName = root["name"].asString();
            double temperature = root["main"]["temp"].asDouble();
            string weatherDescription = root["weather"][0]["description"].asString();

            cout << "City: " << cityName << endl;
            cout << "Temperature: " << temperature << "°C" << endl;
            cout << "Weather: " << weatherDescription << endl;
        }
        else {
            cerr << "Failed to parse JSON: " << errors << endl;
            return 1;
        }
    }

    return 0;
}