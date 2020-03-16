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
Version: 3.0

bug
    out_of_range izuzetak kod naredbi koje primaju argumente

*/
using namespace usr;

int main(){

    clearr();

    User user; //korisnik
    user.setLogged(false); //postavljamo korisnika na offline (probaj preko konstruktora da rijesis ovo)
    std::string cmd; //komanda koju unosi
    std::vector<std::string> parseCmd;
    std::vector<std::string> commands = {"login", "where", "go", "create", "list", "print", "find", "findDat", "logout"}; //moguce komande
    std::vector<std::string>::iterator it;

    bool check = true;
    while(check){
        //std::cout<<"prijava "<<user.getLogged()<<"\n\n";
        if(user.getLogged() == false)
            std::cout<<"shellapp> ";
        else
            std::cout<<user.getName()<<"$~ ";

        std::getline(std::cin, cmd);
        parseCmd = parsing::parse(cmd);
        if(parseCmd.size() != 0){
            it = find(commands.begin(), commands.end(), parseCmd.front());
            if(it != commands.end()){
                //std::cout<<"Komanda postoji\n";

                user.execute(parseCmd, user);

            }else {
                std::cout<<"Naredba "<<parseCmd.front()<<" ne postoji!\n";
            }
        }
    }







/*

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

    */

return 0;
}
