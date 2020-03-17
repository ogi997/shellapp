#include<iostream>
#include<app.h>
//#include<unistd.h>
//#include<cstdlib>
//#include<fstream>
//#include<sys/stat.h>
//#include<dirent.h>
//#include<sys/types.h>
#include<algorithm>
//srediti ove includ

/*
Author: Ognjen
Project: OOS Project Shell app
Version: 3.0

bug
    out_of_range izuzetak kod naredbi koje primaju argumente

*/
int main(){

    clearr();

    usr::User user; //korisnik
    //user.setLogged(false); //postavljamo korisnika na offline (probaj preko konstruktora da rijesis ovo)
    
    std::string cmd; //komanda koju unosi
    std::vector<std::string> parseCmd;
    std::vector<std::string> commands = {"login", "where", "go", "create", "list", "print", "find", "findDat", "logout"}; //moguce komande
    std::vector<std::string>::iterator it;

    bool check = true;
    while(check){

        if(user.getLogged() == false)
            std::cout<<"shellapp> ";
        else
            std::cout<<user.getName()<<"$~ ";

        std::getline(std::cin, cmd);
        parseCmd = parsing::parse(cmd);

        if(parseCmd.size() != 0){
            it = find(commands.begin(), commands.end(), parseCmd.front());
            if(it != commands.end())
                user.execute(parseCmd, user);
            else 
                std::cout<<" Naredba "<<parseCmd.front()<<" ne postoji!\n";
        }
    }

return 0;
}
