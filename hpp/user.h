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
        User(){this->logged = false;}

        void login();
        void setLogged(bool logged);
        void execute(std::vector<std::string>& parseCmd, User& user);
        void setName(std::string& name);
        void setPassword(std::string& password);
        void setPath(std::string& path);
        bool getLogged();
        std::string& getPassword();
        std::string& getName();
        std::string& getPath();
    };
}
