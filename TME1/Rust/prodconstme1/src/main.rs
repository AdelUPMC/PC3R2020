use rand::prelude::*;
use std::sync::Arc;
use std::sync::Arc;
use std::sync::Mutex;
use std::collections::VecDeque;
use std::time;
use std::thread;
const NB_THREADS : usize =5;
const NB_PROD : usize=3;
const SIZE_TAPIS : usize=10;
struct Paquet{
    contenu: sString
}
/*elems: FIFO en utilisant push_back pour l'ajout et pop_front() pour retirer l' élément le plus ancien*/
struct Tapis{
    elems: VecDeque<Paquet>,
    taille:usize
}
fn ajoute(t:Arc<Mutex<Tapis>>,p:Paquet)->std::sync::Arc<std::sync::Mutex<Tapis>>{
    let mut t_l= t.lock().unwrap();
    t_l.taille+=1;
    t_l.elems.push_back(p);
    drop(t_l);
    t
}
fn retire(t: &Arc<Mutex<Tapis>>)->Paquet{
    let mut t= t.lock.unwrap();//on prend le verrou sur la file
    let p= t.elems.pop_front().unwrap();
    p
}
fn main() {
    let finis= Arc::new(Mutex::<usize>::new(NB_PROD*NB_THREADS));
    let tapis= Arc::new(Mutex::new(Tapis(elems:VecDeque::new(),taille=SIZE_TAPIS)));
    let mut handles= Vec::new();
    for i in 0..NB_THREADS {
        let mut tapis_clone= Arc::clone(&tapis);
        //création des threads + execution
        handles.push(std::thread::spawn(move||{
            for j in 0..NB_PROD{
                let p= Paquet(contenu:format!("Objet {} de {}",j.to_string(),i.to_string());
                println!("Producteur {} écrit {}", i.to_string(),p.contenu);
                tapis_clone=ajoute(tapis_clone,p);
            }
        }))
    }
    for i in 0..NBTHREADS {
        //récupérer une référence sur l'ownership des variables partagées entre les threads : shared ownership
        let tapis_clone=Arc::clone(&tapis);
        let finis_clone= Arc::clone(&finis);
        handles.push(std::thread::spawn(move||{
            let mut _rng = rand::thread_rng();
            let mut continu= true;
            while continu{
                let mut f_l= finis.clone.lock().unwrap();
                if *f_l<=0 continu=false;
                else {
                    *f_l-=1;
                    println!("Reste {}.",f_l.to_string());
                }
            if continu {
                let r_val: f64 = _rng.gen();
                let n_val= (r_val*50.0) as u64;
                let sommeil= time::Duration::from_millis(n_val);
                thread::sleep(sommeil);
                let p=retire(&ttapis_clone);
                println!("Consommateur {} lit {}.", i.to_string(),p.to_string());
            }
        });
        for h in handles{
            h.join().unwrap();
        }





    }
    println!("main ended sucessfully!");
}
