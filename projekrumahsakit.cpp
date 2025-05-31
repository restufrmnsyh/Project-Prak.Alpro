#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Pasien {
    int id;
    string namaPasien;
    int umur;
    string penyakit;
};

struct dokter {
    int id;
    string nama;
    string spesialis;
};

const int MAX_PASIEN = 100;
const int MAX_DOKTER = 50;

string admin_username = "AdminGacor";
string admin_pass  = "123123123";
string data_pasien_filename = "pasien.txt";
string data_dokter_filename = "dokter.txt";


string username, password;
int menuadmin;

bool loginAdmin();
void menuAdmin(Pasien pasien[], int &jumlahPasien, dokter dokterList[], int &jumlahDokter);

// Fungsi-fungsi manajemen pasien dan dokter (diisi nanti)
void bacaPasienDariFile(Pasien pasien[], int &jumlahPasien);
void bacaDokterDariFile(dokter dokterList[], int &jumlahDokter);



int main() {
    Pasien pasien[MAX_PASIEN];
    dokter dokterList[MAX_DOKTER];
    int jumlahPasien = 0, jumlahDokter = 0;

    char ulang;
    do {
        system("cls");
        cout << "====== SISTEM INFORMASI RUMAH SAKIT ======\n";
        cout << "1. Admin\n";
        cout << "2. Pelanggan (belum tersedia)\n";
        cout << "Pilih: ";
        int menu; cin >> menu;
        cin.ignore();

        if (menu == 1) {
            if (loginAdmin()) {
                // Baca data dari file ke array sebelum masuk menu
                bacaPasienDariFile(pasien, jumlahPasien);
                bacaDokterDariFile(dokterList, jumlahDokter);
                menuAdmin(pasien, jumlahPasien, dokterList, jumlahDokter);
            } else {
                cout << "Gagal login 3 kali. Program berhenti.\n";
                return 0;
            }
        } else {
            cout << "Fitur pelanggan belum tersedia.\n";
        }

        cout << "\nKembali ke menu utama? (y/n): ";
        cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');

    return 0;
}

bool loginAdmin() {

    int percobaan = 3;
    while (percobaan--) {
        cout << "\nLogin Admin\n";
        cout << "Username: "; getline(cin, username);
        cout << "Password: "; getline(cin, password);
        if (username == admin_username && password == admin_pass) {
            cout << "Login berhasil.\n";
            return true;
        } else {
            cout << "Username / Password Salah! Percobaan tersisa: " << percobaan << endl;
        }
    }
    return false;
}

void simpanPasienKeFile(Pasien p) {
    ofstream file(data_pasien_filename, ios::app);
    file << p.id << ";" << p.namaPasien << ";" << p.umur << ";" << p.penyakit << "\n";
    file.close();
}

void simpanDokterKeFile(dokter d) {
    ofstream file(data_dokter_filename, ios::app);
    file << d.id << ";" << d.nama << ";" << d.spesialis << "\n";
    file.close();
}


void bubbleSortPasien(Pasien arr[], int n) {
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j].namaPasien > arr[j + 1].namaPasien)
                swap(arr[j], arr[j + 1]);
}

void tampilkanPasien(Pasien pasien[], int jumlahPasien) {
    if (jumlahPasien == 0) {
        cout << "Belum ada data pasien.\n";
        return;
    }

    bubbleSortPasien(pasien, jumlahPasien);
    cout << "=== DAFTAR PASIEN (Urut Nama) ===\n";
    for (int i = 0; i < jumlahPasien; ++i) {
        cout << "ID: " << pasien[i].id << " | ";
        cout << "Nama: " << pasien[i].namaPasien << " | ";
        cout << "Umur: " << pasien[i].umur << " | ";
        cout << "Penyakit: " << pasien[i].penyakit << endl;
    }
}

void bubbleSortDokter(dokter arr[], int n) {
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j].nama > arr[j + 1].nama)
                swap(arr[j], arr[j + 1]);
}

