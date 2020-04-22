//
// Created by 829795 on 20/03/2020.
//

#ifndef INC_829795_BST_H
#define INC_829795_BST_H

#include <algorithm>
#include <ostream>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
 * Classe di eccezione se il valore cercato non è presente nell'albero.
 * E' generato in subtree
 */
class bst_value_not_found {

};

/**
 * Classe di eccezione se il nodo puntato dall'iteratore è nullptr.
 * E' generato in operator++ in const_iterator
 */
class bst_out_of_bound {
};

template<typename T, typename C, typename E>
class bst {

    /**
     * Struttura che implementa un nodo della BST
     */
    struct node {
        T value;
        node *left;
        node *right;
        node *parent;

        /**
         * Costruttore di default
         */
        node() : left(nullptr), right(nullptr), parent(nullptr) {}

        /**
         * Costruttore secondario che inizializza il nodo
         * @param value valore del nodo
         */
        explicit node(const T &value) : value(value), left(nullptr), right(nullptr), parent(nullptr) {}


        /**
         * Distruttore
         */
        virtual ~node() {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }

    }; //struttura nodo

    node *_root; ///< puntatore alla root del BST
    unsigned int _size; ///< numero di elementi del BST
    C _conf; ///< oggetto funtore per l'ordinamento del BST
    E _eql; ///< oggetto funtore per l'ugualianza

    /**
     * Cancella il contenuto del BST con l'ausilio di clear_helper
     */
    void clear() {
        clear_helper(_root);
        _root = nullptr;
    }

    /**
     * Funzione che permette di eliminare in modo ricorsivo elementi dell'albero dato un nodo specifico.
     * @param n nodo da analizzare
     */
    void clear_helper(node *n) {
        if (n != nullptr) {
            clear_helper(n->left);
            clear_helper(n->right);
            _size--;
            delete n;
            n = nullptr;
        }
    }

    /**
     * Funzione che permette di copiare in modo ricorsivo elementi dell'albero dato un nodo specifico
     */
    void copy_helper(node *n) {
        if (n == nullptr) return;
        add(static_cast<T>(n->value));
        copy_helper(n->left);
        copy_helper(n->right);
    }

    /**
 * Cerca il nodo con lo stesso valore dato
 * @param value valore da cercare
 * @return puntatore del nodo trovato, altrimenti nullptr
 */
    node *find_helper(const T &value) const {
        node *current = _root;
        while (current != nullptr && !_eql(current->value, value)) {
            if (_conf(value, current->value))
                current = current->left;
            else
                current = current->right;
        }
        return current;
    }


public:
    /**
     * Costruttore di default
     */
    bst() : _root(nullptr), _size(0) {}

    /**
     * Costruttore di copia
     * @param other BST da copiare
     * @throw eccezione di allocazione di memoria
     */
    bst(const bst &other) : _root(nullptr), _size(0) {
        node *current = other._root;

        try {
            copy_helper(current);
        } catch (...) {
            clear();
            throw;
        }
    }


    /**
     * Costruttore che riempie l'albero data una sequenza generica di dati
     * identificata dall'identificatore di inizio e di fine.
     * @tparam IterT tipo di iteratore
     * @param begin
     * @param end
     */
    template<typename IterT>
    bst(IterT begin, IterT end)
            : _root(nullptr), _size(0) {
        try {
            while (begin != end) {
                add(static_cast<T>(*begin));
                ++begin;
            }
        }
        catch (...) {
            clear();
            throw;
        }
    }

    /**
     * Operatore di assegnamento
     * @param other BST da copiare
     * @return reference a this
     */
    bst &operator=(const bst &other) {
        if (this != &other) {
            bst tmp(other);
            std::swap(_root, tmp._root);
            std::swap(_size, tmp._size);
        }
        return *this;
    }

    /**
     * Distruttore
     */
    virtual ~bst() {
        clear();
    }

    /**
     * Ritorna il numero di elementi del BST
     * @return numero di elementi inseriti
     */
    unsigned int size() const {
        return _size;
    }

    /**
     * Aggiunge un nodo al BST con il valore passato
     * @param value valore del nodo
     */
    void add(const T &value) {
        node *tmp = new node(value);
        if (_root == nullptr) {
            _root = tmp;
        } else {
            node *current = _root;
            node *trailing = _root;
            while (current != nullptr) {
                trailing = current;
                if (_eql(current->value, value)) {
                    delete tmp;
                    return;
                } else if (_conf(value, current->value))
                    current = current->left;
                else current = current->right;
            }
            tmp->parent = trailing;
            if (_conf(value, trailing->value))
                trailing->left = tmp;
            else trailing->right = tmp;
        }
        _size++;
    }

