
Programul porneste prin a verifica ce argument a fost dat pentru comanda (compress -c, decompress -d sau mirror -m), dupa care se executa functiile specifice:
-pentru comanda de compress se deschid fisierele, se apeleaza functia compress, dupa care se scrie in fisierul de iesire numarul de noduri total, numarul de frunze si vectorul efectiv comprimat.
-pentru comanda de decompress se deschid fisierele si se citesc numarul de frunze si noduri, dupa care se citeste vectorul comprimat si se apeleaza functia decompress.
-pentru comanda mirror se deschid fisierele, se apeleaza functia compress, dupa care functia mirror, iar in final se decompreseaza inapoi imaginea.

Functia medie calculeaza media pixelilor dintr-o imagine data si returneaza un nod cu valorile respective.

Functia shouldcontinue calculeaza mean-ul unei imagini si verifica daca este mai mare sau nu fata de factorul de compresie. In functie de aceasta returneaza daca trebuie continuata comprimarea sau nu.

Functia split primeste o imagine-matrice si o pozitie. In functie de pozitia primita functia va returna o imagine-matrice ce va reprezenta coltul respectiv al imaginii intregi.

Functia compress porneste prin a citi din fisierul ppm dimensiunea si celelalte date din header, dupa care imaginea in sine ce este salvata intr-o matrice de valori RGB (struct Image). Dupa aceea sunt initializate 2 queue-uri, unul pentru noduri, iar celalt pentru imagini, fiecare avand functiile aferente de init, enqueue, dequeue, similare cu cele din laboratoare. Apoi, bag imaginea initiala si primul nod in queue-uri, apoi intr-un loop scot primul nod si prima imagine, ii pun indexul fiecarui corner al nodului, apoi folosesc functia de split pentru a obtine alte 4 imagini mai mici, pe care le bag in queue, si folosind functia de medie, bag urmatoarele 4 noduri in queue. Daca cumva imaginea nu mai trebuie comprimata, indexii vor fi -1 si split-ul nu va mai avea loc. La final, nodul pe care l-am scos din queue va fi bagat in vectorul final compresat. 

Functia decompress initializeaza o imagine al carui size este dat de primul nod din vectorul comprimat, dupa care apelez functia advance pornind de la coordonatele 0,0.

Functia advance este o functie recursiva ce primeste coordonatele din coltul stanga sus al patratului analizat. Daca se ajunge la un nod frunza atunci zona acoperita de acel nod va fi umpluta cu culoarea nodului. Daca nu, se merge mai departe, functia fiind reapelata cu coordonatele respective fiecarui patrat mai mic.

Functia mirror parcurge fiecare nod din vectorul compresat si inverseaza indexul aferent tipului de mirror (daca vertical top cu bottom sau left cu right daca orizontal).