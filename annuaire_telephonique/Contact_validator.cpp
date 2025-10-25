#define _CRT_SECURE_NO_WARNINGS 
#include "Contact_validator.h"
#include <cctype>
#include <ctime>
#include <algorithm>

bool Contact_validator::is_valid_last_name(const std::string& last_name) {
    std::string trimmed_last_name = trim(last_name);
    if (trimmed_last_name.empty()) {
        return false;
    }
    // Doit commencer par une lettre.
    if (!std::isalpha(trimmed_last_name[0])) {
        return false;
    }
    // Ne doit pas commencer ou finir par un tiret.
    if (trimmed_last_name.front() == '-' || trimmed_last_name.back() == '-') {
        return false;
    }

    // CORRECTION : Regex sans espaces et plus simple
    std::regex name_pattern("^[A-Za-zÀ-ÿ][A-Za-zÀ-ÿ0-9\\- ]*[A-Za-zÀ-ÿ0-9]$");
    return std::regex_match(trimmed_last_name, name_pattern);
}

bool Contact_validator::is_valid_first_name(const std::string& first_name) {
    std::string trimmed_first_name = trim(first_name);
    if (trimmed_first_name.empty()) {
        return false;
    }
    // Doit commencer par une lettre.
    if (!std::isalpha(trimmed_first_name[0])) {
        return false;
    }
    // Ne doit pas commencer ou finir par un tiret.
    if (trimmed_first_name.front() == '-' || trimmed_first_name.back() == '-') {
        return false;
    }

    // CORRECTION : Regex sans espaces et plus simple
    std::regex first_name_pattern("^[A-Za-zÀ-ÿ][A-Za-zÀ-ÿ0-9\\- ]*[A-Za-zÀ-ÿ0-9]$");
    return std::regex_match(trimmed_first_name, first_name_pattern);
}

bool Contact_validator::is_valid_patronymic(const std::string& patronymic) {
    std::string trimmed_patronymic = trim(patronymic);
    if (trimmed_patronymic.empty()) {
        return false;
    }
    // Doit commencer par une lettre.
    if (!std::isalpha(trimmed_patronymic[0])) {
        return false;
    }
    // Ne doit pas commencer ou finir par un tiret.
    if (trimmed_patronymic.front() == '-' || trimmed_patronymic.back() == '-') {
        return false;
    }

    // CORRECTION : Regex sans espaces et plus simple
    std::regex name_pattern("^[A-Za-zÀ-ÿ][A-Za-zÀ-ÿ0-9\\- ]*[A-Za-zÀ-ÿ0-9]$");
    return std::regex_match(trimmed_patronymic, name_pattern);
}

bool Contact_validator::is_valid_email(const std::string& email) {
    std::string trimmed = trim(email);

    // Vérifier la présence d'un @
    size_t at_pos = trimmed.find('@');
    if (at_pos == std::string::npos) return false;

    // Séparer et trimmer les parties
    std::string local_part = trim(trimmed.substr(0, at_pos));
    std::string domain_part = trim(trimmed.substr(at_pos + 1));

    if (local_part.empty() || domain_part.empty()) return false;

    // Reconstruire sans espaces autour du @
    std::string clean_email = local_part + "@" + domain_part;

    // Regex stricte : uniquement lettres et chiffres latins
    std::regex pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    return std::regex_match(clean_email, pattern);
}

bool Contact_validator::is_valid_phone_number(const std::string& phone_number) {

    std::vector<std::regex> patterns = {
        std::regex(R"(^\+7812\d{7}$)"), // +7 suivi de 10 chiffres
        std::regex(R"(^8812\d{7}$)"), // 8 suivi de 10 chiffres
        std::regex(R"(^\+7\(812\)\d{7}$)"), // +7(812) suivi de 7 chiffres
        std::regex(R"(^8\(812\)\d{7}$)"), // 8(812) suivi de 7 chiffres
        std::regex(R"(^\+7\(812\)\d{3}-\d{2}-\d{2}$)"), // +7(812)XXX-XX-XX
        std::regex(R"(^8\(812\)\d{3}-\d{2}-\d{2}$)") // 8(812)XXX-XX-XX
    };

    for (const auto& pattern : patterns) {
        if (std::regex_match(phone_number, pattern)) {
            return true;
        }
    }
    return false;
}

bool Contact_validator::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Contact_validator::getDaysInMonth(int month, int year) {
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        return isLeapYear(year) ? 29 : 28;
    default:
        return 0; // Mois invalide
    }
}

bool Contact_validator::is_valid_date_of_birth(const std::string& date_of_birth) {
    // Format attendu: "DD/MM/YYYY"
    if (date_of_birth.length() != 10 || date_of_birth[2] != '/' || date_of_birth[5] != '/') {
        return false;
    }

    try {
        int day = std::stoi(date_of_birth.substr(0, 2));
        int month = std::stoi(date_of_birth.substr(3, 2));
        int year = std::stoi(date_of_birth.substr(6, 4));

        // Vérifier les plages valides pour le jour, le mois et l'année
        if (year < 1900 || month < 1 || month > 12 || day < 1) {
            return false;
        }

        int days_in_month = getDaysInMonth(month, year);
        if (day > days_in_month) {
            return false;
        }

        // Vérifier que la date n'est pas dans le futur
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        if (year > (now->tm_year + 1900) ||
            (year == (now->tm_year + 1900) && month > (now->tm_mon + 1)) ||
            (year == (now->tm_year + 1900) && month == (now->tm_mon + 1) && day > now->tm_mday)) {
            return false;
        }
        return true;
    }
    catch (...) {
        return false;
    }
}

bool Contact_validator::validate_contact(const std::string& last_name, const std::string& first_name,
    const std::string& patronymic, const std::string& email,
    const std::string& date_str) {
    return is_valid_last_name(last_name) &&
        is_valid_first_name(first_name) &&
        is_valid_patronymic(patronymic) &&
        is_valid_email(email) &&
        is_valid_date_of_birth(date_str);
}

std::string Contact_validator::trim(const std::string& str) {
    const std::string whitespace = " \t\n\r\f\v";
    size_t first = str.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, (last - first + 1));
}