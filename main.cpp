#include<iostream>
#include<user.h>
#include<unistd.h>
#include<cstdlib>
#include<fstream>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
int main(){

  
    User user; //korisnik
    std::string cmd; //komanda koju unosi
    std::vector<std::string> parseCmd;

    bool check = false;
    do{
        if(user.login()){
            check = true;
        }
    }while(!check);

    while(user.getLogged()){

        std::cout<<user.getName()<<"@ "; //ima bug dva puta se ispisuje
        std::cin.ignore(0);
        std::getline(std::cin,cmd); //zbog ovog vj
        parseCmd = parsing::parse(cmd);
        if(parseCmd.size() != 0){
            if(parseCmd.front() == "where"){
                //prikazuje putanju do tekuceg direktorijuma
                
                //popravi pamcenje putanje ima bug kao i za naredbu ..
                if(parseCmd.size() > 1){
                    std::cout<<"Komanda where nema argmente\n";
                }
                else{
                    std::cout<<user.getPath()<<"\n"; //izvrsena komanda
                }

            }else if(parseCmd.front() == "go"){
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
                    user.setPath(newDir);
                    std::cout<<"proslo";
                }

            }else if(parseCmd.front() == "create"){
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



            }else if(parseCmd.front() == "list"){
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
                

            }else if(parseCmd.front() == "print"){
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

            }else if(parseCmd.front() == "find"){
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

            }else if(parseCmd.front() =="findDat"){
                //findDat datoteka putanja
                //pretrazuje stablo gdje je root putanja dok ne nadje datoteku
                //skontaj sta ova naredba treba radi uopste

                /* ideja je da pretrazimo sve 
                    direktorijume i samo provjeravamo ukoliko postoji
                    taj fajl stampaj ga na konzolu i tjt

                */


            }else if(parseCmd.front() == "logout"){
                user.setLogged(false);
                std::cout<<user.getName()<<" se odjavljuje\n";
                sleep(2);
            }else{
                std::cout<<"Unijeli ste nepostojecu komandu\n";
            }
        }
        
    }

return 0;
}
