

// ============================================================
// TUGAS BESAR STRUKTUR DATA - MULTI LINKED LIST (LIST TO LIST)
// Studi Kasus: Anime Convention & Cosplay Competition
// Parent : Kompetisi (tema, kategori, lokasi)
// Child  : Peserta   (nama, karakterCosplay, skorPenilaian)
// ============================================================
#include <cctype>
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

// ======================== ADT ========================
struct Peserta {
    string nama;
    string karakterCosplay;
    double skorPenilaian;
    Peserta* next;
};

struct Kompetisi {
    string tema;
    string kategori;
    string lokasi;
    Peserta* firstChild;   // kepala child list milik kompetisi ini
    Kompetisi* next;       // kompetisi berikutnya pada parent list
};

struct ListMLL {
    Kompetisi* firstParent;
};

// ======================== UTIL ========================
void initList(ListMLL& L) { L.firstParent = nullptr; }

string toLower(string s) {
    for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

bool sama(const string& a, const string& b) { return toLower(a) == toLower(b); }

string bacaString(const string& prompt) {
    string s;
    do {
        cout << prompt;
        getline(cin, s);
        if (s.empty()) cout << "  [!] Input tidak boleh kosong.\n";
    } while (s.empty());
    return s;
}

double bacaSkor(const string& prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= 0 && x <= 100) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Skor harus angka 0 - 100.\n";
    }
}

int bacaInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Masukkan angka yang valid.\n";
    }
}

// ======================== PARENT LIST ========================
Kompetisi* createParent(const string& tema, const string& kategori, const string& lokasi) {
    Kompetisi* p = new Kompetisi;
    p->tema = tema;
    p->kategori = kategori;
    p->lokasi = lokasi;
    p->firstChild = nullptr;
    p->next = nullptr;
    return p;
}

