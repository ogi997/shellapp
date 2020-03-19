#include<iostream>
#include<app.h>
#include<unistd.h>
#include<cstdlib>
#include<fstream>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include<cstring>
#include<stdexcept> //exception kao i std::out_of_range

//pomocne globalne promjenjive

static short count; 

//pomocna funkcija

void clearr(){
    system("clear"); //naci alternativu za ovo
}


void validPath(std::string& path){
      struct stat myFile;
    if(stat(path.c_str(), &myFile) < 0){
        std::cout<<" Putanja ne postoji!\n";
    }else if(!S_ISDIR(myFile.st_mode)){
        std::cout<<" Putanja nije direktorijum!\n";
    }
}

void tree(std::string& path, const int root){
    //provjera validnosti putanje
    if(!count)
        validPath(path);
    count++;



    std::string newPath;
    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if(!dir)
        return;

    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0){ //tip je char* pa mora preko strcmp
            for(int i = 0; i<root; i++){
                if(i % 2 == 0 || i == 0)
                    std::cout<<"";
                else
                    std::cout<<" ";
            }

            std::cout<<" "<<dp->d_name<<"\n";
            newPath = path + "/" + dp->d_name;
            tree(newPath, root+2);
        }
    }
}

void search(std::string& path, std::string& key){
    //validacija putanje
    if(!count)
        validPath(path);
    count++;


    std::string newPath;
    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if(!dir)
        return;

    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0){
            if(dp->d_name == key){ //voditi racuna o mogucem bagu d_name je char*, key je string
                std::cout<<path<<"\n";
            }

            newPath = path + "/" + dp->d_name;
            search(newPath, key);
        }
    }
}

//namespace usr

void usr::User::login(){
    
    std::string ime;
    std::string lozinka;

    std::cout<<"Korisnicko ime: ";
    std::cin>>ime;

    std::cout<<"Lozinka: ";
    std::cin>>lozinka;

    std::string userAndPass = ime + " " + lozinka;
    bool found = false;

    std::ifstream file;
    std::string line;
    try{
        file.open("baza.txt"); //moguca greska pri otvaranju datoteke baza.txt
        while(getline(file,line) && !found){
            if(!line.compare(userAndPass)){
                found = true;
            }
        }
    }catch(const std::ifstream::failure& e){
        std::cout<<" Greska sa bazom podataka!\n";
    }
    
    file.close();
    
    if(found){

        std::string putanja = "/home/"+ime;
        int check = chdir(putanja.c_str());

        /*ovdje moze doci do greske, recimo da korisnik postoji i loguje se a njegov folder
        se ne nalazi u /home/ direktorijumu  -- else bi trebao da sprijeci testirati*/
        if(!check){
            char cwd[1024];
            getcwd(cwd,sizeof(cwd));
            this->path = cwd;
        }else{
            std::cout<<" Korisnik "<<ime<<" nije sistemski registrovan na Linux OS.\n";
            return;
        }


        this->logged = true;
        this->setName(ime);
        this->setPassword(lozinka);
        
        std::cout<<" Uspjesno ste se prijavili na sistem.\n";

        std::cin.ignore(100, '\n');//cisti bafer - poslije std::cin ostaje enter za svaki slucaj 100 karaktera nek obrise

        //cisti ekran
        sleep(1);
        clearr();

    }else{
        std::cout<<" Vase korisnicko ime ili lozinka su netacni!\n";
    }

}

void usr::User::execute(std::vector<std::string>& parseCmd, usr::User& user){

    cmd::Command comm; //objekat u kome se izvode operacije
    
    if(!parseCmd.front().compare("login")){
        
        if(user.getLogged()){
            std::cout<<" Prijavljeni ste na sistem. Odjavite se pomocu logout.\n";
            return;
        }
        
        user.login();
    }

    if(!parseCmd.front().compare("where")){
        comm.where(parseCmd, user);
    }else if(!parseCmd.front().compare("go")){
        comm.go(parseCmd, user);
    }else if(!parseCmd.front().compare("create")){
        comm.create(parseCmd, user);
    }else if(!parseCmd.front().compare("list")){
        count = 0; //vezano za validaciju putanje
        comm.list(parseCmd, user);
    }else if(!parseCmd.front().compare("print")){
        comm.print(parseCmd, user);
    }else if(!parseCmd.front().compare("find")){
        comm.find(parseCmd, user);
    }else if(!parseCmd.front().compare("findDat")){
        count = 0; //vezano za validaciju putanje
        comm.findDat(parseCmd, user);
    }else 
        if(!parseCmd.front().compare("logout")){
        comm.logout(parseCmd, user);
    }     
}

