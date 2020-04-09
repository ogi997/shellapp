#include<string>
#include<vector>

class User{};
namespace function{
    
    std::vector<std::string> parse(std::string& cmd);
    std::string getExtension(std::string& path);
    void clearr();
    void tree(std::string& path, int root);
    void search(std::string& path, std::string& key, int& check);
    bool validPath(std::string& path);
    bool isFileEmpty(std::ifstream& file);

    void program(usr::User& user);

}