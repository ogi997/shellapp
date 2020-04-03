#include<command.h> //ukljucuje user.h (a user.h ukljucuje string i vector)
#include<function.h>
#include<iostream>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<cstring>
#include<stdexcept> //exception kao i std::out_of_range
#include<fstream>

//pomocna funkcija

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

std::string function::getExtension(const std::string& path){
    //provjera ekstenzije za komandu print
    /*
        trazi . u stringu (zadnju sa desne strane) i uzima sve desno od nje
    */
    short pos = path.rfind('.', path.length());
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
    }else if(!S_ISDIR(myFile.st_mode)){
        std::cout<<" Putanja nije direktorijum!\n";
        return false;
    }
    return true;
}

void function::tree(std::string& path, const int root){

    std::string newPath;
    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if(!dir)//ako nije moguce otvoriti direktorijum izbaci iz funkcije samo
        return;

    while((dp = readdir(dir)) != NULL){
        //zaobilazi foldere . i .. (sistemski folderi) i zaobilazi sve foldere cije ime pocinje sa . (skriveni folderi)
        if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0 && dp->d_name[0] != '.'){ //tip je char* pa mora preko strcmp
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

std::vector<std::string> function::parse(const std::string& cmd){
    
    std::vector<std::string> parseCommand;
    if(cmd.size() == 0)
        return std::vector<std::string>(); //vratice prazan vector

    int start;
    int end = 0;

    while((start = cmd.find_first_not_of(' ', end)) != std::string::npos){
        end = cmd.find(' ', start);
        parseCommand.push_back(cmd.substr(start,end-start));
    }

    return parseCommand;
}