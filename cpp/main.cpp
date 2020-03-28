#include<iostream>
#include<algorithm>
#include<command.h> //ukljucuje user.h (a user.h ukljucuje string i vector)
#include<function.h>
/*
Author: Ognjen
Project: OOS Project Shell app
OS: Linux Ubuntu
Version: 5.0

makeup:
    pri unosu lozinki da se pojavljuju * (pozeljno) (realno necu uraditi mrsko mi)

potencijalni bug:
    /

bug:
    popravljeni svi bug koji su bili nadjeni
    ponovo testirati aplikaciju

*/
int main(){

    function::clearr();

    usr::User user; //korisnik
    
    std::string cmd; //komanda koju unosi
    std::vector<std::string> parseCmd;
    std::vector<std::string> commands = {"login", "where", "go", "create", "list", "print", "find", "findDat", "logout"}; //moguce komande
    std::vector<std::string>::iterator it;

    bool check = true;
    while(check){

        if(user.getLogged() == false){
            std::cout<<"shellapp> ";
        }
        else{
            std::cout<<user.getName()<<"$ ";
        }

        std::getline(std::cin, cmd);
        parseCmd = function::parse(cmd); //parsiranje naredbe

        if(parseCmd.size() != 0){//ukoliko se stisne enter (prazan unos) trazice ponovni unos (nece doci do izvrsenja nikakvih komandi)
            it = find(commands.begin(), commands.end(), parseCmd.front());
            if(it != commands.end())
                user.execute(parseCmd, user);
            else 
                std::cout<<" Naredba "<<parseCmd.front()<<" ne postoji!\n";
        }

    }

return 0;
}