# Sistem de Dispecerizare a Urgențelor (Tip 112)

## 📌 Despre Proiect
Acesta este un program scris în C care simulează modul în care funcționează un dispecerat de urgențe reale. 
Programul preia incidente (accidente, incendii etc.), le ordonează în funcție de cât de grave sunt (high, medium, low)
și trimite automat echipajele disponibile pentru a le rezolva. 

Este o temă de facultate creată pentru a exersa lucrul cu memoria și structurile de date de bază.

## 🛠️ Structuri de date folosite
Pentru a face sistemul să funcționeze corect, am construit de la zero următoarele structuri:
* **Liste dublu înlănțuite circulare (cu santinelă):** Aici am salvat toate incidentele și toate intervențiile pentru
a le putea parcurge și afișa.
* **Cozi (Queues):** Le-am folosit ca niște „săli de așteptare”. Am făcut 3 cozi pentru incidente (în funcție de
prioritate) și o coadă pentru echipajele libere.
* **Stivă (Stack):** Aici am ținut minte istoricul echipajelor trimise pe teren. Dacă s-a făcut o greșeală, stiva
mă ajută să dau "Undo" rapid și să întorc echipajul la bază.

## 🚀 Ce știe să facă programul?
Programul citește dintr-un fișier o listă de comenzi și reacționează la ele:
* `ADD_INCIDENT` - Înregistrează o urgență nouă și o pune în coada potrivită.
* `CHECK_UNITS_AVAILABILITY` - Afișează câte echipaje sunt libere la acel moment.
* `DISPATCH` - Cuplează automat cea mai gravă urgență cu primul echipaj liber disponibil.
* `UNDO_LAST_DISPATCH` - Anulează ultima acțiune de Dispatch (folosind stiva) și eliberează echipajul.
* `SOLVED_INCIDENT` - Marchează un incident ca fiind rezolvat cu succes.
* Funcții de afișare (`SHOW_UNIT`, `SHOW_INCIDENT`, `SHOW_INTERVENTIONS`) - Arată starea curentă a echipajelor și a urgențelor din sistem.

## 📂 Organizarea codului
* `tema.c` - Fișierul principal care citește comenzile și le execută.
* `tasks.h` - Aici se află funcțiile pentru comenzi (Dispatch, Undo, Solved etc.).
* `structuri.h` - Aici am definit structurile de date (Echipaj, Incident, Intervenție) și funcțiile de creare/ștergere a lor.
