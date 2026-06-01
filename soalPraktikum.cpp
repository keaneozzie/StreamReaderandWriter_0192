#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <stdexcept>
#include <limits>

using namespace std;

// ==========================================
// 1. STRUKTUR KELAS TOKOELEKTRONIK (OOP)
// ==========================================
class TokoElektronik {
private:
    // Array berkapasitas 3 elemen untuk enkapsulasi barang di etalase
    array<string, 3> etalase;

public:
    // Constructor untuk mengisi data awal secara otomatis
    TokoElektronik() {
        etalase[0] = "Laptop ASUS ROG";
        etalase[1] = "Smartphone Samsung S24";
        etalase[2] = "Smart TV LG 43 Inch";
    }

    // Method untuk mengambil produk berdasarkan nomorRak dengan Exception Handling
    string ambilProduk(size_t nomorRak) {
        try {
            // Menggunakan .at() untuk mendeteksi out_of_range secara otomatis
            return etalase.at(nomorRak);
        }
        catch (const out_of_range& e) {
            // 2. MEKANISME EXCEPTION HANDLING KUSTOM
            // Melemparkan kembali (throw) pesan error kustom sesuai permintaan
            throw runtime_error("Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!");
        }
    }
};

// ==========================================
// 3. FUNGSI-FUNGSI MANAJEMEN GUDANG (CRUD FILE I/O)
// ==========================================
const string FILE_GUDANG = "gudang.txt";

// Fungsi Helper untuk membersihkan buffer input (mencegah bug cin)
void bersihkanBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// CREATE: Menambahkan barang baru ke file
void tambahBarang() {
    ofstream file(FILE_GUDANG, ios::app);
    if (!file) {
        cout << "Error: Gagal membuka file gudang!\n";
        return;
    }

    string namaBarang;
    cout << "Masukkan nama barang baru: ";
    bersihkanBuffer();
    getline(cin, namaBarang);

    if (!namaBarang.empty()) {
        file << namaBarang << endl;
        cout << "=> Barang [ " << namaBarang << " ] berhasil ditambahkan ke gudang.\n";
    }
    file.close();
}

// READ: Membaca dan menampilkan isi file
void tampilkanGudang() {
    ifstream file(FILE_GUDANG);
    cout << "\n=========================================\n";
    cout << "      DAFTAR STOK BARANG DI GUDANG       \n";
    cout << "=========================================\n";

    if (!file) {
        cout << "(Gudang kosong atau file belum dibuat)\n";
        cout << "=========================================\n";
        return;
    }

    string namaBarang;
    int nomor = 1;
    bool adaBarang = false;

    while (getline(file, namaBarang)) {
        if (!namaBarang.empty()) {
            cout << nomor << ". " << namaBarang << endl;
            nomor++;
            adaBarang = true;
        }
    }

    if (!adaBarang) {
        cout << "(Tidak ada data barang di dalam file)\n";
    }
    cout << "=========================================\n";
    file.close();
}

// UPDATE: Mengubah data barang berdasarkan nomor urut
void updateBarang() {
    tampilkanGudang();
    ifstream fileBaca(FILE_GUDANG);
    if (!fileBaca) return;

    // Membaca seluruh data ke memori temporer
    array<string, 100> tempGudang; 
    int totalBarang = 0;
    string baris;

    while (getline(fileBaca, baris) && totalBarang < 100) {
        if (!baris.empty()) {
            tempGudang[totalBarang] = baris;
            totalBarang++;
        }
    }
    fileBaca.close();

    if (totalBarang == 0) return;

    int pilihan;
    cout << "Pilih nomor barang yang ingin diubah: ";
    cin >> pilihan;

    if (pilihan < 1 || pilihan > totalBarang) {
        cout << "=> Nomor barang tidak valid!\n";
        return;
    }

    string namaBaru;
    cout << "Masukkan nama baru untuk [ " << tempGudang[pilihan - 1] << " ]: ";
    bersihkanBuffer();
    getline(cin, namaBaru);

    // Menulis ulang file dengan data yang diperbarui
    ofstream fileTulis(FILE_GUDANG, ios::trunc);
    for (int i = 0; i < totalBarang; i++) {
        if (i == pilihan - 1) {
            fileTulis << namaBaru << endl;
        } else {
            fileTulis << tempGudang[i] << endl;
        }
    }
    fileTulis.close();
    cout << "=> Data barang berhasil diperbarui!\n";
}

