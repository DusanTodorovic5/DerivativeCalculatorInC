#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct tnode {
	char info;
	struct tnode* left;
	struct tnode* right;
	struct tnode* izvod;
} tnode;
typedef struct stnode {
	struct tnode* info;
	struct stnode* next;
} stnode;
typedef struct snode {
	char info;
	struct snode* next;
} snode;
typedef struct dnode {
	double* vrednost;
	struct dnode* next;
} dnode;
typedef struct szaglavlje {
	struct snode* head;
} szaglavlje;
typedef struct stzaglavlje {
	struct stnode* head;
} stzaglavlje;
typedef struct dzaglavlje {
	struct dnode* head;
}dzaglavlje;
typedef struct Tablica {
	double vrednost;
	char znak;
} Tablica;
void pusht(stzaglavlje*, stnode*);
void pushtn(stzaglavlje*, tnode*);
tnode* popt(stzaglavlje*);
void pushs(szaglavlje*, char);
char pops(szaglavlje*);
void napraviTree(stzaglavlje*, stzaglavlje*);
int prioritet(char);
stnode* newNode(char);
tnode* newNodeT(char);
void printPrefix(tnode*);
tnode* GetStablo();
void PrintDrvo(tnode*);
void pushd(dzaglavlje*, double);
double popd(dzaglavlje*);
double racunaj(tnode*, Tablica*);
Tablica* unosVrednosti(Tablica*);
void freeStablo(tnode*);
tnode* izvod(tnode*, char);
tnode* kopiraj(tnode*);
void stampajTekst();
int main() {
	tnode* stablo = NULL;
	Tablica tab[26];
	for (int i = 0; i < 26; i++) {
		tab[i].vrednost = (double)(i + 1);
		tab[i].znak = 'A' + i;
	}
	int izbor = 0;
	stampajTekst();
	char zaizvod = 'A';
	while (1) {
		printf("Unesi redni broj komande koju zelite pokrenuti :  ");
		scanf("%d", &izbor);
		printf("\n\n");
		if (izbor == 0) break;
		switch (izbor) {
		case 1: if (stablo != NULL) {
			freeStablo(stablo);
		}
			  stablo = GetStablo(); break;
		case 2:if (stablo != NULL) { printf("Prefix : "); printPrefix(stablo); 	printf("\n\n"); }
			  else printf("Prvo unesite stablo!!\n\n");
			break;
		case 3: if (stablo != NULL) {
			printf("\n\n");	PrintDrvo(stablo); printf("\n\n");
		}
			  else printf("Prvo unesite stablo!!\n\n");
			break;
		case 4: if (stablo != NULL) {
			unosVrednosti(tab);
		}
			  else printf("Prvo unesite stablo!!\n\n");
			break;
		case 5: if (stablo != NULL) {
			if (stablo != NULL) printf("\n\nVrednost izraza je : %lf\n\n", racunaj(stablo, tab));
		}
			  else printf("Prvo unesite stablo!!\n\n");
			break;
		case 6: if (stablo != NULL) {
			printf("Unesite promenjivu po kojoj diferencirate : ");
			scanf(" %c", &zaizvod); stablo = izvod(stablo, zaizvod, tab);
			printf("\n\n");	PrintDrvo(stablo); printf("\n\n");
		}
			  else printf("Prvo unesite stablo!!\n\n");
			break;
		case 7: stampajTekst(); break;
		default: printf("\n\n  UNESITE KOREKTAN REDNI BROJ NAREDBE!!  \n\n"); break;
		}
	}
	if (stablo != NULL) {
		freeStablo(stablo);
	}
	return 0;
}
void pushs(szaglavlje* stek, char znak) {
	snode* n = (snode*)calloc(1, sizeof(snode));
	n->info = znak;
	n->next = stek->head;
	stek->head = n;
}
char pops(szaglavlje* stek) {
	char k = stek->head->info;
	snode* rez = stek->head;
	stek->head = stek->head->next;
	free(rez);
	return k;
}
void pusht(stzaglavlje* stablo, stnode* n) {
	n->next = stablo->head;
	stablo->head = n;
}
void pushtn(stzaglavlje* stek, tnode* n) {
	stnode* n1 = (stnode*)calloc(1, sizeof(stnode));
	n1->info = n;
	n1->next = stek->head;
	stek->head = n1;
}
tnode* popt(stzaglavlje* stablo) {
	if (stablo->head == NULL) {
		printf("Underflow");
		exit(-1);
	}
	tnode* rez;
	rez = stablo->head->info;
	stnode* rez1 = stablo->head;
	stablo->head = stablo->head->next;
	return rez;
}
stnode* newNode(char znak) {
	stnode* n = (stnode*)calloc(1, sizeof(stnode));
	n->info = (tnode*)calloc(1, sizeof(tnode));
	n->info->right = NULL;
	n->info->left = NULL;
	n->info->info = znak;
	n->info->izvod = NULL;
	return n;
}

