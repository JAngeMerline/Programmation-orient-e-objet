#pragma once

#include "contact.h"
#include <vector>
#include <string>
#include <functional>
#include <fstream>  // Ajout nécessaire pour std::ifstream/std::ofstream

class phone_book {
private:
    std::vector<contact> contacts;
    std::string file_path;

public:
    // Constructeur
    phone_book(const std::string& file_path = "contacts.dat");

    // Gestion des contacts
    bool add_contact(const contact& new_contact);
    bool remove_contact(const std::string& last_name, const std::string& first_name, const std::string& patronymic);
    bool update_contact(const std::string& last_name, const std::string& first_name, const std::string& patronymic, const contact& updated_contact);

    // Recherche
    std::vector<contact> search_by_last_name(const std::string& last_name) const;
    std::vector<contact> search_by_first_name(const std::string& first_name) const;
    std::vector<contact> search_by_patronymic(const std::string& patronymic) const;
    std::vector<contact> search_by_full_name(const std::string& last_name, const std::string& first_name, const std::string& patronymic) const;
    std::vector<contact> search_by_phone_number(const std::string& phone_number) const;
    std::vector<contact> search_by_email(const std::string& email) const;

    // Tri
    void sort_by_last_name();
    void sort_by_first_name();
    void sort_by_patronymic();
    void sort_by_full_name();
    void sort_by_birth_date();
    void sort_by_email();

    // Accès aux contacts
    const std::vector<contact>& get_all_contacts() const;
    contact* get_contact(size_t index);
    const contact* get_contact(size_t index) const;
    size_t get_contact_count() const;

    // Persistance
    void load_from_file();
    bool save_to_file() const;

    // Validation
    bool validate_contact(const contact& c) const;

private:
    // Fonctions de comparaison
    static bool compare_by_last_name(const contact& a, const contact& b);
    static bool compare_by_first_name(const contact& a, const contact& b);
    static bool compare_by_patronymic(const contact& a, const contact& b);
    static bool compare_by_full_name(const contact& a, const contact& b);
    static bool compare_by_birth_date(const contact& a, const contact& b);
    static bool compare_by_email(const contact& a, const contact& b);

    // Sérialisation des contacts - DÉCLARATIONS SEULEMENT
    bool serialize_contact(std::ofstream& file, const contact& c) const;
    bool deserialize_contact(std::ifstream& file, contact& c);  // DÉCLARATION seulement
};