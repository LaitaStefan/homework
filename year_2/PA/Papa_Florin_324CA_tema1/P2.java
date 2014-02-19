//Papa Florin, 324CA

import java.util.Vector;

@SuppressWarnings("unchecked")
public class P2 {
    Vector<Integer> elem=new Vector(); //vectorul de sloturi
    Integer nr_elem,            //numarul de sloturi de timp
            size,               //dimensiunea vectorului de intrare
            suma_maxima;
    
    public P2(String inFile) {
        InputFileParser2 fp = new InputFileParser2(inFile);
        size = fp.getSize();
        nr_elem = fp.getElemNr();     
        elem = fp.getElem(); 
    }
    
    public void writeToFile(String outFile,Vector<Integer> rez) {
        OutputFileParser2 of;
        of = new OutputFileParser2(outFile,suma_maxima,rez.size()/2,rez);
    }
    
    public Vector<Integer> get_elements() {
        return elem;
    }
    
    public Vector<Integer> get_intervals(Vector<Integer> costs) {
        MyInterval[][] max_sums=new MyInterval[size][nr_elem];
        int[][] maxes = new int[size][nr_elem]; //retin maximul de pe fiecare coloana a matricii
                                                // max_sums, de la linia 0 la linia curenta 
        
        for(int i=0;i<size;i++) {
            max_sums[i][0] = new MyInterval(0,i,i);
        }
        
        for(int i=1;i<size;i++) {
            for(int j=1;j<nr_elem;j++) {
                if(j>i) break;
                
                int var1 = max_sums[i-1][j-1].sum+costs.get(i);
                int var2 = maxes[i-1][j-1];
                
                if(var1>var2) {
                    max_sums[i][j]=new MyInterval(var1,max_sums[i-1][j-1].from,i);
                    if(i==j) maxes[i][j] = max_sums[i][j].sum;
                    else    maxes[i][j]=max(max_sums[i][j].sum,maxes[i-1][j]);
                }
                else {
                    max_sums[i][j]=new MyInterval(var2,i,i);
                    if(i==j) maxes[i][j] = max_sums[i][j].sum;
                    else    maxes[i][j]=max(max_sums[i][j].sum,maxes[i-1][j]);
                }
            }
        }
        
        //reconstruire intervale
        Vector<Integer> intervals = new Vector();
        suma_maxima = maxes[size-1][nr_elem-1];
        int crt_col = nr_elem-1;
        int crt_line = get_max(max_sums,crt_col,size-1,maxes[size-1][nr_elem-1]).index;
        
        while(true) {
            intervals.add(max_sums[crt_line][crt_col].from+1);
            intervals.add(crt_line+1);
            int aux=max_sums[crt_line][crt_col].from;
            crt_col = crt_col - (crt_line - max_sums[crt_line][crt_col].from) - 1;
            if(crt_col<0) break;
            crt_line = get_max(max_sums,crt_col,aux-1,maxes[aux-1][crt_col]).index;
            
        }
        return intervals;
    }
    Integer max(Integer a,Integer b) {
            if(a>b) return a;
            return b;
    }
    
    MyInterval get_max(MyInterval max_sums[][],int j,int i,int mx) {
        MyInterval max=max_sums[j][j];
        if(max.sum==mx) return max;
        for(int k=j+1;k<=i;k++) {
            if(max_sums[k][j].sum==mx) return max_sums[k][j];
        }
        return max;
    }
    
    public static void main(String arg[]) {
        P2 p = new P2("date.in");
        Vector<Integer> result = p.get_intervals(p.get_elements());
        p.writeToFile("date.out", result);
    }
    
}
