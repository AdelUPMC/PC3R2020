import java.util.ArrayList;

public class Tapis {
	ArrayList<Paquet> file;
	int capacity;
	/*
	Object full;//verrou full
	Object empty;//verrou empty
	*/
	public Tapis(int capacity) {
		file= new ArrayList<Paquet>();
		this.capacity = capacity;
		//full = new Object();
		//empty=new Object();
		
	}
	public synchronized void enfiler(Paquet p,int id) {
		//file pleine -> mise en attente du thread
		while(file.size()==capacity) {
			try {
				System.out.println("file pleine : P"+Integer.toString(id)+" s'endort");
                wait();
            } catch (InterruptedException e)  {
                Thread.currentThread().interrupt(); 
                e.printStackTrace();
            }
		}
		file.add(p);
		notifyAll();
	}
	public String toString() {
		String s="[ ";
		for(int i=0;i< file.size();i++) {
			s=s.concat(file.get(i).getNom());
			if(i==file.size()-1) break;
			s=s.concat(",");
			
		}
		s=s.concat("]");
		return s;
		
	}
	public synchronized Paquet defiler(int id) {
		Paquet first;
		//file vide -> mise en attente du thread
		while(file.size()==0) {
			try {
				System.out.println("file vide : C"+Integer.toString(id)+" s'endort");
                wait();
            } catch (InterruptedException e)  {
                Thread.currentThread().interrupt(); 
                e.printStackTrace();
            }
		}
		first=file.remove(0);
		notifyAll();
		return first;
	}
	
}
