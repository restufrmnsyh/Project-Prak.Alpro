#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

struct Pasien {
    string id;
    string namaPasien;
    int umur;
    string penyakit;
    int kelaskamar; // 1 = VIP, 2 = Kelas I, 3 = Kelas II, 4 = Kelas III
};

struct dokter {
    string id;
    string nama;
    string spesialis;
};

struct JadwalDokter {
    string idDokter;
    string hari;
    string jam;
};

string namaKelasKamar[4] = {
    "VIP", //int klskamar = 1
    "Kelas I", //int klskamar = 2
    "Kelas II", //int klskamar = 3
    "Kelas III" //int klskamar = 4
};

int tarifPerKelas[4] = {
    600000, // VIP
    350000, // Kelas I
    225000, // Kelas II
    125000  // Kelas III
};

const int MAX_PASIEN = 100;
const int MAX_DOKTER = 50;
const int MAX_JADWAL = 100;

string admin_username = "AdminGacor";
string admin_pass  = "123123123";
string data_pasien_filename = "pasien.txt";
string data_dokter_filename = "dokter.txt";
string data_jadwal_filename = "jadwal.txt";
string data_pasien_opname_filename = "pasien_opname.txt";

string username, password;
int menuadmin;

bool loginAdmin();
void menuAdmin(Pasien pasien[], int &jumlahPasien, dokter dokterList[], int &jumlahDokter, JadwalDokter jadwalList[], int &jumlahJadwal);
void menupelanggan(Pasien pasien[], int &jumlahPasien, dokter dokterList[], int jumlahDokter, JadwalDokter jadwalList[], int jumlahJadwal);

// Fungsi untuk mengonversi kelas kamar ke string
//di struct, tipe kamar bentuknya cuma int 1-4. nnt dikonversi pake 3 fungsi ini (tergantung butuhnya yg apa)
string kelaskamar(Pasien pasien[], int index);
int depositawal(Pasien pasien[], int index); //DP = tarif per hari x 5
int tarifPerHari(Pasien pasien[], int index);
void jengukpasien(Pasien pasien[], int jumlahPasien);
void daftaropname(Pasien pasien[], int &jumlahPasien); //kayak input pasien dari admin tapi cuma nambah 1 org

string kelaskamar(Pasien pasien[], int index){
    return namaKelasKamar[(pasien[index].kelaskamar - 1)];
}

int tarifPerHari(Pasien pasien[], int index){
    return tarifPerKelas[(pasien[index].kelaskamar - 1)];
}

int depositawal(Pasien pasien[], int index){
    return (tarifPerHari(pasien, index) * 5);
}

// Fungsi-fungsi manajemen pasien dan dokter
void bacaPasienDariFile(Pasien pasien[], int &jumlahPasien);
void bacaDokterDariFile(dokter dokterList[], int &jumlahDokter);
void bacaJadwalDariFile(JadwalDokter jadwalList[], int &jumlahJadwal);
void tampilkanPasien(Pasien pasien[], int jumlahPasien);
void tampilkanDokter(dokter dokterList[], int jumlahDokter);
void tampilkanJadwal(JadwalDokter jadwalList[], int idx,int jumlahJadwal, dokter dokterList[], int jumlahDokter);
void inputPasien(Pasien pasien[], int &jumlahPasien);
void inputDokter(dokter dokterList[], int &jumlahDokter);
void hapusPasien(Pasien pasien[], int &jumlahPasien);
void hapusDokter(dokter dokterList[], int &jumlahDokter, JadwalDokter jadwalList[], int &jumlahJadwal);
void bubbleSortPasien(Pasien arr[], int n);
void bubbleSortDokter(dokter arr[], int n);
void tambahJadwal(JadwalDokter jadwalList[], int &jumlahJadwal, dokter dokterList[], int jumlahDokter);
void hapusJadwalDokter(JadwalDokter jadwalList[], int &jumlahJadwal, dokter dokterList[], int jumlahDokter);