int prioritet(char znak) { // +- 1; */ 2; ^4;(10
	if (znak == '+' || znak == '-') return 1;
	if (znak == '*' || znak == '-') return 2;
	if (znak == '^') return 4;
	else return 10;
}
void napraviTree(stzaglavlje* oper, stzaglavlje* opr) {
	stnode* rez = (stnode*)calloc(1, sizeof(stnode));
	rez->info = (tnode*)calloc(1, sizeof(tnode));
	rez->info = popt(opr);
	rez->info->left = popt(oper);
	rez->info->right = popt(oper);
	pusht(oper, rez);
}
tnode* GetStablo() {
	printf("Unesi izraz ->   ");
	stzaglavlje stekopr, stekoper;
	szaglavlje chars;
	stekoper.head = NULL; stekopr.head = NULL; chars.head = NULL;
	char* izraz, ch;
	izraz = (char*)malloc(sizeof(char));
	*izraz = '\0';
	int n = 0;
	ch = getchar();
	while ((ch = getchar()) != '\n') {
		izraz[n] = ch;
		n++;
		izraz = (char*)realloc(izraz, (n + 1) * (sizeof(char)));
	}
	izraz[n] = '\0';
	for (int i = 0; i < n; i++) {
		pushs(&chars, izraz[i]);
	}
	while (chars.head != NULL) {
		char rez = pops(&chars);
		if (rez >= 'A' && rez <= 'Z') pusht(&stekoper, newNode(rez));
		if (rez >= '0' && rez <= '9') pusht(&stekoper, newNode(rez));
		if (rez == ')') pusht(&stekopr, newNode(rez));
		if (rez == '-' && chars.head->info == '(') {
			stnode* tt = newNode(rez);
			tt->info->left = popt(&stekoper);
			pusht(&stekoper, tt);
			rez = pops(&chars);
			stnode* ttt = popt(&stekopr);
			continue;
		}
		if (rez == ',') {
			while (stekopr.head->info->info != ')') {
				napraviTree(&stekoper, &stekopr);
			}
			tnode* k = popt(&stekopr);
			pusht(&stekopr, newNode(')'));
			continue;
		}
		if (chars.head != NULL && rez == '(') {
			if (chars.head->info == 'a' || chars.head->info == 'e') {
				while (stekopr.head->info->info != ')') {
					napraviTree(&stekoper, &stekopr);
				}
				tnode* k = popt(&stekopr);
				stnode* tt = newNode(pops(&chars));
				tt->info->left = popt(&stekoper);
				tt->info->right = NULL;
				pusht(&stekoper, tt);
				continue;
			}
			if (chars.head->info == 'l') {
				while (stekopr.head->info->info != ')') {
					napraviTree(&stekoper, &stekopr);
				}
				char c = popt(&stekopr)->info;
				stnode* tt = newNode(pops(&chars));
				tt->info->left = popt(&stekoper);
				tt->info->right = popt(&stekoper);
				pusht(&stekoper, tt);
				continue;
			}
		}
		if (rez == '+' || rez == '-' || rez == '/' || rez == '*' || rez == '^') {
			int p = 0;
			while (!p) {
				if (stekopr.head == NULL || stekopr.head->info->info == ')' ||
					((prioritet(rez) >= prioritet(stekopr.head->info->info)) && rez != '^') || (prioritet(rez) > prioritet(stekopr.head->info->info) && rez == '^')) {
					pusht(&stekopr, newNode(rez)); p = 1;
				}
				else {
					napraviTree(&stekoper, &stekopr);
				}
			}
		}
		if (rez == '(') {
			while (stekopr.head->info->info != ')') {
				napraviTree(&stekoper, &stekopr);
			}
			tnode* k = popt(&stekopr);
		}
	}
	free(izraz);
	while (stekoper.head->next != NULL) napraviTree(&stekoper, &stekopr);
	free(stekopr.head);
	return stekoper.head->info;
}
void printPrefix(tnode* p) {
	if (p == NULL)
		return;
	stzaglavlje stek;
	stek.head = NULL;
	pushtn(&stek, p);
	while (stek.head != NULL) {
		tnode* rez = popt(&stek);
		printf("%c ", rez->info);
		if (rez->right != NULL) pushtn(&stek, rez->right);
		if (rez->left != NULL) pushtn(&stek, rez->left);
	}
}
void fooPrint(char znak, int level) {
	if (level == 0) printf("%c%c %c\n", 201, 175, znak);
	else if (level == 1)printf("%c%c%c %c\n", 204, 205, 175, znak);
	else {
		printf("%c", 186);
		for (int i = 0; i < level - 1; i++) printf(" ");
		printf("%c", 204);
		printf("%c %c\n", 175, znak);
	}
}
void PrintDrvo(tnode* stablo) {
	if (stablo == NULL)
		return;
	tnode* p = stablo;
	stzaglavlje stek;
	stek.head = NULL;
	//pushtn(&stek, p);
	int level = 0;
	printf("\n#####    Horizontalni prikaz, gore je levi sin, a dole je desni sin     #####\n\n");
	do {
		while (p != NULL) {
			fooPrint(p->info, level);
			pushtn(&stek, p); level++;
			p = p->left;
		}
		p = popt(&stek); level--;
		if (p != NULL) {
			if (p->right != NULL) {
				pushtn(&stek, NULL);
				p = p->right;
				level++;
			}
			else p = NULL;
		}
	} while (stek.head != NULL);
	printf("%c\n\n", 188);
}
Tablica* unosVrednosti(Tablica* tab) {
	printf("Koliko vrednosti zelis uneti: ");
	int n = 0;
	scanf("%d", &n);
	char c; double t;
	for (int i = 0; i < n; i++) {
		printf("Koju promenjivu zelite uneti? :  ");
		scanf(" %c", &c);
		printf("Koju vrednost zelite uneti?  :   ");
		scanf("%lf", &t);
		for (int j = 0; j < 26; j++) {
			if (c == tab[j].znak) {
				tab[j].vrednost = t;
			}
		}
	}
	return tab;
}
double conv(char c, Tablica* tab) {
	if (c >= '0' && c <= '9')
		return (double)(c - '0');
	for (int i = 0; i < 26; i++) {
		if (c == tab[i].znak) return tab[i].vrednost;
	}
	return 1;
}
double izrbin(char znak, double prvi, double drugi) {
	double res = 0;
	switch (znak) {
	case '+': res = prvi + drugi; break;
	case '-': res = prvi - drugi; break;
	case '*': res = prvi * drugi; break;
	case '/': res = prvi / drugi; break;
	case 'l': res = (log(prvi) / log(drugi)); break;
	case '^': res = pow(prvi, drugi); break;
	}
	return res;
}
double izrun(char znak, double vred) {
	switch (znak) {
	case '-': return vred * (-1);
	case 'e': return exp(vred);
	case 'a': return fabs(vred);
	}
}
void pushd(dzaglavlje* stek, double vrednost) {
	dnode* n = (dnode*)calloc(1, sizeof(dnode));
	n->vrednost = (double*)calloc(1, sizeof(double));
	*(n->vrednost) = vrednost;
	n->next = stek->head;
	stek->head = n;
}
double popd(dzaglavlje* stek) {
	double k = *(stek->head->vrednost);
	dnode* rez = stek->head;
	stek->head = stek->head->next;
	//free(rez);
	return k;
}
double racunaj(tnode* stablo, Tablica* tab) {
	if (stablo == NULL)
		return 0;
	tnode* p = stablo;
	stzaglavlje stek;
	stek.head = NULL;
	dzaglavlje dstek;
	stek.head = NULL;
	do {
		while (p != NULL) {
			if (p->right != NULL)
				pushtn(&stek, p->right);
			pushtn(&stek, p);
			p = p->left;
		}
		p = popt(&stek);
		if (stek.head != NULL && stek.head->info == p->right) {
			tnode* t = popt(&stek);
			pushtn(&stek, p);
			p = p->right;
		}
		else { //racunaj
			if ((p->info >= 'A' && p->info <= 'Z') || (p->info >= '0' && p->info <= '9'))
				pushd(&dstek, conv(p->info, tab));
			else if (p->info != 'e' && p->info != 'a') {
				if (p->info == '-' && p->right == NULL) {
					pushd(&dstek, izrun(p->info, popd(&dstek)));
				}
				else {
					double drugi = popd(&dstek);
					double prvi = popd(&dstek);
					pushd(&dstek, izrbin(p->info, prvi, drugi));
				}
			}
			else if (p->info == 'e' || p->info == 'a') {
				pushd(&dstek, izrun(p->info, popd(&dstek)));
			}
			p = NULL;
		}
	} while (stek.head != NULL);
	printf("\n");
	double t = popd(&dstek);
	return t;
}
tnode* newNodeT(char c) {
	tnode* t = (tnode*)calloc(1, sizeof(tnode));
	t->info = c;
	t->left = NULL;
	t->right = NULL;
	t->izvod = NULL;
	return t;
}
tnode* izvod(tnode* stablo, char znak, Tablica* tab) {
	if (stablo == NULL)
		return 0;
	tnode* p = stablo;
	stzaglavlje stek;
	stek.head = NULL;
	do {
		while (p != NULL) {
			if (p->right != NULL)
				pushtn(&stek, p->right);
			pushtn(&stek, p);
			p = p->left;
		}
		p = popt(&stek);
		if (stek.head != NULL && stek.head->info == p->right) {
			tnode* t = popt(&stek);
			pushtn(&stek, p);
			p = p->right;
		}
		else { //racunaj
			if ((p->info >= 'A' && p->info <= 'Z') || p->info >= '0' && p->info <= '9') {
				if (p->info == znak) {
					p->izvod = newNodeT('1');
					p->izvod->left = NULL;
					p->izvod->right = NULL;
				}
				else {
					p->izvod = newNodeT('0');
					p->izvod->left = NULL;
					p->izvod->right = NULL;
				}
			}
			else {
				if (p->info == '+') {
					if (p->left->izvod->info == '0' && p->right->izvod->info == '0') {
						p->izvod = newNodeT('0');
					}
					else if (p->left->izvod->info == '0') {
						p->izvod = kopiraj(p->right->izvod);
					}
					else if (p->right->izvod->info == '0') {
						p->izvod = kopiraj(p->left->izvod);
					}
					else {
						p->izvod = newNodeT('+');
						p->izvod->right = kopiraj(p->right->izvod);
						p->izvod->left = kopiraj(p->left->izvod);
					}
				}
				else if (p->info == '-') {
					if (p->right == NULL) {
						p->izvod = newNodeT('-');
						p->izvod->left = kopiraj(p->left->izvod);
						p->izvod->right = NULL;
					}
					else {
						if (p->left->izvod->info == '0' && p->right->izvod->info == '0') {
							p->izvod = newNodeT('0');
							p->izvod->left = NULL;
							p->izvod->right = NULL;
						}
						else if (p->left->izvod->info == '0') {
							p->izvod = newNodeT('-');
							p->izvod->right = NULL;
							p->izvod->left = kopiraj(p->right->izvod);
						}
						else if (p->right->izvod->info == '0') {
							p->izvod = kopiraj(p->left->izvod);
						}
						else {
							p->izvod = newNodeT('-');
							p->izvod->right = kopiraj(p->right->izvod);
							p->izvod->left = kopiraj(p->left->izvod);
						}
					}
				}
				else if (p->info == '*') {
					if (p->left->izvod->info == '0' && p->right->izvod->info == '0') {
						p->izvod = newNodeT('0');
						p->izvod->left = NULL;
						p->izvod->right = NULL;
					}
					else if (p->left->izvod->info == '0') {
						p->izvod = newNodeT('*');
						p->izvod->left = kopiraj(p->right->izvod);
						p->izvod->right = kopiraj(p->left);
					}
					else if (p->right->izvod->info == '0') {
						p->izvod = newNodeT('*');
						p->izvod->left = kopiraj(p->right);
						p->izvod->right = kopiraj(p->left->izvod);
					}
					else {
						p->izvod = newNodeT('+');
						p->izvod->left = newNodeT('*');
						p->izvod->right = newNodeT('*');
						p->izvod->left->left = kopiraj(p->right);
						p->izvod->left->right = kopiraj(p->left->izvod);
						p->izvod->right->left = kopiraj(p->left);
						p->izvod->right->right = kopiraj(p->right->izvod);
					}
				}
				else if (p->info == '/') {
					if (p->left->izvod->info == '0' && p->right->izvod->info == '0')
						p->izvod = newNodeT('0');
					else if (p->left->izvod->info == '0') {
						p->izvod = newNodeT('/');
						p->izvod->right = newNodeT('^');
						p->izvod->right->left = kopiraj(p->right);
						p->izvod->right->right = newNodeT('2');
						p->izvod->left = newNodeT('-');
						p->izvod->left->left = newNodeT('*');
						p->izvod->left->left->left = kopiraj(p->left);
						p->izvod->left->left->right = kopiraj(p->right->izvod);
					}
					else if (p->right->izvod->info == '0') {
						p->izvod = newNodeT('/');
						p->izvod->right = newNodeT('^');
						p->izvod->right->left = kopiraj(p->right);
						p->izvod->right->right = newNodeT('2');
						p->izvod->left = newNodeT('*');
						p->izvod->left->left = kopiraj(p->right);
						p->izvod->left->right = kopiraj(p->left->izvod);
					}
					else {
						p->izvod = newNodeT('/');
						p->izvod->right = newNodeT('^');
						p->izvod->right->left = kopiraj(p->right);
						p->izvod->right->right = newNodeT('2');
						p->izvod->left = newNodeT('-');
						p->izvod->left->left = newNodeT('*');
						p->izvod->left->right = newNodeT('*');
						p->izvod->left->left->right = kopiraj(p->right);
						p->izvod->left->left->left = kopiraj(p->left->izvod);
						p->izvod->left->right->right = kopiraj(p->right->izvod);
						p->izvod->left->right->left = kopiraj(p->left);
					}
				}
				else if (p->info == '^') {
					if (p->left->izvod->info == '0' && p->right->izvod->info == '0')
						p->izvod = newNodeT('0');
					else if (p->left->izvod->info == '0') {
						if (p->right->izvod->info == '1') {
							p->izvod = newNodeT('*');
							p->izvod->left = newNodeT('^');
							p->izvod->left->left = kopiraj(p->left);
							p->izvod->left->right = kopiraj(p->right);
							p->izvod->right = newNodeT('l');
							p->izvod->right->left = kopiraj(p->left);
							p->izvod->right->right = newNodeT('e');
							p->izvod->right->right->left = newNodeT('1');
							p->izvod->right->right->right = NULL;
						}
						else {
							p->izvod = newNodeT('*');
							p->izvod->left = izvod(p->right, znak, tab);
							p->izvod->right = newNodeT('*');
							p->izvod->right->left = newNodeT('^');
							p->izvod->right->left->left = kopiraj(p->left);
							p->izvod->right->left->right = kopiraj(p->right);
							p->izvod->right->right = newNodeT('l');
							p->izvod->right->right->left = kopiraj(p->left);
							p->izvod->right->right->right = newNodeT('e');
							p->izvod->right->right->right->left = newNodeT('1');
							p->izvod->right->right->right->right = NULL;
						}
					}
					else {
						p->izvod = newNodeT('*');
						p->izvod->left = kopiraj(p->right);
						p->izvod->right = newNodeT('^');
						p->izvod->right->left = kopiraj(p->left);
						p->izvod->right->right = newNodeT('-');
						p->izvod->right->right->left = kopiraj(p->right);
						p->izvod->right->right->right = newNodeT('1');
					}
				}
				else if (p->info == 'e') {
					if (p->left->izvod->info == '1') {
						p->izvod = kopiraj(p);
					}
					else if (p->left->izvod->info == '0') {
						p->izvod = newNodeT('0');
					}
					else {
						p->izvod = newNodeT('*');
						p->izvod->right = kopiraj(p);
						p->izvod->left = izvod(p->left, znak, tab);
					}
				}
				else if (p->info == 'l') {
					if (p->left->izvod->info == '0' && p->right->izvod->info == '0') {
						p->izvod = newNodeT('0');
					}
					else if (p->left->izvod->info == '1' && p->right->izvod->info == '1') {
						p->izvod = newNodeT('1');
					}
					else if (p->right->izvod->info == '0') {
						if (p->left->izvod->info == '1') {
							p->izvod = newNodeT('/');
							p->izvod->left = newNodeT('1');
							p->izvod->right = newNodeT('*');
							p->izvod->right->left = kopiraj(p->left);
							p->izvod->right->right = newNodeT('l');
							p->izvod->right->right->left = kopiraj(p->right);
							p->izvod->right->right->right = newNodeT('e');
							p->izvod->right->right->right->left = newNodeT('1');
						}
						else {
							p->izvod = newNodeT('/');
							p->izvod->left = izvod(p->left, znak, tab);
							p->izvod->right = newNodeT('*');
							p->izvod->right->left = kopiraj(p->left);
							p->izvod->right->right = newNodeT('l');
							p->izvod->right->right->left = kopiraj(p->right);
							p->izvod->right->right->right = newNodeT('e');
							p->izvod->right->right->right->left = newNodeT('1');
						}
					}
					else if (p->left->izvod->info == '0') {
						if (p->right->izvod->info == '1') {
							p->izvod = newNodeT('-');
							p->izvod->right = NULL;
							p->izvod->left = newNodeT('/');
							p->izvod->left->left = newNodeT('l');
							p->izvod->left->left->left = kopiraj(p->left);
							p->izvod->left->left->right = newNodeT('e');
							p->izvod->left->left->right->left = newNodeT('1');
							p->izvod->left->right = newNodeT('*');
							p->izvod->left->right->left = p->right;
							p->izvod->left->right->right = newNodeT('^');
							p->izvod->left->right->right->left = newNodeT('l');
							p->izvod->left->right->right->left->left = kopiraj(p->right);
							p->izvod->left->right->right->left->right = newNodeT('e');
							p->izvod->left->right->right->left->right->left = newNodeT('1');
							p->izvod->left->right->right->left->right->right = NULL;
							p->izvod->left->right->right->right = newNodeT('2');
						}
						else {
							p->izvod = newNodeT('-');
							p->izvod->right = NULL;
							p->izvod->left = newNodeT('/');
							p->izvod->left->left = newNodeT('*');
							p->izvod->left->left->right = izvod(p->right, znak, tab);
							p->izvod->left->left->left = newNodeT('l');
							p->izvod->left->left->left->left = kopiraj(p->left);
							p->izvod->left->left->left->right = newNodeT('e');
							p->izvod->left->left->left->right->left = newNodeT('1');
							p->izvod->left->right = newNodeT('*');
							p->izvod->left->right->left = kopiraj(p->right);
							p->izvod->left->right->right = newNodeT('^');
							p->izvod->left->right->right->left = newNodeT('l');
							p->izvod->left->right->right->left->left = kopiraj(p->right);
							p->izvod->left->right->right->left->right = newNodeT('e');
							p->izvod->left->right->right->left->right->left = newNodeT('1');
							p->izvod->left->right->right->left->right->right = NULL;
							p->izvod->left->right->right->right = newNodeT('2');
						}
					}
					else {
						tnode* t = newNodeT('/');
						t->left = newNodeT('l');
						t->left->left = kopiraj(p->left);
						t->left->right = newNodeT('e');
						t->left->right->left = newNodeT('1');
						t->right = newNodeT('l');
						t->right->left = kopiraj(p->right);
						t->right->right = newNodeT('e');
						t->right->right->left = newNodeT('1');
						p->izvod = izvod(t, znak, tab);
						freeStablo(t);
					}
				}
				else if (p->info == 'a') {
					if (p->left->izvod->info == '0') {
						p->izvod = newNodeT('0');
					}
					else {
						double t = racunaj(p->left, tab);
						if (t < 0) {
							p->izvod = newNodeT('-');
							p->izvod->right = NULL;
							if (p->left->izvod->info == '1') {
								p->izvod->left = newNodeT('1');
							}
							else {
								p->izvod->left = izvod(p->left, znak, tab);
							}
						}
						else {
							if (p->left->izvod->info == '1') {
								p->izvod = newNodeT('1');
								p->izvod->right = NULL;
							}
							else {
								p->izvod = kopiraj(p->left->izvod);
							}
						}
					}
				}
			}
			p = NULL;
		}
	} while (stek.head != NULL);
	tnode* t = stablo->izvod;
	//freeStablo(stablo);
	return t;
}
void brisi(tnode** nod)
{
	(*nod)->left = NULL;
	(*nod)->right = NULL;
	free(*nod);
	*nod = NULL;
	nod = NULL;
}