void usr::User::setLogged(bool logged){
    this->logged = logged;
}

void usr::User::setPath(std::string& path){
    this->path = path;
}

std::string& usr::User::getPath(){
    return this->path;
}

bool usr::User::getLogged(){
    return this->logged;
}

std::string& usr::User::getName(){
    return this->name;
}

std::string& usr::User::getPassword(){
    return this->password;
}

void usr::User::setName(std::string& name){
    this->name = name;
}

void usr::User::setPassword(std::string& password){
    this->password = password;
}


//namespace cmd

void cmd::Command::where(std::vector<std::string>& parseCmd, usr::User& user){
//where - pokazuje trenutno

    if(!user.getLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 1){
        std::cout<<" Komanda where nema argmente\n Ispravan unos: where.\n";
        return;
    }

    std::cout<<user.getPath()<<"\n"; //izvrsena komanda
}

void cmd::Command::go(std::vector<std::string>& parseCmd, usr::User& user){
//go putanja - mijenja tekuci direktorijum korisnika. 
    
    if(!user.getLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 2){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Ispravan unos: go putanja!\n";
        return;
    }
    std::string newDir;
    try{
        newDir = parseCmd.at(1); //moguce out_of_range
    }catch(const std::out_of_range& oor){
        std::cout<<" Ispravan unos: go putanja.\n";
        return;
    }

    int ret = chdir(newDir.c_str());
        
    if(!ret){
        char cwd[1024];
        getcwd(cwd, sizeof(cwd)); //funkcija prima char*
        std::string newPath = cwd;
        user.setPath(newPath);
        std::cout<<" Komanda uspjesno izvrsena.\n";
    }else{
        std::cout<<" Unijeta putanja ne postoji!\n";
    }

}

void cmd::Command::create(std::vector<std::string>& parseCmd, usr::User& user){
//[-d] putanja ako ima -d ide direktorijum
//ako ga nema onda neka datoteka
//ako nema putanje kreira u tom folderu u kom se nalazi

    if(!user.getLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 3){ //u slucaju unosa nepotrebnih argumenata
        std::cout<<" Ispravan unos: create [-d] putanja!\n";
        return;
    }

    std::string path;
    std::string opcion;
    try{
        opcion = parseCmd.at(1); //moguce out_of_range
    }catch(const std::out_of_range& oor){
        std::cout<<" Ispravan unos: create [-d] putanja!\n";
        return;
    }
    
    if(!opcion.compare("-d")){
        //kreirati direktorijum
        try{
            path = parseCmd.at(2); //moguce out_of_range
        }catch(const std::out_of_range& oor){
            std::cout<<" Ispravan unos: create [-d] putanja!\n";
            return;
        }
        int check = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if(!check){
            std::cout<<" Direktorijum je kreiran.\n";
            return; //direktorijum je kreiran naredba prestaje sa izvrsenjem
        }else{
            std::cout<<" Direktorijum nije kreiran.\n Ispravan unos: create [-d] putanja.\n";
            return; //neuspjesno kreiranje direktorijuma vratiti korisnika na kom. liniju
        }
    }

    try{
        std::string path = parseCmd.at(1); //moguce out_of_range
        std::ifstream file(path.c_str());
        if(!file.is_open()){
            std::ofstream newFile(path.c_str());
            if(!newFile)
                std::runtime_error(" Datoteka nije kreirana.\n Ispravan unos: create [-d] putanja.\n");
            else
                std::cout<<" Datoteka je kreirana.\n";
        }else{
            std::cout<<" Datoteka sa tim imenom vec postoji u direktorijumu!\n";
        }
    }catch(const std::out_of_range& oor){
        std::cout<<" Ispravan unos: create [-d] putanja\n";
        return;
    }catch(const std::exception& e){
        //std::cout<<" Datoteka nije kreirana\n Ispravan unos: create [-d] putanja\n";
        std::cout<<e.what();
        return;
    }

}

void cmd::Command::list(std::vector<std::string>& parseCmd, usr::User& user){
//list [putanja]
//izlistava sadrzaj direktorijuma na zadatoj putanji
//ako nema putanje onda samo sadrzaj tekuceg direktorijuma
  
    if(!user.getLogged()){
        std::cout<<" Niste prijavljeni\n";
        return;
    }

    if(parseCmd.size() > 2){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Ispravan unos: list [putanja]\n";
        return;
    }

    std::string path;
    try{
        path = parseCmd.at(1); //moguce out_of_range
        tree(path, 0);
    }catch(const std::out_of_range& oor){
        //ako uhvati out_of_range listaj folder gdje se nalazi
        path = user.getPath();
        tree(path, 0);
    }
}

