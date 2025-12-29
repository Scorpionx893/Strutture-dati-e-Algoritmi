#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;
// Struttura dati che puo essere utilizzata per implementare una coda 
//con priorita`Minimo Heap (min-heap) come albero completo Astrazione 

template <typename T>
struct Node {
     T valore;
     Node* sinistro;
     Node* destro;
     Node* genitore;
     Node(T val) : valore(val), sinistro(nullptr), destro(nullptr), genitore(nullptr) {}
};


template <typename T>
class minHeap {
private:
    Node<T>* root;
    int size;
     // Funzione ausiliaria 
    void heapify(Node<T>* x); //versione Down-Heap serve a ripristinare l'ordine quando viene violato

public:
    minHeap() : root(nullptr), size(0) {}
    
    // Metodi di classe
    T valoreDelMinimo();
    T estrazioneDelMinimo();
    void inserimento(T valore);
    void decrementoChiave(Node<T>* x, T nuovaChiave);
    
   
 // Funzione ausiliaria 
    Node<T>* trovaPadre(Node<T>* radice, int indiceNodo);
    int getSize() const { return size; }
    Node<T>* getRoot() const { return root; }

    // Metodi di visualizzazione
    void stampaHeap(Node<T>* nodo, const string& prefix = "", bool isLeft = true);

};
// Funzione ausiliaria per trovare il padre del nuovo nodo
template <typename T>
Node<T>* minHeap<T>::trovaPadre(Node<T>* radice, int indiceNodo){
    if (indiceNodo <= 1) return nullptr;

    Node<T>* corrente = radice;
    int i = indiceNodo;

    // Seguiamo il cammino dal nodo alla root, ma ci fermiamo al padre
    while (i > 1) {
        i /= 2;  // andiamo al padre nell'indicizzazione
        if (i == 1) return corrente;  // No: dobbiamo scendere

    // Meglio invertire: partiamo dalla root e seguiamo i bit di (indiceNodo / 2)
    corrente = radice;
    int padre_ind = indiceNodo / 2;

    // Ora scendiamo seguendo i bit di padre_ind (da MSB a LSB, ma ignoriamo il primo)
    // Usiamo un ciclo che divide padre_ind
    int temp = padre_ind;
    while (temp > 1) {
        if (temp % 2 == 1) {
            corrente = corrente->destro;
        } else {
            corrente = corrente->sinistro;
        }
        temp /= 2;
    }
    return corrente;
}
}


// Visualizzazione con connettori per una stampa più leggibile
template <typename T>
void minHeap<T>::stampaHeap(Node<T>* nodo, const string& prefix, bool isLeft)
{
    if (nodo == nullptr) return;

    if (nodo->destro)
        stampaHeap(nodo->destro, prefix + (isLeft ? "|   " : "    "), false);

    cout << prefix << "+-- " << nodo->valore << "\n";

    if (nodo->sinistro)
        stampaHeap(nodo->sinistro, prefix + (isLeft ? "    " : "|   "), true);
}

// Operazione ritorna il valore minimo senza rimuoverlo O(1)
template <typename T> 
T minHeap<T>::valoreDelMinimo() {
    if(root == nullptr) {
        throw runtime_error("Heap vuoto");
    }
    return root->valore;
}

template <typename T>
void minHeap<T>::inserimento(T valore) {
    size++; // Incrementa il conteggio nodi
    Node<T>* nuovoNodo = new Node<T>(valore); // Punto 2 del pseudo codice

    if (root == nullptr) {
        root = nuovoNodo;
        return;
    }

    // Trova il padre usando il conteggio size
    Node<T>* p = trovaPadre(root, size); 
    nuovoNodo->genitore = p; // Punto 4 immagine

    if (p->sinistro == nullptr) p->sinistro = nuovoNodo;
    else p->destro = nuovoNodo;

    // RISALITA ()
    Node<T>* x = nuovoNodo;
    while (x->genitore != nullptr && x->genitore->valore > x->valore) { // Punto 5
        // Swap manuale dei valori
        T temp = x->genitore->valore;
        x->genitore->valore = x->valore;
        x->valore = temp; // Punto 6

        x = x->genitore; // Punto 7
    }

}

