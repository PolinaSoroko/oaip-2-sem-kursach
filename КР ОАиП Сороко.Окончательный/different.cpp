#include"Header.h"
int current_user_id = -1;
extern Admin admin;
void writeAdmintoFile() {

    ofstream fout(Admin_FILE_OF_DATA, ios::app);
    if (!fout.is_open()) {
        cout << "\033[31mОшибка открытия файла для записи данных администратора!\033[0m" << endl;

    }
    fout << admin.username << " "
        << admin.password << " "
        << admin.email << " "
        << admin.id << endl;

}

//--------------------------------------------- Авторизация для админа и кандидата ---------------------------------------------

void login() {
    ifstream fin(Admin_FILE_OF_DATA);
    if (!fin) {
        cout << "\033[31mОшибка: не удалось открыть файл.\033[0m\n";
        return;
    }
    fin >> admin.username >> admin.password;
    fin.close();

    Candidate* arr_of_candidates = nullptr;
    int number_of_candidates = 0;
    readFileCandidates(arr_of_candidates, number_of_candidates);

    string username, password;
    cout << "\033[93mВведите логин: \033[0m";
    inputString(username);

    // --- Администратор ---
    if (username == admin.username) {
        int adminAttempts = 3;
        while (adminAttempts--) {
            cout << "\033[93mВведите пароль \033[4mадминистратора\033[0m\033[93m:\033[0m ";
            inputString(password);
            if (password == admin.password) {
                cout << "\033[3;92mВы успешно вошли как администратор!\033[0m\n";
                delete[] arr_of_candidates;
                AdminMenu();
                return;
            }
            else {
                if (adminAttempts > 0)
                    cout << "\033[31mНеверный пароль. Осталось попыток:\033[0m " << adminAttempts << "\n";
            }
        }
        cout << "\033[31mПревышено количество попыток входа администратора.\033[0m\n";
        delete[] arr_of_candidates;
        return;
    }

    // --- Пользователь ---
    int userAttempts = 3;
    while (userAttempts--) {
        bool loginFound = false;

        for (int i = 0; i < number_of_candidates; ++i) {
            if (arr_of_candidates[i].username == username) {
                loginFound = true;
                cout << "\033[93mВведите пароль:\033[0m ";
                inputString(password);

                if (arr_of_candidates[i].password == password) {
                    cout << "\033[3;92mУспешный вход. Добро пожаловать,\033[0m " << arr_of_candidates[i].username << "\033[3;92m!\033[0m\n";
                    current_user_id = arr_of_candidates[i].id;
                    delete[] arr_of_candidates;
                    UserMenu();
                    return;
                }
                else {
                    if (userAttempts > 0)
                        cout << "\033[31mНеверный пароль. Осталось попыток:\033[0m  " << userAttempts << "\n";
                    break;
                }
            }
        }

        if (!loginFound) {
            cout << "\033[95mПользователь с таким логином не найден.\033[0m \n";
            cout << "\033[30;105mХотите зарегистрироваться? \033[0m\033[90m(1 - Да, 2 - Нет):\033[0m ";
            int choice;
            failInt(choice);

            if (choice == 1) {
                addCandidateToFile(arr_of_candidates, number_of_candidates);
                delete[] arr_of_candidates;
                readFileCandidates(arr_of_candidates, number_of_candidates);
                Candidate& last_registered = arr_of_candidates[number_of_candidates - 1];
                current_user_id = last_registered.id;
                cout << "\033[3;92mУспешная регистрация и вход. Добро пожаловать,\033[0m " << last_registered.username << "\033[3;92m!\033[0m\n";
                UserMenu();
                return;
            }
            else {
                break;
            }
        }
    }
    delete[] arr_of_candidates;
}



//--------------------------------------------- Меню админа и кандидатов ---------------------------------------------

