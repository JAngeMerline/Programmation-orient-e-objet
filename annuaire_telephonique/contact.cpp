#define _CRT_SECURE_NO_WARNINGS
#include "contact.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Constructeur avec paramètres
contact::contact(const std::string& last_name,
    const std::string& first_name,
    const std::string& patronymic,
    const std::string& address,
    const std::chrono::system_clock::time_point& birth_date,
    const std::string& mail,
    const std::vector<phone_number>& phone_numbers)
    : last_name(last_name),
    first_name(first_name),
    patronymic(patronymic),
    address(address),
    birth_date(birth_date),
    mail(mail),
    phone_numbers(phone_numbers) {
}

// Getters
std::string contact::get_last_name() const {
    return last_name;
}

std::string contact::get_first_name() const {
    return first_name;
}

std::string contact::get_patronymic() const {
    return patronymic;
}

std::string contact::get_address() const {
    return address;
}

std::chrono::system_clock::time_point contact::get_birth_date() const {
    return birth_date;
}

std::string contact::get_email() const {
    return mail;
}

std::vector<contact::phone_number> contact::get_phone_numbers() const {
    return phone_numbers;
}

// Setters
void contact::set_last_name(const std::string& last_name) {
    this->last_name = last_name;
}

void contact::set_first_name(const std::string& first_name) {
    this->first_name = first_name;
}

void contact::set_patronymic(const std::string& patronymic) {
    this->patronymic = patronymic;
}

void contact::set_address(const std::string& address) {
    this->address = address;
}

void contact::set_birth_date(const std::chrono::system_clock::time_point& birth_date) {
    this->birth_date = birth_date;
}

void contact::set_email(const std::string& mail) {
    this->mail = mail;
}

void contact::set_phone_numbers(const std::vector<phone_number>& phone_numbers) {
    this->phone_numbers = phone_numbers;
}

// Gestion des numéros de téléphone
void contact::add_phone_number(const phone_number& phone_number) {
    phone_numbers.push_back(phone_number);
}

void contact::remove_phone_number(const std::string& number) {
    phone_numbers.erase(
        std::remove_if(phone_numbers.begin(), phone_numbers.end(),
            [&](const phone_number& pn) {
                return pn.number == number;
            }),
        phone_numbers.end());
}

void contact::clear_phone_numbers() {
    phone_numbers.clear();
}

// Surcharge de new et delete
void* contact::operator new(std::size_t size) {
    std::cout << "Allocation d'un contact, taille: " << size << " octets" << std::endl;
    return ::operator new(size);
}

void contact::operator delete(void* ptr) noexcept {
    std::cout << "liberation de contact" << std::endl;
    ::operator delete(ptr);
}

// Méthodes utilitaires
std::string contact::tostring() const {
    std::stringstream ss;

    ss << "lastname " << last_name << "\n";
    ss << "Firstname" << first_name << "\n";
    ss << "Patronyme: " << patronymic << "\n";
    ss << "Adress: " << address << "\n";
    ss << "Email: " << mail << "\n";

    // Conversion de la date de naissance
    auto birth_time = std::chrono::system_clock::to_time_t(birth_date);
    char date_buffer[100];
    std::strftime(date_buffer, sizeof(date_buffer), "%d/%m/%Y", std::localtime(&birth_time));
    ss << "birthday" << date_buffer << "\n";

    // Numéros de téléphone
    ss << "number phone \n";
    for (const auto& pn : phone_numbers) {
        std::string type_str;
        switch (pn.type) {
        case professional:
            type_str = "Professional";
            break;
        case personal:
            type_str = "Personal";
            break;
        case fax:
            type_str = "Fax";
            break;
        default:
            type_str = "Inconnu";
            break;
        }
        ss << "  " << pn.number << " (" << type_str << ")\n";
    }

    return ss.str();
}

bool contact::isValid() const {
   
    return !last_name.empty() &&
        !first_name.empty() &&
		!patronymic.empty() &&
        !mail.empty() &&
        !phone_numbers.empty();
}