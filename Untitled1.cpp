#include<iostream>
#include<string>
#include<fstream>	
#include<windows.h>
#include<conio.h>

using namespace std;

struct NODEVIDU
{
	string info;
	NODEVIDU *pNext;
};

struct AWORD
{
	string tu;
	string loai;
	string nghia[5];
	NODEVIDU *first;
};

struct NODEWORD
{
	AWORD info;
	NODEWORD *left;
	NODEWORD *right;
};

struct hashtable {
	NODEWORD *head;
	NODEWORD *tail;
};

void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

// Hàm tô màu.
void textcolor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

// Hàm d?ch chuy?n con tr? d?n t?a d? x, y.
void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x - 1,y - 1 };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void tachchuoi(string line, NODEWORD *&tmp);
void ChenVdVaoDauDanhSach(NODEVIDU *&first, string vidu);

int indexHashtable(string &tu) {
	if (tu.empty())
		return -1;

	char x = tu[0];
	return x <= 'z' && x >= 'a' ? x - 'a' : -1;//l?y v? trí m?ng bam
}

void veGiaoDienChiTietTu(NODEWORD *tmp) {
	int dongHienTai = 0;
	textcolor(63);
	if (tmp == NULL) {
		gotoxy(73, 10);
		cout << "Tu nay khong co trong tu dien!";
		return;
	}

	// in ra cac chi tiet cua tu
	textcolor(63);
	gotoxy(73, 10);
	cout << tmp->info.tu;
	gotoxy(73, 11);
	cout << "Loai tu: " << tmp->info.loai << "\n";
	gotoxy(73, 12);
	cout << "Nghia: " << tmp->info.nghia[0];

	if (!tmp->info.nghia[1].empty()) {
		gotoxy(78, 13);
		cout << ", " << tmp->info.nghia[1];
	}
	if (!tmp->info.nghia[2].empty()) {
		gotoxy(78, 14);
		cout << ", " << tmp->info.nghia[2];
	}
	if (!tmp->info.nghia[3].empty()) {
		gotoxy(78, 15);
		cout << ", " << tmp->info.nghia[3];
	}
	if (!tmp->info.nghia[4].empty()) {
		gotoxy(78, 16);
		cout << ", " << tmp->info.nghia[4];
	}

	NODEVIDU *v = tmp->info.first;
	gotoxy(73, 17);
	cout << "Vi du:";
	while (v != NULL) {

		cout << v->info << ";";
		v = v->pNext;
	}
}

void printscreen(NODEWORD *tmp) {

	if (tmp == NULL) {
		gotoxy(73, 10);
		cout << "Khong co trong tu dien";
		return;
	}

	textcolor(63);
	gotoxy(73, 10);
	cout << tmp->info.tu;
	gotoxy(73, 11);
	cout << "Loai tu: " << tmp->info.loai << "\n";
	gotoxy(73, 12);
	cout << "Nghia: " << tmp->info.nghia[0];
	if (!tmp->info.nghia[1].empty()) {
		gotoxy(78, 13);
		cout << ", " << tmp->info.nghia[1];
	}
	if (!tmp->info.nghia[2].empty()) {
		gotoxy(78, 14);
		cout << tmp->info.nghia[2];
	}
	if (!tmp->info.nghia[3].empty()) {
		gotoxy(78, 15);
		cout << tmp->info.nghia[3];
	}
	if (!tmp->info.nghia[4].empty()) {
		gotoxy(78, 16);
		cout << tmp->info.nghia[4];
	}
	gotoxy(73, 17);
	cout << "Vi du: " << tmp->info.first->info;
}

bool compare(string &cantim, NODEWORD *dangdo) {
	return (dangdo->info.tu).find(cantim) != string::npos;
}

