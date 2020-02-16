
public class Compteur {
	private int value;

	public Compteur(int value) {
		this.value = value;
	}

	public synchronized int getValue() {
		return value;
	}
	

	public synchronized void increment() {
        value++;
    }
	public synchronized void decrement() {
        value--;
    }
	
	
}
