#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define PUSTE '='
#define WCIECIE 4

struct wezel {
    int w;
    struct wezel *lsyn;
    struct wezel *psyn;
    struct wezel *nast;
};
typedef struct wezel Twezel;

Twezel *nowy(int w, Twezel *lsyn, Twezel *psyn) {
    Twezel *wynik = malloc(sizeof *wynik);
    assert(wynik != NULL);
    wynik->w = w;
    wynik->lsyn = lsyn;
    wynik->psyn = psyn;
    return wynik;
}

int nastepny(void) {
    int c;
    do {
        c = getchar();
    } while (isspace(c));
    return c;
}

Twezel *czytaj(void) {
    int c = nastepny();
    if (c == PUSTE) {
        return NULL;
    } else {
        assert(c != EOF);
        ungetc(c, stdin);
        int w;
        int r = scanf_s("%d", &w);
        assert(r == 1);
        Twezel *l = czytaj();
        Twezel *p = czytaj();
        return nowy(w, l, p);
    }
}

void pisz(Twezel *d, int s) {
    printf("%*s", s, "");
    if (d == NULL) {
        printf("%c\n", PUSTE);
    } else {
        printf("%d\n", d->w);
        pisz(d->lsyn, s + WCIECIE);
        pisz(d->psyn, s + WCIECIE);
    }
}

void usun(Twezel *d) {
    if (d != NULL) {
        usun(d->lsyn);
        usun(d->psyn);
        free(d);
    }
}

struct listaw {
    int w;
    struct listaw *nast;
};

typedef struct listaw Tlistaw;

Tlistaw *newNodew(int w) {
    Tlistaw *stackNode = (Tlistaw *) malloc(sizeof(Tlistaw));
    stackNode->w = w;
    stackNode->nast = NULL;
    return stackNode;
}

struct lista {
    Twezel *d;
    struct lista *nast;
};

typedef struct lista Tlista;

Tlista *newNode(Twezel *d) {
    Tlista *stackNode = (Tlista *) malloc(sizeof(Tlista));
    stackNode->d = d;
    stackNode->nast = NULL;
    return stackNode;
}

bool isEmpty(Tlista *root) {
    return !root;
}

void push(Tlista **root, Twezel *d) {
    Tlista *stackNode = newNode(d);
    stackNode->nast = *root;
    *root = stackNode;
}

Twezel *pop(Tlista **root) {
    if (isEmpty(*root))
        return NULL;
    Tlista *temp = *root;
    *root = (*root)->nast;
    Twezel *popped = temp->d;
    free(temp);
    return popped;
}

Twezel *peek(Tlista *root) {
    if (isEmpty(root))
        return NULL;
    return root->d;
}

void piszl(Tlista *l) {
    while (l) {
        printf("%d ", l->d->w);
        l = l->nast;
    }
}

void piszlw(Tlistaw *l) {
    while (l) {
        printf("%d ", l->w);
        l = l->nast;
    }
}

void usunl(Tlista *l) {
    Tlista *tmp;
    while (l != NULL) {
        tmp = l;
        l = l->nast;
        free(tmp);
    }
}

void usunlw(Tlistaw *l) {
    Tlistaw *tmp;
    while (l != NULL) {
        tmp = l;
        l = l->nast;
        free(tmp);
    }
}

int wysokosc(Twezel *d) {
    if (d) {
        int hl = 0, hp = 0;
        if (d->lsyn == d->psyn) return 0;
        if (d->lsyn)
            hl = wysokosc(d->lsyn) + 1;
        if (d->psyn)
            hp = wysokosc(d->psyn) + 1;
        if (hl > hp) return hl;
        else return hp;
    } else return 0;
}

int wysokosc_min(Twezel *d) {
    if (d) {
        int hl = -1, hp = -1;
        if (d->lsyn == d->psyn) return 0;
        if (d->lsyn) {
            hl = wysokosc_min(d->lsyn) + 1;
            if (!d->psyn) return hl;
        }
        if (d->psyn) {
            hp = wysokosc_min(d->psyn) + 1;
            if (!d->lsyn) return hp;
        }
        if (hl <= hp) return hl;
        else return hp;
    } else return 0;
}

