#pragma once
#include <iostream>
#include <clocale>
#include <locale>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <windows.h> 

using namespace std;

struct Candidate {
    string username;
    string password;
    string email;
    int id;
    string register_time;
};

struct Admin {
    string username = "admin";
    string password = "admin12345";
    string email = "admin@gmail.com";
    int id = 11111111;
};

struct Vacancy {
    int id;
    string position;
    int salary;
    string location;
    string requirements;
    string conditions;
    string day;
    int count_of_open_vacancies;
    int number_of_responses=0;
};
struct Response {
    int user_id;
    int vacancy_id;
};
struct UserCount {
    int user_id;
    int count;
};

const string Candidates_FILE_OF_DATA = "Candidates.txt";
const string Admin_FILE_OF_DATA = "AdminData.txt";
const string Vacancies_FILE_OF_DATA = "Vacancies.txt";

void writeAdmintoFile();

void login();


void FirstMenu();
void AdminMenu();
void UserMenu();

void addCandidateToFile(Candidate*& arr_of_candidates, int& number_of_candidates);
void writeFileCandidates(Candidate* arr_of_candidates, int number_of_candidates);
void readFileCandidates(Candidate*& arr_of_candidates, int& number_of_candidates);
void showCandidateArray(Candidate* arr_of_candidates, int number_of_candidates);

void filterByUsername(Candidate* arr_of_candidates, int number_of_candidates);
void filterByEmail(Candidate* arr_of_candidates, int number_of_candidates);
void filterByRegister_time(Candidate* arr_of_candidates, int number_of_candidates);
void filterCandidateMenu();

void searchByUsername(Candidate* arr_of_candidates, int number_of_candidates);
void searchByEmail(Candidate* arr_of_candidates, int number_of_candidates);
void searchByRegister_time(Candidate* arr_of_candidates, int number_of_candidates);
void searchCandidateMenu();

void sortByUsername(Candidate* arr_of_candidates, int number_of_candidates);
void sortByEmail(Candidate* arr_of_candidates, int number_of_candidates);
void sortByRegister_time(Candidate* arr_of_candidates, int number_of_candidates);
void sortCandidateMenu();

void delCandidateFromArray(Candidate*& arr_of_candidates, int& number_of_candidates);
void ManageCandidates();

int generateRandomUniqueID(Candidate* arr_of_candidates, int number_of_candidates);
int generateRandomUniqueIDForVacancy(Vacancy* arr_of_vacancies, int number_of_vacancies);
string getCurrentTime();
string readQuotedField(istringstream& iss);
void printCandidateTable(Candidate* arr_of_candidates, int number_of_candidates);
void printVacancyTable(Vacancy* arr_of_vacancies, int number_of_vacancies);
void topTableVacancies();
void topTableCandidates();

void addVacancyToFile(Vacancy* arr_of_vacancies, int number_of_vacancies);
void readFileVacancies(Vacancy*& arr_of_vacancies, int& number_of_vacancies);
void writeFileVacancies(Vacancy* arr_of_vacancies, int number_of_vacancies);
void showVacancyArray(Vacancy* arr_of_vacancies, int number_of_vacancies,int role, int current_user_id);

void delVacancyFromArray(Vacancy*& arr_of_vacancies, int& number_of_vacancies);
void removeResponsesForVacancy(int deleted_vacancy_id);
void removeResponsesForCandidate(int deleted_user_id);
void editVacancy(Vacancy*& arr_of_vacancies, int& number_of_vacancies);
void filterVacancyMenu(int role, int current_user_id);
void filterBySalary(Vacancy* arr_of_vacancies, int number_of_vacancies);
void filterByPosition(Vacancy* arr_of_vacancies, int number_of_vacancies);
void filterByEmployment(Vacancy* arr_of_vacancies, int number_of_vacancies);

void searchVacancyMenu(int role, int current_user_id);
void searchBySalary(Vacancy* arr_of_vacancies, int number_of_vacancies);
void searchByPosition(Vacancy* arr_of_vacancies, int number_of_vacancies);
void searchByEmployment(Vacancy* arr_of_vacancies, int number_of_vacancies);


void sortBySalary(Vacancy* arr_of_vacancies, int number_of_vacancies);
void sortByPosition(Vacancy* arr_of_vacancies, int number_of_vacancies);
void sortByEmployment(Vacancy* arr_of_vacancies, int number_of_vacancies);
void sortVacancyMenu();

void respondToVacancy(Vacancy* arr_of_vacancies, int number_of_vacancies, int user_id);
void readFileResponses(Response*& responses, int& count);
void writeFileResponses(Response* responses, int count);


void inputString(string& input);
void failInt(int& value);

//void loginUserMenu();
//void loginAdmin();
//void loginUser();