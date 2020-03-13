#include<user.h>
#include<iostream>
#include<fstream>
 bool User::login(){
    std::string ime;
    std::string lozinka;

    std::cout<<"Korisnicko ime-> ";
    std::cin>>ime;

    std::cout<<"Lozinka-> ";
    std::cin>>lozinka;

    std::string userAndPass = ime + " " + lozinka;
    bool found = false;

    std::ifstream file;
    file.open("baza.txt");

    std::string line;
    if(file.is_open()){
        while(getline(file,line) && !found){
            if(line.compare(userAndPass) == 0){
                found = true;
            }
        }
        file.close();
    }else {
        std::cout<<"Doslo je do greske sa bazom podataka\n";
    }
    if(found){
        this->logged = true;
        std::cout<<"Uspjesno ste se prijavili na sistem\n";
        this->setName(ime);
        this->setPassword(lozinka);
        this->path = "/home/"+ime;
        return true;
    }else{
        std::cout<<"Vase korisnicko ime ili lozinka su netacni\n";
    }

    return false;

 }

 //void User::setLogged(bool logged){
  //  this->logged = logged;
// }
void User::setPath(std::string& path){
    this->path = path;
}

std::string& User::getPath(){
    return this->path;
}

 bool User::getLogged(){
    return this->logged;
 }

 std::string& User::getName(){
    return this->name;
 }

 std::string& User::getPassword(){
    return this->password;
 }

 void User::setName(std::string& name){
    this->name = name;
}

void User::setPassword(std::string& password){
    this->password = password;
}

//namespace parsing
std::vector<std::string> parsing::parse(const std::string& cmd){
    std::vector<std::string> parseCommand;
    if(cmd.size() == 0)
        return std::vector<std::string>();

    int start;
    int end = 0;

    while((start = cmd.find_first_not_of(' ', end)) != std::string::npos){
        end = cmd.find(' ', start);
        parseCommand.push_back(cmd.substr(start,end-start));
    }

    return parseCommand;
}