Kompetisi* findParent(const ListMLL& L, const string& tema) {
    Kompetisi* cur = L.firstParent;
    while (cur != nullptr) {
        if (sama(cur->tema, tema)) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// insert last; menolak tema duplikat (kunci unik = tema)
bool insertParent(ListMLL& L, Kompetisi* p) {
    if (findParent(L, p->tema) != nullptr) return false;
    if (L.firstParent == nullptr) {
        L.firstParent = p;
    } else {
        Kompetisi* cur = L.firstParent;
        while (cur->next != nullptr) cur = cur->next;
        cur->next = p;
    }
    return true;
}

// bebaskan seluruh child milik sebuah parent (anti memory leak)
int hapusSemuaChild(Kompetisi* parent) {
    int n = 0;
    Peserta* cur = parent->firstChild;
    while (cur != nullptr) {
        Peserta* del = cur;
        cur = cur->next;
        delete del;
        n++;
    }
    parent->firstChild = nullptr;
    return n;
}

bool deleteParent(ListMLL& L, const string& tema) {
    if (L.firstParent == nullptr) return false;       // list kosong
    Kompetisi* prev = nullptr;
    Kompetisi* cur = L.firstParent;
    while (cur != nullptr && !sama(cur->tema, tema)) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;                 // tidak ditemukan
    if (prev == nullptr) L.firstParent = cur->next;   // hapus head
    else prev->next = cur->next;                      // hapus tengah / tail
    int n = hapusSemuaChild(cur);
    cout << "  (" << n << " peserta ikut terhapus)\n";
    delete cur;
    return true;
}

void printParent(const ListMLL& L) {
    if (L.firstParent == nullptr) {
        cout << "  Belum ada kompetisi terdaftar.\n";
        return;
    }
    int i = 1;
    for (Kompetisi* p = L.firstParent; p != nullptr; p = p->next) {
        cout << "  " << i++ << ". Tema     : " << p->tema << "\n"
             << "     Kategori : " << p->kategori << "\n"
             << "     Lokasi   : " << p->lokasi << "\n";
    }
}

// ======================== CHILD LIST ========================
Peserta* createChild(const string& nama, const string& karakter, double skor) {
    Peserta* c = new Peserta;
    c->nama = nama;
    c->karakterCosplay = karakter;
    c->skorPenilaian = skor;
    c->next = nullptr;
    return c;
}

Peserta* findChild(const Kompetisi* parent, const string& nama) {
    if (parent == nullptr) return nullptr;
    Peserta* cur = parent->firstChild;
    while (cur != nullptr) {
        if (sama(cur->nama, nama)) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// Cari berdasarkan karakter cosplay (kriteria tambahan)
Peserta* findChildByKarakter(const Kompetisi* parent, const string& karakter) {
    if (parent == nullptr) return nullptr;
    Peserta* cur = parent->firstChild;
    while (cur != nullptr) {
        if (sama(cur->karakterCosplay, karakter)) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// insert last; nama unik dalam satu kompetisi
bool insertChild(Kompetisi* parent, Peserta* c) {
    if (parent == nullptr) return false;
    if (findChild(parent, c->nama) != nullptr) return false;
    if (parent->firstChild == nullptr) {
        parent->firstChild = c;
    } else {
        Peserta* cur = parent->firstChild;
        while (cur->next != nullptr) cur = cur->next;
        cur->next = c;
    }
    return true;
}

bool deleteChild(Kompetisi* parent, const string& nama) {
    if (parent == nullptr || parent->firstChild == nullptr) return false;
    Peserta* prev = nullptr;
    Peserta* cur = parent->firstChild;
    while (cur != nullptr && !sama(cur->nama, nama)) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    if (prev == nullptr) parent->firstChild = cur->next;  // hapus head
    else prev->next = cur->next;                          // hapus tengah / tail
    delete cur;
    return true;
}

void printChild(const Kompetisi* parent) {
    if (parent == nullptr) return;
    if (parent->firstChild == nullptr) {
        cout << "     Belum ada peserta terdaftar\n";
        return;
    }
    int i = 1;
    for (Peserta* c = parent->firstChild; c != nullptr; c = c->next) {
        cout << "     " << i++ << ". " << c->nama
             << " | Karakter: " << c->karakterCosplay
             << " | Skor: " << fixed << setprecision(1) << c->skorPenilaian << "\n";
    }
}

// ======================== TRAVERSAL ========================
void printAll(const ListMLL& L) {
    if (L.firstParent == nullptr) {
        cout << "  Belum ada kompetisi terdaftar.\n";
        return;
    }
    int i = 1;
    for (Kompetisi* p = L.firstParent; p != nullptr; p = p->next) {
        cout << "  [" << i++ << "] " << p->tema << " (" << p->kategori
             << ", " << p->lokasi << ")\n";
        printChild(p);
    }
}

// ======================== FITUR TAMBAHAN ========================
int countChild(const Kompetisi* parent) {
    int n = 0;
    if (parent == nullptr) return 0;
    for (Peserta* c = parent->firstChild; c != nullptr; c = c->next) n++;
    return n;
}

Peserta* findTopScore(const Kompetisi* parent) {
    if (parent == nullptr || parent->firstChild == nullptr) return nullptr;
    Peserta* best = parent->firstChild;
    for (Peserta* c = best->next; c != nullptr; c = c->next)
        if (c->skorPenilaian > best->skorPenilaian) best = c;
    return best;
}

void printStatistik(const ListMLL& L) {
    if (L.firstParent == nullptr) {
        cout << "  Belum ada kompetisi terdaftar.\n";
        return;
    }
    for (Kompetisi* p = L.firstParent; p != nullptr; p = p->next) {
        cout << "  " << p->tema << " -> " << countChild(p) << " peserta";
        Peserta* top = findTopScore(p);
        if (top) cout << " | Skor tertinggi: " << top->nama << " ("
                      << fixed << setprecision(1) << top->skorPenilaian << ")";
        cout << "\n";
    }
}

// ======================== DEALOKASI SEMUA ========================
void destroyAll(ListMLL& L) {
    while (L.firstParent != nullptr) {
        Kompetisi* del = L.firstParent;
        L.firstParent = del->next;
        hapusSemuaChild(del);
        delete del;
    }
}

// ======================== HANDLER MENU ========================
void menuTambahParent(ListMLL& L) {
    string tema = bacaString("  Tema     : ");
    string kat  = bacaString("  Kategori : ");
    string lok  = bacaString("  Lokasi   : ");
    Kompetisi* p = createParent(tema, kat, lok);
    if (insertParent(L, p)) cout << "  >> Kompetisi berhasil ditambahkan.\n";
    else { cout << "  >> Gagal: tema \"" << tema << "\" sudah ada.\n"; delete p; }
}

void menuCariParent(ListMLL& L) {
    string tema = bacaString("  Cari tema: ");
    Kompetisi* p = findParent(L, tema);
    if (p) {
        cout << "  >> Ditemukan!\n"
             << "     Tema     : " << p->tema << "\n"
             << "     Kategori : " << p->kategori << "\n"
             << "     Lokasi   : " << p->lokasi << "\n"
             << "     Peserta  : " << countChild(p) << "\n";
    } else cout << "  >> Kompetisi \"" << tema << "\" tidak ditemukan.\n";
}

void menuHapusParent(ListMLL& L) {
    if (L.firstParent == nullptr) { cout << "  >> List kompetisi kosong, tidak ada yang dihapus.\n"; return; }
    string tema = bacaString("  Tema kompetisi yang dihapus: ");
    if (deleteParent(L, tema)) cout << "  >> Kompetisi berhasil dihapus.\n";
    else cout << "  >> Kompetisi \"" << tema << "\" tidak ditemukan.\n";
}

void menuTambahChild(ListMLL& L) {
    if (L.firstParent == nullptr) { cout << "  >> Belum ada kompetisi. Tambahkan kompetisi dulu.\n"; return; }
    string tema = bacaString("  Tema kompetisi tujuan: ");
    Kompetisi* p = findParent(L, tema);
    if (!p) { cout << "  >> Kompetisi \"" << tema << "\" tidak ditemukan.\n"; return; }
    string nama = bacaString("  Nama peserta     : ");
    string kar  = bacaString("  Karakter cosplay : ");
    double skor = bacaSkor("  Skor (0-100)     : ");
    Peserta* c = createChild(nama, kar, skor);
    if (insertChild(p, c)) cout << "  >> Peserta berhasil ditambahkan ke " << p->tema << ".\n";
    else { cout << "  >> Gagal: nama \"" << nama << "\" sudah terdaftar di kompetisi ini.\n"; delete c; }
}

void menuCariChild(ListMLL& L) {
    string tema = bacaString("  Tema kompetisi: ");
    Kompetisi* p = findParent(L, tema);
    if (!p) { cout << "  >> Kompetisi \"" << tema << "\" tidak ditemukan.\n"; return; }
    if (p->firstChild == nullptr) { cout << "  >> Kompetisi ini belum punya peserta.\n"; return; }
    cout << "  Cari berdasarkan: 1) Nama  2) Karakter cosplay\n";
    int m = bacaInt("  Pilih: ");
    string key = bacaString(m == 2 ? "  Karakter: " : "  Nama    : ");
    Peserta* c = (m == 2) ? findChildByKarakter(p, key) : findChild(p, key);
    if (c) {
        cout << "  >> Ditemukan: " << c->nama << " | " << c->karakterCosplay
             << " | Skor " << fixed << setprecision(1) << c->skorPenilaian << "\n";
        cout << "  Daftar peserta " << p->tema << ":\n";
        printChild(p);
    } else cout << "  >> Peserta \"" << key << "\" tidak ditemukan di " << p->tema << ".\n";
}

void menuHapusChild(ListMLL& L) {
    string tema = bacaString("  Tema kompetisi: ");
    Kompetisi* p = findParent(L, tema);
    if (!p) { cout << "  >> Kompetisi \"" << tema << "\" tidak ditemukan.\n"; return; }
    if (p->firstChild == nullptr) { cout << "  >> Kompetisi ini belum punya peserta, tidak ada yang dihapus.\n"; return; }
    string nama = bacaString("  Nama peserta yang dihapus: ");
    if (deleteChild(p, nama)) cout << "  >> Peserta berhasil dihapus.\n";
    else cout << "  >> Peserta \"" << nama << "\" tidak ditemukan.\n";
}

void menuPrintChild(ListMLL& L) {
    string tema = bacaString("  Tema kompetisi: ");
    Kompetisi* p = findParent(L, tema);
    if (!p) { cout << "  >> Kompetisi \"" << tema << "\" tidak ditemukan.\n"; return; }
    cout << "  Peserta " << p->tema << ":\n";
    printChild(p);
}

// ======================== DEMO 8 SKENARIO ========================
void garis(const string& judul) {
    cout << "\n========== " << judul << " ==========\n";
}

void jalankanDemo(ListMLL& L) {
    destroyAll(L);
    initList(L);

    garis("SKENARIO 1: Tambah & tampilkan parent");
    insertParent(L, createParent("Shounen", "Solo", "Main Hall"));
    insertParent(L, createParent("Vtuber", "Duo/Grup", "Stage B"));
    insertParent(L, createParent("Mecha & Armor", "Solo", "Hall C"));
    printParent(L);

    garis("SKENARIO 2: Pencarian parent (tema = Vtuber)");
    Kompetisi* v = findParent(L, "Vtuber");
    if (v) cout << "  Ditemukan -> Kategori: " << v->kategori << ", Lokasi: " << v->lokasi << "\n";
    else cout << "  Tidak ditemukan.\n";

    garis("SKENARIO 3: Tambah child pada parent terpilih");
    Kompetisi* s = findParent(L, "Shounen");
    insertChild(s, createChild("Raka", "Luffy", 88.5));
    insertChild(s, createChild("Dimas", "Zoro", 85.0));
    insertChild(s, createChild("Tari", "Nami", 90.0));
    insertChild(v, createChild("Hana & Sinta", "Pekora & Moona", 92.0));
    insertChild(v, createChild("Kevin", "Gawr Gura", 87.0));
    cout << "  3 peserta -> Shounen, 2 peserta -> Vtuber, Mecha & Armor dikosongkan.\n";

    garis("SKENARIO 4: Cari & tampilkan child (Dimas di Shounen)");
    Peserta* d = findChild(s, "Dimas");
    if (d) cout << "  Ditemukan: " << d->nama << " | " << d->karakterCosplay << " | " << d->skorPenilaian << "\n";
    cout << "  Daftar peserta Shounen:\n";
    printChild(s);

    garis("SKENARIO 5: Hapus child (Raka / head Shounen)");
    cout << (deleteChild(s, "Raka") ? "  Raka berhasil dihapus.\n" : "  Gagal menghapus.\n");
    cout << "  Head baru Shounen: " << s->firstChild->nama << "\n";
    printChild(s);

    garis("SKENARIO 6: Hapus parent (Vtuber)");
    cout << (deleteParent(L, "Vtuber") ? "  Kompetisi Vtuber berhasil dihapus.\n" : "  Gagal menghapus.\n");
    printParent(L);

    garis("SKENARIO 7: printAll()");
    printAll(L);

    garis("SKENARIO 8: Penanganan kondisi kosong");
    Kompetisi* m = findParent(L, "Mecha & Armor");
    cout << "  Cari peserta 'Budi' di Shounen: "
         << (findChild(s, "Budi") ? "ditemukan" : "tidak ditemukan (aman)") << "\n";
    cout << "  Hapus peserta di Mecha & Armor (kosong): "
         << (deleteChild(m, "Budi") ? "terhapus" : "tidak ada yang dihapus (aman)") << "\n";
    ListMLL kosong; initList(kosong);
    cout << "  Hapus parent pada list kosong: "
         << (deleteParent(kosong, "Apa saja") ? "terhapus" : "list kosong (aman)") << "\n";
    cout << "  printAll() pada list kosong:\n";
    printAll(kosong);
    cout << "\n  Demo selesai. Data demo masih tersimpan, silakan lanjut eksplor lewat menu.\n";
}

// ======================== MAIN ========================
void tampilMenu() {
    cout << "\n==================================================\n"
         << "  ANIME CONVENTION & COSPLAY COMPETITION (MLL)\n"
         << "==================================================\n"
         << "  --- PARENT (Kompetisi) ---\n"
         << "   1. Tambah kompetisi\n"
         << "   2. Cari kompetisi\n"
         << "   3. Hapus kompetisi\n"
         << "   4. Tampilkan semua kompetisi\n"
         << "  --- CHILD (Peserta) ---\n"
         << "   5. Tambah peserta\n"
         << "   6. Cari peserta\n"
         << "   7. Hapus peserta\n"
         << "   8. Tampilkan peserta suatu kompetisi\n"
         << "  --- TRAVERSAL & EKSTRA ---\n"
         << "   9. Tampilkan seluruh data (printAll)\n"
         << "  10. Statistik (jumlah peserta & skor tertinggi)\n"
         << "  11. Jalankan demo 8 skenario pengujian\n"
         << "   0. Keluar\n"
         << "==================================================\n";
}

int main() {
    ListMLL L;
    initList(L);
    int pilih;
    do {
        tampilMenu();
        pilih = bacaInt("Pilih menu: ");
        cout << "\n";
        switch (pilih) {
            case 1:  menuTambahParent(L); break;
            case 2:  menuCariParent(L); break;
            case 3:  menuHapusParent(L); break;
            case 4:  printParent(L); break;
            case 5:  menuTambahChild(L); break;
            case 6:  menuCariChild(L); break;
            case 7:  menuHapusChild(L); break;
            case 8:  menuPrintChild(L); break;
            case 9:  printAll(L); break;
            case 10: printStatistik(L); break;
            case 11: jalankanDemo(L); break;
            case 0:  cout << "  Terima kasih! Membersihkan memori...\n"; break;
            default: cout << "  [!] Menu tidak valid.\n";
        }
    } while (pilih != 0);
    destroyAll(L);
    return 0;
}