void cmd::Command::print(std::vector<std::string>& parseCmd, usr::User& user){
//print datoteka
//ispisuje sadrzaj tekstualne datoteke na konzolu
//ako nije tekstualna ispisati odgovarajucu poruku

    if(!user.getLogged()){
        std::cout<<" Niste prijavljeni\n";
        return;
    }


    if(parseCmd.size() > 2){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Ispravan unos: print datoteka\n";
        return;
    }
    
    std::string line;
    

    try{
        std::string path = user.getPath() + "/" + parseCmd.at(1); //moguci out_of_range
        std::ifstream infile(path.c_str()); //moguca greska pri otvaranju datoteke
        if(!infile)
            throw std::runtime_error("Greska pri otvaranju datoteke\n Ispravan unos: print datoteka\n");
        
        //provjera da li je datoteka prazna
        infile.seekg(0, std::ios::end);
        if(!infile.tellg()){
            std::cout<<" Datoteka je prazna\n";
            return;
        }
        infile.seekg(0, std::ios::beg); //vracanje pokazivaca na pocetak datoteke posto nije prazna

        
        while(std::getline(infile,line)){
            std::cout<<" "<<line<<"\n";
        }
        infile.close();
              
    }catch(const std::out_of_range& oor){
        std::cout<<" Ispravan unos: print datoteka\n";
        return;
    }catch(const std::exception& ex){
        std::cout<<ex.what();
        return;
    }
}

void cmd::Command::find(std::vector<std::string>& parseCmd, usr::User& user){
//find "tekst" datoteka
//pretrazuje sadrzaj datoteka u odnosu na zadati tekst

    if(!user.getLogged()){
        std::cout<<" Niste prijavljeni\n";
        return;
    }

    if(parseCmd.size() > 3){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Ispravan unos: find \"tekst\" datoteka\n";
        return;
    }

    std::string path;
    std::string search;
    std::string line;

    try{
        search = parseCmd.at(1);//moguce out_of_range
        path = parseCmd.at(2); //moguce out_of_range
        std::ifstream file(path.c_str());
        
        if(!file){
            throw std::runtime_error("Greska pri otvaranju datoteke\n Ispravan unos: print datoteka\n");
        }


        int pos;
        while(file.good()){
            getline(file,line);
            pos = line.find(search.c_str());
            if(pos != std::string::npos){
                std::cout<<" Tekst pronadjen"<<" na "<<pos+1<<" poziciji\n";
                return;
            }else{
                std::cout<<" Trazeni tekst ne postoji u navedenoj datoteci\n";
            }
        }
        file.close();
    }catch(const std::out_of_range& oor){
        std::cout<<" Ispravan unos: find \"tekst\" datoteka\n";
        return;
    }catch(const std::exception& ex){
        std::cout<<ex.what();
        return;
    }
   
}


void cmd::Command::findDat(std::vector<std::string>& parseCmd, usr::User& user){
//findDat datoteka putanja
//pretrazuje stablo gdje je root putanja dok ne nadje datoteku

    if(!user.getLogged()){
        std::cout<<" Niste prijavljeni\n";
        return;
    }

    if(parseCmd.size() > 3){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Ispravan unos: findDat datoteka putanja\n";
        return;
    }

    try{
        std::string path = parseCmd.at(2); //moguce out_of_range
        std::string file = parseCmd.at(1); //moguce out_of_range
        search(path,file);
    }catch(const std::out_of_range& oor){
        std::cout<<" Ispravan unos: findDat datoteka putanja\n";
        return;
    }
}

void cmd::Command::logout(std::vector<std::string>& parseCmd, usr::User& user){
//logout komanda, ako korisnik nije prijavljen ne moze se odjaviti u suprotnom moze...

    if(!user.getLogged()){
        std::cout<<" Niste prijavljeni\n";
        return;
    }

    if(parseCmd.size() > 1){
        std::cout<<" logout nema argumente!\n";
        return;
    }


    user.setLogged(false);
    std::cout<<" "<<user.getName()<<" se odjavljuje\n";
    sleep(2);
    
}

//namespace parsing

std::vector<std::string> parsing::parse(const std::string& cmd){
    
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