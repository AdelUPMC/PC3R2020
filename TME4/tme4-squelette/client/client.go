package main

import (
	"fmt"
	"math/rand"
	"os"
	"regexp"
	"strconv"
	"time"
	"bufio"
    "io"

	st "./structures" // contient la structure Personne
	tr "./travaux" // contient les fonctions de travail sur les Personnes
)

var ADRESSE string = "localhost"                           // adresse de base pour la Partie 2
var FICHIER_SOURCE string = "./1-rne-cm.txt" // fichier dans lequel piocher des personnes
var TAILLE_SOURCE int = 450000                             // inferieure au nombre de lignes du fichier, pour prendre une ligne au hasard
var TAILLE_G int = 5                                       // taille du tampon des gestionnaires
var NB_G int = 2                                           // nombre de gestionnaires
var NB_P int = 2                                           // nombre de producteurs
var NB_O int = 4                                           // nombre d'ouvriers
var NB_PD int = 2                                          // nombre de producteurs distants pour la Partie 2

var pers_vide = st.Personne{Nom: "", Prenom: "", Age: 0, Sexe: "M"} // une personne vide

// paquet de personne, sur lequel on peut travailler, implemente l'interface personne_int
type personne_emp struct {
	lecteur chan couple_si
	individu st.Personne
	ligne int
	afaire []func(st.Personne) st.Personne
	statut string
}

// paquet de personne distante, pour la Partie 2, implemente l'interface personne_int
type personne_dist struct {
	// A FAIRE
}

type couple_si struct{
	ligne string
	numligne int
}

type couple_ich struct {
	chp chan st.Personne
	numligne int
	}

// interface des personnes manipulees par les ouvriers, les
type personne_int interface {
	initialise()          // appelle sur une personne vide de statut V, remplit les champs de la personne et passe son statut à R
	travaille()           // appelle sur une personne de statut R, travaille une fois sur la personne et passe son statut à C s'il n'y a plus de travail a faire
	vers_string() string  // convertit la personne en string
	donne_statut() string // renvoie V, R ou C
}

// fabrique une personne à partir d'une ligne du fichier des conseillers municipaux
// à changer si un autre fichier est utilisé
func personne_de_ligne(l string) st.Personne {
	separateur := regexp.MustCompile("\u0009") // oui, les donnees sont separees par des tabulations ... merci la Republique Francaise
	separation := separateur.Split(l, -1)
	naiss, _ := time.Parse("2/1/2006", separation[7])
	a1, _, _ := time.Now().Date()
	a2, _, _ := naiss.Date()
	agec := a1 - a2
	return st.Personne{Nom: separation[4], Prenom: separation[5], Sexe: separation[6], Age: agec}
}

// *** METHODES DE L'INTERFACE personne_int POUR LES PAQUETS DE PERSONNES ***

/*Ajout TME4 pour initialize*/
func ReadLine(r io.Reader, lineNum int) (line string, lastLine int, err error) {
    sc := bufio.NewScanner(r)
    for sc.Scan() {
        lastLine++
        if lastLine == lineNum {
            // you can return sc.Bytes() if you need output in []bytes
            return sc.Text(), lastLine, sc.Err()
        }
    }
    return line, lastLine, io.EOF
}
func lecteur(filename string, cl chan couple_si) {
	flag.Parse()

	file, err := os.Open(filename)
	if err != nil {
		panic(err)
	}

	defer file.Close()

	reader := bufio.NewReader(file)
	num:=0
	reader.ReadLine()
	num++
	for {
		line, _, err := reader.ReadLine()
		num++

		if err == io.EOF {
			break
		}
		cl <- couple_si{ligne:string(line),numligne:num}
	}

	close(cl)
}

