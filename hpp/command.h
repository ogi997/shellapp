#include<user.h>

namespace cmd {
    
    class Command {
    public:

        void where(std::vector<std::string>& parseCmd, usr::User& user);
        void go(std::vector<std::string>& parseCmd, usr::User& user);
        void create(std::vector<std::string>& parseCmd, usr::User& user);
        void list(std::vector<std::string>& parseCmd, usr::User& user);
        void print(std::vector<std::string>& parseCmd, usr::User& user);
        void find(std::vector<std::string>& parseCmd, usr::User& user);
        void findDat(std::vector<std::string>& parseCmd, usr::User& user);
        void logout(std::vector<std::string>& parseCmd, usr::User& user);
	    void exit(std::vector<std::string>& parseCmd, usr::User& user);
    };
}