NODEWORD *search(hashtable *dictionary, string &searchword) {
	int x = indexHashtable(searchword);
	if (x < 0) {
		return NULL;
	}
	NODEWORD *head = dictionary[x].head;
	while (head != NULL) {
		if (compare(searchword, head)) {
			return head;
		}
		head = head->right;
	}
	return NULL;
}

void addAscending(hashtable &h, NODEWORD *newWord) {
	NODEWORD *left = 0;
	NODEWORD *cur = h.head;
	for (; cur&&newWord->info.tu > cur->info.tu; left = cur, cur = cur->right);
	if (left) {
		left->right = newWord;
		newWord->right = cur;
		newWord->left = left;
		if (cur)cur->left = newWord;
		else h.tail = newWord;
	}
	else {
		if (!h.head) h.tail = newWord;
		newWord->right = h.head;
		newWord->left = 0;
		if (h.head)h.head->left = newWord;
		h.head = newWord;
	}
}



/*l?y chu*/
void importWord(hashtable *&dictionary, string line) {
	if (line[0] >= 'a' and line[0] <= 'z') {
		NODEWORD *newword = new NODEWORD();
		tachchuoi(line, newword);
		newword->left = NULL;
		newword->right = NULL;
		addAscending(dictionary[line[0] - 'a'], newword);
	}
}

int demSoTu(hashtable *tudien) {
	int count = 0;
	for (int i = 0; i < 26; i++) {
		if (tudien[i].head == NULL) continue;
		NODEWORD *p = tudien[i].head;
		while (p != NULL) {
			count++;
			p = p->right;
		}
	}
	return count;
}

void readf(hashtable *&dictionary) {
	ifstream fi("data.txt");
	string line;
	if (!fi) {
		cerr << "Error while opening file\n";
		return;
	}
	while (getline(fi, line)) {
		importWord(dictionary, line);
	}
	fi.close();
}

void writef(hashtable *dictionary) {
	ofstream fo("data.txt");//fix l?i txt how -xóa t? d?ng nghia
	string data;
	if (fo.is_open()) {
		for (int i = 0; i < 26; i++) {
			NODEWORD *head = dictionary[i].head;
			NODEVIDU *p = new NODEVIDU();
			while (head != NULL) {
				//t?
				fo << head->info.tu;
				//lo?i t?
				fo << "@" << head->info.loai;
				//nghia
				fo << "@" << head->info.nghia[0]
					<< "@" << head->info.nghia[1]
					<< "@" << head->info.nghia[2]
					<< "@" << head->info.nghia[3]
					<< "@" << head->info.nghia[4];
				//các ví d?
				//fo << "@" << head->info.first->info<<"@"<<endl;
				NODEVIDU *vd = head->info.first;
				while (vd != NULL) {
					fo << "@" << vd->info;
					vd = vd->pNext;
				}
				fo << "@" << endl;
				head = head->right;
			}
		}
		fo.close();
	}
}

string fixtext(string t);
void deleteword(hashtable &h, string &s) {
	if (h.head == 0) {
		return;
	}
	NODEWORD *left = 0;
	NODEWORD *cur = h.head;
	while (cur) {
		if (cur->info.tu == s) {
			if (left == 0) {
				if (h.head->right) {
					h.head->right->left = 0;
				}
				h.head = h.head->right;
			}
			else {
				left->right = cur->right;
				if (cur->right)cur->right->left = left;
			}
			delete cur;
			return;
		}
		left = cur;
		cur = cur->right;
	}
}

void deleteWord(hashtable *dictionary) {
	string word;
	cout << "Nhap tu can xoa: ";
	getline(cin, word);
	string fixword = fixtext(word);
	if ('a' <= fixword[0] and fixword[0] <= 'z') {
		deleteword(dictionary[fixword[0] - 'a'], fixword);
		writef(dictionary);
	}
}