    /**
     * Funzione che cerca il valore dato con l'ausilio di find_helper
     * @param value valore da cercare
     * @return true se il valore è presente, false altrimenti
     */
    bool find(const T &value) const {
        return find_helper(value) != nullptr;
    }

    /**
     * Sotto albero del BST
     * @param value root del sotto albero
     * @return un oggetto bst
     * @throw eccezione di allocazione di memoria
     */
    bst subtree(const T &value) const {
        node *root_value = find_helper(value);
        if (root_value == nullptr) {
            throw bst_value_not_found();
        }
        bst sub_tree;
        try {
            if (root_value->parent == nullptr) return *this;
            sub_tree.copy_helper(root_value);
        } catch (...) {
            sub_tree.clear();
            throw;
        }
        return sub_tree;
    }

    /**
     * Iteratore costante di farward
     */
    class const_iterator {
        const node *_n;

    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T *pointer;
        typedef const T &reference;

        const_iterator() : _n(nullptr) {
        }

        const_iterator(const const_iterator &other) : _n(other._n) {
        }

        const_iterator &operator=(const const_iterator &other) {
            _n = other._n;
            return *this;
        }

        ~const_iterator() {
        }

        // Ritorna il dato riferito dall'iteratore (dereferenziamento)
        reference operator*() const {
            return _n->value;
        }

        // Ritorna il puntatore al dato riferito dall'iteratore
        pointer operator->() const {
            return &(_n->value);
        }

        // Operatore di iterazione post-incremento
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            if (_n == nullptr) throw bst_out_of_bound();
            if (_n->right != nullptr) {
                _n = _n->right;
                while (_n->left != nullptr) {
                    _n = _n->left;
                }
            } else {
                node *parent = _n->parent;
                while (parent != nullptr && _n == parent->right) {
                    _n = parent;
                    parent = _n->parent;
                }
                _n = parent;
            }
            return tmp;
        }

        // Operatore di iterazione pre-incremento
        const_iterator &operator++() {
            if (_n == nullptr) throw bst_out_of_bound();
            if (_n->right != nullptr) {
                _n = _n->right;
                while (_n->left != nullptr) {
                    _n = _n->left;
                }
            } else {
                node *parent = _n->parent;
                while (parent != nullptr && _n == parent->right) {
                    _n = parent;
                    parent = _n->parent;
                }
                _n = parent;
            }
            return *this;
        }

        // Uguaglianza
        bool operator==(const const_iterator &other) const {
            return (_n == other._n);
        }

        // Diversita'
        bool operator!=(const const_iterator &other) const {
            return (_n != other._n);
        }

    private:

        friend class bst;

        //costruttore di default
        const_iterator(const node *n) : _n(n) {}


    };

    /**
     * Ritorna l'iteratore all'inzizio del bst ( con la logica: il più in basso a sinistra)
     * @return iteratore a inizio sbt
     */
    const_iterator begin() const {
        node *tmp = _root;
        if (tmp != nullptr)
            while (tmp->left != nullptr) {
                tmp = tmp->left;
            }
        return const_iterator(tmp);
    }

    /*
     * Ritorna l'iteratore alla fine del bst
     *
     * @return iteratore a fine bst
     */
    const_iterator end() const {
        return const_iterator(nullptr);
    }

};//classe bst

/*
 * Ridefinizione dell'operatore operator<< per la stampa del contenuto del bst
 * @param os oggetto stream di output
 * @param bst albero da stampare
 * @return reference allo stream di output
 */
template<typename T, typename C, typename E>
std::ostream &operator<<(std::ostream &os, const bst<T, C, E> &bst) {
    typename ::bst<T, C, E>::const_iterator i, ie;
    i = bst.begin();
    ie = bst.end();

    while (i != ie) {
        os << *i << ' ';
        ++i;
    }

    return os;
}

/*
 * Funzione che stampa i valori di bst che rispettano un predicato P
 * @param bst albero da stampare
 * @param predicato da confrontare
 */
template<typename T, typename C, typename E, typename P>
void printIF(const bst<T, C, E> &tree, P predicato) {
    typename bst<T, C, E>::const_iterator i, ie;
    i = tree.begin();
    ie = tree.end();

    while (i != ie) {
        if (predicato(*i)) {
            std::cout << *i << " ";
        }
        ++i;
    }
    std::cout << std::endl;

}

#endif //INC_829795_BST_H
