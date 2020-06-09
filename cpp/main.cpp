#include<iostream>
#include<algorithm>
#include<command.h> //ukljucuje user.h (a user.h ukljucuje string i vector)
#include<function.h>

/*
Author: Ognjen
Project: OOS Project Shell app
OS: Linux Ubuntu
Version: 0.8.0

makeup:
    /

potencijalni bug:
    /

bug:
    Testirati aplikaciju

*/

int main(){

    function::clearr();

    usr::User user; //korisnik
    
    while(true){
        function::program(user);
    }

    return 0;
}
