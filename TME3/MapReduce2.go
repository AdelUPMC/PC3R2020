package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"os"
	"time"
)

type paquet struct {
	arrivee string
	depart  string
	arret   int
}
type message struct {
	cout    chan paquet
	content paquet
}

func travailleur(chanLecteur chan string, chanServeur chan message, chanReducteur chan paquet, fin chan int) {
	for line := range chanLecteur {
		fmt.Println("reçu ", line)
		/*
			TODO convert line to paquet
		*/

		p := paquet{depart: "19:20:00", arrivee: "19:30:00", arret: 0} // pour le test a supp
		chanStoTravailleur := make(chan paquet)
		m := message{content: p, cout: chanStoTravailleur}

		chanServeur <- m // envoi au serveur

		pnew := <-chanStoTravailleur // recoit du serveur
		fmt.Println("Travailleur: arret =", string(pnew.arret))
	}
	fmt.Println("travailleur a fini")
	fin <- 0
}

func lecteur(filename string, cl chan string) {
	flag.Parse()
	fmt.Println("Hello World")

	file, err := os.Open(filename)
	if err != nil {
		panic(err)
	}

	defer file.Close()

	reader := bufio.NewReader(file)

	for {
		line, _, err := reader.ReadLine()

		if err == io.EOF {
			break
		}
		cl <- string(line)

	}
	close(cl)
}

func serveur(cs chan message, fin chan int) {
	fmt.Println("Hello World2")
	for {
		select {
		case <-fin:
			fmt.Println("C'est fini !!")
			break
		case mess := <-cs:
			go func(m message) {
				/*
					TODO Calcul du temp d'arret a partir de 2 string
				*/
				m.content.arret = 600
				fmt.Println("Serveur: content =", m.content.arret)
				m.cout <- m.content
			}(mess)
		}
	}
	close(cs)
}

/*cp: chan de paquets transformés*/
func reducteur(cp chan paquet, out chan int, findutemps chan int) {
	temps_arret := 0
	nbpaquets := 0
	for {
		select {
		case <-findutemps:
			res := temps_arret / (nbpaquets + 1)
			fmt.Println("resultat ", res)
			out <- res
			break
		case p := <-cp:
			temps_arret += p.arret
			nbpaquets++
			fmt.Println("resultat  reducteur")
		}
	}
}

func main() {
	chanLtoTravailleur := make(chan string)
	chanTtoserveur := make(chan message)
	chanTtoReducteur := make(chan paquet)
	finTravailleur := make(chan int)
	finServeur := make(chan int)
	finReducteur := make(chan int)
	finalResultat := make(chan int)
	nbtravailleur := 2

	/*
		TODO donner le fichier en parametre
	*/

	go func() { lecteur("extrait.txt", chanLtoTravailleur) }()
	for i := 1; i <= nbtravailleur; i++ {
		go func() { travailleur(chanLtoTravailleur, chanTtoserveur, chanTtoReducteur, finTravailleur) }()
	}
	go func() { serveur(chanTtoserveur, finServeur) }()
	go func() { reducteur(chanTtoReducteur, finalResultat, finReducteur) }()

	for i := 1; i <= nbtravailleur; i++ {
		<-finTravailleur
	} // fini qu'une fois aue tous les travailleurs ont terminer

	finServeur <- 0
	fmt.Println("Calcul ...")
	time.Sleep(time.Millisecond * 7500)
	finReducteur <- 0
	moyenne := <-finalResultat
	fmt.Println("Fin Serveur: content =", string(moyenne))
}
