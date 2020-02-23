package main

import (
	"fmt"
	"strings"
)



type paquet struct {
	arrivee string
	depart  string
	arret   int
}



func getPaquetFromLineTest() paquet{
     line:="OCESN006749F0200211549,19:20:00,19:20:00,StopPoint:OCETGV INOUI-87686006,0,\"\",0,0,"
     s := strings.Split(line, ",")
     //fmt.Println("s:",s[1])
     p := paquet{depart: s[1], arrivee: s[2], arret: 0}
     return p
}

func getPaquetFromLine(line string) paquet{
     s := strings.Split(line, ",")
     p := paquet{depart: s[1], arrivee: s[2], arret: 0}
     return p
}


func main() {

  /*uniquement pour tester, penser à utiliser plutôt getPaquetFromLine dans les algos */
  p:=getPaquetFromLineTest()

  fmt.Println("paquet: ",p)
}
