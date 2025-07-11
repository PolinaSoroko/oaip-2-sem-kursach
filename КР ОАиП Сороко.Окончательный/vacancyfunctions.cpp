#include"Header.h"

//--------------------------------------------- Функции для работы с ВАКАНСИЯМИ ---------------------------------------------

void addVacancyToFile(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    Vacancy new_vacancy;
    new_vacancy.id = generateRandomUniqueIDForVacancy(arr_of_vacancies, number_of_vacancies);
    cout <<"\033[90mID-вакансии (сгенерировано автоматически):\033[0m " << new_vacancy.id<<endl;
    cout << "\033[36mВведите название должности:\033[0m ";
    inputString(new_vacancy.position);
    cout << "\033[36mВведите зарплату (от...(в $)):\033[0m ";
    failInt(new_vacancy.salary);
    cout << "\033[36mВведите место/удаленно:\033[0m ";
    inputString(new_vacancy.location);
    cout << "\033[36mВведите требования:\033[0m ";
    inputString(new_vacancy.requirements);
    cout << "\033[36mВведите условия:\033[0m ";
    inputString(new_vacancy.conditions);
    cout << "\033[36mЗанятость(полная/частичная):\033[0m ";
    inputString(new_vacancy.day);
    cout << "\033[36mКоличество вакансий:\033[0m ";
    failInt(new_vacancy.count_of_open_vacancies);


    ofstream fout(Vacancies_FILE_OF_DATA, ios::app);
    if (fout.is_open()) {
        fout << new_vacancy.id << ' '
            << "\"" << new_vacancy.position << "\"" << ' '
            << new_vacancy.salary << ' '
            << "\"" << new_vacancy.location << "\"" << ' '
            << "\"" << new_vacancy.requirements << "\"" << ' '
            << "\"" << new_vacancy.conditions << "\"" << ' '
            << "\"" << new_vacancy.day << "\"" << ' '
         << new_vacancy.count_of_open_vacancies << ' ' 
        << new_vacancy.number_of_responses << ' ' << '\n';
        fout.close();
    }
    else {
        cout << "\033[31m Ошибка при открытии файла для записи данных о вакансии.\033[0m\n";
    }
}
void delVacancyFromArray(Vacancy*& arr_of_vacancies, int& number_of_vacancies) {
    readFileVacancies(arr_of_vacancies, number_of_vacancies);

    if (number_of_vacancies == 0) {
        cout << "\033[35mНет вакансий для удаления.\033[0m\n";
        return;
    }

    int id, index;
    bool found = false;

    do {
        cout << "\033[91mВведите ID вакансии для удаления:\033[0m ";
        failInt(id);
        found = false;

        for (int i = 0; i < number_of_vacancies; ++i) {
            if (id == arr_of_vacancies[i].id) {
                index = i;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\033[35mВакансии с таким ID не существует. Повторите ввод.\033[0m\n";
        }
    } while (!found);

    Vacancy* new_arr = new Vacancy[number_of_vacancies - 1];
    for (int i = 0, j = 0; i < number_of_vacancies; ++i) {
        if (i != index) {
            new_arr[j++] = arr_of_vacancies[i];
        }
    }

    delete[] arr_of_vacancies;
    arr_of_vacancies = new_arr;
    number_of_vacancies--;

    cout << "\033[3;92mВакансия успешно удалена.\033[0m\n";
    writeFileVacancies(arr_of_vacancies, number_of_vacancies);

    removeResponsesForVacancy(id);
}


void removeResponsesForVacancy(int deleted_vacancy_id) {
    Response* responses = nullptr;
    int count = 0;
    readFileResponses(responses, count);

    if (count == 0 || !responses) return;

    Response* filtered = new Response[count];
    int new_count = 0;

    for (int i = 0; i < count; ++i) {
        if (responses[i].vacancy_id != deleted_vacancy_id) {
            filtered[new_count++] = responses[i];
        }
    }

    UserCount* user_counts = new UserCount[new_count];
    int user_count_size = 0;

    for (int i = 0; i < new_count; ++i) {
        int uid = filtered[i].user_id;
        bool found = false;

        for (int j = 0; j < user_count_size; ++j) {
            if (user_counts[j].user_id == uid) {
                user_counts[j].count++;
                found = true;
                break;
            }
        }

        if (!found) {
            user_counts[user_count_size].user_id = uid;
            user_counts[user_count_size].count = 1;
            user_count_size++;
        }
    }

    Response* final_responses = new Response[new_count];
    int final_count = 0;

    for (int i = 0; i < new_count; ++i) {
        int uid = filtered[i].user_id;
        for (int j = 0; j < user_count_size; ++j) {
            if (user_counts[j].user_id == uid && user_counts[j].count > 0) {
                final_responses[final_count++] = filtered[i];
                break;
            }
        }
    }

    writeFileResponses(final_responses, final_count);

    delete[] responses;
    delete[] filtered;
    delete[] final_responses;
    delete[] user_counts;
}


void readFileVacancies(Vacancy*& arr_of_vacancies, int& number_of_vacancies) {
    static bool file_notified = false;
    ifstream fin(Vacancies_FILE_OF_DATA);
    if (!fin.is_open()) {
        if (!file_notified) {
            cout << "\033[31mФайл не найден. Будет создан новый для записи данных о вакансиях.\033[0m\n";
            file_notified = true;
        }
        arr_of_vacancies = nullptr;
        number_of_vacancies = 0;
        return;
    }

    const int initial_capacity = 10;
    int capacity = initial_capacity;
    arr_of_vacancies = new Vacancy[capacity];
    number_of_vacancies = 0;

    string line;
    while (getline(fin, line)) {
        istringstream iss(line);
        Vacancy temp;

        iss >> temp.id;
        temp.position = readQuotedField(iss);
        iss >> temp.salary;
        temp.location = readQuotedField(iss);
        temp.requirements = readQuotedField(iss);
        temp.conditions = readQuotedField(iss);
        temp.day = readQuotedField(iss);
        iss >> temp.count_of_open_vacancies;
        iss >> temp.number_of_responses;

        if (!iss.fail()) {
            if (number_of_vacancies >= capacity) {
                capacity *= 2;
                Vacancy* new_arr = new Vacancy[capacity];
                for (int i = 0; i < number_of_vacancies; ++i) {
                    new_arr[i] = arr_of_vacancies[i];
                }
                delete[] arr_of_vacancies;
                arr_of_vacancies = new_arr;
            }
            arr_of_vacancies[number_of_vacancies++] = temp;
        }
    }

    fin.close();

}
void writeFileVacancies(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    ofstream fout(Vacancies_FILE_OF_DATA, ios::out);
    if (!fout.is_open()) {
        cout << "\033[31mОшибка открытия файла для записи!\033[0m" << endl;
        return;
    }

    if (number_of_vacancies == 0) {
        fout << "Список вакансий пуст.\n";
        fout.close();
        return;
    }

    for (int i = 0; i < number_of_vacancies; ++i) {
        fout << arr_of_vacancies[i].id << " "
            << "\"" << arr_of_vacancies[i].position << "\"" << " "
            << arr_of_vacancies[i].salary << " "
            << "\"" << arr_of_vacancies[i].location << "\"" << " "
            << "\"" << arr_of_vacancies[i].requirements << "\"" << " "
            << "\"" << arr_of_vacancies[i].conditions << "\"" << " "
            << "\"" << arr_of_vacancies[i].day << "\"" <<" " 
        << arr_of_vacancies[i].count_of_open_vacancies << " "
        << arr_of_vacancies[i].number_of_responses << " " << endl;
    }
    fout.close();
}
void showVacancyArray(Vacancy* arr_of_vacancies, int number_of_vacancies,int role, int current_user_id) {
    readFileVacancies(arr_of_vacancies, number_of_vacancies);
    if (number_of_vacancies == 0) {
        cout << "\033[35mСписок вакансий пуст.\033[0m\n";
        return;
    }
    else {
        printVacancyTable(arr_of_vacancies, number_of_vacancies);
    }
    if (role == 1) {
        respondToVacancy(arr_of_vacancies, number_of_vacancies, current_user_id);
    }
}

void editVacancy(Vacancy*& arr_of_vacancies, int& number_of_vacancies) {
    readFileVacancies(arr_of_vacancies, number_of_vacancies);
    if (number_of_vacancies == 0) {
        cout << "\033[35m Нет вакансий для редактирования.\033[0m\n";
        return;
    }
    int id, index;
    bool found = false;

    do {
        cout << "\033[95mВведите ID вакансии для редактирования:\033[0m ";
        failInt(id);
        found = false;

        for (int i = 0; i < number_of_vacancies; ++i) {
            if (id == arr_of_vacancies[i].id) {
                index = i;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\033[35mВакансии с таким ID не существует. Повторите ввод.\033[0m\n";
        }
    } while (!found);

    Vacancy& new_vacancy = arr_of_vacancies[index];
    int choice;
    do {
        cout << "\n\033[30;105mЧто вы хотите изменить?\033[0m\n"
            << "\033[90m1 - \033[95mНазвание должности\033[0m\n"
            << "\033[90m2 - \033[95mЗарплату\033[0m\n"
            << "\033[90m3 - \033[95mМесто работы\033[0m\n"
            << "\033[90m4 - \033[95mТребования\033[0m\n"
            << "\033[90m5 - \033[95mУсловия\033[0m\n"
            << "\033[90m6 - \033[95mВремя занятости\033[0m\n"
            <<"\033[90m7 - \033[95mКоличество\033[0m\n"
            << "\033[90m8 - \033[95mНазад\033[0m\n";
        failInt(choice);

        switch (choice) {
        case 1:
            cout << "\033[95mВведите новое название должности:\033[0m ";
            inputString(new_vacancy.position);
            break;
        case 2:
            cout << "\033[95mВведите новую зарплату (от...(в $)):\033[0m ";
            failInt(new_vacancy.salary);
            break;
        case 3:
            cout << "\033[95mВведите новое место/удаленно:\033[0m ";
            inputString(new_vacancy.location);
            break;
        case 4:
            cout << "\033[95mВведите новые требования:\033[0m ";
            inputString(new_vacancy.requirements);
            break;
        case 5:
            cout << "\033[95mВведите новые условия:\033[0m ";
            inputString(new_vacancy.conditions);
            break;
        case 6:
            cout << "\033[95mПолный рабочий день?\033[0m ";
            inputString(new_vacancy.day);
            break;
        case 7:
            cout << "\033[95mВведите количество вакансий данной категории:\033[0m ";
            failInt(new_vacancy.count_of_open_vacancies);
            break;
        case 8:
            return;
        default:
            cout << "\033[31mНекорректный ввод. Попробуйте снова.\033[0m\n";
            break;
        }

        writeFileVacancies(arr_of_vacancies, number_of_vacancies);

    } while (choice != 8);
}

void filterBySalary(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    int salary_query;
    cout << "\033[96mВведите минимальную зарплату для фильтрации:\033[0m ";
    failInt(salary_query);
    bool found = false;
    for (int i = 0; i < number_of_vacancies; ++i) {
        if (arr_of_vacancies[i].salary >= salary_query) {
            if (!found) {
                topTableVacancies();
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(6) << arr_of_vacancies[i].id << "|"
                << setw(20) << arr_of_vacancies[i].position << "|"
                << "\033[96m" << setw(8)  << arr_of_vacancies[i].salary << "\033[0m" << "|"
                << setw(20) << arr_of_vacancies[i].location << "|"
                << setw(18) << arr_of_vacancies[i].requirements << "|"
                << setw(20) << arr_of_vacancies[i].conditions << "|"
                << setw(18) << arr_of_vacancies[i].day << "|"
            << setw(10) << arr_of_vacancies[i].count_of_open_vacancies << "|" 
            << setw(10) << arr_of_vacancies[i].number_of_responses << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mВакансии с такой зарплатой не найдены.\033[0m\n";
    }
}
void filterByPosition(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    string position_query;
    cout << "\033[96mВведите должность для фильтрации:\033[0m ";
    inputString(position_query);
    bool found = false;
    for (int i = 0; i < number_of_vacancies; ++i) {
        if (arr_of_vacancies[i].position.find(position_query) != string::npos) {
            if (!found) {
                topTableVacancies();
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(6) << arr_of_vacancies[i].id << "|"
                << "\033[96m" << setw(20) << arr_of_vacancies[i].position << "\033[0m" << "|"
                << setw(8) << arr_of_vacancies[i].salary << "|"
                << setw(20) << arr_of_vacancies[i].location << "|"
                << setw(18) << arr_of_vacancies[i].requirements << "|"
                << setw(20) << arr_of_vacancies[i].conditions << "|"
                << setw(18) << arr_of_vacancies[i].day << "|"
                << setw(10) << arr_of_vacancies[i].count_of_open_vacancies << "|"
                << setw(10) << arr_of_vacancies[i].number_of_responses << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mВакансии данной должности не найдены.\033[0m\n";
    }
}
void filterByEmployment(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    string day_query;
    cout << "\033[96mВведите тип занятости (например, 'полная', 'частичная'):\033[0m ";
    inputString(day_query);
    bool found = false;
    for (int i = 0; i < number_of_vacancies; ++i) {
        if (arr_of_vacancies[i].day.find(day_query) != string::npos) {
            if (!found) {
                topTableVacancies();
            }
           
                cout << left << setw(3) << i + 1 << "|"
                << setw(6) << arr_of_vacancies[i].id << "|"
                << setw(20) << arr_of_vacancies[i].position << "|"
                << setw(8) << arr_of_vacancies[i].salary << "|"
                << setw(20) << arr_of_vacancies[i].location << "|"
                << setw(18) << arr_of_vacancies[i].requirements << "|"
                << setw(20) << arr_of_vacancies[i].conditions << "|"
                    << "\033[96m" << setw(18) << arr_of_vacancies[i].day << "\033[0m" << "|"
                << setw(10) << arr_of_vacancies[i].count_of_open_vacancies << "|"
                << setw(10) << arr_of_vacancies[i].number_of_responses<< endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mВакансии с таким типом занятости не найдены.\033[0m\n";
    }
}
void filterVacancyMenu(int role, int current_user_id) {
    Vacancy* arr_of_vacancies = nullptr;
    int number_of_vacancies = 0;
    readFileVacancies(arr_of_vacancies, number_of_vacancies);

    int choice;
    do {
        cout << "\n\033[30;106mПо какому параметру будет проводиться фильтрация?\033[0m\n"
            << "\033[90m1 - \033[96mПо зарплате\033[0m\n"
            << "\033[90m2 - \033[96mПо должности\033[0m\n"
            << "\033[90m3 - \033[96mПо занятости\033[0m\n"
            << "\033[90m4 - \033[96mНазад\033[0m\n"
            << "\033[30;106mВыбор:\033[0m ";
        failInt(choice);

        switch (choice) {
        case 1: filterBySalary(arr_of_vacancies, number_of_vacancies);
            if (role == 1) {
                respondToVacancy(arr_of_vacancies, number_of_vacancies, current_user_id);
            }break;
        case 2: filterByPosition(arr_of_vacancies, number_of_vacancies);
            if (role == 1) {
                respondToVacancy(arr_of_vacancies, number_of_vacancies, current_user_id);
            }break;
        case 3: filterByEmployment(arr_of_vacancies, number_of_vacancies);
            if (role == 1) {
                respondToVacancy(arr_of_vacancies, number_of_vacancies, current_user_id);
            }break;
        case 4: break;
        default: cout << "\033[31mНекорректный ввод. Повторите.\033[0m\n"; break;
        }
    } while (choice != 4);

    delete[] arr_of_vacancies;
}

void searchBySalary(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    int salary_query;
    cout << "\033[96mВведите зарплату для поиска(в $): \033[0m";
    failInt(salary_query);
    bool found = false;
    for (int i = 0; i < number_of_vacancies; ++i) {
        if (arr_of_vacancies[i].salary == salary_query) {
            if (!found) {
                topTableVacancies();
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(6) << arr_of_vacancies[i].id << "|"
                << setw(20) << arr_of_vacancies[i].position << "|"
                << "\033[96m" << setw(8) << arr_of_vacancies[i].salary << "\033[0m" << "|"
                << setw(20) << arr_of_vacancies[i].location << "|"
                << setw(18) << arr_of_vacancies[i].requirements << "|"
                << setw(20) << arr_of_vacancies[i].conditions << "|"
                << setw(18) << arr_of_vacancies[i].day << "|"
                << setw(10) << arr_of_vacancies[i].count_of_open_vacancies << "|"
                << setw(10) << arr_of_vacancies[i].number_of_responses<< endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mВакансии с такой зарплатой не найдены.\033[0m\n";
    }
}
void searchByPosition(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    string position;
    cout << "\033[96mВведите должность для поиска:\033[0m";
    inputString(position);
    bool found = false;
    for (int i = 0; i < number_of_vacancies; ++i) {
        if (arr_of_vacancies[i].position == position) {
            if (!found) {
                topTableVacancies();
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(6) << arr_of_vacancies[i].id << "|"
                << "\033[96m" << setw(20) << arr_of_vacancies[i].position << "\033[0m" << "|"
                << setw(8) << arr_of_vacancies[i].salary << "|"
                << setw(20) << arr_of_vacancies[i].location << "|"
                << setw(18) << arr_of_vacancies[i].requirements << "|"
                << setw(20) << arr_of_vacancies[i].conditions << "|"
                << setw(18) << arr_of_vacancies[i].day << "|"
                << setw(10) << arr_of_vacancies[i].count_of_open_vacancies << "|"
                << setw(10) << arr_of_vacancies[i].number_of_responses << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mВакансии по данной должности не найдены.\033[0m\n";
    }
}
void searchByEmployment(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    string day;
    cout << "\033[96mВведите тип занятости (например, 'полная', 'частичная'):\033[0m ";
    inputString(day);
    bool found = false;
    for (int i = 0; i < number_of_vacancies; ++i) {
        if (arr_of_vacancies[i].day == day) {
            if (!found) {
                topTableVacancies();
            }
            cout << left << setw(3) << i + 1 << "|"
                << setw(6) << arr_of_vacancies[i].id << "|"
                << setw(20) << arr_of_vacancies[i].position << "|"
                << setw(8) << arr_of_vacancies[i].salary << "|"
                << setw(20) << arr_of_vacancies[i].location << "|"
                << setw(18) << arr_of_vacancies[i].requirements << "|"
                << setw(20) << arr_of_vacancies[i].conditions << "|"
                << "\033[96m" << setw(18) << arr_of_vacancies[i].day << "\033[0m" << "|"
                << setw(10) << arr_of_vacancies[i].count_of_open_vacancies << "|"
                << setw(10) << arr_of_vacancies[i].number_of_responses << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "\033[35mВакансии с указанной занятостью не найдены.\033[0m\n";
    }
}
void searchVacancyMenu(int role,int current_user_id) {
    Vacancy* arr_of_vacancies = nullptr;
    int number_of_vacancies = 0;
    readFileVacancies(arr_of_vacancies, number_of_vacancies);

    int choice;
    do {
        cout << "\n\033[30;106mПо какому параметру будет проводиться поиск?\033[0m\n"
            << "\033[90m1 -\033[96m По зарплате\033[0m\n"
            << "\033[90m2 -\033[96m По должности\033[0m\n"
            << "\033[90m3 -\033[96m По занятости\033[0m\n"
            << "\033[90m4 -\033[96m Назад\033[0m\n"
            << "\033[30;106mВыбор:\033[0m ";
        failInt(choice);

        switch (choice) {
        case 1: searchBySalary(arr_of_vacancies, number_of_vacancies);
            if (role == 1) {
                respondToVacancy(arr_of_vacancies, number_of_vacancies, current_user_id);
            }
            break;
        case 2: searchByPosition(arr_of_vacancies, number_of_vacancies); 
            if (role == 1) {
                respondToVacancy(arr_of_vacancies, number_of_vacancies, current_user_id);
            }
            break;
        case 3: searchByEmployment(arr_of_vacancies, number_of_vacancies); 
            if (role == 1) {
            respondToVacancy(arr_of_vacancies, number_of_vacancies, current_user_id);
        } 
            break;
        case 4: break;
        default: cout << "\033[31mНекорректный ввод. Повторите.\033[0m\n"; break;
        }
    } while (choice != 4);

    delete[] arr_of_vacancies;
}

void sortBySalary(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    for (int gap = number_of_vacancies / 2; gap > 0; gap /= 2) {//(метод Шелла)
        for (int i = gap; i < number_of_vacancies; ++i) {
            Vacancy temp = arr_of_vacancies[i];
            int j;
            for (j = i; j >= gap && arr_of_vacancies[j - gap].salary > temp.salary; j -= gap) {
                arr_of_vacancies[j] = arr_of_vacancies[j - gap];
            }
            arr_of_vacancies[j] = temp;
        }
    }

    cout << "\n\033[96mОтсортировано по зарплате:\033[0m\n";

    printVacancyTable(arr_of_vacancies, number_of_vacancies);

}
void sortByPosition(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    for (int gap = number_of_vacancies / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < number_of_vacancies; ++i) {
            Vacancy temp = arr_of_vacancies[i];
            int j;
            for (j = i; j >= gap && arr_of_vacancies[j - gap].position > temp.position; j -= gap) {
                arr_of_vacancies[j] = arr_of_vacancies[j - gap];
            }
            arr_of_vacancies[j] = temp;
        }
    }

    cout << "\n\033[96mОтсортировано по должности:\033[0m\n";
    printVacancyTable(arr_of_vacancies, number_of_vacancies);

}
void sortByEmployment(Vacancy* arr_of_vacancies, int number_of_vacancies) {
    for (int gap = number_of_vacancies / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < number_of_vacancies; ++i) {
            Vacancy temp = arr_of_vacancies[i];
            int j;
            for (j = i; j >= gap && arr_of_vacancies[j - gap].day > temp.day; j -= gap) {
                arr_of_vacancies[j] = arr_of_vacancies[j - gap];
            }
            arr_of_vacancies[j] = temp;
        }
    }

    cout << "\n\033[96mОтсортировано по типу занятости:\033[0m\n";

    printVacancyTable(arr_of_vacancies, number_of_vacancies);

}
void sortVacancyMenu() {
    Vacancy* arr_of_vacancies = nullptr;
    int number_of_vacancies = 0;
    readFileVacancies(arr_of_vacancies, number_of_vacancies);

    int choice;
    do {
        cout << "\n\033[30;106mПо какому параметру будет проводиться сортировка?\033[0m\n"
            << "\033[90m1 - \033[96mПо зарплате\033[0m\n"
            << "\033[90m2 - \033[96mПо должности\033[0m\n"
            << "\033[90m3 - \033[96mПо занятости\033[0m\n"
            << "\033[90m4 - \033[96mНазад\033[0m\n"
            << "\033[30;106mВыбор:\033[0m ";
        failInt(choice);

        switch (choice) {
        case 1: sortBySalary(arr_of_vacancies, number_of_vacancies); break;
        case 2: sortByPosition(arr_of_vacancies, number_of_vacancies); break;
        case 3: sortByEmployment(arr_of_vacancies, number_of_vacancies); break;
        case 4: break;
        default: cout << "\033[31mНекорректный ввод. Повторите.\033[0m\n"; break;
        }
    } while (choice != 4);

    delete[] arr_of_vacancies;
}

void readFileResponses(Response*& responses, int& count) {
    ifstream fin("responses.txt");
    count = 0;
    responses = nullptr;

    if (!fin.is_open()) return;

    fin >> count;
    if (count == 0) return;

    responses = new Response[count];
    for (int i = 0; i < count; ++i) {
        fin >> responses[i].user_id >> responses[i].vacancy_id;
    }

    fin.close();
}

void writeFileResponses(Response* responses, int count) {
    ofstream fout("responses.txt");
    fout << count << "\n";
    for (int i = 0; i < count; ++i) {
        fout << responses[i].user_id << " " << responses[i].vacancy_id << "\n";
    }
    fout.close();
}


void respondToVacancy(Vacancy* arr_of_vacancies, int number_of_vacancies, int user_id) {
    if (number_of_vacancies == 0) {
        cout << "\033[35mНет доступных вакансий.\033[0m\n";
        return;
    }

    int ch;
    cout << "\033[30;103mХотите откликнуться на вакансию?\033[0m \033[90m(1 - Да, 2 - Нет) \n\033[93mВаш выбор:\033[0m ";
    failInt(ch);
    if (ch != 1) return;

    int id, index;
    bool found = false;

    do {
        cout << "\033[93mВведите ID вакансии, на которую хотите откликнуться:\033[0m ";
        failInt(id);
        found = false;

        for (int i = 0; i < number_of_vacancies; ++i) {
            if (id == arr_of_vacancies[i].id) {
                index = i;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\033[35mВакансии с таким ID не существует. Повторите ввод.\033[0m\n";
        }
    } while (!found);
    Response* responses = nullptr;
    int response_count = 0;
    readFileResponses(responses, response_count);

    for (int i = 0; i < response_count; ++i) {
        if (responses[i].user_id == user_id && responses[i].vacancy_id == id) {
            cout << "\033[93mВы уже откликались на эту вакансию.\033[0m\n";
            delete[] responses;
            return;
        }
    }

    Response* updated = new Response[response_count + 1];
    for (int i = 0; i < response_count; ++i)
        updated[i] = responses[i];
    updated[response_count] = { user_id, id };

    delete[] responses;
    responses = updated;
    response_count++;

    Vacancy& new_vacancy = arr_of_vacancies[index];
    cout << "\n\033[30;103mВы откликнулись на вакансию:\033[0m\n";
    cout << "\033[93mДолжность:\033[0m " << new_vacancy.position << "\n"
        << "\033[93mЗарплата:\033[0m $" << new_vacancy.salary << "\n"
        << "\033[93mМесто:\033[0m " << new_vacancy.location << "\n";

    new_vacancy.number_of_responses++;
    writeFileVacancies(arr_of_vacancies, number_of_vacancies);
    writeFileResponses(responses, response_count);

    cout << "\033[3;92mОтклик отправлен успешно!\033[0m\n";

    delete[] responses;
}


