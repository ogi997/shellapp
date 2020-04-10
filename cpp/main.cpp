#include<iostream>
#include<algorithm>
#include<command.h> //ukljucuje user.h (a user.h ukljucuje string i vector)
#include<function.h>

/*
Author: Ognjen
Project: OOS Project Shell app
OS: Linux Ubuntu
Version: 6.0

makeup:
   smisljenja dva nova nacina za izvrsavanje komande create preko std::find i preko for petlje
   doraditi jedan od ta dva

potencijalni bug:
    /

bug:
    popravljeni svi bug koji su bili nadjeni
    ponovo testirati aplikaciju

*/

int main(){

    function::clearr();

    usr::User user; //korisnik
    
    while(true){
        function::program(user);
    }

    return 0;
}
