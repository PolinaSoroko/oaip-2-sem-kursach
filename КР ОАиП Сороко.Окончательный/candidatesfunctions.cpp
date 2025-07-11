#include"Header.h"

//--------------------------------------------- Функции для работы с КАНДИДАТАМИ ---------------------------------------------
void ManageCandidates() {
    srand(time(0));
    int number_of_candidates = 0;
    Candidate* arr_of_candidates = nullptr;
    readFileCandidates(arr_of_candidates, number_of_candidates);

    int choice;

    do {
        cout << "\n\033[30;46mМеню управления кандидатами:\033[0m";
        cout << "\n\033[90m1 - \033[36mПоказать список кандидатов\033[0m";
        cout << "\n\033[90m2 - \033[36mДобавить кандидата\033[0m";
        cout << "\n\033[90m3 - \033[36mУдалить кандидата\033[0m";
        cout << "\n\033[90m4 - \033[36mПоиск кандидатов\033[0m";
        cout << "\n\033[90m5 - \033[36mФильтрация кандидатов\033[0m";
        cout << "\n\033[90m6 - \033[36mСортировка кандидатов\033[0m";
        cout << "\n\033[90m7 - \033[36mВыход\033[0m\n";
        cout << "\033[30;46mВаш выбор:\033[0m ";
        failInt(choice);

        switch (choice) {
        case 1:
            readFileCandidates(arr_of_candidates, number_of_candidates);
            showCandidateArray(arr_of_candidates, number_of_candidates);
            break;
        case 2:
            addCandidateToFile(arr_of_candidates, number_of_candidates);
            readFileCandidates(arr_of_candidates, number_of_candidates);
            break;
        case 3:
            delCandidateFromArray(arr_of_candidates, number_of_candidates);
            break;
        case 4:
            searchCandidateMenu(); break;
        case 5:
            filterCandidateMenu(); break;
        case 6:
            sortCandidateMenu(); break;
        case 7:
            writeFileCandidates(arr_of_candidates, number_of_candidates);
            return;
            break;
        default:
            cout << "\033[31mНекорректный выбор!\033[0m\n";
            break;
        }
    } while (choice != 4);

    delete[] arr_of_candidates;
    return;
}