string fixtext(string t) {
	if (t != "") {
		while (t[0] == ' ') {
			t.erase(0, 1);//xóa v? trí d?u d?u tiên -- có nghia là xóa kho?ng cách nó dó 
		}
		while (t[t.length() - 1] == ' ') {
			t.erase(t.length() - 1, 1);//xóa v? trí cu?i cùng
		}
		for (short i = 0; i < t.length(); i++) {
			if (t[i] >= 'A'&&t[i] <= 'Z') {
				t[i] += 32;//chuy?n ch? hoa thành ch? thu?ng
			}
		}
		return t;
	}
	else
	{
		return t;
	}
}

void addWord(hashtable  *dictionary) {
	NODEWORD *p = new NODEWORD();
	p->info.first = NULL;

	p->left = NULL;
	p->right = NULL;

	cout << ">>Nhap tu: ";
	getline(cin, p->info.tu);


	cout << "Nhap loai tu: ";
	getline(cin, p->info.loai);

	cout << "Nhap nghia 1: ";
	getline(cin, p->info.nghia[0]);

	char cont;
	cout << ">>Nhap nghia 2 [Y/n]?";
	cin >> cont;
	if (cont == 'Y' or cont == 'y') {
		cout << "  Nhap nghia 2: ";
		cin.ignore();
		getline(cin, p->info.nghia[1]);

		cout << ">>Nhap nghia 3 [Y/n]?";
		cin >> cont;
		if (cont == 'Y' or cont == 'y') {
			cout << "  Nhap nghia 3: ";
			cin.ignore();
			getline(cin, p->info.nghia[2]);
		}
		else {
			p->info.nghia[2] = "";
		}

		cout << ">>Nhap nghia 4 [Y/n]?";
		cin >> cont;
		if (cont == 'Y' or cont == 'y') {
			cout << "  Nhap nghia 4: ";
			cin.ignore();
			getline(cin, p->info.nghia[3]);
		}
		else {
			p->info.nghia[3] = "";
		}

		cout << ">>Nhap nghia 5 [Y/n]?";
		cin >> cont;
		if (cont == 'Y' or cont == 'y') {
			cout << "  Nhap nghia 5: ";
			cin.ignore();
			getline(cin, p->info.nghia[4]);
		}
		else {
			p->info.nghia[4] = "";

		}
	}
	else {
		p->info.nghia[1] = p->info.nghia[2];
		p->info.nghia[2] = p->info.nghia[3];
		p->info.nghia[3] = p->info.nghia[4];
	}

	int count = 0;
	string temp;
	while (true) {
		temp = "";
		cin.ignore(1);
		cout << "Nhap vi du " << ++count << ": ";
		getline(cin, temp);

		if (temp == "")break;
		ChenVdVaoDauDanhSach(p->info.first, temp);
	}
	hashtable &hashtable = dictionary[indexHashtable(p->info.tu)];
	addAscending(hashtable, p);
	writef(dictionary);
}

