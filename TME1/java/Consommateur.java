public class Consommateur implements Runnable {
	int id;
	Compteur cpt;//ressource partagée
	Tapis t;
	
	
	public Consommateur(int id, Compteur cpt, Tapis t) {
		super();
		this.id = id;
		this.cpt = cpt;
		this.t = t;
	}


	@Override
	public void run() {
		System.out.println("C"+Integer.toString(id)+" thread créée");
		while(cpt.getValue()>0) {
			synchronized(this) {
				Paquet p= t.defiler(id);
				String cons="C".concat(Integer.toString(id));
				System.out.println(cons+" mange "+p.getNom());
				cpt.decrement();
				System.out.println("C"+Integer.toString(id)+": cpt="+cpt.getValue());
				//System.out.println(t.toString());
			}
		
		}
		System.out.println("C"+Integer.toString(id)+": Compteur de consommations à 0");
		// TODO Auto-generated method stub
		
	}

}