void FirstMenu() {
    int choice;
    do {
        
        cout << "\033[90m1 - \033[94mАвторизация\033[0m\n"
            << "\033[90m2 - \033[94mВыход из программы\033[0m\n"
            << "\033[30;104mВаш выбор:\033[0m ";

        failInt(choice);
        switch (choice) {
        case 1:login(); break;
        case 2:return; break;
        default: cout << "\033[31mНекорректный ввод. Попробуйте снова.\033[0m\n";
        }
    } while (choice != 2);
}
void AdminMenu() {
    int role = 0;
    int number_of_vacancies = 0;
    Vacancy* arr_of_vacancies = nullptr;
    readFileVacancies(arr_of_vacancies, number_of_vacancies);
    int choice;
    do {
        cout << "\n\033[30;104mМеню администратора:\033[0m\n"
            << "\033[90m1 - \033[94mПросмотр вакансий в табличной форме\033[0m\n"
            << "\033[90m2 - \033[94mДобавление вакансии\033[0m\n"
            << "\033[90m3 - \033[94mРедактирование вакансии\033[0m\n"
            << "\033[90m4 - \033[94mУдаление вакансии\033[0m\n"
            << "\033[90m5 - \033[94mПоиск вакансии\033[0m\n"
            << "\033[90m6 - \033[94mФильтрация вакансий\033[0m\n"
            << "\033[90m7 - \033[94mСортировка вакансий\033[0m\n"
            << "\033[90m8 - \033[94mУправление кандидатами\033[0m\n"
            << "\033[90m9 - \033[94mНазад\033[0m\n"
            << "\033[30;104mВаш выбор:\033[0m ";

        failInt(choice);

        switch (choice) {
        case 1: showVacancyArray(arr_of_vacancies, number_of_vacancies,role, current_user_id); break;
        case 2: addVacancyToFile(arr_of_vacancies, number_of_vacancies); break;
        case 3: editVacancy(arr_of_vacancies, number_of_vacancies); break;
        case 4: delVacancyFromArray(arr_of_vacancies, number_of_vacancies); break;
        case 5: searchVacancyMenu(role, current_user_id); break;
        case 6: filterVacancyMenu(role, current_user_id); break;
        case 7: sortVacancyMenu(); break;
        case 8: ManageCandidates(); break;
        case 9: return; break;
        default: cout << "\033[31mНекорректный ввод. Попробуйте снова.\033[0m\n";
        }
    } while (true);
}
void UserMenu() {
    int role = 1;
    int number_of_vacancies = 0;
    Vacancy* arr_of_vacancies = nullptr;
    readFileVacancies(arr_of_vacancies, number_of_vacancies);
    int choice;
    do {
        cout << "\n\033[30;104mМеню пользователя:\033[0m\n"
            << "\033[90m1 - \033[94mПросмотр вакансий в табличной форме\033[0m\n"
            << "\033[90m2 - \033[94mПоиск вакансий\033[0m\n"
            << "\033[90m3 - \033[94mФильтрация вакансий\033[0m\n"
            << "\033[90m4 - \033[94mНазад\033[0m\n"
            << "\033[30;104mВаш выбор:\033[0m ";

        failInt(choice);

        switch (choice) {
        case 1: showVacancyArray(arr_of_vacancies, number_of_vacancies,role, current_user_id); break;
        case 2: searchVacancyMenu(role, current_user_id); readFileVacancies(arr_of_vacancies, number_of_vacancies);  break;
        case 3: filterVacancyMenu(role, current_user_id); readFileVacancies(arr_of_vacancies, number_of_vacancies);  break;
        case 4: return;
        default: cout << "\033[31mНекорректный ввод. Попробуйте снова.\033[0m\n";
        }
    } while (true);
}

//-------------------------------------------- Генерация ID и время регистрации, форматированный вывод---------------------------------------------

int generateRandomUniqueID(Candidate* arr_of_candidates, int number_of_candidates) {
    int id;
    bool unique;
    do {
        id = rand() % 90000000 + 10000000;
        unique = true;
        for (int i = 0; i < number_of_candidates; ++i) {
            if (arr_of_candidates[i].id == id || id == 11111111) {
                unique = false;
                break;
            }
        }
    } while (!unique);
    return id;
}
int generateRandomUniqueIDForVacancy(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    int id;
    bool unique;
    do {
        id = rand() % 900 + 100;
        unique = true;
        
        for (int i = 0; i < number_of_vacancies; ++i) {
            if (arr_of_vacancies[i].id == id) {
                unique = false;
                break;
            }
        }
    } while (!unique);
    return id;
}
string getCurrentTime() {
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);

    char buf[20];
    strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M", &localTime);

    return string(buf);
}

void printCandidateTable(Candidate* arr_of_candidates, int number_of_candidates) {
    topTableCandidates();
    for (int i = 0; i < number_of_candidates; ++i) {
        cout << left << setw(3) << i + 1 << "|"
            << setw(20) << arr_of_candidates[i].username << "|"
            << setw(20) << arr_of_candidates[i].password << "|"
            << setw(20) << arr_of_candidates[i].email << "|"
            << setw(10) << arr_of_candidates[i].id << "|"
            << setw(20) << arr_of_candidates[i].register_time << "|" << endl;

    }
}
void topTableCandidates() {
    cout << string(100, '-') << endl;
    cout << left
        << setw(4) << "№"
        << setw(21) << "Логин"
        << setw(22) << "Пароль"
        << setw(21) << "Email"
        << setw(11) << "ID"
        << setw(20) << "Дата регистрации"
        << endl;
    cout << string(100, '-') << endl;
}

