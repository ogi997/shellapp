#include<iostream>
#include<app.h>
#include<unistd.h>
#include<cstdlib>
#include<fstream>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>

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
            comm.create(parseCmd);
        }
        if(parseCmd.front() == "list"){
            std::cout<<"Naredba list\n";
            comm.list(parseCmd);
        }
       
        if(parseCmd.front() == "print"){
            std::cout<<"Naredba print\n";
            comm.print(parseCmd, user);
        }
        
        if(parseCmd.front() == "find"){
            std::cout<<"Naredba find\n";
            comm.find(parseCmd);
        }
        
        if(parseCmd.front() == "findDat"){
            std::cout<<"Naredba fidnDat\n";
            comm.findDat(parseCmd);
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

 //prikazuje putanju do tekuceg direktorijuma
                
                //popravi pamcenje putanje ima bug kao i za naredbu ..
                if(parseCmd.size() > 1){
                    std::cout<<"Komanda where nema argmente\n";
                }
                else{
                    std::cout<<user.getPath()<<"\n"; //izvrsena komanda
                }



}


void cmd::Command::go(std::vector<std::string>& parseCmd,usr::User& user){

//go putanja - mijenja tekuci direktorijum korisnika. 
                //Novi direktorijum je naveden kao putanja
                if(parseCmd.size() == 1){
                    std::cout<<"go putanja\n";
                }
                if(parseCmd.size() > 2){
                    std::cout<<"Komanda treba da ima samo jedan argument\n"<<"go putanja\n";
                }
                std::string newDir = parseCmd.at(1);
                int ret = chdir(newDir.c_str());
                if(!ret){
                   // std::string newPath = "/home/ognjen/"+newDir;
                    char cwd[1024];
                    getcwd(cwd, sizeof(cwd));
                    std::string newPath = cwd;
                    user.setPath(newPath);
                    std::cout<<"proslo\n";
                }



}

void cmd::Command::create(std::vector<std::string>& parseCmd){
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

void cmd::Command::list(std::vector<std::string>& parseCmd){
//list [putanja]
                //izlistava sadrzaj direktorijuma na zadatoj putanji
                //ako nema putanje onda samo sadrzaj tekuceg direktorijuma
                struct dirent* entry;
                if(parseCmd.size() > 2){
                    std::cout<<"Komanda ima previse argumenata\n";
                }
                if(parseCmd.size() == 2){
                std::string path = parseCmd.at(1);
                DIR* dir = opendir(path.c_str());
                if(dir == NULL){
                    std::cout<<"Greska"; //uradi nesto prekini vidi kasnije
                }
                while((entry = readdir(dir)) != NULL){
                    std::cout<<entry->d_name<<"\n";
                }
                closedir(dir);
            }
                

}

void cmd::Command::print(std::vector<std::string>& parseCmd,usr::User& user){
 //print datoteka
                //ispisuje sadrzaj tekstualne datoteke na konzolu
                //ako nije tekstualna ispisati odgovarajucu poruku
                if(parseCmd.size() == 1){
                    std::cout<<"Komanda nema dovoljno argumenata\n"<<"print datoteka";
                }

                //popraviti path da bi i ova naredba radila ispravno...
               std::string line;
               std::string path = user.getPath() + parseCmd.at(1);
               //std::cout<<path;
               std::ifstream infile(path.c_str());
               while(std::getline(infile,line)){
                std::cout<<line<<"\n";
               }
               infile.close();

}

void cmd::Command::find(std::vector<std::string>& parseCmd){
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

void cmd::Command::findDat(std::vector<std::string>& parseCmd){
 //findDat datoteka putanja
                //pretrazuje stablo gdje je root putanja dok ne nadje datoteku
                //skontaj sta ova naredba treba radi uopste

                /* ideja je da pretrazimo sve 
                    direktorijume i samo provjeravamo ukoliko postoji
                    taj fajl stampaj ga na konzolu i tjt

                */

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