void tampilkanDokter(dokter dokterList[], int jumlahDokter) {
    if (jumlahDokter == 0) {
        cout << "Belum ada data dokter.\n";
        return;
    }

    bubbleSortDokter(dokterList, jumlahDokter);
    cout << "=== DAFTAR DOKTER (Urut Nama) ===\n";
    for (int i = 0; i < jumlahDokter; ++i) {
        cout << "ID: " << dokterList[i].id << " | ";
        cout << "Nama: " << dokterList[i].nama << " | ";
        cout << "Spesialis: " << dokterList[i].spesialis << endl;
    }
}

void hapusPasien(Pasien pasien[], int &jumlahPasien) {
    int id;
    cout << "Masukkan ID pasien yang ingin dihapus: ";
    cin >> id;
    bool found = false;
    for (int i = 0; i < jumlahPasien; ++i) {
        if (pasien[i].id == id) {
            for (int j = i; j < jumlahPasien - 1; ++j)
                pasien[j] = pasien[j + 1];
            jumlahPasien--;
            found = true;
            break;
        }
    }
    if (found) {
        ofstream file(data_pasien_filename);
        for (int i = 0; i < jumlahPasien; ++i)
            file << pasien[i].id << ";" << pasien[i].namaPasien << ";" << pasien[i].umur << ";" << pasien[i].penyakit << "\n";
        file.close();
        cout << "Data pasien berhasil dihapus.\n";
    } else {
        cout << "Data tidak ditemukan.\n";
    }
    system("pause");
}

void hapusDokter(dokter dokterList[], int &jumlahDokter) {
    int id;
    cout << "Masukkan ID dokter yang ingin dihapus: ";
    cin >> id;
    bool found = false;
    for (int i = 0; i < jumlahDokter; ++i) {
        if (dokterList[i].id == id) {
            for (int j = i; j < jumlahDokter - 1; ++j)
                dokterList[j] = dokterList[j + 1];
            jumlahDokter--;
            found = true;
            break;
        }
    }
    if (found) {
        ofstream file(data_dokter_filename);
        for (int i = 0; i < jumlahDokter; ++i)
            file << dokterList[i].id << ";" << dokterList[i].nama << ";" << dokterList[i].spesialis << "\n";
        file.close();
        cout << "Data dokter berhasil dihapus.\n";
    } else {
        cout << "Data tidak ditemukan.\n";
    }
    system("pause");
}

bool idPasienSudahAda(Pasien pasien[], int jumlahPasien, int id) {
    for (int i = 0; i < jumlahPasien; ++i) {
        if (pasien[i].id == id)
            return true;
    }
    return false;
}

bool idDokterSudahAda(dokter dokterList[], int jumlahDokter, int id) {
    for (int i = 0; i < jumlahDokter; ++i) {
        if (dokterList[i].id == id)
            return true;
    }
    return false;
}

void inputPasien(Pasien pasien[], int &jumlahPasien) {
    int jumlahInput;
    cout << "Masukkan jumlah pasien yang ingin ditambahkan: ";
    cin >> jumlahInput;
    cin.ignore();

    ofstream file("pasien.txt", ios::app); // langsung buka file di sini

    for (int i = 0; i < jumlahInput; ++i) {
        Pasien p;
        cout << "\n--- Pasien ke-" << i + 1 << " ---\n";

        cout << "ID: ";
        cin >> p.id;
        cin.ignore();

        if (idPasienSudahAda(pasien, jumlahPasien, p.id)) {
            cout << "Gagal! ID pasien sudah digunakan. Silakan input ulang pasien ini.\n";
            --i; // ulangi input
            continue;
        }

        cout << "Nama: ";
        getline(cin, p.namaPasien);

        cout << "Umur: ";
        cin >> p.umur;
        cin.ignore();

        cout << "Penyakit: ";
        getline(cin, p.penyakit);

        // Simpan ke array
        pasien[jumlahPasien++] = p;

        // Simpan ke file langsung
        file << p.id << ";" << p.namaPasien << ";" << p.umur << ";" << p.penyakit << "\n";

        cout << "Pasien berhasil ditambahkan.\n";
    }

    file.close();
    system("pause");
}



