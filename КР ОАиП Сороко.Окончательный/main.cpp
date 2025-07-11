#include"Header.h"
Admin admin;
int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "russian");
    ifstream fin(Admin_FILE_OF_DATA, ios::ate);
    if (!fin.is_open()) {
        cout << "\033[31mОшибка открытия файла для записи!Создан новый файл для записи данных администратора.\033[0m" << endl;
        ofstream fout(Admin_FILE_OF_DATA);
        if (!fout) {
            cout << "\033[31mОшибка создания файла!\033[0m" << endl;

        }
        fout.close();
        writeAdmintoFile();
    }
    else {
        if (fin.tellg() == 0) {
            writeAdmintoFile();
        }
        fin.close();
    }
    FirstMenu();
}