// DELETE: Menghapus data barang berdasarkan nomor urut
void hapusBarang() {
    tampilkanGudang();
    ifstream fileBaca(FILE_GUDANG);
    if (!fileBaca) return;

    array<string, 100> tempGudang;
    int totalBarang = 0;
    string baris;

    while (getline(fileBaca, baris) && totalBarang < 100) {
        if (!baris.empty()) {
            tempGudang[totalBarang] = baris;
            totalBarang++;
        }
    }
    fileBaca.close();

    if (totalBarang == 0) return;

    int pilihan;
    cout << "Pilih nomor barang yang ingin dihapus: ";
    cin >> pilihan;

    if (pilihan < 1 || pilihan > totalBarang) {
        cout << "=> Nomor barang tidak valid!\n";
        return;
    }

    // Menulis ulang file tanpa menyertakan barang yang dihapus
    ofstream fileTulis(FILE_GUDANG, ios::trunc);
    string barangDihapus = tempGudang[pilihan - 1];
    
    for (int i = 0; i < totalBarang; i++) {
        if (i != pilihan - 1) {
            fileTulis << tempGudang[i] << endl;
        }
    }
    fileTulis.close();
    cout << "=> Barang [ " << barangDihapus << " ] berhasil dihapus dari gudang!\n";
}

// SIMULASI ETALASE: Menguji Exception Handling (Poin 4)
void jalankanSimulasiEtalase(TokoElektronik& toko) {
    cout << "\n=========================================\n";
    cout << "       SIMULASI PENGAMBILAN ETALASE      \n";
    cout << "=========================================\n";

    // Skenario 1: Ambil indeks ke-1 (Sukses)
    cout << "[Skenario 1] Mencoba mengambil barang di rak indeks 1...\n";
    try {
        string barang = toko.ambilProduk(1);
        cout << "Hasil: BERHASIL menemukan -> " << barang << "\n\n";
    }
    catch (const runtime_error& e) {
        cout << "Hasil: " << e.what() << "\n\n";
    }

    // Skenario 2: Ambil indeks ke-5 (Gagal & Lempar Pesan Kustom)
    cout << "[Skenario 2] Mencoba mengambil barang di rak indeks 5...\n";
    try {
        string barang = toko.ambilProduk(5);
        cout << "Hasil: BERHASIL menemukan -> " << barang << "\n\n";
    }
    catch (const runtime_error& e) {
        cout << "Hasil Terangkap: " << e.what() << "\n";
    }
    cout << "=========================================\n";
}

// ==========================================
// 4. MAIN FUNCTION & CLI MENU LOOP
// ==========================================
int main() {
    TokoElektronik tokoGibran;
    int pilihanMenu;

    do {
        // Otomatis membaca dan menampilkan isi gudang setiap kali menu utama dimuat
        tampilkanGudang();

        cout << "--- SISTEM MANAJEMEN TOKO GIBRAN JAYA ---\n";
        cout << "1. Tambah Barang Baru (Create)\n";
        cout << "2. Ubah Data Barang (Update)\n";
        cout << "3. Hapus Data Barang (Delete)\n";
        cout << "4. Jalankan Simulasi Etalase (Exception Handling)\n";
        cout << "5. Keluar Aplikasi\n";
        cout << "Pilih opsi menu (1-5): ";
        
        // Validasi input menu agar tidak loop terus menerus jika diinput karakter/huruf
        if (!(cin >> pilihanMenu)) {
            cout << "Input harus berupa angka!\n";
            bersihkanBuffer();
            system("pause");
            continue;
        }

        switch (pilihanMenu) {
            case 1:
                tambahBarang();
                break;
            case 2:
                updateBarang();
                break;
            case 3:
                hapusBarang();
                break;
            case 4:
                jalankanSimulasiEtalase(tokoGibran);
                break;
            case 5:
                cout << "\nTerima kasih telah menggunakan sistem Toko Gibran Jaya!\n";
                break;
            default:
                cout << "Pilihan menu tidak tersedia. Silakan coba lagi.\n";
        }
        
        if (pilihanMenu != 5) {
            cout << endl;
            system("pause"); // Berhenti sejenak sebelum layar dibersihkan (khusus Windows)
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        }

    } while (pilihanMenu != 5);

    return 0;
}