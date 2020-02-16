
public class Producteur implements Runnable{
	Tapis t;//ressource partagée
	String nomproduit;
	int cible;
	int id;
	
	public Producteur(Tapis t, String nomproduit, int cible,int id) {
		super();
		this.t = t;
		this.nomproduit = nomproduit;
		this.cible = cible;
		this.id=id;
	}
	

	@Override
	public void run() {
		System.out.println("P"+Integer.toString(id)+" thread créée");
		int nbprod=0;
		while(nbprod<cible) {
			synchronized(this) {
				String nomp=nomproduit.concat(" "+Integer.toString(nbprod+1));
				Paquet p= new Paquet(nomp);
				t.enfiler(p,id);
				System.out.println("P"+Integer.toString(id)+" enfile "+nomp);
				nbprod++;
				//System.out.println(t.toString());
			}
			
		}
		System.out.println("P"+Integer.toString(id)+": Cible de production atteinte ");
		
	}

}