void addCandidateToFile(Candidate*& arr_of_candidates, int& number_of_candidates) {
   
    Candidate candidate;
    cout << "\033[95mВведите логин:\033[0m ";
    inputString(candidate.username);
    while (true) {
        bool exists = false;
        for (int i = 0; i < number_of_candidates; i++) {
            if (candidate.username == arr_of_candidates[i].username || candidate.username == "admin") {
                exists = true;
                break;
            }
        }

        if (exists) {
            cout << "\033[35mТакой логин уже существует. Введите новый:\033[0m ";
            inputString(candidate.username);
        }
        else {
            break;
        }
    }

    cout << "\033[95mВведите пароль: \033[0m";
    inputString(candidate.password);
    while (true) {
        bool exists = false;
        for (int i = 0; i < number_of_candidates; i++) {
            if (candidate.password == arr_of_candidates[i].password || candidate.password == "admin12345") {
                exists = true;
                break;
            }
        }

        if (exists) {
            cout << "\033[35mТакой пароль уже существует. Введите новый:\033[0m ";
            inputString(candidate.password);
        }
        else {
            break;
        }
    }
    cout << "\033[95mВведите email (без @gmail.com):\033[0m ";
    inputString(candidate.email);
    while (true) {
        bool exists = false;
        for (int i = 0; i < number_of_candidates; i++) {
            if (candidate.email + "@gmail.com" == arr_of_candidates[i].email || candidate.email + "@gmail.com" == "admin@gmail.com") {
                exists = true;
                break;
            }
        }

        if (exists) {
            cout << "\033[35mТакой email уже существует. Введите новый:\033[0m ";
            inputString(candidate.email);
        }
        else {
            break;
        }
    }
    candidate.id = generateRandomUniqueID(arr_of_candidates, number_of_candidates);

    cout << "\033[90mID (сгенерировано автоматически):\033[0m " << candidate.id << endl;

    candidate.register_time = getCurrentTime();
    cout << "\033[90mВремя регистрации: \033[0m" << candidate.register_time << endl;

    ofstream fout(Candidates_FILE_OF_DATA, ios::app);
    if (fout.is_open()) {
        fout << candidate.username << " "
            << candidate.password << " "
            << candidate.email << "@gmail.com" << ' '
            << candidate.id << ' '
            << candidate.register_time << '\n';
        fout.close();
    }
    else {
        cout << "\033[31mОшибка при открытии файла для записи данных кандидата.\033[0m\n";
    }
}
void readFileCandidates(Candidate*& arr_of_candidates, int& number_of_candidates) {
    static bool file_notified = false;
    ifstream fin(Candidates_FILE_OF_DATA);
    if (!fin.is_open()) {
        if (!file_notified) {
            cout << "\033[31mФайл не найден. Будет создан новый для записи данных о пользователях.\033[0m\n";
            file_notified = true;
        }
        arr_of_candidates = nullptr;
        number_of_candidates = 0;
        return;
    }
    const int initial_capacity = 10;
    int capacity = initial_capacity;
    arr_of_candidates = new Candidate[capacity];
    number_of_candidates = 0;

    Candidate temp;
    while (fin >> temp.username >> temp.password >> temp.email >> temp.id) {
        fin.ignore();
        getline(fin, temp.register_time);

        if (number_of_candidates >= capacity) {
            capacity *= 2;
            Candidate* new_arr = new Candidate[capacity];
            for (int i = 0; i < number_of_candidates; ++i) {
                new_arr[i] = arr_of_candidates[i];
            }
            delete[] arr_of_candidates;
            arr_of_candidates = new_arr;
        }

        arr_of_candidates[number_of_candidates++] = temp;
    }

    fin.close();
}
void showCandidateArray(Candidate* arr_of_candidates, int number_of_candidates) {
    if (number_of_candidates == 0) {
        cout << "\033[35mСписок кандидатов пуст.\033[0m\n";
        return;
    }
    topTableCandidates();

    for (int i = 0; i < number_of_candidates; ++i) {
        cout << left
            << setw(3) << i + 1 << "|"
            << setw(20) << arr_of_candidates[i].username << "|"
            << setw(20) << arr_of_candidates[i].password << "|"
            << setw(20) << arr_of_candidates[i].email << "|"
            << setw(10) << arr_of_candidates[i].id << "|"
            << setw(20) << arr_of_candidates[i].register_time << "|"
            << endl;
    }
}