template <typename T>
void minHeap<T>::heapify(Node<T>* x) {
    if (x == nullptr) return ; // Caso base

    Node<T>* y = x->sinistro; // Punto 2: y = left(x)
    Node<T>* z = x->destro;   // Punto 3: z = right(x)
    Node<T>* min = x;         // Punto 4: min = x

    // Punto 5-6: IF y != NULL AND key(y) < key(x)
    if (y != nullptr && y->valore < x->valore) {
        min = y;
    }

    // Punto 7-8: IF z != NULL AND key(z) < key(min)
    if (z != nullptr && z->valore < min->valore) {
        min = z;
    }

    // Punto 9-11: IF min != x
    if (min != x) {
        // Punto 10: swap manuale dei valori
        T temp = x->valore;
        x->valore = min->valore;
        min->valore = temp;

        // Punto 11: heapify ricorsivo sul nuovo nodo "min"
        heapify(min);
    }
}

template <typename T>
T minHeap<T>::estrazioneDelMinimo() {
    if (root == nullptr) {
        throw runtime_error("Heap vuoto");
    }

    T minValore = root->valore;

    if (size == 1) { // Caso in cui c'è solo la radice
        delete root;
        root = nullptr;
        size--;
        return minValore;
    }

    // Trova l'ultimo nodo usando il conteggio size
    Node<T>* ultimoNodo = trovaPadre(root, size); 
    if (ultimoNodo->destro != nullptr) {
        ultimoNodo = ultimoNodo->destro;
    } else {
        ultimoNodo = ultimoNodo->sinistro;
    }

    // Sostituisci il valore della radice con l'ultimo nodo
    root->valore = ultimoNodo->valore;

    // Rimuovi l'ultimo nodo
    if (ultimoNodo->genitore->destro == ultimoNodo) {
        ultimoNodo->genitore->destro = nullptr;
    } else {
        ultimoNodo->genitore->sinistro = nullptr;
    }
    delete ultimoNodo;

    size--; 

    // Ripristina la proprietà di heap
    heapify(root); 

    return minValore; 
}

template <typename T>
void minHeap<T>::decrementoChiave(Node<T>* x, T nuovaChiave) {
    if (x == nullptr) {
        throw runtime_error("Nodo nullo passato a decrementoChiave");
    }

    if (nuovaChiave > x->valore) {
        throw runtime_error("Decrease-key: la nuova chiave deve essere minore o uguale");
    }

    // 2. key(x) = k
    x->valore = nuovaChiave;

    // 3. p = parent(x)
    Node<T>* p = x->genitore;

    // 4. while (p != NULL and key(p) > key(x))
    while (p != nullptr && p->valore > x->valore) {
        // 5. swap(x, p) → ma scambiamo solo i valori, non i nodi!
        T temp = p->valore;
        p->valore = x->valore;
        x->valore = temp;

        // 6. x = p
        x = p;
        // 7. p = parent(x)
        p = p->genitore;
    }
}

int main() {
    minHeap<int> heap;
    heap.inserimento(10);
        heap.stampaHeap(heap.getRoot());
        cout <<"-----------------------------" <<endl;

    heap.inserimento(5);
        heap.stampaHeap(heap.getRoot());
                cout <<"-----------------------------" <<endl;


    heap.inserimento(20);
        heap.stampaHeap(heap.getRoot());
                cout <<"-----------------------------" <<endl;


    heap.inserimento(3);
        heap.stampaHeap(heap.getRoot());
                cout <<"-----------------------------" <<endl;


    heap.inserimento(6);
        heap.stampaHeap(heap.getRoot());
                cout <<"-----------------------------" <<endl;


    heap.inserimento(15);
        heap.stampaHeap(heap.getRoot());
                cout <<"-----------------------------" <<endl;


    heap.inserimento(30);
        heap.stampaHeap(heap.getRoot());
                cout <<"-----------------------------" <<endl;


    heap.inserimento(8);
        heap.stampaHeap(heap.getRoot());
                cout <<"-----------------------------" <<endl;


    heap.inserimento(1);

        heap.stampaHeap(heap.getRoot());
                cout <<"-----------------------------" <<endl;

    cout << "Valore minimo: " << heap.valoreDelMinimo() << endl; // Dovrebbe stampare 3


    
    
    
    int estratto = heap.estrazioneDelMinimo();
cout << "\nStampa dopo l'estrazione del minimo (" << estratto << "):" << endl;
heap.stampaHeap(heap.getRoot());
cout <<"-----------------------------" <<endl;

    heap.decrementoChiave(heap.getRoot()->sinistro, 2); // Decrementa il valore del nodo sinistro della radice a 2
    cout << "\nStampa dopo il decremento chiave:" << endl;
    heap.stampaHeap(heap.getRoot());

 return 0;
}