void inputDokter(dokter dokterList[], int &jumlahDokter) {
    int jumlahInput;
    cout << "Masukkan jumlah dokter yang ingin ditambahkan: ";
    cin >> jumlahInput;
    cin.ignore();

    ofstream file("dokter.txt", ios::app); // Buka file hanya sekali untuk semua input

    for (int i = 0; i < jumlahInput; ++i) {
        dokter d;
        cout << "\n--- Dokter ke-" << i + 1 << " ---\n";

        cout << "ID: ";
        cin >> d.id;
        cin.ignore();

        if (idDokterSudahAda(dokterList, jumlahDokter, d.id)) {
            cout << "Gagal! ID dokter sudah digunakan. Silakan input ulang dokter ini.\n";
            --i; // ulangi input
            continue;
        }

        cout << "Nama: ";
        getline(cin, d.nama);

        cout << "Spesialis: ";
        getline(cin, d.spesialis);

        dokterList[jumlahDokter++] = d;

        // Simpan langsung ke file
        file << d.id << ";" << d.nama << ";" << d.spesialis << "\n";

        cout << "Dokter berhasil ditambahkan.\n";
    }

    file.close();
    system("pause");
}

void bacaPasienDariFile(Pasien pasien[], int &jumlahPasien) {
    ifstream file(data_pasien_filename);
    string line;
    jumlahPasien = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, nama, umurStr, penyakit;

        getline(ss, idStr, ';');
        getline(ss, nama, ';');
        getline(ss, umurStr, ';');
        getline(ss, penyakit, ';');

        pasien[jumlahPasien].id = stoi(idStr);
        pasien[jumlahPasien].namaPasien = nama;
        pasien[jumlahPasien].umur = stoi(umurStr);
        pasien[jumlahPasien].penyakit = penyakit;
        jumlahPasien++;
    }

    file.close();
}

void bacaDokterDariFile(dokter dokterList[], int &jumlahDokter) {
    ifstream file(data_dokter_filename);
    string line;
    jumlahDokter = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, nama, spesialis;

        getline(ss, idStr, ';');
        getline(ss, nama, ';');
        getline(ss, spesialis, ';');

        dokterList[jumlahDokter].id = stoi(idStr);
        dokterList[jumlahDokter].nama = nama;
        dokterList[jumlahDokter].spesialis = spesialis;
        jumlahDokter++;
    }

    file.close();
}




void menuAdmin(Pasien pasien[], int &jumlahPasien, dokter dokterList[], int &jumlahDokter) {
    do {
        system("cls");
        cout << "===== MENU ADMIN =====\n";
        cout << "1. Tambah Pasien\n";
        cout << "2. Lihat Pasien\n";
        cout << "3. Hapus Pasien\n";
        cout << "4. Tambah Dokter\n";
        cout << "5. Lihat Dokter\n";
        cout << "6. Hapus Dokter\n";
        cout << "0. Logout\n";
        cout << "Pilih: ";
        cin >> menuadmin; cin.ignore();

        switch (menuadmin) {
            case 1: 
                inputPasien(pasien, jumlahPasien); 
                break;
            case 2: 
                tampilkanPasien(pasien, jumlahPasien);
                system("pause");
                break;
            case 3: 
                hapusPasien(pasien, jumlahPasien);
                break;
            case 4: 
                inputDokter(dokterList, jumlahDokter); 
                break;
            case 5: 
                tampilkanDokter(dokterList, jumlahDokter); 
                system("pause"); 
                break;
            case 6:
                hapusDokter(dokterList, jumlahDokter); 
                break;
            case 0: 
                cout << "Logout berhasil.\n";
                break;
            default: cout << "Pilihan tidak tersedia.\n";
        }
    } while (menuadmin != 0);
}