Tlista *NajdluzszaGalaz(Twezel *d) {
    Tlista *p = NULL;
    if (d->lsyn && d->psyn) {
        if (wysokosc(d->lsyn) >= wysokosc(d->psyn))
            p = NajdluzszaGalaz(d->lsyn);
        else
            p = NajdluzszaGalaz(d->psyn);
    } else if (d->lsyn)
        p = NajdluzszaGalaz(d->lsyn);
    else if (d->psyn)
        p = NajdluzszaGalaz(d->psyn);
    Tlista *l = (Tlista *) malloc(sizeof(Tlista));
    l->d = d;
    l->nast = p;
    return l;
}

void ListaBST(Tlista **l, Twezel *d) {
    if (d->lsyn) ListaBST(l, d->lsyn);
    push(l, d);
    if (d->psyn) ListaBST(l, d->psyn);
}

Tlista *ListaWszystich(Twezel *d1, Twezel *d2) {
    Tlista *l = NULL;
    Tlista *s1 = NULL;
    ListaBST(&s1, d1);
    Tlista *s2 = NULL;
    ListaBST(&s2, d2);
    while (!isEmpty(s1) || !isEmpty(s2)) {
        if (isEmpty(s1)) {
            Tlista *p = newNode(pop(&s2));
            p->nast = l;
            l = p;
        } else if (isEmpty(s2)) {
            Tlista *p = newNode(pop(&s1));
            p->nast = l;
            l = p;
        } else {
            if (peek(s1)->w > peek(s2)->w) {
                Tlista *p = newNode(pop(&s1));
                p->nast = l;
                l = p;
            } else if (peek(s1)->w < peek(s2)->w) {
                Tlista *p = newNode(pop(&s2));
                p->nast = l;
                l = p;
            } else {
                pop(&s1);
                Tlista *p = newNode(pop(&s2));
                p->nast = l;
                l = p;
            }
        }
    }
    return l;
}

void ListaLewych(Tlista **l, Twezel **d) {
    while ((*d)->lsyn && (*d)->w == (*d)->lsyn->w) {
        push(l, (*d)->lsyn);
        if ((*d)->lsyn->lsyn)
            (*d)->lsyn = (*d)->lsyn->lsyn;
        else
            (*d)->lsyn = NULL;
    }
    if ((*d)->lsyn) ListaLewych(l, &(*d)->lsyn);
}

Tlista *UsunPowtorzenia(Twezel **d) {
    Tlista *s = NULL;
    ListaLewych(&s, d);
    return s;
}

int BliskieLiscie(Twezel *d) {
    int x, xl, xp;
    if (d->lsyn && d->psyn) {
        x = wysokosc_min(d->lsyn) + wysokosc_min(d->psyn) + 2;
        xl = BliskieLiscie(d->lsyn);
        xp = BliskieLiscie(d->psyn);
        if (xl < 0) xl = x;
        if (xp < 0) xp = x;
        if (xl <= xp && xl <= x) return xl;
        else if (xp <= xl && xp <= x) return xp;
        else return x;
    } else if (d->lsyn && !d->psyn)
        return BliskieLiscie(d->lsyn);
    else if (d->psyn && !d->lsyn)
        return BliskieLiscie(d->psyn);
    return -1;
}

bool CzyBST(Twezel *d, int min, int max) {
    if (d->lsyn) {
        if (d->lsyn->w >= d->w) return 0;
        if (d->lsyn->w <= min) return 0;
        if (!CzyBST(d->lsyn, min, d->w)) return 0;
    }
    if (d->psyn) {
        if (d->psyn->w <= d->w) return 0;
        if (d->psyn->w >= max) return 0;
        if (!CzyBST(d->psyn, d->w, max)) return 0;
    }
    return 1;
}

int DoBST(Twezel *d) {
    if (d->lsyn && d->psyn) {
        if (d->lsyn->w > d->w && d->psyn->w < d->w) {
            int t = d->lsyn->w;
            d->lsyn->w = d->psyn->w;
            d->psyn->w = t;
        }
        if (d->lsyn->w < d->w && d->psyn->w > d->w) {
            if (DoBST(d->lsyn) && DoBST(d->psyn)) return 1;
            else return 0;
        } else return 0;
    } else if (d->lsyn && !d->psyn) {
        if (d->lsyn->w < d->w) return 1;
        else return 0;
    } else if (d->psyn && !d->lsyn) {
        if (d->psyn->w > d->w) return 1;
        else return 0;
    }
    return 1;
}

bool PrawieBST(Twezel *d) {
    DoBST(d);
    return CzyBST(d, INT_MIN, INT_MAX);
}

