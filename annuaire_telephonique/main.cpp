#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <chrono>
#include "contact.h"
#include "phone_book.h"
#include "Contact_validator.h"
#include <ctime>

using namespace std::chrono;

void display_contact(const contact& c) {
    auto birth_time_t = system_clock::to_time_t(c.get_birth_date());

    std::cout << "Lastname: " << c.get_last_name() << "\n";
    std::cout << "Firstname: " << c.get_first_name() << "\n";
    std::cout << "Patronymic: " << c.get_patronymic() << "\n"; 
    std::cout << "Address: " << c.get_address() << "\n";
    std::cout << "birthday: " << std::ctime(&birth_time_t);
    std::cout << "Email: " << c.get_email() << "\n";

    // Affichage des numéros de téléphone
    std::cout << "phone number :\n";
    for (const auto& phone : c.get_phone_numbers()) {
        std::string typeStr;
        switch (phone.type) {
        case contact::phone_type::professional:
            typeStr = "Professional";
            break;
        case contact::phone_type::personal:
            typeStr = "Personal";
            break;
        case contact::phone_type::fax:
            typeStr = "Fax";
            break;
        }
        std::cout << "  - " << phone.number << " (" << typeStr << ")\n";
    }
    std::cout << "--------------------------\n";
}

int main() {
    phone_book pb;

    // Créer une date de naissance
    std::tm tm = {};
    tm.tm_year = 2001- 1900;
    tm.tm_mon = 12 -1;
    tm.tm_mday = 28;
    std::time_t birth_time = std::mktime(&tm);
    auto birth_date = system_clock::from_time_t(birth_time);

    // Créer un contact
    contact Merline("JOSEPH", "Ange", "Merline", "28, Gragdanski prospect",
        birth_date, "josephangemerline99@gmail.com",
        { {"+78121234567", contact::phone_type::personal} });

    // DEBUG : Test de validation des shamps
    std::cout << "Lastname 'JOSEPH' : " << Contact_validator::is_valid_last_name("JOSEPH") << std::endl;
    std::cout << "firstname 'Ange' : " << Contact_validator::is_valid_first_name("Ange") << std::endl;
    std::cout << "Patronymic 'Merline' : " << Contact_validator::is_valid_patronymic("Merline") << std::endl;
    std::cout << "Email: " << Contact_validator::is_valid_email("josephangemerline99@gmail.com") << std::endl;
    std::cout << "birthday '28/12/2001' : " << Contact_validator::is_valid_date_of_birth("28/12/2001") << std::endl;
	std::cout << "Phone number '+78121234567' : " << Contact_validator::is_valid_phone_number("+78121234567") << std::endl;

    // Ajouter le contact au répertoire
    if (pb.add_contact(Merline)) {
        std::cout << "contact ajoute avec succes" << std::endl;
    }
    else {
        std::cout << "l'ajout du contact a echouer " << std::endl;
        return 1;
    }

    // Rechercher le contact par nom de famille
    auto results = pb.search_by_last_name("JOSEPH");
    std::cout << "resultat de recherche (" << results.size() << " le contact existe):\n";

    for (const auto& c : results) {
        display_contact(c);
    }

    return 0;
}