void obrisi(tnode** nod)
{
	brisi(nod);
	*nod = NULL;
	nod = NULL;
}
void freeStablo(tnode* stablo) {
	if (stablo == NULL)
		return;
	tnode* p = stablo;
	stzaglavlje stek;
	stek.head = NULL;
	do {
		while (p != NULL) {
			if (p->right != NULL)
				pushtn(&stek, p->right);
			pushtn(&stek, p);
			p = p->left;
		}
		p = popt(&stek);
		if (stek.head != NULL && stek.head->info == p->right) {
			p->left = NULL;
			p->right = NULL;
			tnode* t = popt(&stek);
			pushtn(&stek, p);
			p = t;
		}
		else {
			if (p != NULL) {
				p->left = NULL;
				p->right = NULL;
				obrisi(&p);
			}
			p = NULL;
		}
	} while (stek.head != NULL);
}
tnode* kopiraj(tnode* nod) {
	if (nod == NULL)
		return NULL;
	tnode* p = nod;
	tnode* t = NULL;
	stzaglavlje stek;
	stzaglavlje stekfr;
	stekfr.head = NULL;
	stek.head = NULL;
	do {
		while (p != NULL) {
			if (p->right != NULL)
				pushtn(&stek, p->right);
			pushtn(&stek, p);
			p = p->left;
		}
		p = popt(&stek);
		if (stek.head != NULL && stek.head->info == p->right) {
			tnode* t = popt(&stek);
			pushtn(&stek, p);
			p = p->right;
		}
		else {
			if (p->left == NULL && p->right == NULL) {
				t = newNodeT(p->info);
				t->left = NULL;
				t->right = NULL;
				pushtn(&stekfr, t);
			}
			else if (p->right == NULL) {
				t = newNodeT(p->info);
				t->left = popt(&stekfr);
				t->right = NULL;
				pushtn(&stekfr, t);
			}
			else {
				t = newNodeT(p->info);
				t->right = popt(&stekfr);
				t->left = popt(&stekfr);
				if (t->info == '*' && t->left->info == '1' && t->right->info == '1') {
					t = newNodeT('1');
				}
				else if (t->info == '*' && t->left->info == '1') {
					t = t->right;
				}
				else if (t->info == '*' && t->right->info == '1') {
					t = t->left;
				}
				pushtn(&stekfr, t);
			}
			p = NULL;
		}
	} while (stek.head != NULL);
	t = popt(&stekfr);
	return t;
}
void stampajTekst() {
	printf("###########################################################\n");
	printf("#            Izaberi opciju                               #\n");
	printf("#                                                         #\n");
	printf("#  1 -> Napravi stablo iz infiksa                         #\n");
	printf("#  2 -> Odstampaj prefix izraz                            #\n");
	printf("#  3 -> Odstampaj stablo                                  #\n");
	printf("#  4 -> Unesi vrednosti promenjivih                       #\n");
	printf("#  5 -> Izracunaj izraz                                   #\n");
	printf("#  6 -> Nadji izvod funkcije                              #\n");
	printf("#  7 -> Ponovo odstampaj meni                             #\n");
	printf("#  0 -> Izlaz                                             #\n");
	printf("#                                                         #\n");
	printf("###########################################################\n");
	printf("#                                                         #\n");
	printf("#   Logaritam A sa osnovom B se zapisuje l(A,B)           #\n");
	printf("#   Exponent od A se zapisuje e(A)                        #\n");
	printf("#   Apsolutna vrednost od A se zapisuje a(A)              #\n");
	printf("#   !!Program ne moze diferencirati X^X!!                 #\n");
	printf("#                                                         #\n");
	printf("#   Ako se ne unese vrednost promenjivih, one             #\n");
	printf("#   dobijaju vrednost rednog broja u abecedi              #\n");
	printf("#                                                         #\n");
	printf("###########################################################\n\n");
}
