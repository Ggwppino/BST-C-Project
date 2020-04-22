#include <iostream>
#include <cassert>
#include "bst.h"

/*
 * Struct point che implementa un punto in 2 dimensioni
 */
struct point {
    int x; ///< coordinata x del punto
    int y; ///< coordinata y del punto

    /*
     * Costruttore che inizializza il punto
     * @param xx coordinata x del punto
     * @param yy coordinata y del punto
     */
    point(int xx, int yy) : x(xx), y(yy) {}

    /**
     * Operatore == per l'equal_tree
     * @param rhs punto da confrontare
     * @return true se sono uguali, false altrimenti
     */
    bool operator==(const point &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    /**
     * Operatore !=
     * @return !(operator==)
     */
    bool operator!=(const point &rhs) const {
        return !(rhs == *this);
    }

};

/*
 * Ridefinizione dell'operatore di stream per la stampa del punto xy
 * @param os oggetto dell'operatore stream
 * @param p punto da stampare
 * @return reference al' output stream
 */
std::ostream &operator<<(std::ostream &os, const point &p) {
    std::cout << "(" << p.x << ";" << p.y << ")";
    return os;
}

/*
 * Funtore per il confronto di 2 interi
 * @param intero a
 * @param intero b
 * @return true se a<b, false altrimenti
 */
struct compare_int {
    bool operator()(int a, int b) const {
        return a < b;
    }
};

/**
 * Funtore per l'uguaglianza tra 2 interi
 * @return true se a==b, false altrimenti
 */
struct equal_int{
    bool operator()(int a, int b) const{
        return a==b;
    }
};

/*
 * Funtore per il confronto di 2 stringhe
 * @param stringa a
 * @param stringa b
 * @return true se la lunghezza di è minore di quella di b, false altrimenti
 */
struct compare_string {
    bool operator()(const std::string &a, const std::string &b) const {
        return (a.size() < b.size());
    }
};

/**
 * Funtore per l'uguaglianza di 2 stringhe
 * @return truen se a==b, false altrimenti
 */
struct equal_string {
    bool operator()(const std::string &a, const std::string &b) const {
        return (a==b);
    }
};

/**
 * Funtore per il confronto di 2 punti
 * @param p1 punto
 * @param p2 punto
 * @return true se la coordinata x di p1 è minore di quella di p2, false altrimenti
 */
struct compare_point {
    bool operator()(const point &p1, const point &p2) const {
        return (p1.x < p2.x);
    }
};

/**
 * Funtore per l'uguaglianza di 2 punti
 * @return true se i 2 punti sono effettivamente uguali, false altrimenti
 */
struct equal_point {
    bool operator()(const point &p1, const point &p2) const {
        return (p1.x==p2.x) && (p1.y==p2.y);
    }
};

/**
 * Funtore per il confronto di due bts
 * @tparam T valore che i due alberi hanno
 * @tparam C funtore di confronto
 * @return true se la dimensione di tree1 è minore di quella di tree2, false altrimenti
 */
template<typename T, typename C, typename E>
struct compare_tree {
    bool operator()(const bst<T, C, E> &tree1, const bst<T, C, E> &tree2) const {
        return (tree1.size() < tree2.size());
    }
};

/*
 * Funtore che controlla se 2 alberi hanno lo stesso contenuto
 * @rerurn true se i due alberi hanno lo stesso contenuto, false altrimenti
 */
template<typename T, typename C, typename E>
struct equal_tree{
    bool operator()(const bst<T, C, E> &tree1, const bst<T, C, E> &tree2) const{
        bool check = false;
        if(tree1.size()!=0 && tree1.size()==tree2.size()) {
            check=true;
            typename ::bst<T, C, E>::const_iterator i, j;
            i = tree1.begin();
            j = tree2.begin();
            do {
                if (*i != *j) check = !check;
                ++i;
                ++j;
            } while (check && *i!=*j);
        }
        return check;
    }
};


typedef bst<int, compare_int, equal_int> bstint;
typedef bst<bstint, compare_tree<int, compare_int, equal_int>, equal_tree<int, compare_int, equal_int>> bstbstint;
typedef bst<bst<point,compare_point,equal_point>, compare_tree<point,compare_point,equal_point>, equal_tree<point, compare_point, equal_point>> bstbstpoint;

/*
 * Funtore per la printIF di un valore intero
 * @param valore intero
 * @return true se il valore inter è maggiore di 5, false altrimenti
 */
struct predicate_int {
    bool operator()(int eval) const {
        return eval > 5;
    }
};

/*
 * Funtore per la printIF di un albero di interi come valore
 * @param albero di interi
 * @return true se la dimensione dell'albero è maggiore di 3, false altrimenti
 */
struct predicate_tree {
    template<typename T, typename C, typename E>
    bool operator()(const bst<T, C, E> &rhs) const {
        return rhs.size() > 3;
    }
};

/*
 * Esegue i test dei metodi fondamentali
 */
void test_metodi_fondamentali() {
    std::cout << "####### Metodi Fondamentali #######" << std::endl;

    bstint bst;

    std::cout << "inserisco i dati in bst... 5,6,3,5(l'ultimo 5 non deve essere inserito) " << std::endl;
    bst.add(5);
    bst.add(6);
    bst.add(3);
    bst.add(5);

    std::cout << "stampa di bst riempito" << std::endl << bst << std::endl;

    bstint bst2(bst);
    std::cout << "stampa di bst2 con bst2(bst)" << std::endl << bst2 << std::endl;

    bstint bst3;
    bst3 = bst;
    std::cout << "stampa di bst3 con bst3 = bst" << std::endl << bst3 << std::endl;
}

/*
 * Esegue i test dei metodi richiesti
 */
void test_uso() {
    std::cout << "####### Test d'uso #######" << std::endl;

    double a[5] = {7.5, 9.9, 12.0, 2.1, 1.0};

    bstint bsti(a, a + 5);

    std::cout << "stampa di bsti con operator<<"
              << std::endl << bsti << std::endl;

    std::cout << "provo printIF con predicate_int" << std::endl;
    printIF(bsti, predicate_int());

    bstint bst;
    bst.add(5);
    bst.add(6);
    bst.add(9);
    bst.add(5);
    std::cout << "stampa di bst: " << bst << std::endl;
    std::cout << "stampa size di bst: " << bst.size() << std::endl;
    assert(bst.size() == 3);


    bool check1 = bst.find(6);
    assert(check1);
    std::cout << "valore 6 trovato!" << std::endl;

    bool check2 = bst.find(10);
    assert(!check2);
    std::cout << "valore 10 non trovato!" << std::endl;

    const bstint bst2 = bst.subtree(6);
    bstint bst3 = bst2.subtree(6);
    bstint bst4;
    try {
        bst4 = bst.subtree(100);
    } catch(bst_value_not_found &e){
        std::cout<< "eccezione chiamata"<<std::endl;
    }
    std::cout << "Stampa di bst2, sottoalbero di bst. I valori dovrebbero essere 6 e 9" << std::endl;
    std::cout << bst2 << std::endl;
    std::cout << "Stampa di bst3, sottoalbero di const bst2. I valori dovrebbero essere 6 e 9" << std::endl;
    std::cout << bst3 << std::endl;
    assert(bst2.size() == 2);
    assert(bst3.size() == 2);
    std::cout << "Cerco il numero 6 in const bst2: "
              << bst2.find(6)
              << std::endl;
}

/*
 * Esegue test sui metodi dell'albero con gli interi
 */
void test_tree_int(bstint &other) {
    std::cout << "######## Test su interi #######" << std::endl;

    std::cout << "Insertimento dei valori 56, 90, 60, 5, 60" << std::endl;
    other.add(56);
    other.add(90);
    other.add(60);
    other.add(5);
    other.add(60);

    std::cout << "Stampa con operator<<" << std::endl;
    std::cout << other << std::endl;

    std::cout << "Dimensione del bst: " << other.size() << std::endl;

    std::cout << "Stampa con iteratori" << std::endl;

    bstint::const_iterator i, ie;

    for (i = other.begin(), ie = other.end(); i != ie; ++i)
        std::cout << *i << " ";

    std::cout << std::endl;
    i = other.begin();
    std::cout << "Provo se parte l'eccezione bst_out_of_bound" << std::endl;
    try {
        while (true) ++i;
    } catch (bst_out_of_bound &e) {
        std::cout << "bst iterator Out of Bound";
    }

    std::cout << std::endl;

    std::cout << "provo printIF con predicate_int" << std::endl;
    printIF(other, predicate_int());

}

/**
 * Esegue i test su una bst intera costante
 */
void test_tree_int_const(const bstint &other) {
    std::cout << "######## Test su interi con riferimento costante #######" << std::endl;


    std::cout << "Stampa con operator<<" << std::endl;
    std::cout << other << std::endl;

    std::cout << "Dimensione del bst: " << other.size() << std::endl;

    std::cout << "Stampa con iteratori" << std::endl;

    bstint::const_iterator i, ie;

    for (i = other.begin(), ie = other.end(); i != ie; ++i)
        std::cout << *i << " ";

    std::cout << std::endl;
    i = other.begin();
    std::cout << "Provo se parte l'eccezione bst_out_of_bound" << std::endl;
    try {
        while (true) ++i;
    } catch (bst_out_of_bound &e) {
        std::cout << "bst iterator Out of Bound";
    }

    std::cout << std::endl;

    bstint bst1 = other.subtree(90);
    std::cout << "provo stampo sottoalbero: " << bst1<< std::endl;


    std::cout << "provo printIF con predicate_int" << std::endl;
    printIF(other, predicate_int());

}

/*
 * Esegue i test sui metodi dell'albero con le stringhe
 */
void test_tree_stringhe() {
    std::cout << "####### Test su stringhe #######" << std::endl;

    bst<std::string, compare_string, equal_string> bsts;
    std::cout << "Insertimento dei valori 'foo', 'bar', 'pippo', 'pluto' " << std::endl;
    bsts.add("foo");
    bsts.add("bar");
    bsts.add("pippo");
    bsts.add("pluto");

    std::cout << "Stampa con operator<<" << std::endl;
    std::cout << bsts << std::endl;

    std::cout << "Dimensione del bsts: " << bsts.size() << std::endl;

    std::cout << "Ricerca di 'bar': " << bsts.find("bar") << std::endl;

    std::cout << "Ricerca di 'baaar': " << bsts.find("baaar") << std::endl;

    bst<std::string, compare_string, equal_string> bsts2 = bsts.subtree("pippo");
    std::cout << "Stampa sottoalbero bsts2 " << bsts2 << std::endl;
}

/*
 * Esegue i test sui metodi dell'albero con la struttura point
 */
void test_tree_point() {
    bst<point, compare_point, equal_point> bstp;

    std::cout << "####### Test sul point #######" << std::endl;

    std::cout << "Inserimento dei punti: (2;2), (7;1), (5;7), (0;0), (1;4)" << std::endl;
    bstp.add(point(2, 2));
    bstp.add(point(7, 1));
    bstp.add(point(5, 7));
    bstp.add(point(0, 0));
    bstp.add(point(1, 4));

    std::cout << "Stampa con operator<<" << std::endl;
    std::cout << bstp << std::endl;

    std::cout << "Dimensione dell'albero: " << bstp.size() << std::endl;

    std::cout << "Stampa con iteratori" << std::endl;

    bst<point, compare_point, equal_point>::const_iterator i, ie;

    for (i = bstp.begin(), ie = bstp.end(); i != ie; ++i)
        std::cout << *i << " ";
    std::cout << std::endl;

    std::cout << "Ricerca di (1;1): " << bstp.find(point(1, 1)) << std::endl;
    std::cout << "Ricerca di (2;2): " << bstp.find(point(2, 2)) << std::endl;

    bst<point, compare_point, equal_point> bstp2;

    bstp2 = bstp.subtree(point(7, 1));
    std::cout << "Stampa sottoalbero bstp2 " << bstp2 << std::endl;

}


/*
 * Esegue i test sui metodi dell'albero con alberi di interi
 */
void test_int_tree_tree() {
    std::cout << "####### Test albero di alberi di interi #######" << std::endl;
    int a[5] = {2, 3, 1, 5, 4};
    int b[7] = {1, 3, 4, 7, 5, 6, 2};
    int c[3] = {1, 2, 3};
    int cbis[3] = {2,3,1};
    int d[6] = {1, 2, 3, 5, 5, 8};


    std::cout << "Inserisco i dati... (se sono duplicati li scarto)" << std::endl;
    bstint tree_array[6] = {
            bstint(a, a + 5),
            bstint(b, b + 7),
            bstint(c, c + 3),
            bstint(c, c + 3), //duplicato
            bstint(cbis, cbis + 3), //duplicato bis
            bstint(d, d + 6)
    };

    std::cout << "Stampa albero di alberi di interi" << std::endl;
    bstbstint bstbstint1(tree_array, tree_array + 6);
    std::cout << bstbstint1 << std::endl;

    std::cout << "printIF usando predicate_tree" << std::endl;
    printIF(bstbstint1, predicate_tree());


    std::cout << "Stampa sotto albero di alberi di interi" << std::endl;
    bstbstint bstbstint2 = bstbstint1.subtree(tree_array[1]);
    std::cout << bstbstint2 << std::endl;

    int e[3] = {2, 1, 3};
    std::cout << "Cerco l'albero 2-1-3 in bstbstint1 con gli stessi valori di bstint3: "
              << bstbstint1.find(bstint(e, e + 3))
              << std::endl;
    std::cout << "Cerco bstint6 in bstbstint1: " << bstbstint1.find(bstint()) << std::endl;
}

/*
 * Esegue i test sui metodi dell'albero con alberi di point
 */
void test_point_tree_tree() {
    std::cout << "####### Test albero di alberi di point #######" << std::endl;
    point a[5] = {point(1,2), point(1,1), point(2,1), point(2,2), point(5,7)};
    point b[7] = {
            point(1,2),
            point(1,1),
            point(2,1),
            point(2,2),
            point(7,6),
            point(6,7),
            point(6,9)
    };
    point c[3] = {point(1,2), point(2,1), point(1,1)};
    point cbis[3] = {point(1,2), point(1,1), point(2,1)};
    point d[7] = {
            point(1,2),
            point(2,1),
            point(1,1),
            point(5,4),
            point(4,5),
            point(5,4),
            point(100,100)
    };
    point e[3] = {point(2,1), point(1,2), point(1,1)};

    std::cout << "Inserisco i dati... (se sono duplicati li scarto)" << std::endl;
    bst<point,compare_point,equal_point> tree_array[6] = {
            bst<point,compare_point,equal_point>(a, a + 5),
            bst<point,compare_point,equal_point>(b, b + 7),
            bst<point,compare_point,equal_point>(c, c + 3),
            bst<point,compare_point,equal_point>(c, c + 3), //duplicato
            bst<point,compare_point,equal_point>(cbis, cbis + 3), //duplicato bis
            bst<point,compare_point,equal_point>(d, d + 7)
    };

    std::cout << "Stampa albero di alberi di punti" << std::endl;
    bstbstpoint bstbstpoint1(tree_array, tree_array + 6);
    std::cout << bstbstpoint1 << std::endl;

    std::cout << "printIF usando predicate_tree" << std::endl;
    printIF(bstbstpoint1, predicate_tree());


    std::cout << "Stampa sotto albero di alberi di point" << std::endl;
    bstbstpoint bstbstpoint2 = bstbstpoint1.subtree(tree_array[1]);
    std::cout << bstbstpoint2 << std::endl;

    bstbstpoint bstbstpoint3;
    std::cout<<"Creo una eccezione bst_value_not_found"<<std::endl;
    try{
        bstbstpoint3 = bstbstpoint1.subtree(bst<point,compare_point,equal_point>());
    }catch(bst_value_not_found &e){
        std::cout<< "Eccezione bst_value_not_found" << std::endl;
    }

    std::cout << "Cerco l'albero (2;1)-(1;2)-(1;1) in bstbstpoint1 con gli stessi valori di bstpoint3: "
              << bstbstpoint1.find(bst<point,compare_point,equal_point>(e, e + 3))
              << std::endl;
    std::cout << "Cerco un albero vuoto in bstbstpoint1: " << bstbstpoint1.find(bst<point,compare_point,equal_point>()) << std::endl;

}


int main() {

    test_metodi_fondamentali();

    test_uso();

    bstint bst;

    test_tree_int(bst);

    test_tree_int_const(bst);

    test_tree_stringhe();

    test_tree_point();

    test_int_tree_tree();

    test_point_tree_tree();

    return 0;
}