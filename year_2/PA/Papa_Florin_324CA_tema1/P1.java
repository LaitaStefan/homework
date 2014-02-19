//Papa Florin, 324CA

import java.util.Collections;
import java.util.Vector;

@SuppressWarnings("unchecked")
public class P1 {
    Vector<Integer> fence=new Vector();
    Integer N,M,L;
    Integer max;
    public P1(String inFile, String outFile) {
        InputFileParser fp = new InputFileParser(inFile);
        N = fp.getN();
        M = fp.getM();
        L = fp.getL();       
        fence = fp.getFence();   
        
        binary_search(fence,1,L);
        Vector<Integer> rez = construct_sol();  //reconstruire solutie pe baza distantei
                                                //maxime posibile continute de variabila max
        OutputFileParser of = new OutputFileParser(outFile, max, rez);
    }
    
    public int getNumberOfElements(Vector<Integer> v, int dist) {
        int last=0,count=2,n=v.size();
        for(int i=1;i<n-1;i++) {
            if(v.get(i)-last >= dist && v.get(n-1)-v.get(i) >= dist) {
                last = v.get(i);
                count++;
            }
        }
        return count;
    }
    
    public void binary_search(Vector<Integer> v, int start, int end) {
        if(start>end) return;
        int mid = start + (end - start)/2;
        int nr = getNumberOfElements(v,mid);    //intoarce numarul de "stalpi" ce pot fi folositi
                                                //pentru a obtine o distanta minima egala cu mid
        if (nr < N-M) {
            binary_search(v, start, mid-1);
        }
        else {
            max = mid; 
            binary_search(v,mid+1,end);
        }   
    }
    
    public Vector<Integer> construct_sol() {
        Vector<Integer> v = new Vector();
        int last=0;
        v.add(fence.get(0));
        for(int i=1;i<N-1;i++) {
            if(fence.get(i)-last >= max && fence.get(N-1)-fence.get(i) >= max) {
                last = fence.get(i);
                v.add(fence.get(i));
            }
        }
        v.add(fence.get(N-1));
        return v;
    }
    
    public static void main(String[] arg) {
        P1 f = new P1("date.in","date.out");
    }
}