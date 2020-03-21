#include<string>
#include<vector>

void clearr();

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

namespace cmd {
    
    class Command {
    public:

        void where(std::vector<std::string>& parseCmd,usr::User& user);
        void go(std::vector<std::string>& parseCmd, usr::User& user);
        void create(std::vector<std::string>& parseCmd, usr::User& user);
        void list(std::vector<std::string>& parseCmd, usr::User& user);
        void print(std::vector<std::string>& parseCmd, usr::User& user);
        void find(std::vector<std::string>& parseCmd, usr::User& user);
        void findDat(std::vector<std::string>& parseCmd, usr::User& user);
        void logout(std::vector<std::string>& parseCmd, usr::User& user);
    };
}

namespace parsing{
    std::vector<std::string> parse(const std::string& cmd);
}