int max_cienkie_pom(Twezel *d) {
    int xl = 0, xp = 0;
    if (d->lsyn)
        xl = max_cienkie_pom(d->lsyn);
    if (d->psyn)
        xp = max_cienkie_pom(d->psyn);
    if (d->lsyn && d->psyn) {
        if (abs(xl) >= abs(xp)) return xl * (-1);
        else return xp * (-1);
    }
    if (d->lsyn && !d->psyn) {
        if (xl >= 0)
            xl++;
        return xl;
    } else if (d->psyn && !d->lsyn) {
        if (xp >= 0)
            xp++;
        return xp;
    }
    return 1;
}

int MaxCienkie(Twezel *d) {
    int x = max_cienkie_pom(d);
    if (x >= 0) return x;
    else return x * (-1);
}

void lustro(Twezel *d) {
    if (d) {
        lustro(d->lsyn);
        lustro(d->psyn);
        Twezel *p = d->lsyn;
        d->lsyn = d->psyn;
        d->psyn = p;
    }
}

bool izomorficzne(Twezel *d1, Twezel *d2) {
    if (d1 == d2) return 1;
    else if (d1 && d2) {
        if (izomorficzne(d1->lsyn, d2->lsyn) && izomorficzne(d1->psyn, d2->psyn))
            return 1;
        else
            return 0;
    } else return 0;
}

bool Symetryczne(Twezel *d) {
    Twezel *dl = d->lsyn;
    Twezel *dp = d->psyn;
    //trzeba stworzyc nowe drzewo
    lustro(dl);
    lustro(dp);
    return (izomorficzne(dl, d->psyn) && izomorficzne(dp, d->lsyn));
}

bool CzyWBST(Twezel *d, int k) {
    if (d == NULL || d->w == k)
        return d;
    if (k < d->w)
        return CzyWBST(d->lsyn, k);
    return CzyWBST(d->psyn, k);
}

bool istniejeP(Twezel **d, Twezel *w, int k) {
    if (w->psyn)
        if (istniejeP(d, w->psyn, k)) return 1;
    if (CzyWBST(*d, w->w - k)) return 1;
    if (w->lsyn)
        if (istniejeP(d, w->lsyn, k)) return 1;
    return 0;
}

bool istnieje(Twezel *d, int k) {
    return istniejeP(&d, d, k);
}

int silnia(int k) {
    int x = 1;
    for (int i = 1; i <= k; i++)
        x *= i;
    return x;
}

Tlistaw *ListaH(Twezel *d) {
    Tlistaw *l = newNodew(wysokosc(d));
    if (d->lsyn) l->nast = ListaH(d->lsyn);
    if (d->psyn) {
        Tlistaw *k = ListaH(d->psyn);
        Tlistaw *p = k;
        if (l->nast) {
            while (k->nast)
                k = k->nast;
            k->nast = l;
            return p;
        } else {
            l->nast = k;
        }
    }
    return l;
}

int rowneH(Twezel *d) {
    int x = wysokosc(d), k = 0;
    Tlistaw *l = ListaH(d);
    for (int i = 0; i < x; i++) {
        Tlistaw *p = l;
        int n = 0;
        while (l) {
            if (l->w == i) n++;
            l = l->nast;
        }
        l = p;
        k += silnia(n) / (silnia(n - 2) * 2);
    }
    return k;
}

int IleWezlow(Twezel *d) {
    if (!d) return 0;
    else return IleWezlow(d->lsyn) + IleWezlow(d->psyn) + 1;
}

int intrygujaceP(Twezel *d, int k, int LczyP) {
    int p = k;
    if (d->lsyn) {
        int pl = intrygujaceP(d->lsyn, k, 1);
        k = pl;
    }
    if (LczyP == 1) {
        if (p == 0)
            k++;
        else {
            if (k % 2 == 1)
                k += 2;
            else
                k++;
        }
    } else {
        if (p == 0)
            k++;
        else {
            if (k % 2 == 1)
                k++;
            else
                k += 2;
        }
    }
    if (d->psyn) {
        int pp = intrygujaceP(d->psyn, k, 2);
        k = pp;
    }
    return
            k;
}

