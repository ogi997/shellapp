#include<iostream>
#include<app.h>
#include<unistd.h>
#include<cstdlib>
#include<fstream>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include<cstring>
//namespace usr

bool usr::User::login(){
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
        //korisnik je uspjesno prijavljen na aplikaciju
        this->logged = true;
        std::cout<<"Uspjesno ste se prijavili na sistem\n";
        this->setName(ime);
        this->setPassword(lozinka);
        //prebacimo korisnika na /home/korisnik i snimimo tu putanju u path
        std::string putanja = "/home/"+ime;
        int check = chdir(putanja.c_str());
        if(!check){ //uspjesno je izvedena promjena direktorijuma
            char cwd[1024];
            getcwd(cwd,sizeof(cwd)); //dosli smo do putanje
            this->path = cwd;// sacuvali smo putanju
        }
        //this->path = "/home/"+ime;
        return true;
    }else{
        std::cout<<"Vase korisnicko ime ili lozinka su netacni\n";
    }

    return false;

}

void usr::User::execute(std::vector<std::string>& parseCmd, usr::User& user){
//smisliti neki bolji nacin za ovo
    cmd::Command comm; //objekat u kome se izvode operacije
    
    if(parseCmd.front() == "where"){
        std::cout<<"Naredba where\n";
        comm.where(parseCmd, user);
    }

    if(parseCmd.front() == "go"){
        std::cout<<"Naredba go";
        comm.go(parseCmd, user);
    }

    if(parseCmd.front() == "create"){
        std::cout<<"Naredba create\n";
        comm.create(parseCmd, user);
    }

    if(parseCmd.front() == "list"){
        std::cout<<"Naredba list\n";
        comm.list(parseCmd, user);
    }
       
    if(parseCmd.front() == "print"){
        std::cout<<"Naredba print\n";
        comm.print(parseCmd, user);
    }
        
    if(parseCmd.front() == "find"){
        std::cout<<"Naredba find\n";
        comm.find(parseCmd, user);
    }
        
    if(parseCmd.front() == "findDat"){
        std::cout<<"Naredba fidnDat\n";
        comm.findDat(parseCmd, user);
    }
        
    if(parseCmd.front() == "logout"){
        std::cout<<"Naredba logout\n";
        comm.logout(user);
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
    if(parseCmd.size() > 1){
        std::cout<<"Komanda where nema argmente\n";
    }else{
        std::cout<<user.getPath()<<"\n"; //izvrsena komanda
    }
}

void cmd::Command::go(std::vector<std::string>& parseCmd, usr::User& user){
//go putanja - mijenja tekuci direktorijum korisnika. 
    if(parseCmd.size() == 1){
        std::cout<<"go putanja\n";
    }

    if(parseCmd.size() > 2){
        std::cout<<"Komanda treba da ima samo jedan argument\n"<<"go putanja\n";
    }

    std::string newDir = parseCmd.at(1);
    int ret = chdir(newDir.c_str());
    
    if(!ret){
        //std::string newPath = "/home/ognjen/"+newDir;
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        std::string newPath = cwd;
        user.setPath(newPath);
        std::cout<<"proslo\n";
    }
}

void cmd::Command::create(std::vector<std::string>& parseCmd, usr::User& user){
//[-d] putanja ako ima -d ide direktorijum
//ako ga nema onda neka datoteka
//ako nema putanje kreira u tom folderu u kom se nalazi

    if(parseCmd.size() == 1){
        std::cout<<"create [-d] putanja\n";
    }
    
    if(parseCmd.size() == 3){
        if(parseCmd.at(1) == "-d"){
            //kreirati direktorijum
            std::string path = parseCmd.at(2);
            int check = mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if(!check){
                std::cout<<"Uspjesno je kreiran direktorijum";
            }
        }
    }

    if(parseCmd.size() == 2){
        std::string path = parseCmd.at(1);
        std::ofstream file (path.c_str());   
    }
}

//pomocna funkcija
void tree(std::string path, const int root){

    std::string newPath;
    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if(!dir)
        return;

    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0){
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

void cmd::Command::list(std::vector<std::string>& parseCmd, usr::User& user){
//list [putanja]
//izlistava sadrzaj direktorijuma na zadatoj putanji
//ako nema putanje onda samo sadrzaj tekuceg direktorijuma
  if(parseCmd.size() == 2){
    //ima putanja nju treba izlistati
    std::string path = parseCmd.at(1);
    tree(path, 0);

  }

   if(parseCmd.size() == 1){
        //nema putanje i tekuci direktorijum mora da rijesi
        std::string path = user.getPath();    
        tree(path, 0);
   }
}

void cmd::Command::print(std::vector<std::string>& parseCmd, usr::User& user){
//print datoteka
//ispisuje sadrzaj tekstualne datoteke na konzolu
//ako nije tekstualna ispisati odgovarajucu poruku
    if(parseCmd.size() == 1){
        std::cout<<"Komanda nema dovoljno argumenata\n"<<"print datoteka";
    }
    
    std::string line;
    std::string path = user.getPath() + "/" + parseCmd.at(1);
    //std::cout<<path;
    std::ifstream infile(path.c_str());
    while(std::getline(infile,line)){
        std::cout<<line<<"\n";
    }
    infile.close();
}

void cmd::Command::find(std::vector<std::string>& parseCmd, usr::User& user){
//find "tekst" datoteka
//pretrazuje sadrzaj datoteka u odnosu na zadati tekst
    std::string search = parseCmd.at(1); //tekst koji trazimo
    std::string line;
    std::ifstream file;
    file.open(parseCmd.at(2).c_str());
    
    if(!file){
        //fajl nije otvoren
        //rijesi preko try catch
        std::cout<<"Fajl ne postoji\n";
    }
    int pos;
    while(file.good()){
        getline(file,line);
        pos = line.find(search.c_str());
        if(pos!=std::string::npos){
            std::cout<<"Pronadjeno\n"<<" na "<<pos<<"\n";
            break; //popravi i ovaj break za sada samo nek prekine rad
        }
    }
}

//pomocna funkcija

void search(std::string path, std::string key){

    std::string newPath;
    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if(!dir)
        return;

    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0){
            if(dp->d_name == key){
             //   std::cout<<"Pronadjeno\n";
                std::cout<<path<<"\n";
            }

           // std::cout<<" "<<dp->d_name<<"\n";
            newPath = path + "/" + dp->d_name;
            search(newPath, key);
        }
    }
}




void cmd::Command::findDat(std::vector<std::string>& parseCmd, usr::User& user){
//findDat datoteka putanja
//pretrazuje stablo gdje je root putanja dok ne nadje datoteku
//skontaj sta ova naredba treba radi uopste

/* ideja je da pretrazimo sve 
direktorijume i samo provjeravamo ukoliko postoji
taj fajl stampaj ga na konzolu i tjt
*/

    //sredi ove if koristi c++
    if(parseCmd.size() > 3){
        std::cout<<"Previse argumenata\n";
        return;
    }

    if(parseCmd.size() < 3){
        std::cout<<"findDat datoteka putanja\n";
        return;
    }

    std::string path = parseCmd.at(2);
    std::string file = parseCmd.at(1);
    search(path,file);

}

void cmd::Command::logout(usr::User& user){
    user.setLogged(false);
    std::cout<<user.getName()<<" se odjavljuje\n";
    sleep(2);
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