void printVacancyTable(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    topTableVacancies();
    for (int i = 0; i < number_of_vacancies; ++i) {
        cout << left << setw(3) << i + 1 << "|"
            << setw(6) << arr_of_vacancies[i].id << "|"
            << setw(20) << arr_of_vacancies[i].position << "|"
            << setw(8) << arr_of_vacancies[i].salary << "|"
            << setw(20) << arr_of_vacancies[i].location << "|"
            << setw(18) << arr_of_vacancies[i].requirements << "|"
            << setw(20) << arr_of_vacancies[i].conditions << "|"
            << setw(18) << arr_of_vacancies[i].day << "|"
            << setw(10) << arr_of_vacancies[i].count_of_open_vacancies << "|"
            << setw(10) << arr_of_vacancies[i].number_of_responses 
            << endl;
    }
}
void topTableVacancies() {
    cout << string(3 + 6 + 20 + 8 + 20 + 18 + 20 + 18 + 10 + 20, '-') << endl;
    cout << left
        << setw(3) << "№" << "|"
        << setw(6) << "ID" << "|"
        << setw(20) << "Должность" << "|"
        << setw(8) << "Зарплата" << "|"
        << setw(20) << "Место" << "|"
        << setw(18) << "Требования" << "|"
        << setw(20) << "Мы предлагаем" << "|"
        << setw(18) << "Занятость" << "|"
        << setw(10) << "Количество" << "|"
        << setw(10) << "Отклики"
    << endl;

    cout << string(3 + 6 + 20 + 8 + 20 + 18 + 20 + 18 + 10 + 20, '-') << endl;
}

string readQuotedField(istringstream& iss) {
    string field, word;
    if (!(iss >> word)) return "";

    if (!word.empty() && word.front() == '"') {
        field = word.substr(1);
        if (word.length() > 1 && word.back() == '"') {
            field = word.substr(1, word.length() - 2);
        }
        else {
            while (iss >> word) {
                if (!word.empty() && word.back() == '"') {
                    field += ' ' + word.substr(0, word.length() - 1);
                    break;
                }
                else {
                    field += ' ' + word;
                }
            }
        }
    }
    else {
        field = word;
    }
    return field;
}


//void loginAdmin() {
//    ifstream fin(Admin_FILE_OF_DATA);
//    if (!fin) {
//        cout << "Ошибка: не удалось открыть файл admin.txt\n";
//        return;
//    }
//    fin >> admin.username >> admin.password;
//    fin.close();
//    string login, password;
//    while (true) {
//        cout << "Введите логин: ";
//        inputString(login);
//        cout << "Введите пароль: ";
//        inputString(password);
//
//        if (admin.username == login && admin.password == password) {
//            cout << "Успешный вход!\n";
//            AdminMenu();
//            return;
//        }
//        else {
//            cout << "Неверный логин или пароль. Попробуйте снова.\n";
//        }
//    }
//
//}
//void loginUser() {
//    Candidate* arr_of_candidates = nullptr;
//    int number_of_candidates = 0;
//    readFileCandidates(arr_of_candidates, number_of_candidates);
//
//    if (number_of_candidates == 0) {
//        cout << "Нет зарегистрированных пользователей.\n";
//        return;
//    }
//
//    string enteredUsername, enteredPassword;
//    cout << "Введите логин: ";
//    inputString(enteredUsername);
//    cout << "Введите пароль: ";
//    inputString(enteredPassword);
//
//    bool found = false;
//    for (int i = 0; i < number_of_candidates; ++i) {
//        if (arr_of_candidates[i].username == enteredUsername && arr_of_candidates[i].password == enteredPassword) {
//            cout << "Успешный вход. Добро пожаловать, " << arr_of_candidates[i].username << "!\n";
//            found = true;
//            UserMenu();
//            break;
//        }
//    }
//    if (!found && enteredUsername!="admin") {
//        cout << "Такого пользователя нет.Сначала зарегистрируйтесь.\n";
//        addCandidateToFile(arr_of_candidates, number_of_candidates);
//        UserMenu();
//    }
//    else if (enteredUsername == "admin") {
//        cout << "Вы не можете зайти с данными администратора.";
//    }
//
//    delete[] arr_of_candidates;
////}
//void loginUserMenu() {
//    int choice;
//    Candidate* arr_of_candidates = nullptr;
//    int number_of_candidates = 0;
//    readFileCandidates(arr_of_candidates, number_of_candidates);
//
//    cout << "Выберите действие:\n" << "1 - Зарегистрироваться\n" << "2 - Войти в аккаунт\n" << "3 - Назад\n";
//    failInt( choice);
//    switch (choice) {
//    case 1:
//        addCandidateToFile( arr_of_candidates, number_of_candidates);
//        UserMenu();
//        break;
//    case 2: loginUser(); break;
//    case 3: return; break;
//    default: cout << "Некорректный ввод.\n"; break;
//    }
//}