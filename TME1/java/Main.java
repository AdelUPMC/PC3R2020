public class Main {
	public final static int NBPROD=3;
	public final static int NBCONS=2;
	public final static int CIBLE=5;
	
	public static Compteur c= new Compteur(NBPROD*CIBLE);//compteur partagée entre les consommateurs
	
	
	public static void main(String[] args) {
		Tapis t= new Tapis(10);
		Thread prod1=new Thread(new Producteur(t,"Pomme",CIBLE,1));
		Thread prod2=new Thread(new Producteur(t,"Poire",CIBLE,2));
		Thread prod3=new Thread(new Producteur(t,"Banane",CIBLE,3));
		
		Thread cons1=new Thread(new Consommateur(1,c,t));
		Thread cons2=new Thread(new Consommateur(2,c,t));
		System.out.println("cpt initial= "+c.getValue());
		//start
		prod1.start();
		prod2.start();
		prod3.start();
		cons1.start();
		cons2.start();
		
		//attente de terminaison des threads
		try {
			prod1.join();
			prod2.join();
			prod3.join();
			cons1.join();
			cons2.join();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		System.out.println(" main sucesfully ended !");



		
	}

}