void editWord(hashtable *dictionary) {
	string searchword;
	cout << "Nhap tu can sua: ";
	getline(cin, searchword);

	//l?y v? trí c?a t?
	int x = indexHashtable(searchword);
	bool flag = false;
	char key;
	if (x < 0) {
		cout << "Khong co tu trong tu dien!";
	}
	else {
		NODEWORD *head = dictionary[x].head;

		while (head != NULL) {
			if (compare(searchword, head)) {//tìm dc t? hi?n th? flag = true
				flag = true;

				//xóa màn hình
				system("cls");

				// show thong tin tu duoc tim thay ra
				cout << "___________________ THONG TIN ______________________________\n";
				printscreen(head);
				cout << "\n___________________________________________________________\n\n";


				// menu thong tin sua tu
				cout << "___________________ SUA TU __________________________";
				cout << "\n\n\n" << "    Loai Tu  [1]          Nghia    [2]      ";
				cout << "\n\n\n" << "    Vi Du    [3]          Thoat    [4] ";
				cout << "\n\n" << "_______________________________________________________";

				int luaChon;
				cout << endl << endl << "\t Lua Chon: ";
				cin >> luaChon;
				cin.ignore();

				//s?a lo?i t?
				if (luaChon == 1) {
					string loaitu;
					cout << "Nhap loai tu: ";
					getline(cin, loaitu);
					head->info.loai = loaitu;
				}
				else if (luaChon == 2) {
					string nghia1, nghia2, nghia3, nghia4, nghia5;
					cout << "Nhap nghia 1: ";
					getline(cin, nghia1);
					head->info.nghia[0] = nghia1;
					char chon;
					cout << endl << "Tiep Tuc Nghia 2: Y|N ?: ";
					cin >> chon;
					cin.ignore();
					if (chon == 'Y' || chon == 'y') {
						cout << endl << "Nhap Nghia 2: ";
						getline(cin, nghia2);
						head->info.nghia[1] = nghia2;

						cout << "Tiep tuc nghia 3: Y|N?";
						cin >> chon;
						cin.ignore();
						if (chon == 'Y' || chon == 'y') {
							cout << endl << "Nhap Nghia 3: ";
							getline(cin, nghia3);
							head->info.nghia[2] = nghia3;

						}

						cout << "Tiep tuc nghia 4: Y|N?";
						cin >> chon;
						cin.ignore();
						if (chon == 'Y' || chon == 'y') {
							cout << endl << "Nhap Nghia 3: ";
							getline(cin, nghia3);
							head->info.nghia[3] = nghia4;

						}

						cout << "Tiep tuc nghia 5: Y|N?";
						cin >> chon;
						cin.ignore();
						if (chon == 'Y' || chon == 'y') {
							cout << endl << "Nhap Nghia 3: ";
							getline(cin, nghia3);
							head->info.nghia[4] = nghia5;
						}
					}
				}
				else if (luaChon == 3) {
					NODEVIDU *vd = head->info.first;
					int count = 0;
					string temp;
					if (vd == NULL) {
						//t? này chua có ví d? nào
						cout << "Them vi du:" << ++count << ": ";
						getline(cin, temp);
						if (temp == "")break;
						ChenVdVaoDauDanhSach(head->info.first, temp);
					}
					else {
						//t? này dã có ít nh?t 1 t?
						while (vd != NULL) {
							cout << "Sua vi du " << ++count << " :";
							getline(cin, temp);
							if (temp == "")break;
							vd->info = temp;
							vd = vd->pNext;
						}
					}
				}
				else {
					//Thoát 
					return;
				}
				writef(dictionary);
				break;
			}
			head = head->right;
		}
		if (flag == false) {
			cout << endl << "Khong co tu " << searchword << " trong tu dien\n\n";
			Sleep(1500);
		}
	}
}

void mainMenu() {
	system("cls");
	cout << "_____________________ MENU _________________________________";
	cout << "\n\n\n" << "         Tim Tu   [1]         Them Tu  [2]";
	cout << "\n\n\n" << "         Xoa Tu   [3]         Sua Tu   [4]";
	cout << "\n\n\n____________________________________________________________";

}

//chèn t? vào d?u danh sách t? di?n
void ChenTuVaoDauDanhSachTuDien(hashtable *&tudien, NODEWORD *p) {
	int i = indexHashtable(p->info.tu);
	//tru?ng h?p t? di?n r?ng
	if (tudien[i].head == NULL) {
		tudien[i].head = tudien[i].tail = p;
	}
	else {
		tudien[i].tail->right = p;
		p->left = tudien[i].tail;
		tudien[i].tail = p;
	}
}

void ChenVdVaoDauDanhSach(NODEVIDU *&first, string vidu) {
	//T?o 1 nút trung gian
	NODEVIDU *p = new NODEVIDU();
	p->info = vidu;
	p->pNext = NULL;
	//tru?ng h?p ds chua có ph?n t? nào
	if (first == NULL) {
		first = p;
	}
	else {
		p->pNext = first;
		first = p;
	}
}