int intrygujacePzmiana(Twezel *d, int k, int LczyP) {
    int p = k;
    if (d->lsyn) {
        int pl = intrygujacePzmiana(d->lsyn, k, 1);
        k = pl;
    }
    if (LczyP == 1) {
        if (p == 0)
            k++;
        else {
            if (k % 2 == 1)
                k += 2;
            else
                k++;
        }
    } else {
        if (p == 0)
            k++;
        else {
            if (k % 2 == 1)
                k++;
            else
                k += 2;
        }
    }
    d->w = k;
    if (d->psyn) {
        int pp = intrygujacePzmiana(d->psyn, k, 2);
        k = pp;
    }
    return k;
}

bool Intrygujace(Twezel *d) {
    if (intrygujaceP(d, 0, 1) > IleWezlow(d)) return 0;
    intrygujacePzmiana(d, 0, 1);
    return 1;
}

int P_NP(Twezel *d) {
    int x = 0;
    if (d->lsyn == d->psyn) return 0;
    if (d->w % 2 == 0) {
        if ((d->lsyn && d->lsyn->w % 2 == 1) || (d->psyn && d->psyn->w % 2 == 1)) x++;
    } else if (d->w % 2 == 1) {
        if ((d->lsyn && d->lsyn->w % 2 == 0) || (d->psyn && d->psyn->w % 2 == 0)) x++;
    }
    int p = x;
    if (d->lsyn)
        x += P_NP(d->lsyn);
    if (d->psyn)
        x += P_NP(d->psyn);
    if (p == 0 && x != 0) x++;
    return x;
}

struct para {
    int ile_centralnych, h_max;
};

struct para IleCentralnychpom(Twezel *d, int g) {
    struct para x = {0, 0};
    if (d->lsyn != NULL) {
        struct para pl = IleCentralnychpom(d->lsyn, g + 1);
        x.ile_centralnych += pl.ile_centralnych;
        x.h_max = pl.h_max;
    }
    if (d->psyn != NULL) {
        struct para pp = IleCentralnychpom(d->psyn, g + 1);
        x.ile_centralnych += pp.ile_centralnych;
        if (x.h_max < pp.h_max)
            x.h_max = pp.h_max;
    }
    if (g == x.h_max) x.ile_centralnych++;
    x.h_max += 1;
    return x;
}

int IleCentralnych(Twezel *d) {
    return IleCentralnychpom(d, 0).ile_centralnych;
}

int tMin(int *A, int n) {
    int w = A[0];
    for (int i = 1; i < n; i++) {
        if (A[i] < w) {
            w = A[i];
        }
    }
    return w;
}

int tMax(int *A, int n) {
    int w = A[0];
    for (int i = 1; i < n; i++) {
        if (A[i] > w) {
            w = A[i];
        }
    }
    return w;
}

int rozdziel(int *A, int *B, int n) {
    if (n == 0) return 0;
    int minA = tMin(A, n);
    int maxB = tMax(B, n);
    if (minA >= maxB) {
        return 0;
    }
    int minB = tMin(B, n);
    int maxA = tMax(A, n);
    if (minB >= maxA) {
        return 0;
    }

    int w1 = 0;
    for (int i = 0; i < n; i++) {
        if (B[i] >= A[i]) {
            w1 = i;
            break;
        }
    }

    int w2 = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (A[i] >= B[i]) {
            w2 = n - i - 1;
            break;
        }
    }

    return min(w1, w2);
}

int main(void) {
    //Twezel *d = czytaj();
    //Twezel *d2 = czytaj();
    //pisz(d, 0);
    //pisz(d2, 0);
    //Tlista *l = NajdluzszaGalaz(d);
    //Tlista *l = ListaWszystich(d, d2);
    //Tlista *l = UsunPowtorzenia(&d);
    //Tlistaw *l = ListaH(d);
    //pisz(d, 0);
    //piszlw(l);
    //usunlw(l);
    //printf("%d", BliskieLiscie(d));
    //printf("%d", PrawieBST(d));
    //printf("%d", MaxCienkie(d));
    //printf("%d", Symetryczne(d));
    //printf("%d", istnieje(d, 15));
    //printf("%d", CzyWBST(d, 15));
    //printf("%d", rowneH(d));
    //printf("%d\n", Intrygujace(d));
    //printf("%d\n", P_NP(d));
    //printf("%d\n", IleCentralnych(d));
    int A[7] = {8, 8, 8, 8, 7, 1, 1};
    int B[7] = {1, 2, 3, 4, 5, 9, 9};
    printf("%d\n", rozdziel(A, B, 7));
    //usun(d);
    //usun(d2);
    return 0;
}
