#include<string>
#include<vector>

namespace usr{

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

        //za sada void
        void execute(std::vector<std::string>& parseCmd, User& user);
    };
}

namespace cmd {
    
    class Command {
    public:
        //za sada sve void
        void where(std::vector<std::string>& parseCmd,usr::User& user);
        void go(std::vector<std::string>& parseCmd, usr::User& user);
        void create(std::vector<std::string>& parseCmd);
        void list(std::vector<std::string>& parseCmd);
        void print(std::vector<std::string>& parseCmd, usr::User& user);
        void find(std::vector<std::string>& parseCmd);
        void findDat(std::vector<std::string>& parseCmd);
        void logout(usr::User& user);
    };
}

namespace parsing{
    std::vector<std::string> parse(const std::string& cmd);
}