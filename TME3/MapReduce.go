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

type paquet struct{
	arrivee string
	depart string
	arret int
	
}
type  message struct{
	cout chan paquet
	content paquet
}
	
func travailleur(cl chan string, cs chan message ,fin chan int){
	line :=<-cl
	fmt.Println("reçu ",line) 
	/*
		convert line to paquet
		
	*/
	p := paquet{depart:"19:20:00",arrivee:"19:30:00",arret:0}
	csout:=make(chan paquet)
	m := message{content :p, cout : csout}
	cs<-m
	pnew:=<-csout
	fmt.Println("Travailleur: arret =",string(pnew.arret))
	fin<-0	
}

func lecteur(filename string, cl chan string,fin chan int){
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
                 cl<- string(line)                 
		 		
       
         }
         fin<-0
}
func  serveur (cs chan message,fin chan int ) {
	   fmt.Println("Hello World2")
		mess:=<-cs
		go func(m message){
			m.content.arret=600
			fmt.Println("Serveur: content =",string(m.content.arret))
			m.cout <- m.content
		}(mess)

	fin<-0
	
}

func main() {
 //read("stop_times.txt")
 cl := make ( chan string)
 cs :=make(chan message)
 fin := make ( chan int)

 
 go func(){lecteur("stop_times.txt",cl,fin)}()
 go func(){travailleur(cl,cs,fin)}()
 go func(){serveur(cs,fin)}()
 <-fin
 <-fin
 <-fin

}
