#include<iostream>
#include<app.h>
#include<unistd.h>
#include<cstdlib>
#include<fstream>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include<algorithm>
//srediti ove includ

/*
Author: Ognjen
Project: OOS Project Shell app
Version: 2.0

bug
    kod prvog pokretanja dva puta ispisuje ime korisnika
    out_of_range izuzetak kod naredbi koje primaju argumente

*/
using namespace usr;

int main(){

  
    User user; //korisnik
    std::string cmd; //komanda koju unosi
    std::vector<std::string> parseCmd;
    std::vector<std::string> commands = {"where", "go", "create", "list", "print", "find", "findDat", "logout"};
    std::vector<std::string>::iterator it;

    bool check = false;
    do{
        if(user.login()){
            check = true;
        }
    }while(!check);

    while(user.getLogged()){

        std::cout<<user.getName()<<"@ "; //ima bug dva puta se ispisuje
        std::getline(std::cin,cmd); //zbog ovog vj
        parseCmd = parsing::parse(cmd);


        if(parseCmd.size() != 0){
            //za slucaj da se stisne enter da ne prodje dzaba
            it = find(commands.begin(), commands.end(), parseCmd.front());
            if(it != commands.end()){
                std::cout<<"Komanda postoji\n";

                user.execute(parseCmd, user);

            }else {
                std::cout<<"Naredba "<<parseCmd.front()<<" ne postoji!\n";
            }
        }
        
    }

return 0;
}