void tachchuoi(string line, NODEWORD *&tmp) {
	int d = 0;
	int i = 0;
	string tach = "";
	while (line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->info.tu = tach;
	tach = "";

	while (line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->info.loai = tach;
	tach = "";

	while (line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->info.nghia[0] = tach;
	tach = "";

	while (line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->info.nghia[1] = tach;
	tach = "";

	while (line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->info.nghia[2] = tach;
	tach = "";

	while (line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->info.nghia[3] = tach;
	tach = "";

	while (line[i] != '@') {
		tach += line[i];
		i++;
	}
	i++;
	tmp->info.nghia[4] = tach;

	tmp->info.first = NULL;
	while (line[i] != NULL) {
		tach = "";
		while (line[i] != '@') {
			tach += line[i];
			i++;
		}
		i++;
		ChenVdVaoDauDanhSach(tmp->info.first, tach);
	}
}

void loading() {
	int i;
	for (i = 0; i < 5; i++)
	{
		cout << i * 25 << "%%%%%";
		Sleep(200);
	}
	Sleep(200);
}

void DrawMainEntrance() {
	system("cls");
	system("color 3F");
	gotoxy(5, 2);
	int choice = 0;

	cout << "ÛÛÛÛÛÛÛÛÛÛÛ  ÛÛÛÛÛÛÛÛÛÛÛ       ÛÛÛÛÛ        ÛÛÛÛÛÛÛÛÛÛ    ÛÛÛÛÛÛÛÛÛÛÛ     "; gotoxy(5, 3);
	cout << "ÛÛÛ     ÛÛÛ  ÛÛÛ              ÛÛÛ ÛÛÛ      ÛÛÛ            ÛÛÛ             "; gotoxy(5, 4);
	cout << "ÛÛÛ     ÛÛÛ  ÛÛÛ             ÛÛÛ   ÛÛÛ     ÛÛÛ            ÛÛÛ             "; gotoxy(5, 5);
	cout << "ÛÛÛÛÛÛÛÛÛÛ   ÛÛÛÛÛÛÛÛÛÛÛ    ÛÛÛ     ÛÛÛ    ÛÛÛ            ÛÛÛÛÛÛÛÛÛÛÛ     "; gotoxy(5, 6);
	cout << "ÛÛÛ          ÛÛÛ           ÛÛÛÛÛÛÛÛÛÛÛÛÛ   ÛÛÛ            ÛÛÛ             "; gotoxy(5, 7);
	cout << "ÛÛÛ          ÛÛÛ          ÛÛÛ         ÛÛÛ  ÛÛÛ            ÛÛÛ             "; gotoxy(5, 8);
	cout << "ÛÛÛ          ÛÛÛÛÛÛÛÛÛÛÛ ÛÛÛ           ÛÛÛ  ÛÛÛÛÛÛÛÛÛÛ    ÛÛÛÛÛÛÛÛÛÛÛ     "; gotoxy(5, 11);


	cout << "**********************************************************************" << endl;
	gotoxy(5, 13);
	cout << "                     ENGLISH - VIETNAMESE DICTIONARY                  " << endl; gotoxy(5, 14);
	printf("                          Tu Dien Anh Viet                       ");
	gotoxy(5, 16);
	cout << "**********************************************************************" << endl; gotoxy(5, 18);
	cout << "\nDang tai du lieu . Vui long cho trong giay lat.\n";


	gotoxy(5, 22);
	loading();
	cout << "\nCo so du lieu tao thanh cong.\n";
	Sleep(1500);
	system("cls");


}

void ThaoTacManHinh();
NODEWORD* InDanhSachTu(hashtable *dictionary, string input, int pos);
NODEWORD* veGiaoDienChinh(hashtable *dictionary, string input, int pos);
void DrawHandleEntrance(hashtable *&dictionary);
void PrintDanhSachTu();
void GiaoDienThemTu();
void GiaoDienSuaTu();
void GiaoDienXoaTu();

void ThaoTacManHinh() {
	system("cls");
	system("color 3F");
	gotoxy(40, 8);
	for (int i = 0; i < 40; i++) {
		cout << char(205);
	}
	gotoxy(40, 8);
	cout << char(201);
	gotoxy(40, 9);
	cout << char(186);
	gotoxy(40, 10);
	cout << char(186);
	gotoxy(44, 10);
	cout << "VUI LONG CHON THAO TAC CAN SU DUNG";
	gotoxy(40, 11);
	cout << char(186);
	gotoxy(40, 12);
	cout << char(186);
	gotoxy(42, 12);
	cout << "1.Xem tat ca danh sach tu";
	gotoxy(40, 13);
	cout << char(186);
	gotoxy(42, 13);
	cout << "2.Them tu vao danh sach tu";
	gotoxy(40, 14);
	cout << char(186);
	gotoxy(42, 14);
	cout << "3.Sua tu vao danh sach tu";
	gotoxy(40, 15);
	cout << char(186);
	gotoxy(42, 15);
	cout << "4.Xoa tu vao danh sach";
	gotoxy(40, 16);
	cout << char(186);
	gotoxy(40, 17);
	cout << char(186);
	gotoxy(40, 18);
	cout << char(186);
	gotoxy(40, 19);
	cout << char(186);
	gotoxy(40, 20);
	cout << char(186);
	gotoxy(40, 20);
	cout << char(200);
	gotoxy(41, 20);
	for (int i = 0; i < 39; i++) {
		cout << char(205);
	}
	gotoxy(80, 20);
	cout << char(188);
	gotoxy(80, 19);
	cout << char(186);
	gotoxy(80, 18);
	cout << char(186);
	gotoxy(80, 17);
	cout << char(186);
	gotoxy(80, 16);
	cout << char(186);
	gotoxy(80, 15);
	cout << char(186);
	gotoxy(80, 14);
	cout << char(186);
	gotoxy(80, 13);
	cout << char(186);
	gotoxy(80, 12);
	cout << char(186);
	gotoxy(80, 11);
	cout << char(186);
	gotoxy(80, 10);
	cout << char(186);
	gotoxy(80, 9);
	cout << char(186);
	gotoxy(80, 8);
	cout << char(187);
	gotoxy(40, 23);
	cout << "Space bar:Quay Lai     \tEnter:Thoat";

	NODEWORD *tuhientai = NULL;
	hashtable *dictionary = new hashtable[26];
	for (int i = 0; i < 26; i++) {
		dictionary[i].tail = NULL;
		dictionary[i].head = NULL;
	}

	// load du lieu 
	readf(dictionary);

	char nChon;
	nChon = _getch();
	if (nChon == '1') {
		PrintDanhSachTu();
	}
	else if (nChon == '2') {
		GiaoDienThemTu();
	}
	else if (nChon == '3') {
		GiaoDienSuaTu();
	}
	else if (nChon == '4') {
		GiaoDienXoaTu();
	}
	else if (nChon == 32) {
		DrawHandleEntrance(dictionary);
	}
	else if (nChon == 13) {
		exit(1);
	}
}

void PrintDanhSachTu() {
	system("cls");
	system("color 3F");
	NODEWORD *tuhientai = NULL;
	hashtable *dictionary = new hashtable[26];
	for (int i = 0; i < 26; i++) {
		dictionary[i].tail = NULL;
		dictionary[i].head = NULL;
	}
	readf(dictionary);
	for (int j = 0; j < 26; ++j) {
		cout << "[" << char(j + 'a') << "] ";
		for (NODEWORD *p = dictionary[j].head; p; p = p->right) {
			cout << p->info.tu << ' ';
		}
		cout << '\n';
	}
	char c;
	c = _getch();
	if (c == 13) {
		ThaoTacManHinh();
	}
}

void GiaoDienThemTu() {
	system("cls");
	system("color 3F");
	NODEWORD *tuhientai = NULL;
	hashtable *dictionary = new hashtable[26];
	for (int i = 0; i < 26; i++) {
		dictionary[i].tail = NULL;
		dictionary[i].head = NULL;
	}
	readf(dictionary);

	addWord(dictionary);

	writef(dictionary);
	cout << "Them tu thanh cong!Quy khach co the vao danh sach tu de kiem tra!";
	Sleep(2000);
	ThaoTacManHinh();
}

void GiaoDienSuaTu() {
	system("cls");
	system("color 3F");
	NODEWORD *tuhientai = NULL;
	hashtable *dictionary = new hashtable[26];
	for (int i = 0; i < 26; i++) {
		dictionary[i].tail = NULL;
		dictionary[i].head = NULL;
	}
	// load du lieu 
	readf(dictionary);

	for (int j = 0; j < 26; ++j) {
		cout << "[" << char(j + 'a') << "] ";
		for (NODEWORD *p = dictionary[j].head; p; p = p->right) {
			cout << p->info.tu << ' ';
		}
		cout << '\n';
	}

	editWord(dictionary);
	ThaoTacManHinh();
}

void GiaoDienXoaTu() {
	system("cls");
	system("color 3F");
	NODEWORD *tuhientai = NULL;
	hashtable *dictionary = new hashtable[26];
	for (int i = 0; i < 26; i++) {
		dictionary[i].tail = NULL;
		dictionary[i].head = NULL;
	}
	readf(dictionary);

	for (int j = 0; j < 26; ++j) {
		cout << "[" << char(j + 'a') << "] ";
		for (NODEWORD *p = dictionary[j].head; p; p = p->right) {
			cout << p->info.tu << ' ';
		}
		cout << '\n';
	}

	deleteWord(dictionary);
	cout << "Da xoa thanh cong!Vui long nhan phim enter de quay lai man hinh thao tac. Cam on!";
	char c;
	c = _getch();
	if (c == 13) {
		ThaoTacManHinh();
	}
}

NODEWORD* InDanhSachTu(hashtable *dictionary, string input, int pos) {

	//in ra ds t?
	NODEWORD *tuhientai = NULL;
	int index = -1;
	if (input != "") {
		index = indexHashtable(input);
	}
	int count = 0;
	for (int i = 0; i < 26; i++) {
		if (dictionary[i].head == NULL)continue;//t?p t? di?n không có t? nào , b? qua
		NODEWORD *p = dictionary[i].head;
		if (index != -1) {
			p = search(dictionary, input);
		}
		while (true) {
			while (pos > 0 && p != NULL) {
				pos--;
				p = p->right;
			}
			if (p == NULL)break;
			if (count == 0) {
				tuhientai = p;
				textcolor(207);
			}
			else {
				textcolor(62);
			}
			gotoxy(9, 9 + count); cout << p->info.tu;
			for (int j = 0; j < 39 - p->info.tu.size(); j++) {
				cout << " ";
			}
			count++;
			p = p->right;
			if (p == NULL) {
				break;
			}
		}
		if (index != -1)break;
		if (count >= 15)break;//ko du?c l?n hon t? l?n hon khung
	}
	textcolor(15);
	return tuhientai;
}

NODEWORD* veGiaoDienChinh(hashtable *dictionary, string input, int pos) {
	system("cls");
	textcolor(63);
	int dongHienTai = 0;

	// ve khung tim kiem
	gotoxy(8, 6); cout << char(201);
	for (int i = 1; i < 41; i++) cout << char(205);
	cout << char(187);
	gotoxy(8, 7); cout << char(186) << " Tim kiem:";
	gotoxy(21, 7); cout << input;
	gotoxy(49, 7); cout << char(186);
	gotoxy(8, 8); cout << char(204);
	for (int i = 1; i < 41; i++) {
		cout << char(205);
	}
	cout << char(185);
	for (int i = 9; i < 23; i++) {
		gotoxy(8, i);
		cout << char(186);
		gotoxy(49, i);
		cout << char(186);
	}
	gotoxy(8, 23); cout << char(200);
	for (int i = 1; i < 41; i++) cout << char(205);
	cout << char(188);

	//v? khung n?i dung
	gotoxy(55, 12); cout << "=";
	for (int i = 1; i < 8; i++) {
		cout << "=";
	}
	cout << ">";
	gotoxy(70, 6);
	for (int i = 1; i < 50; i++) {
		cout << char(205);
	}
	gotoxy(70, 6); cout << char(201);
	gotoxy(70, 7); cout << char(186);
	gotoxy(87, 7); cout << "Ket qua tim kiem";
	gotoxy(70, 8); cout << char(186);
	for (int i = 1; i < 49; i++) {
		cout << char(205);
	}
	gotoxy(70, 9); cout << char(200);
	gotoxy(70, 8); cout << char(204);

	gotoxy(119, 8); cout << char(186);
	gotoxy(119, 7); cout << char(186);
	gotoxy(119, 6); cout << char(187);
	gotoxy(70, 9); cout << char(186);
	for (int i = 9; i < 23; i++) {
		gotoxy(70, i); cout << char(186);
	}
	gotoxy(70, 22); cout << char(186);
	gotoxy(70, 23); cout << char(200);
	for (int i = 1; i < 49; i++) {
		cout << char(205);
	}
	gotoxy(119, 8); cout << char(185);
	gotoxy(119, 23); cout << char(188);
	for (int i = 22; i >= 9; i--) {
		gotoxy(119, i); cout << char(186);

	}
	gotoxy(8, 26); cout << "Esc:Thoat\t \t    Tab:Thao tac";
	gotoxy(21, 7);

	NODEWORD *tuHienTai = InDanhSachTu(dictionary, input, pos);

	// neu dang o man hinh chinh,
	// dua con tro nhap nhay ve lai khung tim kiem
	gotoxy(21 + input.size(), 7);
	return tuHienTai;
}

void DrawHandleEntrance(hashtable *&dictionary) {
	NODEWORD *tuHienTai = NULL;
	string input = "";
	int keyCode = 0;
	int pos = 0;
	int soLuongTu = demSoTu(dictionary);
	while (true) {
		textcolor(63);
		tuHienTai = veGiaoDienChinh(dictionary, input, pos);
		keyCode = _getch();
		switch (keyCode)
		{
		case 8://backspacce
			input = input.substr(0, input.size() - 1); // xoa ky tu cuoi
			break;
		case 9://Ch?n Thao tác là tab
			ThaoTacManHinh();
			break;
		case 13://enter vào màn hinh t? chi ti?t
			veGiaoDienChiTietTu(tuHienTai);
			_getch();
			/*		Sleep(2500);*/
			break;
		case 27://esc thoát chuong trình
			exit(1);
			break;
		case 224: {
			int key = _getch();
			textcolor(63);
			if (key == 72 && pos > 0) { pos--; } // UP
			if (key == 80 && pos < min(soLuongTu, 14) - 1) { pos++; } // DOWN
			if (pos < 0) pos = 0;
			if (pos > soLuongTu - 1) pos = soLuongTu - 1;	// de khi di chuyen xuong cuoi ds con 1 tu
			break;
		}
		default:
			if (keyCode >= 97 && keyCode <= 122) {
				input += char(keyCode);
			}
		}
	}
}
int main() {
	NODEWORD *tuhientai = NULL;
	hashtable *dictionary = new hashtable[26];
	for (int i = 0; i < 26; i++) {
		dictionary[i].tail = NULL;
		dictionary[i].head = NULL;
	}
	readf(dictionary);

	DrawMainEntrance();
	DrawHandleEntrance(dictionary);
	system("pause");
}
