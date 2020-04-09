#include<string>
#include<vector>

namespace usr{

    class User{
    private:
        bool logged;
        std::string name;
        std::string password;
        std::string path;

    public:
        //User(){this->logged = false;}
        User(); //konstruktor
        void login();
        void setLogged(bool logged);
        void setName(std::string name);
        void setPassword(std::string password);
        void setPath(std::string path);
        bool isLogged();
        std::string& getPassword();
        std::string& getName();
        std::string& getPath();
        void execute(std::vector<std::string>& parseCmd, User& user);
    };
}
