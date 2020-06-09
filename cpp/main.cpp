#include<iostream>
#include<algorithm>
#include<command.h> //ukljucuje user.h (a user.h ukljucuje string i vector)
#include<function.h>

/*
Author: Ognjen
Project: OOS Project Shell app
OS: Linux Ubuntu
Version: 7.0

makeup:
    skontati za komandu find kako treba da funkcionise
    kod komande create da li treba voditi racuna o privilegijama ???

potencijalni bug:
    /

bug:
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
