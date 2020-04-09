#include<iostream>
#include<algorithm>
#include<command.h> //ukljucuje user.h (a user.h ukljucuje string i vector)
#include<function.h>

/*
Author: Ognjen
Project: OOS Project Shell app
OS: Linux Ubuntu
Version: 5.0

makeup:
   bolji nacin za izvrsavanje komande create

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
