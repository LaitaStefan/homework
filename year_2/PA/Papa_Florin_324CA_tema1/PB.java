//Papa Florin, 324CA

import java.util.Collections;
import java.util.Vector;

@SuppressWarnings("unchecked")
public class PB {
    
    public PB(String inFile, String outFile) {
        P2 p = new P2(inFile);
        Vector<Integer> elem = p.get_elements();
        Integer minPoz = get_minPoz(elem);
        Vector<Integer> rearranged = rearrange(elem, minPoz);       
        
        Vector<Integer> result = p.get_intervals(rearranged);
        refactor(result,minPoz,rearranged.size());
        p.writeToFile("date.out", result);
    }
    
    public void refactor(Vector<Integer> v, Integer m, Integer size) {
        Vector<MyPair> auxPair = new Vector();
        for(int i=0;i<v.size();i=i+2) {
            Integer aux = v.get(i);
            v.set(i, (aux + m)%size);
            aux = v.get(i+1);
            v.set(i+1, (aux + m)%size);
            auxPair.add(new MyPair(v.get(i),v.get(i+1)));
        }
        
        Collections.sort(auxPair);
        
        for(int i=0;i<v.size();i=i+2) {
            v.set(i, auxPair.get(i/2).start);
            v.set(i+1, auxPair.get(i/2).end);
        }
    }
    
    public Integer get_minPoz(Vector<Integer> v) {
        Integer min = 0;
        for(int i=1;i<v.size();i++) {
            if(v.get(i)<v.get(min)) min = i;
        }
        return min;
    }
    
    public Vector<Integer> rearrange(Vector<Integer> v, Integer min) {
        Vector<Integer> aux = new Vector();
        for(int i=min;i<v.size();i++) {
            aux.add(v.get(i));
        }
        for(int i=0;i<min;i++) {
            aux.add(v.get(i));
        }
        return aux;
    }
    
    public static void main(String arg[]) {
        PB p = new PB("date.in","date.out");
    }   
}