int main() {
    Pasien pasien[MAX_PASIEN];
    dokter dokterList[MAX_DOKTER];
    JadwalDokter jadwalList[MAX_JADWAL];
    int jumlahPasien = 0, jumlahDokter = 0, jumlahJadwal = 0;

    char ulang;
    do {
        system("cls");
        cout << "====== SISTEM INFORMASI RUMAH SAKIT ======\n";
        cout << "1. Admin\n";
        cout << "2. Pelanggan\n";
        cout << "Pilih: ";
        int menu; cin >> menu;
        cin.ignore();

        if (menu == 1) {
            if (loginAdmin()) {
                // Baca data dari file ke array sebelum masuk menu
                bacaPasienDariFile(pasien, jumlahPasien);
                bacaDokterDariFile(dokterList, jumlahDokter);
                bacaJadwalDariFile(jadwalList, jumlahJadwal);
                menuAdmin(pasien, jumlahPasien, dokterList, jumlahDokter, jadwalList, jumlahJadwal);
            } else {
                cout << "Gagal login 3 kali. Program berhenti.\n";
                return 0;
            }
        } else if (menu == 2) {
                bacaPasienDariFile(pasien, jumlahPasien);
                bacaDokterDariFile(dokterList, jumlahDokter);
                bacaJadwalDariFile(jadwalList, jumlahJadwal);
                menupelanggan(pasien, jumlahPasien, dokterList, jumlahDokter, jadwalList, jumlahJadwal);
        } else {
            cout << "Menu tidak tersedia.\n";
        }
        cout << "Kembali ke menu utama? (y/n): ";
        cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');

    return 0;
}

bool loginAdmin() {
    int percobaan = 3;
    while (percobaan--) {
        cout << "===== Login Admin ======\n";
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

void bubbleSortPasien(Pasien arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            string nama1 = arr[j].namaPasien;
            string nama2 = arr[j + 1].namaPasien;
            // Konversi ke lowercase
            for (char& c : nama1) c = tolower(c);
            for (char& c : nama2) c = tolower(c);
            if (nama1 > nama2) {
                Pasien temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void tampilkanPasien(Pasien pasien[], int jumlahPasien) {
    if (jumlahPasien == 0) {
        cout << "Belum ada data pasien.\n";
        return;
    }
    bubbleSortPasien(pasien, jumlahPasien); // Urutkan berdasarkan nama secara ascending
    // Garis atas tabel
    cout << setfill('-') << setw(73) << "-" << setfill(' ') << endl;
    // Header kolom
    cout << "| " << setw(6)  << left << "ID"
         << "| " << setw(22) << left << "Nama"
         << "| " << setw(6)  << left << "Umur"
         << "| " << setw(11) << left << "Tipe Kamar"
         << "| " << setw(17) << left << "Penyakit" << "|\n";
    // Garis bawah header
    cout << setfill('-') << setw(73) << "-" << setfill(' ') << endl;
    // Isi tabel
    for (int i = 0; i < jumlahPasien; ++i) {
        cout << "| " << setw(6)  << left << pasien[i].id
             << "| " << setw(22) << left << pasien[i].namaPasien
             << "| " << setw(6)  << left << pasien[i].umur
             << "| " << setw(11) << left << namaKelasKamar[pasien[i].kelaskamar - 1]
             << "| " << setw(17) << left << pasien[i].penyakit << "|\n";
    }
    // Garis akhir tabel
    cout << setfill('-') << setw(73) << "-" << setfill(' ') << endl;
}

void bubbleSortDokter(dokter arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            string nama1 = arr[j].nama;
            string nama2 = arr[j + 1].nama;
            // Konversi ke lowercase
            for (char& c : nama1) c = tolower(c);
            for (char& c : nama2) c = tolower(c);
            if (nama1 > nama2) {
                dokter temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void tampilkanDokter(dokter dokterList[], int jumlahDokter) {
    if (jumlahDokter == 0) {
        cout << "Belum ada data dokter.\n";
        return;
    }

    bubbleSortDokter(dokterList, jumlahDokter); // Urutkan berdasarkan nama secara ascending

    // Garis atas tabel
    cout << setfill('-') << setw(55) << "-" << setfill(' ') << endl;

    // Header kolom
    cout << "| " << setw(6)  << left << "ID"
         << "| " << setw(22) << left << "Nama"
         << "| " << setw(20) << left << "Spesialis" << "|\n";

    // Garis bawah header
    cout << setfill('-') << setw(55) << "-" << setfill(' ') << endl;

    // Isi tabel
    for (int i = 0; i < jumlahDokter; ++i) {
        cout << "| " << setw(6)  << left << dokterList[i].id
             << "| " << setw(22) << left << dokterList[i].nama
             << "| " << setw(20) << left << dokterList[i].spesialis << "|\n";
    }
    // Garis akhir tabel
    cout << setfill('-') << setw(55) << "-" << setfill(' ') << endl;
}

void hapusPasienOpname(const string &id){
    ifstream file(data_pasien_opname_filename);
    ofstream tempFile("temp_opname.txt");
    string line;
    while(getline(file, line)) {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ';');
        if (idStr != id) {
            tempFile << line << "\n"; // Simpan baris yang tidak dihapus
        }
    }
    file.close();
    tempFile.close();
    // Ganti file asli dengan file sementara
    remove(data_pasien_opname_filename.c_str());
    rename("temp_opname.txt", data_pasien_opname_filename.c_str());
}

void hapusPasien(Pasien pasien[], int &jumlahPasien) {
    if (jumlahPasien == 0) {
        cout << "BELUM ADA DATA PASIEN.\n";
        return;
    }

    tampilkanPasien(pasien, jumlahPasien);
    string id;
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
        // Hapus data pasien opname juga jika ada
        hapusPasienOpname(id);
        ofstream file(data_pasien_filename);
        for (int i = 0; i < jumlahPasien; ++i)
            file << pasien[i].id << ";" << pasien[i].namaPasien << ";" << pasien[i].umur << ";" << pasien[i].penyakit << ";" << pasien[i].kelaskamar <<"\n";
        file.close();
        cout << "[SUCCES]Data pasien berhasil dihapus.\n";
    } else {
        cout << "[FAILED]Data tidak ditemukan.\n";
    }
    char ulang;
    cout << "Ingin Hapus Lagi? (y/n): "; cin >> ulang;
    if(ulang == 'y' || ulang == 'Y'){
        hapusPasien(pasien, jumlahPasien);
    } else {
        cout << "Kembali ke menu utama.\n";
    }
}

void hapusDokter(dokter dokterList[], int &jumlahDokter, JadwalDokter jadwalList[], int &jumlahJadwal) {
    if (jumlahDokter == 0) {
        cout << "BELUM ADA DATA DOKTER.\n";
        return;
    }
    tampilkanDokter(dokterList, jumlahDokter);
    string id;
    cout << "Masukkan ID dokter yang ingin dihapus: ";
    cin >> id;
    bool found = false;
    for (int i = 0; i < jumlahDokter; ++i) {
        if (dokterList[i].id == id) {
            for (int j = i; j < jumlahDokter - 1; ++j)
                dokterList[j] = dokterList[j + 1];
            jumlahDokter--;
            found = true;
            // Hapus jadwal dokter yang terkait
            int k = 0;
            while(k < jumlahJadwal){
                if(jadwalList[k].idDokter == id){
                    for(int a = k; a < jumlahJadwal - 1; ++a)
                        jadwalList[a] = jadwalList[a + 1];
                    jumlahJadwal--;
                } else {
                    k++;
                }
            }
            break;
        }
    }
    if (found) {
        //Tulis ulang file dokter
        ofstream fileDokter(data_dokter_filename);
        for (int i = 0; i < jumlahDokter; ++i)
            fileDokter << dokterList[i].id << ";" << dokterList[i].nama << ";" << dokterList[i].spesialis << "\n";
        fileDokter.close();
        //Tulis Ulang File Jadwal
        ofstream fileJadwal(data_jadwal_filename);
        for(int i = 0; i < jumlahJadwal; ++i)
        fileJadwal << jadwalList[i].idDokter << ";" << jadwalList[i].hari << ";" << jadwalList[i].jam << endl;
        fileJadwal.close();
        cout << "[SUCCES]Data dokter dan jadwal terkait berhasil dihapus.\n";
    } else {
        cout << "[FAILED]Data tidak ditemukan.\n";
    }
    char ulang;
    cout << "Ingin Hapus Lagi? (y/n): "; cin >> ulang;
    if(ulang == 'y' || ulang == 'Y'){
        hapusDokter(dokterList, jumlahDokter, jadwalList, jumlahJadwal);
    } else {
        cout << "Kembali ke menu utama.\n";
    }
}

bool idPasienSudahAda(Pasien pasien[], int jumlahPasien, string id) {
    for (int i = 0; i < jumlahPasien; ++i) {
        if (pasien[i].id == id)
            return true;
    }
    return false;
}

bool idDokterSudahAda(dokter dokterList[], int jumlahDokter, string id) {
    for (int i = 0; i < jumlahDokter; ++i) {
        if (dokterList[i].id == id)
            return true;
    }
    return false;
}

void inputPasien(Pasien pasien[], int &jumlahPasien) {
    cout << "========= MENU INPUT PASIEN =========\n";
    int jumlahInput;
    cout << "Masukkan jumlah pasien yang ingin ditambahkan: ";
    cin >> jumlahInput;
    cin.ignore();

    ofstream file(data_pasien_filename, ios::app); // langsung buka file di sini

    for (int i = 0; i < jumlahInput; ++i) {
        Pasien p;
        cout << "\n--- Pasien ke-" << i + 1 << " ---\n";
        cout << "ID: "; cin >> p.id;
        cin.ignore();

        if (idPasienSudahAda(pasien, jumlahPasien, p.id)) {
            cout << "Gagal! ID pasien sudah digunakan. Silakan input ulang.\n";
            --i; // ulangi input
            continue;
        }

        cout << "Nama: "; getline(cin, p.namaPasien);
        cout << "Umur: "; cin >> p.umur;
        cin.ignore();
        cout << "Penyakit: "; getline(cin, p.penyakit);

        // Tampilkan pilihan kelas kamar
            cout << endl << setfill('=') << setw(70) << "" << endl;
            cout << "| " << setfill(' ') 
            << left << setw(5) << "Nomor" << " | " 
            << left << setw(25) << "Tipe Kamar" << " | " 
            << left << setw(13) << "Deposit Awal" << " | "
            << left << setw(14) << "Tarif per Hari" << " | " << endl;
            cout << setfill('-') << setw(70) << "" << endl;
            for (int j = 0; j < 4; j++) {
                cout << "| " << setfill(' ') 
                << left << setw(5) << j + 1<< " | " 
                << left << setw(25) << namaKelasKamar[j] << " | " 
                << left << setw(13) << (tarifPerKelas[j] * 5) << " | "
                << left << setw(14) << tarifPerKelas[j] << " | " << endl;   
            }
            cout << setfill('=') << setw(70) << "" << endl << endl;

        // Input kelas kamar dengan validasi
        do {
            cout << "Kelas Kamar (1-4): ";
            cin >> p.kelaskamar;
            if (p.kelaskamar < 1 || p.kelaskamar > 4) {
                cout << "Kelas kamar tidak valid. Silakan pilih antara 1-4.\n";
            }
        } while (p.kelaskamar < 1 || p.kelaskamar > 4);

        // Simpan ke array
        pasien[jumlahPasien++] = p;

        // Simpan ke file langsung
        file << p.id << ";" << p.namaPasien << ";" << p.umur << ";" << p.penyakit << ";" << p.kelaskamar << "\n";

        cout << "Silakan melakukan pembayaran deposit awal sebesar Rp" 
             << depositawal(pasien, jumlahPasien - 1) << " ke loket utama.\n";
        cout << "[SUCCES]Pasien berhasil ditambahkan.\n";
    }
    file.close();
}

void inputDokter(dokter dokterList[], int &jumlahDokter) {
    system("cls");
    cout << "========= MENU INPUT DOKTER =========\n";
    int jumlahInput;
    cout << "Masukkan jumlah dokter yang ingin ditambahkan: ";
    cin >> jumlahInput;
    cin.ignore();

    ofstream file(data_dokter_filename, ios::app); // Buka file hanya sekali untuk semua input
    for (int i = 0; i < jumlahInput; ++i) {
        dokter d;
        cout << "\n--- Dokter ke-" << i + 1 << " ---\n";
        cout << "ID: "; cin >> d.id;
        cin.ignore();

        if (idDokterSudahAda(dokterList, jumlahDokter, d.id)) {
            cout << "Gagal! ID dokter sudah digunakan. Silakan input ulang dokter ini.\n";
            --i; // ulangi input
            continue;
        }

        cout << "Nama: "; getline(cin, d.nama);
        cout << "Spesialis: "; getline(cin, d.spesialis);
        dokterList[jumlahDokter++] = d;

        // Simpan langsung ke file
        file << d.id << ";" << d.nama << ";" << d.spesialis << "\n";
        cout << "[SUCCESS]Dokter berhasil ditambahkan.\n";
    }
    file.close();
}

void bacaPasienDariFile(Pasien pasien[], int &jumlahPasien) {
    ifstream file(data_pasien_filename);
    string line;
    jumlahPasien = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, nama, umurStr, penyakit, kelaskamarStr;

        getline(ss, idStr, ';');
        getline(ss, nama, ';');
        getline(ss, umurStr, ';');
        getline(ss, penyakit, ';');
        getline(ss, kelaskamarStr, ';');

        pasien[jumlahPasien].id = idStr;
        pasien[jumlahPasien].namaPasien = nama;
        pasien[jumlahPasien].umur = stoi(umurStr);
        pasien[jumlahPasien].penyakit = penyakit;
        pasien[jumlahPasien].kelaskamar = stoi(kelaskamarStr); 
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

        dokterList[jumlahDokter].id = idStr;
        dokterList[jumlahDokter].nama = nama;
        dokterList[jumlahDokter].spesialis = spesialis;
        jumlahDokter++;
    }
    file.close();
}

void tambahJadwal(JadwalDokter jadwalList[], int &jumlahJadwal, dokter dokterList[], int jumlahDokter) {
    if (jumlahDokter == 0) {
        cout << "BELUM ADA DATA DOKTER. JADWAL BELUM BISA DITAMBAHKAN\n";
        return;
    }
    string id;
    cout << "Masukkan ID dokter yang ingin ditambahkan jadwal: ";
    cin >> id;
    cin.ignore();

    // Validasi ID dokter
    bool ditemukan = false;
    for (int i = 0; i < jumlahDokter; i++) {
        if (dokterList[i].id == id) {
            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) {
        cout << "ID dokter tidak ditemukan.\n";
        return;
    }

    // Cek apakah jadwal sudah ada untuk dokter ini
    bool sudahAdaJadwal = false;
    for (int i = 0; i < jumlahJadwal; i++) {
        if (jadwalList[i].idDokter == id) {
            sudahAdaJadwal = true;
            break;
        }
    }
    if (sudahAdaJadwal) {
        cout << "Dokter ini sudah memiliki jadwal. Silakan hapus jadwal lama sebelum menambahkan yang baru.\n";
        tambahJadwal(jadwalList, jumlahJadwal, dokterList, jumlahDokter);
        return;
    }

    JadwalDokter j;
    j.idDokter = id;
    cout << "Masukkan hari praktek (cth: Senin): ";
    getline(cin, j.hari);
    cout << "Masukkan jam praktek (cth: 08.00-12.00): ";
    getline(cin, j.jam);

    jadwalList[jumlahJadwal++] = j;

    ofstream file(data_jadwal_filename, ios::app);
    file << j.idDokter << ";" << j.hari << ";" << j.jam << endl;
    file.close();
    cout << "[SUCCESS]Jadwal berhasil ditambahkan.\n";
    char ulang;
    cout << "Ingin Tambah Jadwal Lagi? (y/n): "; cin >> ulang;
    if(ulang == 'y' || ulang == 'Y'){
        tambahJadwal(jadwalList, jumlahJadwal, dokterList, jumlahDokter);
    } else {
        cout << "Kembali ke menu utama.\n";
    }
}

void bacaJadwalDariFile(JadwalDokter jadwalList[], int &jumlahJadwal) {
    ifstream file(data_jadwal_filename);
    string line;
    jumlahJadwal = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, hari, jam;
        getline(ss, idStr, ';');
        getline(ss, hari, ';');
        getline(ss, jam);

        jadwalList[jumlahJadwal].idDokter = idStr;
        jadwalList[jumlahJadwal].hari = hari;
        jadwalList[jumlahJadwal].jam = jam;
        jumlahJadwal++;
    }
    file.close();
}

void tampilkanJadwal(JadwalDokter jadwalList[], int idx, int jumlahJadwal, dokter dokterList[], int jumlahDokter) {
    if (jumlahJadwal == 0) {
        cout << "Belum ada data jadwal dokter.\n";
        return;
    }
    
    if (idx >= jumlahJadwal) return; // Basis rekursif

    string namaDokter = "Tidak ditemukan";
    for (int j = 0; j < jumlahDokter; j++) {
        if (jadwalList[idx].idDokter == dokterList[j].id) {
            namaDokter = dokterList[j].nama;
            break;
        }
    }
    cout << "| ID Dokter   : " << setw(20) << left << jadwalList[idx].idDokter << "|\n";
    cout << "| Nama Dokter : " << setw(20) << left << namaDokter << "|\n";
    cout << "| Hari        : " << setw(20) << left << jadwalList[idx].hari << "|\n";
    cout << "| Jam         : " << setw(20) << left << jadwalList[idx].jam << "|\n";
    cout << setfill('-') << setw(37) << "-" << setfill(' ') << endl;
    // Panggil rekursif untuk menampilkan jadwal berikutnya
    tampilkanJadwal(jadwalList, idx + 1, jumlahJadwal, dokterList, jumlahDokter);
}

void hapusJadwalDokter(JadwalDokter jadwalList[], int &jumlahJadwal, dokter dokterList[], int jumlahDokter) {
    if(jumlahJadwal == 0) {
        cout << "BELUM ADA DATA JADWAL DOKTER.\n";
        return;
    }
    string id;
    cout << "Masukkan ID dokter yang jadwalnya ingin dihapus: "; cin >> id;
    bool found = false;
    for(int i = 0; i < jumlahJadwal; i++){
        if(jadwalList[i].idDokter == id){
            found = true;
            for(int j = i; j < jumlahJadwal - 1; j++)
                jadwalList[j] = jadwalList[j + 1];
            jumlahJadwal--;
            break;
        }
    }
    if(found){
        // Tulis ulang file jadwal
        ofstream file(data_jadwal_filename);
        for(int i = 0; i < jumlahJadwal; i++)
            file << jadwalList[i].idDokter << ";" << jadwalList[i].hari << ";" << jadwalList[i].jam << endl;
        file.close();
        cout << "[SUCCESS]Jadwal dokter berhasil dihapus.\n";
    } else {
        cout << "[FAILED]ID dokter tidak ditemukan.\n";
    }
    char ulang;
    cout << "Ingin Hapus Jadwal Lagi? (y/n): "; cin >> ulang;
    if(ulang == 'y' || ulang == 'Y'){
                        cout << "======== HAPUS JADWAL DOKTER =========\n";
        tampilkanJadwal(jadwalList, 0, jumlahJadwal, dokterList, jumlahDokter);
        hapusJadwalDokter(jadwalList, jumlahJadwal, dokterList, jumlahDokter);
    } else {
        cout << "Kembali ke menu utama.\n";
    }
}

void menuAdmin(Pasien pasien[], int &jumlahPasien, dokter dokterList[], int &jumlahDokter, JadwalDokter jadwalList[], int &jumlahJadwal) {
    do {
        system("cls");
        cout << "======== SISTEM INFORMASI RUMAH SAKIT =======\n";
        cout << "+=============== MENU ADMIN==================+\n";
        cout << "| 1. Tambah Pasien                           |\n";                         
        cout << "| 2. Lihat Pasien                            |\n";
        cout << "| 3. Hapus Pasien                            |\n";
        cout << "| 4. Tambah Dokter                           |\n";
        cout << "| 5. Lihat Dokter                            |\n";
        cout << "| 6. Hapus Dokter                            |\n";
        cout << "| 7. Tambah Jadwal Dokter                    |\n";
        cout << "| 8. Lihat Jadwal Dokter                     |\n";
        cout << "| 9. Hapus Jadwal Dokter                     |\n";
        cout << "| 0. Logout                                  |\n";
        cout << "+============================================+\n";
        cout << "Pilih: ";
        cin >> menuadmin; cin.ignore();

        switch (menuadmin) {
            case 1: 
                system("cls");
                inputPasien(pasien, jumlahPasien); 
                break;
            case 2: 
                cout << "=========== DAFTAR PASIEN ============\n";
                tampilkanPasien(pasien, jumlahPasien);
                break;
            case 3: 
                cout << "=========== HAPUS PASIEN ============\n";
                hapusPasien(pasien, jumlahPasien);
                break;
            case 4: 
                inputDokter(dokterList, jumlahDokter); 
                break;
            case 5: 
                cout << "=================== DAFTAR DOKTER ====================\n";
                tampilkanDokter(dokterList, jumlahDokter); 
                break;
            case 6:
                cout << "=================== HAPUS DOKTER ====================\n";
                hapusDokter(dokterList, jumlahDokter, jadwalList, jumlahJadwal); 
                break;
            case 7: 
                tambahJadwal(jadwalList, jumlahJadwal, dokterList, jumlahDokter);
                break;
            case 8: 
                cout << "========== JADWAL DOKTER ===========\n";
                tampilkanJadwal(jadwalList, 0 ,jumlahJadwal, dokterList, jumlahDokter);
                break;
            case 9: 
                cout << "======== HAPUS JADWAL DOKTER =========\n";
                tampilkanJadwal(jadwalList, 0, jumlahJadwal, dokterList, jumlahDokter);
                hapusJadwalDokter(jadwalList, jumlahJadwal, dokterList, jumlahDokter);
                break;        
            case 0: 
                cout << "Logout berhasil.\n";
                break;
            default: cout << "Pilihan tidak tersedia.\n";
            break;
        }
        system("pause");
    } while (menuadmin != 0);
}

bool idOpnameSudahAda(string id) {
    ifstream file(data_pasien_opname_filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ';');
        if (!idStr.empty() && idStr == id) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void daftaropname(Pasien pasien[], int &jumlahPasien) {
    int i = jumlahPasien;
    cin.ignore();
    cout << "Nama Pasien: "; getline(cin, pasien[i].namaPasien);
    cout << "Umur Pasien: "; cin >> pasien[i].umur;
    cin.ignore();
    cout << "Penyakit Pasien: "; getline(cin, pasien[i].penyakit);

    // Tampilkan pilihan kelas kamar
    cout << endl << setfill('=') << setw(70) << "" << endl;
    cout << "| " << setfill(' ') 
         << left << setw(5) << "Nomor" << " | " 
         << left << setw(25) << "Tipe Kamar" << " | " 
         << left << setw(13) << "Deposit Awal" << " | "
         << left << setw(14) << "Tarif per Hari" << " | " << endl;
    cout << setfill('-') << setw(70) << "" << endl;
    for (int j = 0; j < 4; j++) {
        cout << "| " << setfill(' ') 
             << left << setw(5) << j + 1 << " | " 
             << left << setw(25) << namaKelasKamar[j] << " | " 
             << left << setw(13) << (tarifPerKelas[j] * 5) << " | "
             << left << setw(14) << tarifPerKelas[j] << " | " << endl;   
    }
    cout << setfill('=') << setw(70) << "" << endl << endl;

    // Input kelas kamar dengan validasi
    do {
        cout << "Kelas Kamar Opname (1-4): "; cin >> pasien[i].kelaskamar;
        if (pasien[i].kelaskamar < 1 || pasien[i].kelaskamar > 4) {
            cout << "Kelas kamar tidak valid. Silakan pilih antara 1-4.\n";
        }
    } while (pasien[i].kelaskamar < 1 || pasien[i].kelaskamar > 4);

    // Validasi ID unik di array dan file opname
    bool idUnik;
    do {
        idUnik = true;
        cout << "ID Pasien: "; cin >> pasien[i].id;

        // Cek di file opname
        if (idOpnameSudahAda(pasien[i].id)) {
            cout << "Pasien dengan ID ini sudah pernah daftar opname. Tidak bisa daftar lagi.\n";
            idUnik = false;
        }
    } while (!idUnik);

    // Tampilkan nota opname
        cout << endl << setfill('=') << setw(42) << "" << endl;
        cout << "| " << left << setfill(' ') << setw(38) << "NOTA PENDAFTARAN OPNAME PASIEN" << " |\n";
        cout << setfill('=') << setw(42) << "" << endl;
        cout << "| " << setfill(' ') 
                << left << setw(16) << "ID Pasien" << ": " << left << setw(20) << pasien[i].id << " |\n";
        cout << "| " << setfill(' ')
            << left << setw(16) << "Nama Pasien" << ": " << left << setw(20) << pasien[i].namaPasien << " |\n"; 
        cout << "| " << setfill(' ') 
            << left << setw(16) << "Umur Pasien" << ": " << left << setw(20) << pasien[i].umur << " |\n";
        cout << "| " << setfill(' ') 
            << left << setw(16) << "Penyakit Pasien" << ": " << left << setw(20) << pasien[i].penyakit << " |\n";
        cout << "| " << setfill(' ') 
            << left << setw(16) << "Kelas Kamar" << ": " << left << setw(20) << kelaskamar(pasien, i) << " |\n";
        cout << setfill('=') << setw(42) << "" << endl << endl;   

        cout << "Silakan melakukan pembayaran deposit awal sebesar Rp " 
        << depositawal(pasien, i) << " ke loket utama.\n";

    // Simpan data ke file
    ofstream simpan(data_pasien_opname_filename, ios::app);
    if (simpan.is_open()) {
        simpan << pasien[i].id << ";"
               << pasien[i].namaPasien << ";"
               << pasien[i].umur << ";"
               << pasien[i].penyakit << ";"
               << pasien[i].kelaskamar << "\n";
        simpan.close();
        cout << "Pasien Berhasil didaftarkan untuk Opname.\n";
    } else {
        cout << "Gagal membuka file untuk menyimpan data.\n";
    }
    jumlahPasien++;
}

void jengukpasien(Pasien pasien[], int jumlahPasien) {
    if (jumlahPasien == 0) {
        cout << "Belum ada data pasien.\n";
        return;
    }

    tampilkanPasien(pasien, jumlahPasien);
    string idJenguk;
    cout << "Masukkan ID pasien yang ingin dijenguk: ";
    cin >> idJenguk;

    // Cari pasien berdasarkan ID
    int idx = -1;
    for (int i = 0; i < jumlahPasien; ++i) {
        if (pasien[i].id == idJenguk) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "ID pasien tidak ditemukan.\n";
        return;
    } else {
        cout << "Jam jenguk pasien dibuka pada jam 10.00-12.00 dan 16.00-18.00.\n";
        cout << "Silakan menjenguk saudara/i " << pasien[idx].namaPasien
             << " di ruangan " << kelaskamar(pasien, idx) << endl;
        system("pause");
        cout << setfill('-') << setw(30) << "" << endl;
    }
}

void bubbleSortDokterByID(dokter arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].id > arr[j + 1].id) {
                dokter temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sequentialSearchPasien(Pasien pasien[], int jumlahpasien){
    cin.ignore();
    string cari;
    cout << "Masukkan ID / Nama pasien yang ingin dicari: ";
    getline(cin >> ws, cari);
    bool ditemukan = false;
    for(int i=0; i < jumlahpasien; i++){
        if(pasien[i].namaPasien == cari || pasien[i].id == cari){
            cout << "Data Pasien Ditemukan:\n";
            cout << "Nama: " << pasien[i].namaPasien << endl;
            cout << "ID: " << pasien[i].id << endl;
            cout << "Umur: " << pasien[i].umur << endl;
            cout << "Penyakit: " << pasien[i].penyakit << endl;
            cout << "Kelas Kamar: " << kelaskamar(pasien, i) << endl;
            ditemukan = true;
            break;
        } 
    }
    if(!ditemukan) {
        cout << "[FAILED]Pasien dengan Nama tersebut tidak ditemukan.\n";
    }
    char ulang;
    cout << "Ingin Cari Lagi? (y/n): "; cin >> ulang;
    if(ulang == 'y' || ulang == 'Y'){
        sequentialSearchPasien(pasien, jumlahpasien);
    };
}

void binarySearchDokter(dokter dokterList[], int jumlahDokter){
    if (jumlahDokter == 0) {
        cout << "Data dokter belum ada.\n";
        system("pause");
        return;
    }
    char urut;
    cout << "Apakah data dokter sudah urut berdasarkan ID? (y/n): ";
    cin >> urut;
    if (urut == 'n' || urut == 'N') {
        bubbleSortDokterByID(dokterList, jumlahDokter);
        cout << "Data dokter telah diurutkan berdasarkan ID.\n";
    }
    string cari;
    cout << "Masukkan Nama / ID dokter yang ingin dicari: ";
    getline(cin >> ws, cari);

    int left = 0, right = jumlahDokter - 1;
    bool ditemukan = false;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (dokterList[mid].id == cari || dokterList[mid].nama == cari) {
            cout << "Data Dokter Ditemukan:\n";
            cout << "ID: " << dokterList[mid].id << endl;
            cout << "Nama: " << dokterList[mid].nama << endl;
            cout << "Spesialis: " << dokterList[mid].spesialis << endl;
            ditemukan = true;
            break;
        } else if (dokterList[mid].id < cari) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (!ditemukan) {
        for (int i = 0; i < jumlahDokter; i++) {
            if (dokterList[i].nama == cari) {
                cout << "Data Dokter Ditemukan:\n";
                cout << "ID: " << dokterList[i].id << endl;
                cout << "Nama: " << dokterList[i].nama << endl;
                cout << "Spesialis: " << dokterList[i].spesialis << endl;
                ditemukan = true;
                break;
            }
        }
    }
    if (!ditemukan) {
        cout << "[FAILED]Dokter dengan Nama / ID tersebut tidak ditemukan.\n";
    }
    char ulang;
    cout << "Ingin Cari Lagi? (y/n): "; cin >> ulang;
    if (ulang == 'y' || ulang == 'Y') {
        binarySearchDokter(dokterList, jumlahDokter);
    };
}

void menupelanggan(Pasien pasien[], int &jumlahPasien, dokter dokterList[], int jumlahDokter, JadwalDokter jadwalList[], int jumlahJadwal) {
    int menupelanggan;
    cout << "+================================================+\n";
    cout << "|================ MENU PELANGGAN ================|\n";
    cout << "| 1. Berobat                                     |\n";
    cout << "| 2. Jenguk Pasien                               |\n";
    cout << "| 3. Daftar Opname                               |\n";
    cout << "| 4. Lihat Jadwal Dokter                         |\n";
    cout << "| 5. Cari Pasien(Sequential Search)              |\n";
    cout << "| 6. Cari Dokter(Binary Search)                  |\n";
    cout << "|0. Exit                                         |\n";
    cout << "+================================================+\n";
    cout << "Silakan Pilih menu: ";
    cin >> menupelanggan;
    switch (menupelanggan) {
        case 1:
            cout << "Silakan menuju loket pendaftaran.\n";
            break;
        case 2:
            jengukpasien(pasien, jumlahPasien);
            break;
        case 3:
            daftaropname(pasien, jumlahPasien);
            break;
        case 4: 
            cout << "========== JADWAL DOKTER ===========\n";
            tampilkanJadwal(jadwalList, 0, jumlahJadwal, dokterList, jumlahDokter);
            break;
        case 5: 
            sequentialSearchPasien(pasien, jumlahPasien);
            break;
        case 6: 
            bubbleSortDokterByID(dokterList, jumlahDokter); // Pastikan data dokter sudah terurut
            binarySearchDokter(dokterList, jumlahDokter);
            break;
        case 0:
            cout << "\nTerima kasih telah menggunakan sistem kami.\n";
            exit(0);
            break;
        default:
            cout << "Pilihan tidak tersedia.\n";
            system("pause");
        break;
    }
}
//TINGGAL TAMBAHIN CASE 1 MENU PELANGGAN(BEROBAT) SAMA RAPIHIN KODE