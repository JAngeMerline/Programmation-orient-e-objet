#pragma once

#include<string> // (nom, adresse, mail)
#include<vector> // (numéros de téléphone)
#include<chrono> // (date de naissance)


class contact
	//pour enumerer les types de numéros de téléphone possibles
{
public:
	enum phone_type {
		professional, 
		personal, 
		fax
	};

	
	struct phone_number { //structure pour stocker un numéro de téléphone et son type
		std::string number;
		phone_type type{};
	};

private: //attributs privés
	std::string last_name;
	std::string first_name;
	std::string patronymic;
	std::string address;
	std::chrono::system_clock::time_point birth_date;
	std::string mail;
	std::vector<phone_number> phone_numbers;

public: //constructeurs 
	contact(const std::string& last_name,  //constructeur avec paramètres
		const std::string& first_name,
		const std::string& patronymic,
		const std::string& address,
		const std::chrono::system_clock::time_point& birth_date,
		const std::string& mail,
		const std::vector<phone_number>& phone_numbers);
	contact() = default; // creer un contact vide
	 
	
	//getters methhodes pour accéder aux attributs privés
	// on met const car on ne veut pas modifier l'objet contact
	std::string get_last_name() const;
	std::string get_first_name() const;
	std::string get_patronymic() const;
	std::string get_address() const;
	std::chrono::system_clock::time_point get_birth_date() const;
	std::string get_email() const;
	std::vector<phone_number> get_phone_numbers() const;

	//setters methodes pour modifier les attributs privés
	// on ne met pas const car on veut modifier l'objet contact
	//parametrees const std ::string& pour éviter la copie inutile
	void set_last_name(const std::string& last_name);
	void set_first_name(const std::string& first_name);
	void set_patronymic(const std::string& patronymic);
	void set_address(const std::string& address);
	void set_birth_date(const std::chrono::system_clock::time_point& birth_date);
	void set_email(const std::string& mail);
	void set_phone_numbers(const std::vector<phone_number>& phone_numbers);

	//gestion des numéros de téléphone
	//ajouter, supprimer, vider
	void add_phone_number(const phone_number& phone_number);
	void remove_phone_number(const std::string& number);
	void clear_phone_numbers();

	//operators
	//constructeurs et operateurs
	contact(const contact& other) = default; //constructeur de copie
	contact(contact&& other) noexcept = default; //constructeur de déplacement
	contact& operator=(const contact& other) = default;//operateur de copie
	contact& operator=(contact&& other) noexcept = default; //operateur de déplacement 
	~contact() = default;//destructeur

	//surcharge de new et delete
	// pour une gestion personnalisée de la mémoire
	//utile pour le debogage ou l'optimisation
	void* operator new(std::size_t size);
	void operator delete(void* ptr) noexcept;

	//methodes utilitaires
	std::string tostring() const; //représentation textuelle du contact
	bool isValid() const; //vérifie si le contact est valide


};