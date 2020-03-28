#include<string>
#include<vector>


namespace function{
    
    std::vector<std::string> parse(const std::string& cmd);
    std::string getExtension(const std::string& path);
    void clearr();
    bool validPath(std::string& path);
    void tree(std::string& path, const int root);
    void search(std::string& path, std::string& key, int& check);
    bool isFileEmpty(std::ifstream& file);

}