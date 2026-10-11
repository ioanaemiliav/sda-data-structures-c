# Sistem de Indexare a Fișierelor (Motor de Căutare)

## 📌 Despre Proiect
Acest proiect implementează un sistem avansat de organizare și indexare a datelor, permițând regăsirea rapidă a fișierelor pe baza unor cuvinte-cheie. Funcționează ca un motor de căutare simplificat: asociază cuvinte-cheie unor fișiere care au un anumit scor de relevanță și permite interogări complexe pentru a găsi cele mai potrivite rezultate.

## 🛠️ Structuri de Date Utilizate
Pentru a garanta un timp de execuție optim, sistemul evită căutările liniare, folosind în schimb structuri de date complexe:
* **Arbore Multicăi de Regăsire (Trie):** Inima sistemului. Fiecare nod reprezintă o literă, iar drumul de la rădăcină spre frunze formează cuvintele-cheie. Nodurile terminale conțin liste de referințe către fișierele care includ acel cuvânt.
* **Max-Heap:** Utilizat exclusiv pentru interogările de tip `TOPK`, pentru a extrage eficient și rapid cele mai relevante `K` fișiere în funcție de scorul lor (sau alfabetic, în caz de egalitate).
* **Liste Dublu Înlănțuite:** Folosite pentru gestionarea memoriei globale a fișierelor și a metadatelor asociate.

## 🚀 Funcționalități și Comenzi
* **Gestionarea fișierelor:** `ADD` (adaugă un fișier nou cu scor și cuvinte-cheie) și `DEL` (șterge un fișier și curăță automat arborele de nodurile inutile).
* **Actualizare dinamică:** `ADDKW` și `DELKW` permit adăugarea sau ștergerea de cuvinte-cheie pentru fișiere deja existente în index.
* **Interogare (Query):** 
  * `FIND`: Găsește toate fișierele care conțin un anumit cuvânt.
  * `TOPK`: Returnează cele mai relevante `K` fișiere pentru un cuvânt dat.
  * `PREFIX` (Bonus): Returnează fișierele care conțin orice cuvânt ce începe cu un anumit prefix, parcurgând subarborele corespunzător.
* `PRINT`: Afișează conținutul complet al indexului în ordine lexicografică.

## 📂 Structura Proiectului
Codul este modularizat pentru a separa logica structurilor de date de comenzile sistemului:
* `tema.c` - Punctul de intrare; citește datele și direcționează comenzile.
* `tasks.h` - Implementarea logicii pentru fiecare comandă în parte.
* `lista.h` - Definirea listei dublu înlănțuite pentru fișiere.
* `arbore.h` - Implementarea arborelui de regăsire (Trie).
* `max-heap.h` - Implementarea Heap-ului pentru interogările TOPK.
* `bonus.h` - Logica de parcurgere a subarborilor pentru operația PREFIX.