void writeFileCandidates(Candidate* arr_of_candidates, int number_of_candidates) {
    ofstream fout(Candidates_FILE_OF_DATA, ios::out);
    if (!fout.is_open()) {
        cout << "\033[31mОшибка открытия файла для записи!\033[0m" << endl;
        return;
    }


    for (int i = 0; i < number_of_candidates; ++i) {
        fout << arr_of_candidates[i].username << " "
            << arr_of_candidates[i].password << " "
            << arr_of_candidates[i].email << " "
            << arr_of_candidates[i].id << " "
            << arr_of_candidates[i].register_time << endl;
    }
    fout.close();
}
void delCandidateFromArray(Candidate*& arr_of_candidates, int& number_of_candidates) {
    readFileCandidates(arr_of_candidates, number_of_candidates);

    if (number_of_candidates == 0) {
        cout << "\033[35mНет кандидатов для удаления.\033[0m\n";
        return;
    }

    int id, index;
    bool found = false;

    do {
        cout << "\033[91mВведите ID кандидата для удаления: \033[0m";
        failInt(id);
        found = false;

        for (int i = 0; i < number_of_candidates; ++i) {
            if (id == arr_of_candidates[i].id) {
                index = i;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "\033[35mКандидата с таким ID не существует. Повторите ввод.\033[0m\n";
        }
    } while (!found);

    Candidate* new_arr = new Candidate[number_of_candidates - 1];
    for (int i = 0, j = 0; i < number_of_candidates; ++i) {
        if (i != index) {
            new_arr[j++] = arr_of_candidates[i];
        }
    }
    delete[] arr_of_candidates;
    arr_of_candidates = new_arr;
    number_of_candidates--;
    writeFileCandidates(arr_of_candidates, number_of_candidates);
    cout << "\033[3;92mКандидат успешно удалён.\033[0m\n";
    removeResponsesForCandidate(id);
}

void filterByUsername(Candidate* arr_of_candidates, int number_of_candidates) {
    string username_query;
    cout << "\033[96mВведите логин для фильтрации:\033[0m ";
    inputString(username_query);
    bool found = false;
    for (int i = 0; i < number_of_candidates; ++i) {
        if (arr_of_candidates[i].username.find(username_query) != string::npos) {
            if (!found) {
                topTableCandidates();;
            }
            cout << left << setw(3) << i + 1 << "|"
                << "\033[96m" << setw(20) << arr_of_candidates[i].username << "\033[0m" << "|"
                << setw(20) << arr_of_candidates[i].password << "|"
                << setw(20) << arr_of_candidates[i].email << "|"
                << setw(10) << arr_of_candidates[i].id << "|"
                << setw(20) << arr_of_candidates[i].register_time << "|" << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mКандидаты не найдены.\033[0m\n";
    }
}
void filterByEmail(Candidate* arr_of_candidates, int number_of_candidates) {
    string email_query;
    cout << "\033[96mВведите email для фильтрации:\033[0m ";
    inputString(email_query);
    bool found = false;
    for (int i = 0; i < number_of_candidates; ++i) {
        if (arr_of_candidates[i].email.find(email_query) != string::npos) {
            if (!found) {
                topTableCandidates();;
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(20) << arr_of_candidates[i].username << "|"
                << setw(20) << arr_of_candidates[i].password << "|"
                << "\033[96m" << setw(20) << arr_of_candidates[i].email << "\033[0m" << "|"
                << setw(10) << arr_of_candidates[i].id << "|"
                << setw(20) << arr_of_candidates[i].register_time << "|" << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mКандидаты не найдены.\033[0m\n";
    }

}
void filterByRegister_time(Candidate* arr_of_candidates, int number_of_candidates) {
    string dateTime_query;
    cout << "\033[96mВведите дату и/или время регистрации:\033[0m ";
    inputString(dateTime_query);
    bool found = false;
    for (int i = 0; i < number_of_candidates; ++i) {
        if (arr_of_candidates[i].register_time.find(dateTime_query) != string::npos) {
            if (!found) {
                topTableCandidates();;
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(20) << arr_of_candidates[i].username << "|"
                << setw(20) << arr_of_candidates[i].password << "|"
                << setw(20) << arr_of_candidates[i].email << "|"
                << setw(10) << arr_of_candidates[i].id << "|"
                << "\033[96m" << setw(20) << arr_of_candidates[i].register_time << "\033[0m" << "|" << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mКандидаты не найдены.\033[0m\n";
    }
}
void filterCandidateMenu() {
    Candidate* arr_of_candidates = nullptr;
    int number_of_candidates = 0;
    readFileCandidates(arr_of_candidates, number_of_candidates);

    int choice;
    do {
        cout << "\n\033[30;106mПо какому параметру будет проводиться фильтрация?\033[0m\n"
            << "\033[90m1 - \033[96mПо логину\033[0m\n"
            << "\033[90m2 - \033[96mПо email\033[0m\n"
            << "\033[90m3 - \033[96mПо дате и времени регистрации\033[0m\n"
            << "\033[90m4 - \033[96mНазад\033[0m\n"
            << "\033[30;106mВыбор:\033[0m ";
        failInt(choice);

        switch (choice) {
        case 1: filterByUsername(arr_of_candidates, number_of_candidates); break;
        case 2: filterByEmail(arr_of_candidates, number_of_candidates); break;
        case 3: filterByRegister_time(arr_of_candidates, number_of_candidates); break;
        case 4: break;
        default: cout << "\033[31mНекорректный ввод. Повторите.\033[0m\n"; break;
        }
    } while (choice != 4);

    delete[] arr_of_candidates;
}

void searchByUsername(Candidate* arr_of_candidates, int number_of_candidates) {
    string username;
    cout << "\033[96mВведите логин для поиска:\033[0m ";
    inputString(username);
    bool found = false;
    for (int i = 0; i < number_of_candidates; ++i) {
        if (arr_of_candidates[i].username == username) {
            if (!found) {
                topTableCandidates();;
            }
            cout << left << setw(3) << i + 1 << "|"
                << "\033[96m" << setw(20) << arr_of_candidates[i].username << "\033[0m" << "|"
                << setw(20) << arr_of_candidates[i].password << "|"
                << setw(20) << arr_of_candidates[i].email << "|"
                << setw(10) << arr_of_candidates[i].id << "|"
                << setw(20) << arr_of_candidates[i].register_time << "|" << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mКандидаты не найдены.\033[0m\n";
    }
}
void searchByEmail(Candidate* arr_of_candidates, int number_of_candidates) {
    string email;
    cout << "\033[96mВведите email для поиска:\033[0m ";
    inputString(email);
    bool found = false;
    for (int i = 0; i < number_of_candidates; ++i) {
        if (arr_of_candidates[i].email == email) {
            if (!found) {
                topTableCandidates();;
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(20) << arr_of_candidates[i].username << "|"
                << setw(20) << arr_of_candidates[i].password << "|"
                << "\033[96m" << setw(20) << arr_of_candidates[i].email << "\033[0m" << "|"
                << setw(10) << arr_of_candidates[i].id << "|"
                << setw(20) << arr_of_candidates[i].register_time << "|" << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mКандидаты не найдены.\033[0m\n";
    }
}
void searchByRegister_time(Candidate* arr_of_candidates, int number_of_candidates) {
    string dateTime;
    cout << "\033[96mВведите дату и время регистрации через пробел:\033[0m ";
    inputString(dateTime);
    bool found = false;
    for (int i = 0; i < number_of_candidates; ++i) {
        if (arr_of_candidates[i].register_time == dateTime) {
            if (!found) {
                topTableCandidates();;
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(20) << arr_of_candidates[i].username << "|"
                << setw(20) << arr_of_candidates[i].password << "|"
                << setw(20) << arr_of_candidates[i].email << "|"
                << setw(10) << arr_of_candidates[i].id << "|"
                << "\033[96m" << setw(20) << arr_of_candidates[i].register_time << "\033[0m" << "|" << endl;

            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mКандидаты не найдены.\033[0m\n";
    }
}
void searchCandidateMenu() {
    Candidate* arr_of_candidates = nullptr;
    int number_of_candidates = 0;
    readFileCandidates(arr_of_candidates, number_of_candidates);

    int choice;
    do {
        cout << "\n\033[30;106mПо какому параметру будет проводиться поиск?\033[0m \n"
            << "\033[90m1 - \033[96mПо логину\033[0m\n"
            << "\033[90m2 - \033[96mПо email\033[0m\n"
            << "\033[90m3 - \033[96mПо дате и времени регистрации\033[0m\n"
            << "\033[90m4 - \033[96mНазад\033[0m\n"
            << "\033[90m\033[30;106mВыбор:\033[0m ";
        failInt(choice);

        switch (choice) {
        case 1: searchByUsername(arr_of_candidates, number_of_candidates); break;
        case 2: searchByEmail(arr_of_candidates, number_of_candidates); break;
        case 3: searchByRegister_time(arr_of_candidates, number_of_candidates); break;
        case 4: break;
        default: cout << "\033[31mНекорректный ввод. Повторите.\033[0m\n"; break;
        }
    } while (choice != 4);

    delete[] arr_of_candidates;
}

void sortByUsername(Candidate* arr_of_candidates, int number_of_candidates) {
    for (int gap = number_of_candidates / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < number_of_candidates; ++i) {
            Candidate temp = arr_of_candidates[i];
            int j;
            for (j = i; j >= gap && arr_of_candidates[j - gap].username > temp.username; j -= gap) {
                arr_of_candidates[j] = arr_of_candidates[j - gap];
            }
            arr_of_candidates[j] = temp;
        }
    }

    cout << "\n\033[96mОтсортировано по логину:\033[0m\n";
    printCandidateTable(arr_of_candidates, number_of_candidates);
}
void sortByEmail(Candidate* arr_of_candidates, int number_of_candidates) {
    for (int gap = number_of_candidates / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < number_of_candidates; ++i) {
            Candidate temp = arr_of_candidates[i];
            int j;
            for (j = i; j >= gap && arr_of_candidates[j - gap].email > temp.email; j -= gap) {
                arr_of_candidates[j] = arr_of_candidates[j - gap];
            }
            arr_of_candidates[j] = temp;
        }
    }

    cout << "\n\033[96mОтсортировано по email:\033[0m\n";
    printCandidateTable(arr_of_candidates, number_of_candidates);
}
void sortByRegister_time(Candidate* arr_of_candidates, int number_of_candidates) {
    for (int gap = number_of_candidates / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < number_of_candidates; ++i) {
            Candidate temp = arr_of_candidates[i];
            int j;
            for (j = i; j >= gap && arr_of_candidates[j - gap].register_time > temp.register_time; j -= gap) {
                arr_of_candidates[j] = arr_of_candidates[j - gap];
            }
            arr_of_candidates[j] = temp;
        }
    }
    cout << "\n\033[96mОтсортировано по дате и времени регистрации:\033[0m\n";
    printCandidateTable(arr_of_candidates, number_of_candidates);
}
void sortCandidateMenu() {
    Candidate* arr_of_candidates = nullptr;
    int number_of_candidates = 0;
    readFileCandidates(arr_of_candidates, number_of_candidates);

    int choice;
    do {
        cout << "\n\033[30;106mПо какому параметру будет проводиться сортировка?\033[0m\n"
            << "\033[90m1 - \033[96mПо логину\033[0m\n"
            << "\033[90m2 - \033[96mПо email\033[0m\n"
            << "\033[90m3 - \033[96mПо дате и времени регистрации\033[0m\n"
            << "\033[90m4 - \033[96mНазад\033[0m\n"
            << "\033[30;106mВыбор:\033[0m ";
        failInt(choice);

        switch (choice) {
        case 1: sortByUsername(arr_of_candidates, number_of_candidates); break;
        case 2: sortByEmail(arr_of_candidates, number_of_candidates); break;
        case 3: sortByRegister_time(arr_of_candidates, number_of_candidates); break;
        case 4: break;
        default: cout << "\033[31mНекорректный ввод. Повторите.\033[0m\n"; break;
        }
    } while (choice != 4);

    delete[] arr_of_candidates;
}

void removeResponsesForCandidate(int deleted_user_id) {
    Response* responses = nullptr;
    int response_count = 0;
    readFileResponses(responses, response_count);

    if (response_count == 0 || !responses) return;

    const int MAX_RESPONSES = response_count;
    int* affected_vacancy_ids = new int[MAX_RESPONSES];
    int* vacancy_response_counts = new int[MAX_RESPONSES];
    int affected_count = 0;

    for (int i = 0; i < response_count; ++i) {
        if (responses[i].user_id == deleted_user_id) {
            int vid = responses[i].vacancy_id;
            bool found = false;
            for (int j = 0; j < affected_count; ++j) {
                if (affected_vacancy_ids[j] == vid) {
                    vacancy_response_counts[j]++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                affected_vacancy_ids[affected_count] = vid;
                vacancy_response_counts[affected_count] = 1;
                affected_count++;
            }
        }
    }

    Vacancy* vacancies = nullptr;
    int vacancy_count = 0;
    readFileVacancies(vacancies, vacancy_count);

    for (int i = 0; i < affected_count; ++i) {
        for (int j = 0; j < vacancy_count; ++j) {
            if (vacancies[j].id == affected_vacancy_ids[i]) {
                vacancies[j].number_of_responses -= vacancy_response_counts[i];
                if (vacancies[j].number_of_responses < 0)
                    vacancies[j].number_of_responses = 0;
                break;
            }
        }
    }

    writeFileVacancies(vacancies, vacancy_count);

    Response* filtered = new Response[response_count];
    int new_count = 0;
    for (int i = 0; i < response_count; ++i) {
        if (responses[i].user_id != deleted_user_id) {
            filtered[new_count++] = responses[i];
        }
    }

    writeFileResponses(filtered, new_count);

    delete[] responses;
    delete[] filtered;
    delete[] affected_vacancy_ids;
    delete[] vacancy_response_counts;
    delete[] vacancies;
}
