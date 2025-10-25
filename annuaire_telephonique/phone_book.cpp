#include "phone_book.h"
#include "Contact_validator.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <ctime>

// Constructeur
phone_book::phone_book(const std::string& file_path) : file_path(file_path) {
    load_from_file();
}

// Ajouter un contact avec validation
bool phone_book::add_contact(const contact& new_contact) {
    if (!validate_contact(new_contact)) {
        std::cerr << "Erreur: Contact invalide - verification des donnees rentres " << std::endl;
        return false;
    }

    // Vérifier si le contact existe déjà
    for (const auto& c : contacts) {
        if (c.get_last_name() == new_contact.get_last_name() &&
            c.get_first_name() == new_contact.get_first_name() &&
            c.get_patronymic() == new_contact.get_patronymic()) {
            std::cerr << "Erreur: Contact déjà existant" << std::endl;
            return false;
        }
    }

    contacts.push_back(new_contact);
    return save_to_file();
}

// Supprimer un contact
bool phone_book::remove_contact(const std::string& last_name, const std::string& first_name, const std::string& patronymic) {
    auto initial_size = contacts.size();

    contacts.erase(std::remove_if(contacts.begin(), contacts.end(),
        [&](const contact& c) {
            return c.get_last_name() == last_name &&
                c.get_first_name() == first_name &&
                c.get_patronymic() == patronymic;
        }), contacts.end());

    bool removed = (contacts.size() < initial_size);
    if (removed) {
        std::cout << "suppression du contact avec succes" << std::endl;
        save_to_file();
    }
    else {
        std::cerr << "Erreur: contact non trouve" << std::endl;
    }
    return removed;
}

// Mettre à jour un contact
bool phone_book::update_contact(const std::string& last_name, const std::string& first_name, const std::string& patronymic, const contact& updated_contact) {
    if (!validate_contact(updated_contact)) {
        std::cerr << "Erreur: donnees de contact invalide" << std::endl;
        return false;
    }

    for (auto& c : contacts) {
        if (c.get_last_name() == last_name &&
            c.get_first_name() == first_name &&
            c.get_patronymic() == patronymic) {
            c = updated_contact;
            std::cout << "mise a jour du contact avec succes " << std::endl;
            return save_to_file();
        }
    }

    std::cerr << "Erreur: Contact  non trouver pour la mise a jour " << std::endl;
    return false;
}

// Recherche par nom
std::vector<contact> phone_book::search_by_last_name(const std::string& last_name) const {
    std::vector<contact> results;
    for (const auto& c : contacts) {
        if (c.get_last_name() == last_name) {
            results.push_back(c);
        }
    }
    return results;
}

// Recherche par prénom
std::vector<contact> phone_book::search_by_first_name(const std::string& first_name) const {
    std::vector<contact> results;
    for (const auto& c : contacts) {
        if (c.get_first_name() == first_name) {
            results.push_back(c);
        }
    }
    return results;
}

// Recherche par patronyme
std::vector<contact> phone_book::search_by_patronymic(const std::string& patronymic) const {
    std::vector<contact> results;
    for (const auto& c : contacts) {
        if (c.get_patronymic() == patronymic) {
            results.push_back(c);
        }
    }
    return results;
}

// Recherche par nom complet
std::vector<contact> phone_book::search_by_full_name(const std::string& last_name, const std::string& first_name, const std::string& patronymic) const {
    std::vector<contact> results;
    for (const auto& c : contacts) {
        if (c.get_last_name() == last_name &&
            c.get_first_name() == first_name &&
            c.get_patronymic() == patronymic) {
            results.push_back(c);
        }
    }
    return results;
}

// Recherche par numéro de téléphone
std::vector<contact> phone_book::search_by_phone_number(const std::string& phone_number) const {
    std::vector<contact> results;
    for (const auto& c : contacts) {
        for (const auto& pn : c.get_phone_numbers()) {
            if (pn.number == phone_number) {
                results.push_back(c);
                break;
            }
        }
    }
    return results;
}

