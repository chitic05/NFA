# chitic++

Aplicatie C++ care citeste un automat finit determinist (DFA) dintr-un fisier text si verifica daca anumite cuvinte sunt acceptate.

## Ce face proiectul

Programul:
1. Parseaza un fisier cu 3 sectiuni: `.states`, `.alphabet`, `.transitions`.
2. Construieste DFA-ul in memorie.
3. Primeste cuvinte ca argumente in linia de comanda.
4. Afiseaza pentru fiecare cuvant `true` sau `false`.

## Compilare

Cerinte:
- CMake >= 3.8
- Un compilator C++ cu suport C++17 (g++, clang++)

Comenzi:

```bash
cmake -S . -B build
cmake --build build
```

Executabilul va fi generat in `build/chitic++`.

## Rulare

Format:

```bash
./build/chitic++ <fisier_model_dfa> <cuvant1> <cuvant2> ...
```

Exemplu:

```bash
./build/chitic++ test.txt 01 0011 1010
```

Output-ul este cate un `true`/`false` pe linie, in ordinea cuvintelor date.

## Sintaxa fisierului DFA

Fisierul trebuie sa contina exact sectiunile:
- `.states`
- `.alphabet`
- `.transitions`

Ordinea recomandata este cea de mai sus.

### 1) Sectiunea `.states`

Fiecare linie defineste o stare. Formate valide:
- `<stare>` pentru stare normala
- `S <stare>` pentru starea initiala (obligatoriu exact una)
- `F <stare>` pentru stare finala (pot fi mai multe)

Exemplu:

```text
.states
S q0
F q1
q2
```

### 2) Sectiunea `.alphabet`

Contine simbolurile alfabetului, fiecare simbol fiind un singur caracter.
Poti pune mai multe simboluri pe aceeasi linie, separate prin spatiu.

Exemplu:

```text
.alphabet
a b 0 1
```

### 3) Sectiunea `.transitions`

Fiecare tranzitie are formatul:

```text
<from> <symbol> <to>
```

Unde:
- `<from>` si `<to>` trebuie sa fie stari definite in `.states`
- `<symbol>` trebuie sa existe in `.alphabet`

Exemplu:

```text
.transitions
q0 a q1
q1 b q0
```

Se accepta si simbol special `ALL` la tranzitii:

```text
q0 ALL q1
```

Acesta creeaza tranzitii din `q0` catre `q1` pentru toate simbolurile din alfabet.

## Comentarii si spatii

- Comentariile sunt suportate cu `#` sau `;`.
- Tot ce este dupa `#` sau `;` pe o linie este ignorat.
- Liniile goale sunt ignorate.
- Numele sectiunilor trebuie sa inceapa cu `.` pe prima coloana (fara spatii inainte).

## Erori frecvente

Programul poate afisa erori daca:
- lipseste sectiunea `.states`, `.alphabet` sau `.transitions`
- nu este definita starea initiala (`S <stare>`)
- o tranzitie foloseste stari nedefinite
- o tranzitie foloseste un simbol care nu este in alfabet

## Exemplu complet (din `test.txt`)

```text
#1-PAR SI 0-IMPAR
.states
   S qEE
   F qEO
   qOE
   qOO

.alphabet
   0 1

.transitions
   qEE 0 qEO
   qEE 1 qOE
   qEO 0 qEE
   qEO 1 qOO
   qOE 0 qOO
   qOE 1 qEE
   qOO 0 qOE
   qOO 1 qEO
```

## Observatie

Daca rulezi programul doar cu fisierul DFA, fara cuvinte, vei primi mesajul:
`You need to give some input!`.
