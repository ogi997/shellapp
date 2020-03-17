#include<string>
#include<vector>

void clearr();

namespace usr{

    class User{
    private:
        std::string name;
        std::string password;
        std::string path;
        bool logged;


       // bool checkDataBase();
    public:
        //konstruktor
        User(){this->logged = false;}

        void login();
        void setLogged(bool logged);
        bool getLogged();
        std::string& getName();
        std::string& getPassword();
        void setName(std::string& name);
        void setPassword(std::string& password);
        void setPath(std::string& path);
        std::string& getPath();

        //za sada void
        void execute(std::vector<std::string>& parseCmd, User& user);
    };
}

namespace cmd {
    
    class Command {
    public:
        //bool login(std::vector<std::string>& parseCmd, usr::User& user);
        //za sada sve void
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