// Recherche par email
std::vector<contact> phone_book::search_by_email(const std::string& email) const {
    std::vector<contact> results;
    for (const auto& c : contacts) {
        if (c.get_email() == email) {
            results.push_back(c);
        }
    }
    return results;
}

// Tri par nom
void phone_book::sort_by_last_name() {
    std::sort(contacts.begin(), contacts.end(), compare_by_last_name);
    save_to_file();
}

// Tri par prénom
void phone_book::sort_by_first_name() {
    std::sort(contacts.begin(), contacts.end(), compare_by_first_name);
    save_to_file();
}

// Tri par patronyme
void phone_book::sort_by_patronymic() {
    std::sort(contacts.begin(), contacts.end(), compare_by_patronymic);
    save_to_file();
}

// Tri par nom complet
void phone_book::sort_by_full_name() {
    std::sort(contacts.begin(), contacts.end(), compare_by_full_name);
    save_to_file();
}

// Tri par date de naissance
void phone_book::sort_by_birth_date() {
    std::sort(contacts.begin(), contacts.end(), compare_by_birth_date);
    save_to_file();
}

// Tri par email
void phone_book::sort_by_email() {
    std::sort(contacts.begin(), contacts.end(), compare_by_email);
    save_to_file();
}

// Accès à tous les contacts
const std::vector<contact>& phone_book::get_all_contacts() const {
    return contacts;
}

// Accès à un contact (modifiable)
contact* phone_book::get_contact(size_t index) {
    if (index >= contacts.size()) {
        return nullptr;
    }
    return &contacts[index];
}

// Accès à un contact (lecture seule)
const contact* phone_book::get_contact(size_t index) const {
    if (index >= contacts.size()) {
        return nullptr;
    }
    return &contacts[index];
}

// Nombre de contacts
size_t phone_book::get_contact_count() const {
    return contacts.size();
}

// Chargement depuis le fichier
void phone_book::load_from_file() {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "CREATION D'UN ANNUAIRE TELEPHONIQUE: " << file_path << std::endl;
        return;
    }

    contacts.clear();
    size_t contact_count = 0;
    file.read(reinterpret_cast<char*>(&contact_count), sizeof(contact_count));

    for (size_t i = 0; i < contact_count; ++i) {
        contact c;
        if (deserialize_contact(file, c) && validate_contact(c)) {
            contacts.push_back(c);
        }
    }

    file.close();
    std::cout << "Chargement de " << contacts.size() << " contacts depuis " << file_path << std::endl;
}

// Sauvegarde dans le fichier
bool phone_book::save_to_file() const {
    std::ofstream file(file_path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << file_path << " en train d'ecrire" << std::endl;
        return false;
    }

    size_t contact_count = contacts.size();
    file.write(reinterpret_cast<const char*>(&contact_count), sizeof(contact_count));

    for (const auto& c : contacts) {
        if (!serialize_contact(file, c)) {
            std::cerr << "Erreur lors de la serialisation  d'un contact" << std::endl;
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}

// Validation d'un contact
bool phone_book::validate_contact(const contact& c) const {
    if (!Contact_validator::is_valid_last_name(c.get_last_name())) {
        std::cerr << "lastname invalide: " << c.get_last_name() << std::endl;
        return false;
    }
    if (!Contact_validator::is_valid_first_name(c.get_first_name())) {
        std::cerr << "Firstname invalide: " << c.get_first_name() << std::endl;
        return false;
    }
    if (!Contact_validator::is_valid_patronymic(c.get_patronymic())) {
        std::cerr << "Patronymic invalide: " << c.get_patronymic() << std::endl;
        return false;
    }
    if (!Contact_validator::is_valid_email(c.get_email())) {
        std::cerr << "Email invalide: " << c.get_email() << std::endl;
        return false;
    }
    if (!Contact_validator::is_valid_date_of_birth(
        std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
            c.get_birth_date().time_since_epoch()).count()))) {
        std::cerr << "birthday invalide" << std::endl;
        return false;
	}

    // Valider tous les numéros de téléphone
    for (const auto& pn : c.get_phone_numbers()) {
        if (!Contact_validator::is_valid_phone_number(pn.number)) {
            std::cerr << "number phone invalide: " << pn.number << std::endl;
            return false;
        }
    }

    return true;
}

