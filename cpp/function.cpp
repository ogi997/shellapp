#include<command.h> //ukljucuje user.h (a user.h ukljucuje string i vector)
#include<function.h>
#include<iostream>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<cstring>
#include<stdexcept> //exception kao i std::out_of_range
#include<fstream>
#include<algorithm>

//pomocna funkcija
void function::program(usr::User& user){
    //code

    std::string cmd; //komanda koju unosi
    std::vector<std::string> parseCmd;
    std::vector<std::string> commands = {"login", "where", "go", "create", "list", "print", "find", "findDat", "logout", "exit"}; //moguce komande
    std::vector<std::string>::iterator it;

    if(user.isLogged() == false){
        std::cout<<"shellapp> ";
    }
    else{
        std::cout<<user.getName()<<"@shell:~$ ";
    }

    std::getline(std::cin, cmd);
    parseCmd = function::parse(cmd); //parsiranje naredbe
    parseCmd = function::parseValidation(parseCmd); //vrsi se validacija parsiranja

    if(parseCmd.size() != 0){ //ukoliko se stisne enter (prazan unos) trazice ponovni unos (nece doci do izvrsenja nikakvih komandi)

        it = find(commands.begin(), commands.end(), parseCmd.front());
        if(it != commands.end()){
            user.execute(parseCmd, user);
        }
        else{
            std::cout<<" Naredba "<<parseCmd.front()<<" ne postoji!\n";
        }
    }
}

bool function::isFileEmpty(std::ifstream& infile){
    //provjera da li je datoteka prazna
    infile.seekg(0, std::ios::end);
    if(!infile.tellg()){
        return true;//datoteka je prazna
    }
    //datoteka nije prazna
    infile.seekg(0, std::ios::beg); //vracanje pokazivaca na pocetak datoteke posto nije prazna
    return false;
}

std::string function::getExtension(std::string& path){
    //provjera ekstenzije za komandu print
    /*
        trazi . u stringu (zadnju sa desne strane) i uzima sve desno od nje
    */
    int pos = path.rfind('.', path.length()); //int
    if( pos != std::string::npos ){
        return (path.substr(pos+1, path.length() - pos));
    }
    return (""); //vrati prazan string ukoliko ne pronadje
}

void function::clearr(){
    system("clear"); //naci alternativu za ovo
}

bool function::validPath(std::string& path){
    //validacija putanje, provjera da li putanja postoji i da li je putanja direktorijum a ne datoteka
    struct stat myFile;
    if(stat(path.c_str(), &myFile) < 0){
        std::cout<<" Putanja ne postoji!\n";
        return false;
    }
    else if(!S_ISDIR(myFile.st_mode)){
        std::cout<<" Putanja nije direktorijum!\n";
        return false;
    }
    return true;
}

void function::tree(std::string& path, int root){

    std::string newPath;
    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if(!dir){//ako nije moguce otvoriti direktorijum izbaci iz funkcije samo
        return;
    }

    while((dp = readdir(dir)) != NULL){
        //zaobilazi foldere . i .. (sistemski folderi) i zaobilazi sve foldere cije ime pocinje sa . (skriveni folderi)
        if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0 && dp->d_name[0] != '.'){  //tip je char* pa mora preko strcmp
            for(int i = 0; i<root; i++){
                if(i % 2 == 0 || i == 0){
                    std::cout<<"";
                }
                else{
                    std::cout<<"    ";
                }
            }

            std::cout<<" "<<dp->d_name<<"\n";
            newPath = path + "/" + dp->d_name;
            tree(newPath, root+2);
        }
    }
    closedir(dir); //ogroman bug bio (ovo ga rjesava)
}

void function::search(std::string& path, std::string& key, int& check){

    std::string newPath;
    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if(!dir)
        return;

    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0 && dp->d_name[0] != '.'){
            //if(dp->d_name == key){ //voditi racuna o mogucem bagu d_name je char*, key je string, da bi se izbjego potencijalni bag uradjen if dole
            if(!strcmp(dp->d_name, key.c_str())){
                std::cout<<" "<<path<<"\n";
                check++;
            }

            newPath = path + "/" + dp->d_name;
            search(newPath, key, check);
        }
    }
    closedir(dir); //i ovdje isti bug
}

//funkcija za parsiranje
std::vector<std::string> function::parse(std::string& cmd){

    std::vector<std::string> parseCommand;
    if(cmd.size() == 0){
        return std::vector<std::string>(); //vratice prazan vector
    }

    size_t start;
    size_t end = 0;

    while((start = cmd.find_first_not_of(' ', end)) != std::string::npos){
        end = cmd.find(' ', start);
        parseCommand.push_back(cmd.substr(start,end-start));
    }

    return parseCommand;
}

//funkcija za validaciju parsiranja
std::vector<std::string> function::parseValidation(std::vector<std::string>& v){

    for(int i = 0; i<v.size(); i++){
            try{
                int size = strlen(v.at(i).c_str());
                if(v.at(i)[size - 1] == '\\'){
                    if(i < v.size()){
                        v.at(i)= v.at(i) + " " + v.at(i+1);
                        if(!v.empty())
                            v.erase(v.begin()+i+1);
                    }
                }
            }catch(std::out_of_range& oor){
                std::cout<<" Greska kod navodjenja putanje.\n";
                return std::vector<std::string>(); //doslo je do greske pri validaciji vecktora nesto nije dobro prekiniti sve
            }
        }

        /*prije vracanja ovog vectora treba izbaciti sve \ iz elemenata jer mijenjaju pravo ime fajlova i dokumenata*/
        for (auto& str : v) {
            str.erase(std::remove(str.begin(), str.end(), '\\'), str.end());
        }

        //vecktor je validan i vraca se
        return v;
}