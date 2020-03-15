#include<string>
#include<vector>

class User{
private:
    std::string name;
    std::string password;
    std::string path;
    bool logged;

   // bool checkDataBase();
public:
    bool login();
    void setLogged(bool logged);
    bool getLogged();
    std::string& getName();
    std::string& getPassword();
    void setName(std::string& name);
    void setPassword(std::string& password);
    void setPath(std::string& path);
    std::string& getPath();
};

namespace parsing{
    std::vector<std::string> parse(const std::string& cmd);
}