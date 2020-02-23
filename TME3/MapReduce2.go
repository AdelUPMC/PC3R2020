package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
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

/*
func getPaquetFromLine(line string) paquet{
     s := strings.Split(line, ",")
     p := paquet{depart: s[1], arrivee: s[2], arret: 0}
     return p
}
*/

func travailleur(chanLecteur chan string, chanServeur chan message, chanReducteur chan paquet, fin chan int) {
	for line := range chanLecteur {
		str := strings.Split(line, ",")
		p := paquet{depart: str[1], arrivee: str[2], arret: 0}
		chanStoTravailleur := make(chan paquet)
		m := message{content: p, cout: chanStoTravailleur}

		chanServeur <- m // envoi au serveur

		pnew := <-chanStoTravailleur // recoit du serveur
		chanReducteur <- pnew
	}
	fmt.Println("travailleur a fini")
	fin <- 0
}

func lecteur(filename string, cl chan string) {
	flag.Parse()

	file, err := os.Open(filename)
	if err != nil {
		panic(err)
	}

	defer file.Close()

	reader := bufio.NewReader(file)

	reader.ReadLine()
	for {
		line, _, err := reader.ReadLine()

		if err == io.EOF {
			break
		}
		cl <- string(line)
	}

	close(cl)
}

func dateToNombre(p string) int {
	s := strings.Split(p, ":")
	h, _ := strconv.Atoi(s[0])
	m, _ := strconv.Atoi(s[1])
	l, _ := strconv.Atoi(s[2])
	res := l + 60*m + 3600*h
	return res
}

func serveur(cs chan message, fin chan int) {
	for {
		select {
		case <-fin:
			break
		case mess := <-cs:
			go func(m message) {
				start := dateToNombre(m.content.depart)
				end := dateToNombre(m.content.arrivee)
				m.content.arret = end - start
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
			res := temps_arret / nbpaquets
			fmt.Println("resultat ", res, nbpaquets)
			out <- res
			break
		case p := <-cp:
			temps_arret += p.arret
			nbpaquets++
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
	nbtravailleur := 4
	/*
		TODO donner le fichier en parametre
	*/

	go func() { lecteur("stop_times.txt", chanLtoTravailleur) }()
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
	time.Sleep(time.Millisecond * 1000)
	finReducteur <- 0
	moyenne := <-finalResultat
	fmt.Println("Resultat finale =", moyenne/60, "minutes", moyenne%60, "secondes")

}