func (p *personne_emp) initialise() {
	//retour := make (chan st.Personne)
	//p.lecteur <- couple_ich{numligne : randInt }
	csi:=<-p.lecteur
	//p.individu=st.Personne{Nom: "", Prenom: "", Age: 0, Sexe: "M"}
	p.statut="V"
	p.ligne=csi.numligne //TODO generation d'un int aleatoirement entre 0 et le nombre de lignes du fichier
	p.individu=personne_de_ligne(csi.ligne)
	//fmt.Println("initilise:", p.individu.Nom, p.individu.Age)
}

func (p *personne_emp) travaille() {
	// A FAIRE
}

func (p *personne_emp) vers_string() string {
	// A FAIRE
}

func (p *personne_emp) donne_statut() string {
	// A FAIRE
}

// *** METHODES DE L'INTERFACE personne_int POUR LES PAQUETS DE PERSONNES DISTANTES (PARTIE 2) ***
// ces méthodes doivent appeler le proxy (aucun calcul direct)

func (p personne_dist) initialise() {
	// A FAIRE
}

func (p personne_dist) travaille() {
	// A FAIRE
}

func (p personne_dist) vers_string() string {
	// A FAIRE
}

func (p personne_dist) donne_statut() string {
	// A FAIRE
}

// *** CODE DES GOROUTINES DU SYSTEME ***

// Partie 2: contacté par les méthodes de personne_dist, le proxy appelle la méthode à travers le réseau et récupère le résultat
// il doit utiliser une connection TCP sur le port donné en ligne de commande
func proxy() {
	// A FAIRE
}

// Partie 1 : contacté par la méthode initialise() de personne_emp, récupère une ligne donnée dans le fichier source
func lecteur() {
	// A FAIRE
}

// Partie 1: récupèrent des personne_int depuis les gestionnaires, font une opération dépendant de donne_statut()
// Si le statut est V, ils initialise le paquet de personne puis le repasse aux gestionnaires
// Si le statut est R, ils travaille une fois sur le paquet puis le repasse aux gestionnaires
// Si le statut est C, ils passent le paquet au collecteur
func ouvrier() {
	// A FAIRE
}

// Partie 1: les producteurs cree des personne_int implementees par des personne_emp initialement vides,
// de statut V mais contenant un numéro de ligne (pour etre initialisee depuis le fichier texte)
// la personne est passée aux gestionnaires
func producteur() {
	// A FAIRE
}

// Partie 2: les producteurs distants cree des personne_int implementees par des personne_dist qui contiennent un identifiant unique
// utilisé pour retrouver l'object sur le serveur
// la creation sur le client d'une personne_dist doit declencher la creation sur le serveur d'une "vraie" personne, initialement vide, de statut V
func producteur_distant() {
	// A FAIRE
}

// Partie 1: les gestionnaires recoivent des personne_int des producteurs et des ouvriers et maintiennent chacun une file de personne_int
// ils les passent aux ouvriers quand ils sont disponibles
// ATTENTION: la famine des ouvriers doit être évitée: si les producteurs inondent les gestionnaires de paquets, les ouvrier ne pourront
// plus rendre les paquets surlesquels ils travaillent pour en prendre des autres
func gestionnaire() {
	// A FAIRE
}

// Partie 1: le collecteur recoit des personne_int dont le statut est c, il les collecte dans un journal
// quand il recoit un signal de fin du temps, il imprime son journal.
func collecteur() {
	// A FAIRE
}

func main() {
	rand.Seed(time.Now().UTC().UnixNano()) // graine pour l'aleatoire
	if len(os.Args) < 3 {
		fmt.Println("Format: client <port> <millisecondes d'attente>")
		return
	}
	port, _ := strconv.Atoi(os.Args[1]) // utile pour la partie 2
	millis, _ := strconv.Atoi(os.Args[2]) // duree du timeout 
	fintemps := make(chan int)
	// A FAIRE
	// creer les canaux
	// lancer les goroutines (parties 1 et 2): 1 lecteur, 1 collecteur, des producteurs, des gestionnaires, des ouvriers
	// lancer les goroutines (partie 2): des producteurs distants, un proxy
	time.Sleep(time.Duration(millis) * time.Millisecond)
	fintemps <- 0
	<-fintemps
}
