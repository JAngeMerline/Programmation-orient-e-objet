#pragma once

#include<string>
#include<vector>
#include<chrono>
#include<regex>

class Contact_validator
{
	
public:
		static bool is_valid_last_name(const std::string& name);
		static bool is_valid_first_name(const std::string& first_name);
		static bool is_valid_patronymic(const std::string& patronymic);
		static bool is_valid_email(const std::string& email);
		static bool is_valid_phone_number(const std::string& phone_number);
		static bool is_valid_date_of_birth(const std::string& date_of_birth);
		static bool validate_contact(const std::string& last_name, const std::string& first_name,
			const std::string& patronymic, const std::string& email,
			const std::string& date_str);


private:
	// Helper functions
	static std::string trim(const std::string& str);
	static bool isLeapYear(int year);
	static int getDaysInMonth(int month, int year);

	
};