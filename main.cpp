#include<iostream>
#include<app.h>
#include<algorithm>

/*
Author: Ognjen
Project: OOS Project Shell app
OS: Linux Ubuntu
Version: 4.0

makeup:
    pri unosu lozinki da se pojavljuju * (pozeljno)

potencijalni bug:
    list - raditi provjeru putanje

bug:
    create - drugi argument moze biti samo -d (ukoliko se navede). Pr create -s putanja kreirace datoteku -s (rijeseno - potrebno testirati)
    print - smije se samo tekstualna datoteka otvoriti u suprotnom ispisati poruku o gresci (istraziti za Linux)
    find - kada ne nadje tekst stampa vise puta poruku da tekst ne postoji (rijeseno - potrebno testirati)
        za unos tipa lozinka123 baza.txt
                     lozinka12  baza.txt
                     l          baza.txt ispisuje isto rjesenje (provjeriti da li to tako treba i sta u stvari komanda find treba da radi)
                     da li se smije uopste jedan karakter poslati kao argument?
    findDat - nema odgovarajuce poruke ukoliko ne nadje datoteku. (rijeseno - potrebno testirati)



*/
int main(){

    clearr();

    usr::User user; //korisnik
    
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