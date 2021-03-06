package main
 import (
         "bufio"
         "flag"
         "fmt"
         "io"
         "os"
         //"time"
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

type paquet struct{
	arrivee string
	depart string
	arret int

}
type  message struct{
	cout chan paquet
	content paquet
}


func lecteur(filename string, cl chan string,finlecteur chan int,termineTravailleurs chan int){
  fmt.Println("Lancement du lecteur")
  flag.Parse()
  file, err := os.Open(filename)
  if err != nil {
          panic(err)
  }

  defer file.Close()

  reader := bufio.NewReader(file)

  reader.ReadLine() //skip the first line
  for {
          line, _, err := reader.ReadLine()

          if err == io.EOF {
            fmt.Println("fin du fichier ",line)
            break;
          }
          /*envoit la ligne à un travailleur*/
          cl<- string(line)

  }
  termineTravailleurs<-0
  finlecteur<-0


}

func travailleur(cl chan string, cs chan message ,fintravailleur chan int,termineTravailleurs chan int){
  fmt.Println("Lancement d'un travailleur")
  for{
    select{
    case line :=<-cl:
    	   fmt.Println("reçu ",line)
    	    p := paquet{depart:"19:20:00",arrivee:"19:30:00",arret:500}
    	    csout:=make(chan paquet)
    	    m := message{content :p, cout : csout}
    	    cs<-m
    	    pnew:=<-csout
    	    fmt.Println("Travailleur: arret =",pnew.arret)
    case <-termineTravailleurs:
          fmt.Println("fin du travailleur")
          fintravailleur<-0
          break;
      }
    }
}


func  serveur (cs chan message,finserveur chan int ) {
	  fmt.Println("Lancement du serveur")

    mess:=<-cs
		go func(m message){
			m.content.arret=600
			fmt.Println("Serveur: content =",m.content.arret)
			m.cout <- m.content
		}(mess)

	finserveur<-0

}

/*cp: chan de paquets transformés*/
func reducteur(cp chan paquet,out chan int,findutemps chan int,finreducteur chan int ){
  temps_arret :=0
  nbpaquets:=0
  for{
    select{
    case <-findutemps:
      out<-temps_arret/nbpaquets
      break;
    case p:=<-cp:
      temps_arret+=p.arret
      nbpaquets++
    }
  }
  finreducteur<-0
}

func main() {
 //read("stop_times.txt")
 cl := make ( chan string)
 cs :=make(chan message)
 finlecteur := make ( chan int)
 fintravailleur := make ( chan int)
 finserveur := make ( chan int)
termineTravailleur := make ( chan int)
 //finreducteur := make ( chan int)

/*
 findutemps := make ( chan int)
 creduct := make ( chan int)
*/

 go func(){lecteur("stop_times.txt",cl,finlecteur,termineTravailleur)}()
 go func(){travailleur(cl,cs,fintravailleur,termineTravailleur)}()
 go func(){serveur(cs,finserveur)}()


 /*sleep du temps précisé en ligne de commande*/
//time.Sleep(10 * time.Second)
 /*envoi du signal après le sleep*/
/*findutemps<-0
moyenne:=<-creduct
fmt.Println("moyenne  =",string(moyenne))
*/
<-finlecteur
<-fintravailleur
//findutemps<-0
<-finserveur
//<-reducteur
}