// Fonctions de comparaison
bool phone_book::compare_by_last_name(const contact& a, const contact& b) {
    return a.get_last_name() < b.get_last_name();
}

bool phone_book::compare_by_first_name(const contact& a, const contact& b) {
    return a.get_first_name() < b.get_first_name();
}

bool phone_book::compare_by_patronymic(const contact& a, const contact& b) {
    return a.get_patronymic() < b.get_patronymic();
}

bool phone_book::compare_by_full_name(const contact& a, const contact& b) {
    if (a.get_last_name() != b.get_last_name()) {
        return a.get_last_name() < b.get_last_name();
    }
    if (a.get_first_name() != b.get_first_name()) {
        return a.get_first_name() < b.get_first_name();
    }
    return a.get_patronymic() < b.get_patronymic();
}

bool phone_book::compare_by_birth_date(const contact& a, const contact& b) {
    return a.get_birth_date() < b.get_birth_date();
}

bool phone_book::compare_by_email(const contact& a, const contact& b) {
    return a.get_email() < b.get_email();
}

// Sérialisation d'un contact
bool phone_book::serialize_contact(std::ofstream& file, const contact& c) const {
    // Sérialiser les champs de base
    auto serialize_string = [&](const std::string& str) {
        size_t length = str.length();
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(str.c_str(), length);
        };

    serialize_string(c.get_last_name());
    serialize_string(c.get_first_name());
    serialize_string(c.get_patronymic());
    serialize_string(c.get_address());
    serialize_string(c.get_email());

    // Sérialiser la date de naissance
    auto birth_time = std::chrono::system_clock::to_time_t(c.get_birth_date());
    file.write(reinterpret_cast<const char*>(&birth_time), sizeof(birth_time));

    // Sérialiser les numéros de téléphone
    const auto& phone_numbers = c.get_phone_numbers();
    size_t phone_count = phone_numbers.size();
    file.write(reinterpret_cast<const char*>(&phone_count), sizeof(phone_count));

    for (const auto& pn : phone_numbers) {
        serialize_string(pn.number);
        int type = static_cast<int>(pn.type);
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
    }

    return file.good();
}

// Désérialisation d'un contact - MÉTHODE CORRIGÉE
bool phone_book::deserialize_contact(std::ifstream& file, contact& c) {
    auto deserialize_string = [&](std::string& str) {
        size_t length = 0;
        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        if (length > 0 && length < 10000) { // Limite de sécurité
            str.resize(length);
            file.read(&str[0], length);
        }
        };

    // Lire tous les champs
    std::string last_name, first_name, patronymic, address, email;
    deserialize_string(last_name);
    deserialize_string(first_name);
    deserialize_string(patronymic);
    deserialize_string(address);
    deserialize_string(email);

    // Date de naissance
    std::time_t birth_time = 0;
    file.read(reinterpret_cast<char*>(&birth_time), sizeof(birth_time));
    auto birth_date = std::chrono::system_clock::from_time_t(birth_time);

    // Utiliser les setters pour initialiser le contact (plus robuste)
    c.set_last_name(last_name);
    c.set_first_name(first_name);
    c.set_patronymic(patronymic);
    c.set_address(address);
    c.set_birth_date(birth_date);
    c.set_email(email);

    // Numéros de téléphone
    size_t phone_count = 0;
    file.read(reinterpret_cast<char*>(&phone_count), sizeof(phone_count));

    for (size_t i = 0; i < phone_count && i < 100; ++i) {
        std::string number;
        deserialize_string(number);

        int type = 0;
        file.read(reinterpret_cast<char*>(&type), sizeof(type));

        if (type >= 0 && type <= 2) {
            contact::phone_number pn;
            pn.number = number;
            pn.type = static_cast<contact::phone_type>(type);
            c.add_phone_number(pn);
        }
    }

    return file.good();
}