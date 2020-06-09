#include<command.h>//ukljucuje i user.h (a user.h ukljucuje string i vector)
#include<function.h>
#include<iostream>
#include<unistd.h>
#include<fstream>
#include<stdexcept> //exception kao i std::out_of_range

//namespace usr
void usr::User::login(){

    std::string ime;
    std::string lozinka;

    std::cout<<" Korisnicko ime: ";
    std::cin>>ime;

    lozinka = getpass(" Lozinka: "); //getpass f-ja iz bib unistd.h unos lozinki kao na terminalu (bez ikakvog prikaza)
    //std::cout<<" Lozinka: ";
    //std::cin>>lozinka;


    std::string userAndPass = ime + " " + lozinka;
    bool found = false;

    std::string line;

    try{
        std::ifstream file("baza/baza.txt");
        if(!file){
            throw std::runtime_error(" Greska sa bazom podataka!\n Pokusajte kasnije.\n");
        }

        while(getline(file,line) && !found){
            if(!line.compare(userAndPass)){
                found = true;
            }
        }

        file.close();
    }
    catch(const std::exception& e){
        std::cout<<e.what();
        std::cin.ignore(100, '\n'); //ciscenje bafera
        return;
    }

    if(found){  //user postoji u bazi
        std::string putanja = "/home/"+ime;
        int check = chdir(putanja.c_str());

        if(!check){
            char cwd[1024];
            getcwd(cwd,sizeof(cwd));
            this->path = cwd;
        }
        else{   //postoji u bazi podataka ali ne i na operativnom sistemu

            std::cout<<" Korisnik "<<ime<<" nije sistemski registrovan na Linux OS.\n";
            std::cin.ignore(100, '\n'); //cisti bafer
            return;
        }

        this->logged = true;
        this->setName(ime);
        this->setPassword(lozinka);

        std::cout<<" Uspjesno ste se prijavili na sistem.\n";

        std::cin.ignore(100, '\n');//cisti bafer - poslije std::cin ostaje enter za svaki slucaj 100 karaktera nek obrise

        sleep(1);
        //cisti ekran
        function::clearr();

    }
    else{
        std::cout<<" Vase korisnicko ime ili lozinka su netacni!\n";
        std::cin.ignore(100, '\n');
    }

}

void usr::User::execute(std::vector<std::string>& parseCmd, usr::User& user){

    cmd::Command comm; //objekat u kome se izvode operacije

    if(!parseCmd.front().compare("login")){

        if(user.isLogged()){
            std::cout<<" Prijavljeni ste na sistem. Odjavite se pomocu logout.\n";
            return;
        }

        user.login();
    }

    if(!parseCmd.front().compare("where")){
        comm.where(parseCmd, user);
    }
    else if(!parseCmd.front().compare("go")){
        comm.go(parseCmd, user);
    }
    else if(!parseCmd.front().compare("create")){
        comm.create(parseCmd, user);
    }
    else if(!parseCmd.front().compare("list")){
        comm.list(parseCmd, user);
    }
    else if(!parseCmd.front().compare("print")){
        comm.print(parseCmd, user);
    }
    else if(!parseCmd.front().compare("find")){
        comm.find(parseCmd, user);
    }
    else if(!parseCmd.front().compare("findDat")){
        comm.findDat(parseCmd, user);
    }
    else if(!parseCmd.front().compare("logout")){
        comm.logout(parseCmd, user);
    }else if(!parseCmd.front().compare("exit")){
        comm.exit(parseCmd, user);
    }
}

usr::User::User(){
    this->logged = false;
}

void usr::User::setLogged(bool logged){
    this->logged = logged;
}

void usr::User::setPath(std::string path){
    this->path = path;
}

std::string& usr::User::getPath(){
    return this->path;
}

bool usr::User::isLogged(){
    return this->logged;
}

std::string& usr::User::getName(){
    return this->name;
}

std::string& usr::User::getPassword(){
    return this->password;
}

void usr::User::setName(std::string name){
    this->name = name;
}

void usr::User::setPassword(std::string password){
    this->password = password;
}
