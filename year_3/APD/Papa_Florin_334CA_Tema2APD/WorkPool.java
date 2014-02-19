//Papa Florin, 334CA

import java.util.Enumeration;
import java.util.LinkedList;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Clasa ce implementeaza un "work pool" conform modelului "replicated workers".
 * Task-urile introduse in work pool sunt obiecte de tipul PartialSolution.
 *
 */
public class WorkPool {
    int nThreads; // nr total de thread-uri worker
    int nWaiting = 0; // nr de thread-uri worker care sunt blocate asteptand un task
    int target_index = -1;
    Vector<MyPair> result;
    Vector<Integer> totals;
    float resemblance[];
    public boolean ready = false; // daca s-a terminat complet rezolvarea

    LinkedList<PartialSolution> tasks = new LinkedList<PartialSolution>();

    /**
     * Constructor pentru clasa WorkPool.
     * @param nThreads - numarul de thread-uri worker
     */
    public WorkPool(int nThreads) {
            this.nThreads = nThreads;
            result = new Vector();
            totals = new Vector();
    }

    /**
     * Functie care incearca obtinera unui task din workpool.
     * Daca nu sunt task-uri disponibile, functia se blocheaza pana cand 
     * poate fi furnizat un task sau pana cand rezolvarea problemei este complet
     * terminata
     * @return Un task de rezolvat, sau null daca rezolvarea problemei s-a terminat 
     */
    public synchronized PartialSolution getWork() {
            if (tasks.size() == 0) { // workpool gol
                    nWaiting++;
                    /* condtitie de terminare:
                     * nu mai exista nici un task in workpool si nici un worker nu e activ 
                     */
                    if (nWaiting == nThreads) {
                            ready = true;
                            /* problema s-a terminat, anunt toti ceilalti workeri */
                            notifyAll();
                            return null;
                    } else {
                            while (!ready && tasks.size() == 0) {
                                    try {
                                            this.wait();
                                    } catch(Exception e) {e.printStackTrace();}
                            }

                            if (ready)
                                /* s-a terminat prelucrarea */
                                return null;

                            nWaiting--;

                    }
            }
            return tasks.remove();
    }


    /**
     * Functie care introduce un task in workpool.
     * @param sp - task-ul care trebuie introdus 
     */
    synchronized void putWork(PartialSolution sp) {
            tasks.add(sp);
            /* anuntam unul dintre workerii care asteptau */
            this.notify();
    }
    
    synchronized void putSolution(ConcurrentHashMap<String, Integer> h, 
                                    String file) {
        int flag = 0;
        
        for(int i = 0; i < result.size(); i++) {
            if(result.get(i).file.equals(file)) {
                result.get(i).hash.add(h);
                flag = 1;
                break;
            }
        }
        
        if(flag == 0) {
            result.add(new MyPair(file, h));
        }
    }

    synchronized void putValues(String s, int val, String f) {
        for(int i = 0; i < result.size(); i++) {
            if(result.get(i).file.equals(f)) {
                if(result.get(i).hash.get(0).get(s) != null) {
                    int oldVal = result.get(i).hash.get(0).get(s);  
                    result.get(i).hash.get(0).put(s, val + oldVal);
                }
                else {
                    result.get(i).hash.get(0).put(s, val);
                }
                break;
            }
        }
    }
    
    synchronized void getTotal() {
        for(int i = 0; i < result.size(); i++) {
            int total = 0;
            Enumeration e = result.get(i).hash.get(0).keys();
            while(e.hasMoreElements()) {
                String s = (String)e.nextElement();
                total += result.get(i).hash.get(0).get(s);
            }
            totals.add(total);
        }
    }

    //sortare descrescatoare dupa asemanare
    void sortResemblance() {
        for(int i = 0; i < result.size() - 1; i++) {
            float max = resemblance[i];
            for(int j = i + 1; j < result.size(); j++) {
                if(resemblance[j] > resemblance[i]) {
                    max = resemblance[j];
                    swap(i, j);
                    swapPair(i, j);
                }
            }
        }
    }
    
    //interschimbare perechi
    void swapPair(int i, int j) {
        MyPair aux = result.get(i);
        result.set(i, result.get(j));
        result.set(j, aux);
    }

    //interschimbare valori
    void swap(int i, int j) {
        float aux = resemblance[i];
        resemblance[i]= resemblance[j];
        resemblance[j] = aux;
    }

    synchronized void set_target_index(String s) {
        resemblance = new float[result.size()];
        
        for(int i = 0; i < result.size(); i++) {
            if(result.get(i).file.equals(s)) {
                target_index = i;
                return;
            }
        }    
    }
    
    synchronized int getTarget() {
        return target_index;
    }
    
    synchronized ConcurrentHashMap<String, Integer> getMap(int i) {
        ConcurrentHashMap<String, Integer> h= result.get(i).hash.get(0);
        return new ConcurrentHashMap<String, Integer>(h);
    }
    
    synchronized int getTotals(int i) {
        return totals.get(i);
    }
    
    synchronized void setResemblance(int i, float val) {
        resemblance[i] = val;
    }
}
