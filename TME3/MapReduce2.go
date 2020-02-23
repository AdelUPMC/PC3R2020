package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"os"
	//         "strings"
)

/*
func lecteur () {}w
func  travailleur () {}
func  serveur () {}
func reducteur(){}

type paquet struct{
	trip_id string
	arrival_time string
	departure_time string
	stop_id string
	stop_sequence string
	stop_headsign int
	pickup_type string
	drop_off_type int
	shape_dist_traveled int
}*/

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
type MyBox struct {
	Items []MyBoxItem
}
*/

func travailleur(cl chan string, cs chan message, cp chan paquet, fin chan int) {
	for line := range cl {
		fmt.Println("reçu ", line[0])
		/*
			convert line to paquet

		*/
		p := paquet{depart: "19:20:00", arrivee: "19:30:00", arret: 0}
		csout := make(chan paquet)
		m := message{content: p, cout: csout}

		cs <- m // envoi au serveur

		pnew := <-csout // recoit du serveur
		fmt.Println("Travailleur: arret =", string(pnew.arret))

	}
	fmt.Println("travailleur a fini")
	fin <- 0

}

func lecteur(filename string, cl chan string) {
	flag.Parse()
	//filename =
	//fmt.Println(filename)
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
	//read("stop_times.txt")
	cl := make(chan string)
	cs := make(chan message)
	cp := make(chan paquet)
	finTravailleur := make(chan int)
	finServeur := make(chan int)
	finReducteur := make(chan int)
	finalResultat := make(chan int)

	nbtravailleur := 2
	go func() { lecteur("extrait.txt", cl) }()
	for i := 1; i <= nbtravailleur; i++ {
		go func() { travailleur(cl, cs, cp, finTravailleur) }()
	}
	go func() { serveur(cs, finServeur) }()
	go func() { reducteur(cp, finalResultat, finReducteur) }()

	for i := 1; i <= nbtravailleur; i++ {
		<-finTravailleur
	}
	finServeur <- 0
	finReducteur <- 0
	fmt.Println("FIN")
	/*sleep du temps précisé en ligne de commande*/
	//TODO
	/*envoi du signal après le sleep*/

	moyenne := <-finalResultat
	fmt.Println("Serveur: content =", string(moyenne))
}
