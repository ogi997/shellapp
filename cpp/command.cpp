#include<command.h> //ukljucuje i user.h (a user.h string i vector)
#include<function.h>
#include<iostream>
#include<unistd.h>
#include<fstream>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdexcept> //exception kao i std::out_of_range
#include<algorithm>

using namespace function; //koristim manje vise sve funkcije ovdje pa uvezi ih sve lakse..

//namespace cmd
void cmd::Command::where(std::vector<std::string>& parseCmd, usr::User& user){
//where - pokazuje trenutno

    if(!user.isLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 1){
        std::cout<<" Komanda where nema argmente.\n Ispravan unos: where\n";
        return;
    }

    std::cout<<" "<<user.getPath()<<"\n"; //izvrsena komanda
}

void cmd::Command::go(std::vector<std::string>& parseCmd, usr::User& user){
//go putanja - mijenja tekuci direktorijum korisnika. 
    
    if(!user.isLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 2){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Naveli ste nepotrebne argumente.\n Ispravan unos: go putanja!\n";
        return;
    }

    std::string newDir;

    try{
        newDir = parseCmd.at(1); //moguce out_of_range

    }catch(const std::out_of_range& oor){
        std::cout<<" Niste naveli putanju.\n Ispravan unos: go putanja\n";
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

    if(!user.isLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 3){ //u slucaju unosa nepotrebnih argumenata
        std::cout<<" Naveli ste nepotrebne argumente.\n Ispravan unos: create [-d] putanja\n";
        return;
    }

//code
    for(int it = 0; it<parseCmd.size(); it++){
        if(parseCmd.at(it)[0] == '-'){
            if(parseCmd.at(it) == "-d"){ //parseCmd.at(it).compare("-d") == 0
                try{
                    std::string path = parseCmd.at(it + 1); //moguci out_of_range
                    
                    int check = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //kreiranje direktorijuma

                    if(!check){
                        std::cout<<" Direktorijum je kreiran.\n";
                        return; //direktorijum je kreiran naredba prestaje sa izvrsenjem
                    }else{
                        /*moguca greska pri kreiranju je da korisnik nema prava kreiranja novog direktorijuma u datom dir
                        */
                        std::cout<<" Direktorijum nije kreiran.\n Provjerite putanju (ime) direktorijuma\n Direktorijum ne smije da se zove \'/\' \'.\' \'..\' \n";
                        return; //neuspjesno kreiranje direktorijuma vratiti korisnika na kom. liniju
                    }
                }catch(std::out_of_range& oor){
                    std::cout<<" Argumenti nisu dobro navedeni\n Ispravan unos: create [-d] putanja\n";
                    return;
                }
            }else {
                std::cout<<" Argument "<< parseCmd.at(it)<< " ne postoji\n";
                return;
            }
        }
    }

    try{
        std::string path;
        if(parseCmd.size() - 1 > 0 && parseCmd.size() < 3){
            path = parseCmd.at(parseCmd.size() - 1);
        }
        else{
            std::cout<<" Ispravan unos: create [-d] putanja\n";
        }
        std::ifstream file(path.c_str());
        if(!file.is_open()){
            std::ofstream newFile(path.c_str());
            if(!newFile){
                std::runtime_error(" Datoteka nije kreirana.\n Ispravan unos: create [-d] putanja\n");
            }
            else{
                std::cout<<" Datoteka je kreirana.\n";//zavrseno je kreiranje datoteke funkcija ce sama da izadje nije potrebno return
                newFile.close(); //zatvaramo datoteku kada je kreiramo
            }
        }else{
            std::cout<<" Provjerite putanju (ime) datoteke.\n Datoteka ne smije da se zove \'/\' \'.\' \'..\' \n";
            file.close(); //zatvaramo datoteku jer nam ne treba
        }
        return;
    }catch(const std::out_of_range& oor){
        std::cout<<" Argumenti nisu dobro navedeni\n Ispravan unos: create [-d] putanja\n";
        return;
    }catch(const std::exception& e){
        std::cout<<" "<<e.what()<<"\n";
        return;
    }
}

void cmd::Command::list(std::vector<std::string>& parseCmd, usr::User& user){
//list [putanja]
//izlistava sadrzaj direktorijuma na zadatoj putanji
//ako nema putanje onda samo sadrzaj tekuceg direktorijuma
  
    if(!user.isLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 2){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Naveli ste nepotrebne argumente\n Ispravan unos: list [putanja]\n";
        return;
    }

    std::string path;
    try{
        path = parseCmd.at(1); //moguce out_of_range
        /*validacija patha*/
        if(validPath(path)){
            tree(path, 0);
        }
    }catch(const std::out_of_range& oor){
        //ako uhvati out_of_range listaj folder gdje se nalazi
        path = user.getPath();
        if(validPath(path)){
            tree(path, 0);
        }
    }
}

void cmd::Command::print(std::vector<std::string>& parseCmd, usr::User& user){
//print datoteka
//ispisuje sadrzaj tekstualne datoteke na konzolu
//ako nije tekstualna ispisati odgovarajucu poruku

    if(!user.isLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 2){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Naveli ste nepotrebne argumente\n Ispravan unos: print datoteka\n";
        return;
    }

    std::string line;
    
    try{
        std::string path = user.getPath() + "/" + parseCmd.at(1); //moguci out_of_range

         //provjera da li je datoteka tekstualna
        std::string ext = getExtension(path);
        if(ext.compare("txt")){
            std::cout<<" Datoteka nije tekstualna.\n";
            return;
        }

        std::ifstream infile(path.c_str());
        if(!infile){
            throw std::runtime_error(" Greska pri otvaranju datoteke.\n Ispravan unos: print datoteka\n");
        }
        
        //validacija da li je uopste navedena datoteka
        struct stat dir;
        stat(path.c_str(), &dir); 
        if(S_ISDIR(dir.st_mode)){
            std::cout<<" Morate navesti datoteku, a ne direktorijum.\n";
            return;
        }
        

        //provjera da li je datoteka prazna
        if(isFileEmpty(infile)){
            std::cout<<" Datoteka je prazna.\n";
            return;//prekini sa izvrsavanjem funkcije
        }
       
        //stampanje sadrzaja datoteke
        while(std::getline(infile,line)){
            std::cout<<" "<<line<<"\n";
        }
        infile.close();//zatvaranje datoteke kada se zavrsi sa radom
              
    }catch(const std::out_of_range& oor){
        std::cout<<" Niste naveli ispravnu datoteku\n Ispravan unos: print datoteka\n";
        return;
    }catch(const std::exception& ex){
        std::cout<<ex.what();
        return;
    }
}

void cmd::Command::find(std::vector<std::string>& parseCmd, usr::User& user){
//find "tekst" datoteka
//pretrazuje sadrzaj datoteka u odnosu na zadati tekst

    if(!user.isLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 3){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Naveli ste nepotrebne argumente \n Ispravan unos: find \"tekst\" datoteka\n";
        return;
    }


    /* komanda ispod ce da obrise " iz teksta koji se trazi, samo skontati sta ova komanda treba u stvari da radi */
    //nesto.erase(std::remove(nesto.begin(), nesto.end(), '"'), nesto.end());


    std::string path;
    std::string search;
    std::string line;

    try{
        search = parseCmd.at(1);//moguce out_of_range
        if(search[0] != '"' || search[search.size() - 1] != '"'){
        	std::cout<<" Neispravan unos argumenta za pretragu\n Ispravan unos: find \"tekst\" datoteka\n";
        	return;
        }
        search.erase(std::remove(search.begin(), search.end(), '"'), search.end()); //remove-erase idom brise " u tekstu
        path = parseCmd.at(2); //moguce out_of_range
        std::ifstream file(path.c_str());
        
        if(!file){
            throw std::runtime_error(" Greska pri otvaranju datoteke.\n Ispravan unos: find \"tekst\" datoteka\n");
        }

        //provjera da li je datoteka prazna
        if(isFileEmpty(file)){
            std::cout<<" Datoteka je prazna.\n";
            return;//prekini sa izvrsavanjem funkcije
        }

        int pos; //int
        int vrsta = 0;
        int temp = 0;
        while(file.good()){
            getline(file,line);
            //ovdje provjeri da li se trazi string u stringu ili...
            pos = line.find(search.c_str());
            if(pos != std::string::npos){
                std::cout<<" Rezultat: " << vrsta + 1 << "\n";
                temp++;
                return;
            }
            vrsta++;
        }

        if(!temp){
        	std::cout<<" Trazeni tekst ne postoji u datoteci\n";
        	return;
        }

        file.close(); //zatvaranje datoteke kada se zavrsi sa radom
    }catch(const std::out_of_range& oor){
        std::cout<<" Nisu navedeni potrebni argumenti.\n Ispravan unos: find \"tekst\" datoteka\n";
        return;
    }catch(const std::exception& ex){
        std::cout<<ex.what();
        return;
    }
   
}

void cmd::Command::findDat(std::vector<std::string>& parseCmd, usr::User& user){
//findDat datoteka putanja
//pretrazuje stablo gdje je root putanja dok ne nadje datoteku
    int check = 0;
    if(!user.isLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 3){//u slucaju unosa nepotrebnih argumenata
        std::cout<<" Navedeni su nepotrebni argumnti.\n Ispravan unos: findDat datoteka putanja\n";
        return;
    }

    try{
        std::string path = parseCmd.at(2); //moguce out_of_range
        std::string file = parseCmd.at(1); //moguce out_of_range
        if(validPath(path)){
            search(path,file, check);
            if(!check){
                std::cout<<" Trazena datoteka ne postoji u navedenom stablu direktorijuma.\n";
            }
        }
    }catch(const std::out_of_range& oor){
        std::cout<<" Nisu navedeni potrebni argumenti.\n Ispravan unos: findDat datoteka putanja\n";
        return;
    }
}

void cmd::Command::logout(std::vector<std::string>& parseCmd, usr::User& user){
//logout komanda, ako korisnik nije prijavljen ne moze se odjaviti u suprotnom moze...

    if(!user.isLogged()){
        std::cout<<" Niste prijavljeni!\n";
        return;
    }

    if(parseCmd.size() > 1){
        std::cout<<" logout nema argumente.\n";
        return;
    }

    user.setLogged(false);
    std::cout<<" "<<user.getName()<<" se odjavljuje.\n";
    sleep(2);
}

void cmd::Command::exit(std::vector<std::string>& parseCmd, usr::User& user){
	if(parseCmd.size() > 1){
		std::cout<<" Komanda exit nema argumente\n";
		return;
	}
	if(user.isLogged()){
		std::cout<<" Da bi izasli iz programa morate se odjaviti pomocu komande logout\n";
		return;
	}
	::std::exit(0);
}